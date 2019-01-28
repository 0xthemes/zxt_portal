<?php
/**
 * @file
 * field_validation_rule.body_min_words.inc
 */

$api = '2.0.0';

$data = $rule = new stdClass();
$rule->disabled = TRUE; /* Edit this to true to make a default rule disabled initially */
$rule->api_version = 2;
$rule->rulename = 'Body Min words';
$rule->name = 'body_min_words';
$rule->field_name = 'body';
$rule->col = 'value';
$rule->entity_type = 'node';
$rule->bundle = 'page';
$rule->validator = 'field_validation_words_validator';
$rule->settings = array(
  'min' => '2',
  'max' => '',
);
$rule->error_message = 'You should enter at least two words.';


$dependencies = array();

$optional = array();

$modules = array(
  0 => 'ctools',
  1 => 'field_validation',
);
