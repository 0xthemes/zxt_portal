<?php
/**
 * @file
 * field_validation_rule.ether_regex.inc
 */

$api = '2.0.0';

$data = $rule = new stdClass();
$rule->disabled = FALSE; /* Edit this to true to make a default rule disabled initially */
$rule->api_version = 2;
$rule->rulename = 'Ether Regex';
$rule->name = 'ether_regex';
$rule->field_name = 'field_tc_ethereum_address';
$rule->col = 'value';
$rule->entity_type = 'user';
$rule->bundle = 'user';
$rule->validator = 'field_validation_pcre_validator';
$rule->settings = array(
  'data' => '/^(0x)?[0-9a-f]{40}$/i',
  'bypass' => 0,
  'roles' => array(
    1 => 0,
    2 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
  ),
  'errors' => 1,
  'condition' => 0,
  'condition_wrapper' => array(
    'condition_field' => '',
    'condition_operator' => 'equals',
    'condition_value' => '',
  ),
);
$rule->error_message = 'Not a Hexa format';


$dependencies = array();

$optional = array();

$modules = array(
  0 => 'ctools',
  1 => 'field_validation',
);
