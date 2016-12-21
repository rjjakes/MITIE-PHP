<?php

/**
 * @file
 * Example usage.
 */

// Initialise the named entitiy recognition class by supplying a model data file:
// An example data file can be downloaded here:
// https://github.com/mit-nlp/MITIE/releases/download/v0.4/MITIE-models-v0.2.tar.bz2
$ner = new MITIENer();

// Load the model file and fet the types of tags this model file can output
$tags = $ner->loadModel("MITIE/MITIE-models/english/ner_model.dat");
//var_dump($tags);

// Perform the extraction. This will populate some variables and arrays inside the class.
// $ner->extraction();

  // Get the results.
//  $entities = $ner->getEntities();
//  var_dump($entities);

//}




