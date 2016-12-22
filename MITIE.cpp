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

// Namespaces to use
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
//  Define the NER class
//
class MITIENer : public Php::Base
{
private:
    string _classname;
    named_entity_extractor _ner;
    std::vector<string> _tagstr;
    std::vector<string> _tokens;
    std::vector<pair<unsigned long, unsigned long> > _chunks;
    std::vector<unsigned long> _chunk_tags;
    std::vector<double> _chunk_scores;


public:
    MITIENer()
    {
//        cout << this << endl;
//        cout << _x << endl;
    }
    
    virtual ~MITIENer()
    {
    }

    virtual void __construct()
    {
    }

    virtual void __destruct()
    {
    }

    void loadModel(Php::Parameters &params)
    {
        // Check the corretc number of parameters have been passed
        if (params.size() != 1)
        {
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

        // Print out what kind of tags this tagger can predict.
        _tagstr = _ner.get_tag_name_strings();

    }

    Php::Value getTags()
    {
        // @todo - check this array is set
        return _tagstr;
    }


    void extraction(Php::Parameters &params)
    {
        if (params.size() != 1)
        {
            cout << "extraction() must contain the path to a text file." << endl;
            return;
        }

        // Before we can try out the tagger we need to load some data.
        _tokens = tokenize_file(params[0]);


        // Now detect all the entities in the text file we loaded and print them to the screen.
        // The output of this function is a set of "chunks" of tokens, each a named entity.
        // Additionally, if it is useful for your application a confidence score for each "chunk"
        // is available by using the predict() method.  The larger the score the more
        // confident MITIE is in the tag.
        _ner.predict(_tokens, _chunks, _chunk_tags, _chunk_scores);

    }

    Php::Value getEntities()
    {
        Php::Array entities;

        // @todo - check input variables exists
        for (unsigned int i = 0; i < _chunks.size(); ++i)
        {
            Php::Array entity;
            std::string tokens = "";

            entity["tag_id"] = (int)_chunk_tags[i];
            entity["score"] = (float)_chunk_scores[i];
            entity["tag"] = _tagstr[_chunk_tags[i]];

            // _chunks[i] defines a half open range in tokens that contains the entity.
            for (unsigned long j = _chunks[i].first; j < _chunks[i].second; ++j)
            {
                if (j != _chunks[i].first)
                    tokens += " ";

                tokens += (string)_tokens[j];
            }

            entity["tokens"] = tokens;

            // Add this entity to the enitites array
            entities[i] = entity;
        }

        return entities;
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

        // define classes
        Php::Class<MITIENer> mITIENer("MITIENer");
        mITIENer.method<&MITIENer::getEntities>("getEntities");
        mITIENer.method<&MITIENer::getTags>("getTags");
        mITIENer.method<&MITIENer::loadModel>("loadModel");
        mITIENer.method<&MITIENer::extraction>("extraction");
        mITIENer.method<&MITIENer::__construct>("__construct");
        
        // add to extension
        extension.add(mITIENer);
        
        // return the module entry
        return extension.module();
    }
}

