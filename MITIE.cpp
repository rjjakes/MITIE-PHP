/**
 *  MITIE.h (MITIE-PHP Project)
 *
 *  An extension that bridges the MITIE information extraction library with PHP
 *  Most of the functionality here is based on the C++ examples in: MITIE/examples/cpp/
 *
 *  @author: https://github.com/rjjakes/
 */
#include <string>
#include <iostream>
#include <phpcpp.h>
#include <mitie.h>
#include <mitie/named_entity_extractor.h>
#include <mitie/conll_tokenizer.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>

/**
 *  Namespace to use
 */
using namespace std;
using namespace mitie;

// ----------------------------------------------------------------------------------------
// NER helper function.
std::vector<string> tokenize_file (
    const string& filename
)
{
    ifstream fin(filename.c_str());
    if (!fin)
    {
        cout << "Unable to load input text file" << endl;
        exit(EXIT_FAILURE);
    }
    // The conll_tokenizer splits the contents of an istream into a bunch of words and is
    // MITIE's default tokenization method.
    conll_tokenizer tok(fin);
    std::vector<string> tokens;
    string token;
    // Read the tokens out of the file one at a time and store into tokens.
    while(tok(token))
        tokens.push_back(token);

    return tokens;
}

// ----------------------------------------------------------------------------------------
/*
void do_MITIE()
{

    cout << "C++ code!" << endl;

    try
    {
        if (1)  //argc != 3)
        {
////            printf("You must give a MITIE ner model file as the first command line argument\n");
////            printf("followed by a text file to process.\n");
////            return EXIT_FAILURE;
        }



////        return EXIT_SUCCESS;
    }
    catch (std::exception& e)
    {
        cout << e.what() << endl;
////        return EXIT_FAILURE;
    }


}
*/


// ----------------------------------------------------------------------------------------
// Demo function placeholder
 /*
static Php::Value my_plus(Php::Parameters &params)
{
    Php::Value r(0);
    
    for (unsigned int i=0; i<params.size(); i++) r += params[i];
    
    return r;
}
*/

// ----------------------------------------------------------------------------------------
//  Define the NER class
//
class MITIENer : public Php::Base
{
private:
    string _classname;
    named_entity_extractor _ner;
    const std::vector<string> _tagstr

public:
    MITIENer()
    {
//        cout << this << endl;
//        cout << _x << endl;
    }
    
    virtual ~MITIENer()
    {
//        cout << "MITIENer::~MITIENer" << endl;
    }

    virtual void __construct()
    {
    }

    virtual void __destruct()
    {
//        cout << "MITIENer::__destruct" << endl;
    }

    void loadModel(Php::Parameters &params)
    {
        // Check the corretc number of parameters have been passed
        if (params.size() != 1) {
            cout << "loadModel must contain the path to a NER model file." << endl;
            return;
        }

        // Load the model file and setup the class ready for extraction

        // Load MITIE's named entity extractor from disk.  Each file in the MITIE-models
        // folder begins with a string containing the name of the serialized class.  In
        // this case classname contains "mitie::named_entity_extractor".  It can be used to
        // identify what is in any particular file.  However, in this example we don't need
        // it so it is just ignored.
        dlib::deserialize(params[0]) >> _classname >> _ner;

        cout << "Loaded NER data file." << endl;

        // Print out what kind of tags this tagger can predict.
        _tagstr = _ner.get_tag_name_strings();
        cout << "The tagger supports "<< _tagstr.size() <<" tags:" << endl;
        for (unsigned int i = 0; i < _tagstr.size(); ++i)
            cout << "   " << _tagstr[i] << endl;

    }

    void extraction(Php::Parameters &params)
    {
        // Before we can try out the tagger we need to load some data.
        std::vector<string> tokens = tokenize_file("MITIE/sample_text.txt");

        std::vector<pair<unsigned long, unsigned long> > chunks;
        std::vector<unsigned long> chunk_tags;
        std::vector<double> chunk_scores;

        // Now detect all the entities in the text file we loaded and print them to the screen.
        // The output of this function is a set of "chunks" of tokens, each a named entity.
        // Additionally, if it is useful for your application a confidence score for each "chunk"
        // is available by using the predict() method.  The larger the score the more
        // confident MITIE is in the tag.
        _ner.predict(tokens, chunks, chunk_tags, chunk_scores);

        // If a confidence score is not necessary for your application you can detect entities
        // using the operator() method as shown in the following line.
        //ner(tokens, chunks, chunk_tags);

        cout << "\nNumber of named entities detected: " << chunks.size() << endl;
        for (unsigned int i = 0; i < chunks.size(); ++i)
        {
            cout << "   Tag " << chunk_tags[i] << ": ";
            cout << "Score: " << fixed << setprecision(3) << chunk_scores[i] << ": ";
            cout << _tagstr[chunk_tags[i]] << ": ";
            // chunks[i] defines a half open range in tokens that contains the entity.
            for (unsigned long j = chunks[i].first; j < chunks[i].second; ++j)
                cout << tokens[j] << " ";
            cout << endl;
        }

        cout << this << endl;
    }
};

/**
 * Export classes via PHP-CPP
 */
// symbols are exported according to the "C" language
extern "C"
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module() 
    { 
        // create extension
        static Php::Extension extension("MITIE","0.1");

        // define the functions
        /*
        extension.add<my_plus>("my_plus", {
            Php::ByVal("a", Php::Type::Numeric),
            Php::ByVal("b", Php::Type::Numeric),
            Php::ByVal("c", "MITIENer"),
            Php::ByRef("d", Php::Type::String)
        });
        */
        
        // add function to extension
//        extension.add<bubblesort>("bubblesort");
//        extension.add<do_MITIE>("do_MITIE");

        // define classes
        Php::Class<MITIENer> mITIENer("MITIENer");
        mITIENer.method<&MITIENer::loadModel>("loadModel");
        mITIENer.method<&MITIENer::extraction>("extraction");
        mITIENer.method<&MITIENer::__construct>("__construct");
        
        // add to extension
        extension.add(mITIENer);
        
        // return the module entry
        return extension.module();
    }
}

