<?php

//get the weight of the term
function zxt_helper_db_single_table_query($table, $fields = NULL, $conditions = NULL, $orderby = NULL, $limit = 0, $fetch = 'all', $fetch_key = '') {
  if(!db_table_exists($table)) {
    return FALSE;
  }

  $query = db_select($table, 'c');
  //Fields
  if($fields) {
    $query->fields('c', $fields);
  }
  else {
    $query->fields('c');
  }

  //Conditions
  if($conditions && count($conditions)) {
    foreach($conditions as $_column => $condition) {
      if(is_array($condition)) {
        $query->condition($_column, $condition['value'], $condition['op']);
      }
      else {
        $query->condition($_column, $condition);
      }
    }
  }

  if($orderby && count($orderby)) {
    foreach($orderby as $_field => $_sort) {
      $query->orderBy($_field, $_sort);//ORDER BY created
    }
  }

  //limits
  if($limit) {
    $query->range(0,$limit);
  }

  $result = $query->execute();
  
  switch($fetch) {
    case 'fetchAllAssoc';
      if($fetch_key) {
        $output = $result->fetchAllAssoc($fetch_key);
        return $output;
      }
    break;
    case 'all';
      return $result->fetchAll();
    break;
    default:
      return $result->fetchAll();
  }
  return $result->fetchAll();
}


//get the weight of the term
function _bet_query_term_weight($tid) {
  return db_query("SELECT weight FROM {taxonomy_term_data} WHERE tid = :tid", array(':tid' => $tid))->fetchField();
}


//get tid and return the term name
function _bet_query_tid2name($tid){
  return db_query("SELECT name FROM {taxonomy_term_data} WHERE tid = :tid", array(':tid' => $tid))->fetchField();
}

//Gets the depth of the term id
function _bet_query_term_depth($tid) {
  $parent = db_query("SELECT parent FROM {taxonomy_term_hierarchy} WHERE tid = :tid", array(':tid' => $tid))->fetchField();
  if($parent == 0) {
    return 1;
  }else  {
    return 1+_bet_query_term_depth($parent);
  }
}

//give id, entity type, bundle, and field name, get double field as array
function _bet_query_get_doublefield( $entityids, $fieldname, $entitytype, $bundle){
  $table = 'field_data_'.$fieldname;
  $first_field = $fieldname.'_first';
  $second_field = $fieldname.'_second';
  $query = db_select($table, 'c');
  //SELECT the fields from node
  $query->fields('c',array($first_field, $second_field, 'entity_id'))
    ->condition('entity_type ', $entitytype)
    ->condition('bundle ', $bundle)
    ->condition('entity_id ', $entityids)
    ->range(0,1000);//LIMIT to 2 records
  $result = $query->execute();
  $output = array();
  $count = 0;
  while($record = $result->fetchAssoc()) {
    $eid = $record['entity_id'];
    $output[$count]['eid'] = $record['entity_id'];
    $output[$count]['1'] = $record[$first_field];
    $output[$count]['2'] = $record[$second_field];
    $count++;
  }
  return $output;
}

//convert the string to maching name
function _bet_query_tomachine_name($string, $seperator = '_') {
  return preg_replace('@[^a-z0-9_]+@',$seperator, strtolower($string));
}

//generate random string
function _bet_query_generateRandomString($length = 10) {
    $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $charactersLength = strlen($characters);
    $randomString = '';
    for ($i = 0; $i < $length; $i++) {
        $randomString .= $characters[rand(0, $charactersLength - 1)];
    }
    return $randomString;
}


//Query a field value by entity type, bundle and id
//@todo how to accomidate the deltas
function _bet_query_get_field( $ids, $entitytype = 'node',
  $bundle = 'bet_templates', $fieldname = 'field_bet_css',
  $keyed = 0, $fields = FALSE,
  $sort_by_ids = TRUE){
  //There must be some ID
  if(empty ($ids)) {
    return false;
  }
  //It should be array
  if(!is_array($ids)) {
    $ids = array($ids);
  }
  //List of fields
  if(!$fields) {
    $fields = array('_value');
  }
  //Count them
  $nof_fields = count($fields);
  $nof_entities = count($ids);

  $table = 'field_data_'.$fieldname;
  if(!db_table_exists($table)) {
    return FALSE;
  }

  $query = db_select($table, 'c');

  //SELECT the fields from node
  $query_fields = array();
  foreach($fields as $field) {
    if (strpos($field, '_') === 0) {
      $query_fields[] = $fieldname . $field;
    }
    else {
      $query_fields[] = $field;
    }
  }
  if($keyed) {
    $query_fields[] = 'entity_id';
  }

  $query->fields('c', $query_fields)
    ->condition('entity_type ', $entitytype)
    ->condition('bundle ', $bundle)
    ->condition('entity_id ', $ids, 'IN');


  if($sort_by_ids && $nof_entities > 1) {
    $sort_string = 'FIELD(entity_id, ' . implode(',', $ids) . ')';
    $query->orderBy($sort_string, 'ASC');
  }

  $query->range(0,100);//LIMIT to 2 records

  $result = $query->execute();
  $output = array();
  $index = 0;
  while($record = $result->fetchAssoc()) {
    if($keyed) {
      $index = $record['entity_id'];
    }
    else {
      $index++;
    }
    if($nof_fields == 1) {
      $output[$index] = $record[$query_fields[0]];
    }
    else {
      $temp_row = array();
      foreach($query_fields as $field) {
        $temp_row[$field] = $record[$field];
      }
      $output[] = $temp_row;
    }
  }
  return $output;
}


//no use for now
function q2_inc_gettermparent($tid) {
  $query = db_select('taxonomy_term_hierarchy', 't');
  //SELECT the fields from node
  $query->fields('t',array('parent'))
    ->condition('tid', $tid)
    ->range(0,100);
  $result = $query->execute();
  $output = array();
  while($record = $result->fetchAssoc()) {
    $output[] = $record['parent'];
  }
  return $output;
}

function q2_inc_getalltermparents($tid) {
  $return = array();
  while(1) {
    $current_parents = q2_inc_gettermparent($tid);
    if(count($current_parents)) {
      foreach($current_parents as $parent) {
          $return[] = $tid = $parent;
      }
    }
    else {
      break;
    }
  }
  return $return;

}


//Get all the asset arrays based on the arguments
//Assets are field collections linked to the libraries
//asset_type : js, css, etc, can be array, its file type in drupal terms
//media_type : code = 1, file = 2 , cdn = 3
//TODO : Make the array more better, make sure ut comes in correct order
//returns array
function q2_inc_getlib2assets($libid = 40, $asset_type = array(20,39), $media_type = 1) {
  $library = taxonomy_term_load($libid);
  if(isset($library->field_assets['und']['0'])) {
    $assets =  $library->field_assets['und'];
  }
  else {
    return FALSE;
  }

  $fc = array();
  $count = 0;

  foreach ($assets as $asset ) {
    $assetid = $asset['value'];//field_collection_field_get_entity($assetID);
    //get the file type
    $asset_typeIDs = q2_inc_gettermreferences('field_data_field_file_type','field_collection_item','field_assets',$asset['value'],'field_file_type_tid' );
    //if type =  js or css
    if(in_array($asset_typeIDs[0], $asset_type)) {
      $code = q2_inc_getcodes($assetid, 'field_collection_item', 'field_assets');
      $fc[$count]['code'] = isset($code[0]) ? $code[0] : NULL;
      $fc[$count]['filetype'] = $asset_typeIDs[0];
      $filename = q2_inc_get_text($assetid,'field_collection_item',  'field_assets', 'field_file_path');
      $fc[$count]['filename'] = $filename[0];
      $cdn = q2_inc_get_link($assetid,'field_collection_item',  'field_assets','field_link');
      $fc[$count]['cdn'] = isset($cdn[0]) ? $cdn[0] : NULL;
      $fc[$count]['assetid'] = $assetid;
      $count++;
    }
    // get the css cods and file as array and return it
  }
  return $fc;
  // get all assets and filter it by asset type
  // get the code for each code field

}

//get libraries and return all the assets and
//storage type is field / file
//wont save files
//unset the 'code' to keep the array lean
//todo : return full array

function q2_inc_assets4libs($libs,$asset_type = array(20,39), $media_type = 1, $storage = 1) {
  $assets = array();
  foreach($libs as $lib) {
    $tempassets = q2_inc_getlib2assets($lib,$asset_type,$media_type);
    if(!empty ($tempassets)) {
      foreach($tempassets as &$tempasset) {
        $assets[] = $tempasset;
        unset($tempasset['code']);
      }
    }
  }
  return $assets;
}

//Get the content of the code field
//todo : filter by file types
function q2_inc_getcodes( $ids, $entitytype = 'field_collection_item', $bundle = 'field_theme_files', $file_types = array()){
  if (!$ids) {
    return false;

  }

  //var_dump(debug_backtrace());
  $query = db_select('field_data_field_code', 'c');
  //SELECT the fields from node
  $query->fields('c',array('field_code_value','entity_id'))
    ->condition('entity_type ', $entitytype)
    ->condition('bundle ', $bundle)
    ->condition('entity_id ', $ids)
    ->range(0,100);//LIMIT to 2 records
  $result = $query->execute();
  $output = array();
  while($record = $result->fetchAssoc()) {
    $output[] = $record['field_code_value'];
  }
  return $output;
}

//Get text field data for any entity
//Get id, entity type, bundle, and field name
function q2_inc_get_text( $ids, $entitytype = 'field_collection_item',
$bundle = 'field_assets', $fieldname = 'field_file_path' , $keyed = 0){
  if(empty ($ids)) {
    return false;
  }
  $table = 'field_data_'.$fieldname;
  $query = db_select($table, 'c');
  //SELECT the fields from node
  $query->fields('c',array($fieldname.'_value' , 'entity_id'))
    ->condition('entity_type ', $entitytype)
    ->condition('bundle ', $bundle)
    ->condition('entity_id ', $ids)
    ->range(0,100);//LIMIT to 2 records
  $result = $query->execute();
  $output = array();
  while($record = $result->fetchAssoc()) {
    if($keyed) {
      $output[$record['entity_id']] = $record[$fieldname.'_value'];
    }
    else {
      $output[] = $record[$fieldname.'_value'];
    }
  }
  return $output;
}

//get term reference from db
function q2_inc_gettermreferences($table, $entity_type, $bundle, $entity_id, $field_name ){
  if(empty($entity_id)) {
    return FALSE;
  }
  $query = db_select($table, 'c');
  //SELECT the fields from node
  $query->fields('c',array($field_name))
    ->condition('entity_type ', $entity_type)
    ->condition('bundle ', $bundle)
    ->condition('entity_id ', $entity_id)
    ->range(0,1000);//LIMIT to 2 records
  $result = $query->execute();
  $output = array();
  while($record = $result->fetchAssoc()) {
    $output[] = $record[$field_name];
  }
  return $output;
}




//Get list of libraries TIDs and gives ordered with dependency
function q2_inc_lib_order($libraries = array()) {
  $tids = array();
  foreach($libraries as $library) {
    $tids[] = $library;
    $temptids = q2_inc_getalltermparents($library);
    foreach ($temptids as $temptid){
      if($temptid) {
        $tids[] = $temptid;
      }
    }
  }
  $array = array();
  foreach ($tids as $tid) {
    $depth = q2_inc_term_depth($tid);
    $weight = q2_inc_term_weight($tid);
    $array[$depth][$weight][] = $tid;
  }
  ksort($array);
  foreach($array as &$subarray) {
    ksort($subarray);
  }
  $output = array();
  foreach ($array as $weight) {
    foreach ($weight as $order) {
      foreach ($order as $value) {
        $output[] = $value;
      }
    }
  }
  return array_unique($output);
}




//Get link field data for any entity
//field_data_field_link
//give id, entity type, bundle, and field name
function q2_inc_get_link( $ids, $entitytype = 'node', $bundle = 'bet_currency', $fieldname = 'field_bet_cur_links'){
  $table = 'field_data_'.$fieldname;
  $query = db_select($table, 'c');
  //SELECT the fields from node
  $query->fields('c')
    ->condition('entity_type ', $entitytype)
    ->condition('bundle ', $bundle)
    ->condition('entity_id ', $ids)
    ->range(0,100);//LIMIT to 2 records
  $result = $query->execute();
  $output = array();
  while($record = $result->fetchAssoc()) {
    $temp_row = array(
      'id' => $record['entity_id'],
      'delta' => $record['delta'],
      'url' => $record[$fieldname . '_url'],
      'title' => $record[$fieldname . '_title'],
      'attributes' => unserialize($record[$fieldname . '_attributes']),
    );
    $output[] = $temp_row;
  }
  return $output;
}

//get single or multiple files from db
function q2_inc_get_files($table, $entity_type, $bundle, $entity_id, $field_name = "field_files_fid", $keyed = 0  ){
  if(empty($entity_id)) {
    return FALSE;
  }
  //send key/value array of $keyed is present
  if ($keyed) {
    $query = db_select($table, 'c');
    //SELECT the fields from node
    $query->fields('c',array($field_name, 'entity_id'))
      ->condition('entity_type ', $entity_type)
      ->condition('bundle ', $bundle)
      ->condition('entity_id ', $entity_id)
      ->range(0,1000);//LIMIT to 2 records
    $result = $query->execute();
    $output = array();
    while($record = $result->fetchAssoc()) {
      $output[$record['entity_id']] = $record[$field_name];
    }
    return $output;
  }
  //associative array of keyed is 0
  else {
    $query = db_select($table, 'c');
    //SELECT the fields from node
    $query->fields('c',array($field_name))
      ->condition('entity_type ', $entity_type)
      ->condition('bundle ', $bundle)
      ->condition('entity_id ', $entity_id)
      ->range(0,1000);//LIMIT to 2 records
    $result = $query->execute();
    $output = array();
    while($record = $result->fetchAssoc()) {
      $output[] = $record[$field_name];
    }
    return $output;
  }
}


//Get field collections id from the nodes
function q2_inc_get_fcs( $ids, $table = "field_data_field_theme_files", $entitytype = 'node', $bundle = 'style', $field_name = "field_theme_files_value", $keyed = 0) {
  if(empty($ids)) {
    return FALSE;
  }
  if(!is_array($ids) ) {
    $ids = array($ids);
  }
  $output = array();
  foreach ($ids as $id) { //make sure the FCs are on the same order as design
    $query = db_select($table, 'c');
    //SELECT the fields from node
    $query->fields('c',array($field_name, 'entity_id'))
      ->condition('entity_type ', $entitytype)
      ->condition('bundle ', $bundle)
      ->condition('entity_id ', $id)
      ->range(0,1000);//LIMIT to X records
    $result = $query->execute();

    while($record = $result->fetchAssoc()) {
      if($keyed) {
        $output[$record['entity_id']][] = $record[$field_name];
      }
      else {
        $output[] = $record[$field_name];
      }
    }
  }
  return $output;
}




//get the designs and return the array for machine and human readable name
//return the region id, machine name, human readable name
//make sure you have multiple machine names together
function q2_inc_design_machinenames($designs){
  $return = array();

  $query = db_select('node', 'n');

  $query->join('field_data_field_style_name', 'm', 'n.nid = m.entity_id');
  $query->join('field_data_field_plugin', 'p', 'n.nid = p.entity_id');
  $query->condition('n.nid', $designs);
  $result = $query
  ->fields('n', array('nid', 'title'))
  ->fields('m', array('field_style_name_value'))
  ->fields('p', array('field_plugin_tid'))
  ->execute();
  $output = array();
  foreach ($result as $row) {
    $output[] = $row;
  }
  return $output;

}

//Table file upload
//get the fcids, and filetype ids
//return only the fcids which contains these filetype id
function q2_inc_filter_fc_by_filetype($fcids, $filetypes) {
  if(!empty($fcids)) {
    $output = array();
    if(!is_array($fcids) ) {
      $fcids = array($fcids);
    }
    foreach ($fcids as $fcid) {
      $query = db_select('field_data_field_file_type', 'c');
      //SELECT the fields from node
      $query->fields('c',array('entity_id'))
        ->condition('entity_id ', $fcid)
        ->condition('field_file_type_tid ', $filetypes)
        ->range(0,1000);//LIMIT to X records
      $result = $query->execute();
      while($record = $result->fetchAssoc()) {
        $output[] = $record['entity_id'];
      }
    }
    return $output;
  }
  return false;

}

//get the fcids, and platform ids
//return only the fcids which contains these platform id
//todo, keep the order of FCid
//  for each fc id, check the platform, if
function q2_inc_filter_fc_by_platform($fcids, $platform) {
  if(!empty($fcids)) {
    $output = array();
    if(!is_array($fcids) ) {
      $fcids = array($fcids);
    }
    foreach($fcids as $fcid) {
      $query = db_select('field_data_field_platform', 'c');
      //SELECT the fields from node
      $query->fields('c',array('entity_id'))
        ->condition('entity_id ', $fcid)
        ->condition('field_platform_tid ', $platform)
        ->range(0,10000);//LIMIT to X records
      $result = $query->execute();

      while($record = $result->fetchAssoc()) {
        $output[] = $record['entity_id'];
      }
    }
    return $output;
  }
  return false;
}





//get the file IDs and field name and return the description of it
function q2_inc_get_files_description($fileid , $field_name = 'field_files'){
  $table = 'field_data_'.$field_name;
  $query = db_select($table, 'c');
  //SELECT the fields from node
  $field_file_id = $field_name.'_fid';
  $field_description = $field_name.'_description';
  $query->fields('c',array($field_file_id, $field_description , 'entity_id'))
    ->condition($field_file_id, $fileid)
    ->range(0,1000);//LIMIT to 2 records
  $result = $query->execute();
  $output = array();
  while($record = $result->fetchAssoc()) {
    $output[$record[$field_file_id]] = $record[$field_description] ;
  }
  return $output;
}




// get the project array and plugin id and return true if part of it.
// module = header, footer etc
function _d7_inc_isplugin_in_project($project, $module) {
  foreach($project as $design) {
    $plugin = q2_inc_gettermreferences("field_data_field_plugin", "node", "style", $design,  "field_plugin_tid");
    if($plugin[0] == $module) {
      return true;
    }
    //get the module of the plug
    //compare with the module
    //return tre

  }
  return false;

}
