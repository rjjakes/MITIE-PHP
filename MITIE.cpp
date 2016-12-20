/**
 *  MITIE.h
 *
 *  An extension that bridges the MITIE information extraction library with PHP
 *
 *  @author: https://github.com/rjjakes/
 */
#include <string>
#include <iostream>
#include <phpcpp.h>
#include <mitie.h>
#include <mitie/total_word_feature_extractor.h>
#include <mitie/ner_feature_extraction.h>
#include <dlib/svm.h>
#include <dlib/vectorstream.h>
#include <dlib/hash.h>
#include <mitie/named_entity_extractor.h>

/**
 *  Namespace to use
 */
using namespace std;


Php::Value bubblesort(Php::Parameters &params)
{
    cout << "start bubblesort" << endl;
    
    // the array that was passed
    Php::Value array(params[0]);
    
    cout << "go return" << endl;
    
    return array;
    
    // size of the array
    int size = array.size();
    
    cout << "convert to native" << endl;
    
    int *x = new int[size];
    for (int i=0; i<size; i++) x[i] = array[i];
    
    cout << "converted" << endl;
    
    
    // outer loop
    for (int i=0; i<size; i++)
    {
        // left value
        int left = x[i];
//        cout << "left: " << left << endl;
        
        // inner loop
        for (int j=i+1; j<size; j++)
        {
            // right value
            int right = x[j];
            
            if (left < right) continue;
            
            // swap values
            x[i] = right;
            x[j] = left;
            left = right;
        }
    }
    
    cout << "algorithm end" << endl;
    
    Php::Value r;
    
    for (int i=0; i<size; i++) r[i] = x[i];
    
    
    delete[] x;
    
    // done
    return r;
}



/**
 *  Our own "my_plus" function that will be available in PHP
 *  @param  environment
 *  @param  params
 *  @return Value
 */
static Php::Value my_plus(Php::Parameters &params)
{
    Php::Value r(0);
    
    for (unsigned int i=0; i<params.size(); i++) r += params[i];
    
    return r;
    
    
//    int p1 = params[0];
//    int p2 = params[1];
//    return p1 + p2;
//    
//    cout << "global g1: " << env["g1"].stringValue() << endl;
//    cout << "global g2: " << env["g2"].stringValue() << endl;
//    
//    Php::Global g(env["g3"]);
//    
//    g = "zo kan het ook";
//    
//    string output = env.call("strtoupper","test in lowercase");
//    
//    cout << "output: " << output << endl;
//    
//    return p1 + p2;
}

/**
 *  Custom class that will be available in PHP
 */
class MyCustomClass : public Php::Base
{
private:
    int _x;
    
public:
    MyCustomClass()
    {
        _x = 3;
        cout << "MyCustomClass::MyCustomClass" << endl;
        cout << this << endl;
        cout << _x << endl;
    }
    
    virtual ~MyCustomClass()
    {
        cout << "MyCustomClass::~MyCustomClass" << endl;
    }

    virtual void __construct()
    {
        cout << "MyCustomClass::__construct" << endl;
    }

    virtual void __destruct()
    {
        cout << "MyCustomClass::__destruct" << endl;
    }
    
    void myMethod(Php::Parameters &params)
    {
        cout << "myMethod GETS CALLED!!!!" << endl;
        cout << this << endl;
        cout << _x << endl;
//        cout << "A: " << _properties["a"] << endl;
//        cout << "Z: " << _properties["z"] << endl;
    }
};

// symbols are exported according to the "C" language
extern "C"
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module() 
    { 
        // create extension
        static Php::Extension extension("MITIE","0.1");

        // define the functions
        extension.add<my_plus>("my_plus", {
            Php::ByVal("a", Php::Type::Numeric),
            Php::ByVal("b", Php::Type::Numeric),
            Php::ByVal("c", "MITIEClass"),
            Php::ByRef("d", Php::Type::String)
        });
        
        extension.add<bubblesort>("bubblesort");
        
        // define classes
        Php::Class<MyCustomClass> myCustomClass("my_class");
        myCustomClass.method<&MyCustomClass::myMethod>("mymethod");
        myCustomClass.method<&MyCustomClass::__construct>("__construct");
        
        // add to extension
        extension.add(myCustomClass);
        
        // return the module entry
        return extension.module();
    }
}

using namespace dlib;

namespace mitie
{

// ----------------------------------------------------------------------------------------

    named_entity_extractor::
    named_entity_extractor(
        const std::vector<std::string>& tag_name_strings_,
        const total_word_feature_extractor& fe_,
        const dlib::sequence_segmenter<ner_feature_extractor>& segmenter_,
        const dlib::multiclass_linear_decision_function<dlib::sparse_linear_kernel<ner_sample_type>,unsigned long>& df_
    ) : tag_name_strings(tag_name_strings_), fe(fe_), segmenter(segmenter_), df(df_)
    {
        // make sure the requirements are not violated.
        DLIB_CASSERT(df.number_of_classes() >= tag_name_strings.size(),"invalid inputs");
        DLIB_CASSERT(segmenter.get_feature_extractor().num_features() == fe.get_num_dimensions(),"invalid inputs");
        std::set<unsigned long> df_tags(df.get_labels().begin(), df.get_labels().end());
        for (unsigned long i = 0; i < tag_name_strings.size(); ++i)
        {
            DLIB_CASSERT(df_tags.count(i) == 1, "The classifier must be capable of predicting each possible tag as output.");
        }

        compute_fingerprint();
    }

    named_entity_extractor::
    named_entity_extractor(const std::string& pureModelName,
                           const std::string& extractorName
    ) {
        std::string classname;
        dlib::deserialize(pureModelName) >> classname;
        if (classname != "mitie::named_entity_extractor_pure_model")
            throw dlib::error(
                    "This file does not contain a mitie::named_entity_extractor_pure_model. Contained: " + classname);

        dlib::deserialize(pureModelName) >> classname >> df >> segmenter >> tag_name_strings;

        dlib::deserialize(extractorName) >> classname;
        if (classname != "mitie::total_word_feature_extractor")
            throw dlib::error(
                    "This file does not contain a mitie::total_word_feature_extractor. Contained: " + classname);

        dlib::deserialize(extractorName) >> classname >> fe;
    }
// ----------------------------------------------------------------------------------------

    void named_entity_extractor::
    predict (
        const std::vector<std::string>& sentence,
        std::vector<std::pair<unsigned long, unsigned long> >& chunks,
        std::vector<unsigned long>& chunk_tags,
        std::vector<double>& chunk_scores
    ) const
    {
        const std::vector<matrix<float,0,1> >& sent = sentence_to_feats(fe, sentence);
        segmenter.segment_sequence(sent, chunks);


        std::vector<std::pair<unsigned long, unsigned long> > final_chunks;
        final_chunks.reserve(chunks.size());
        chunk_tags.clear();
        chunk_scores.clear();
        // now label each chunk
        for (unsigned long j = 0; j < chunks.size(); ++j)
        {
            const std::pair<unsigned long, double> temp = df.predict(extract_ner_chunk_features(sentence, sent, chunks[j]));
            const unsigned long tag = temp.first;
            const double score = temp.second;

            // Only output this chunk if it is predicted to be an entity.  Recall that if
            // the classifier outputs a ID outside the range of our labels then it's
            // predicting "this isn't an entity at all".
            if (tag < tag_name_strings.size())
            {
                final_chunks.push_back(chunks[j]);
                chunk_tags.push_back(tag);
                chunk_scores.push_back(score);
            }
        }

        final_chunks.swap(chunks);
    }

// ----------------------------------------------------------------------------------------

    void named_entity_extractor::
    operator() (
        const std::vector<std::string>& sentence,
        std::vector<std::pair<unsigned long, unsigned long> >& chunks,
        std::vector<unsigned long>& chunk_tags
    ) const
    {
        const std::vector<matrix<float,0,1> >& sent = sentence_to_feats(fe, sentence);
        segmenter.segment_sequence(sent, chunks);


        std::vector<std::pair<unsigned long, unsigned long> > final_chunks;
        final_chunks.reserve(chunks.size());
        chunk_tags.clear();
        // now label each chunk
        for (unsigned long j = 0; j < chunks.size(); ++j)
        {
            const unsigned long tag = df(extract_ner_chunk_features(sentence, sent, chunks[j]));

            // Only output this chunk if it is predicted to be an entity.  Recall that if
            // the classifier outputs a ID outside the range of our labels then it's
            // predicting "this isn't an entity at all".
            if (tag < tag_name_strings.size())
            {
                final_chunks.push_back(chunks[j]);
                chunk_tags.push_back(tag);
            }
        }

        final_chunks.swap(chunks);
    }

// ----------------------------------------------------------------------------------------

}
