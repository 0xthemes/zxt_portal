<?php
/**
 * @file
 * field_validation_rule.unique_value.inc
 */

$api = '2.0.0';

$data = $rule = new stdClass();
$rule->disabled = FALSE; /* Edit this to true to make a default rule disabled initially */
$rule->api_version = 2;
$rule->rulename = 'Unique Value';
$rule->name = 'unique_value';
$rule->field_name = 'field_tc_ethereum_address';
$rule->col = 'value';
$rule->entity_type = 'user';
$rule->bundle = 'user';
$rule->validator = 'field_validation_unique_validator';
$rule->settings = array(
  'data' => 'global',
  'per_user' => 0,
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
$rule->error_message = 'This address is already entered by someone else';


$dependencies = array();

$optional = array();

$modules = array(
  0 => 'ctools',
  1 => 'field_validation',
);
