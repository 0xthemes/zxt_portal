<?php
/**
 * @file
 * zxt_terms.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function zxt_terms_taxonomy_default_vocabularies() {
  return array(
    'tc_browser_support' => array(
      'name' => 'Browser Support',
      'machine_name' => 'tc_browser_support',
      'description' => 'List of browsers',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'tc_categories' => array(
      'name' => 'Categories',
      'machine_name' => 'tc_categories',
      'description' => 'Categories',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'tc_compatible_with' => array(
      'name' => 'Compatible With',
      'machine_name' => 'tc_compatible_with',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'tc_files_included' => array(
      'name' => 'Files Included',
      'machine_name' => 'tc_files_included',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'tc_frameworks' => array(
      'name' => 'Frameworks',
      'machine_name' => 'tc_frameworks',
      'description' => 'List of all the frameworks',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'tc_layout' => array(
      'name' => 'Layout',
      'machine_name' => 'tc_layout',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'tc_platform' => array(
      'name' => 'Platform',
      'machine_name' => 'tc_platform',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'tc_properties' => array(
      'name' => 'Properties',
      'machine_name' => 'tc_properties',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'tc_tags' => array(
      'name' => 'Tags',
      'machine_name' => 'tc_tags',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'userpoints' => array(
      'name' => 'Userpoints',
      'machine_name' => 'userpoints',
      'description' => 'Automatically created by the userpoints module',
      'hierarchy' => 0,
      'module' => 'userpoints',
      'weight' => 0,
    ),
  );
}
