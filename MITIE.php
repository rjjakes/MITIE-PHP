<?php

/**
 * @file
 * Example usage.
 */

// Initialise the named entitiy recognition class by supplying a model data file:
// An example data file can be downloaded here:
// https://github.com/mit-nlp/MITIE/releases/download/v0.4/MITIE-models-v0.2.tar.bz2
$ner = new MITIENer();

// Load the model file and build the types of tags this model file can output
$ner->loadModel("MITIE/MITIE-models/english/ner_model.dat");
var_dump($ner->getTags());

// Perform the extraction. This will populate the entities object inside the class.
$ner->extraction();
var_dump($ner->getEntities());

/*
$entities = array(
  array(
    'tag_id' => 0,
    'score' => 0.634,
    'tag' => 'ORGANISATION',
    'token' => 'Kew Gardens'
  ),
  array(
    'tag_id' => 2,
    'score' => 0.134,
    'tag' => 'PERSON',
    'token' => 'John Doe'
  ),
);
*/






