<?php
/**
 * @file
 * field_validation_rule.eth_address_length.inc
 */

$api = '2.0.0';

$data = $rule = new stdClass();
$rule->disabled = FALSE; /* Edit this to true to make a default rule disabled initially */
$rule->api_version = 2;
$rule->rulename = 'Ether address length';
$rule->name = 'eth_address_length';
$rule->field_name = 'field_tc_ethereum_address';
$rule->col = 'value';
$rule->entity_type = 'user';
$rule->bundle = 'user';
$rule->validator = 'field_validation_length_validator';
$rule->settings = array(
  'min' => '42',
  'max' => '42',
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
$rule->error_message = 'Proper ether address should starts with 0x and contains 42 characters';


$dependencies = array();

$optional = array();

$modules = array(
  0 => 'ctools',
  1 => 'field_validation',
);
