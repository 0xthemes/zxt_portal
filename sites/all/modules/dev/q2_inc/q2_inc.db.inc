<?php

//get the weight of the term
function q2_inc_term_weight($tid) {
  $weight = db_query("SELECT weight FROM {taxonomy_term_data} WHERE tid = :tid", array(':tid' => $tid))->fetchField();
  return $weight;
}	


//get tid and return the term name
function q2_inc_tid2name($tid){
  $name = db_query("SELECT name FROM {taxonomy_term_data} WHERE tid = :tid", array(':tid' => $tid))->fetchField();
  return $name;
}

//get main path and array of folders to create inside
function q2_inc_create_multiple_dirs($main_path, array $dirs) {
  foreach($dirs as $dir) {
    $project_path = $main_path.'/'.$dir;
    if(!file_prepare_directory($project_path )) {
      drupal_mkdir($project_path ,0755, TRUE); 
    }
  }


}

//copy the whole directory
function q2_inc_dircopy($source, $dest, $permissions = 0755)
{
    // Check for symlinks
    if (is_link($source)) {
        return symlink(readlink($source), $dest);
    }

    // Simple copy for a file
    if (is_file($source)) {
        return copy($source, $dest);
    }

    // Make destination directory
    if (!is_dir($dest)) {
        mkdir($dest, $permissions);
    }

    // Loop through the folder
    $dir = dir($source);
    while (false !== $entry = $dir->read()) {
        // Skip pointers
        if ($entry == '.' || $entry == '..') {
            continue;
        }

        // Deep copy directories
        q2_inc_dircopy("$source/$entry", "$dest/$entry", $permissions);
    }

    // Clean up
    $dir->close();
    return true;
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
  return   $return;
  
}

//Gets the depth of the term id
function q2_inc_term_depth($tid) {
  $parent = db_query("SELECT parent FROM {taxonomy_term_hierarchy} WHERE tid = :tid", array(':tid' => $tid))->fetchField();
  if($parent == 0) {
    return 1;
  }else  {
    return 1+q2_inc_term_depth($parent);
  }
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

//get the design and gives you screenshots folder
//todo, take file_load and pass as array.
function q2_inc_merge_images(array $images, $path, $destimg = "ada.adad.jpeg") {
  $height = 0;
  $width = 0;
  $count = count($images);
  foreach($images as $image) {
    $height = $height + $image['height'];
  }
  $dst_im  = imagecreatetruecolor($image['width'], $height);
  $final_image = drupal_realpath(file_create_filename($destimg, $path));
  $height = 0;
  foreach($images as $image) {
    $src_im = imagecreatefromjpeg (drupal_realpath($image['uri']));
    imagecopy ($dst_im , $src_im , 0 , $height , 0 , 0 , $image['width'] , $image['height'] )  ;
    $height = $height + $image['height'];
    }
  if(imagejpeg ($dst_im , $final_image, 100 )) {
   return $final_image;
  }
}

function q2_inc_imagefid_to_imagearray($fids){
  $images = array();
  foreach($fids as $fid) {
    $images[] = file_load($fid);
  }
  return $images;
}

//get the design and collect all the css files and move to folder
function q2_inc_design2cssfiles(array $design) {
   //get all 3rd part css files
   //combain 
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

//Creates unique project folder for development 
function q2_inc_project_folder($name, $pid = 0, $uid = 0, $machine_name = '', $type = 'html') {
  //$uniquename = q2_inc_tomachine_name($name).'_'.$type.'_'.time().'_'.q2_inc_generateRandomString(10);
  //$uniquename = q2_inc_tomachine_name($name).'_'.$type;
  //uid-project-id-platform-project-name
  
  if(!trim($machine_name)) {
    $machine_name = q2_inc_tomachine_name($name);
  }
  return $uid.'-'.$pid.'-'.$type.'-'.$machine_name;
  
}

//generate random string
function q2_inc_generateRandomString($length = 10) {
    $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $charactersLength = strlen($characters);
    $randomString = '';
    for ($i = 0; $i < $length; $i++) {
        $randomString .= $characters[rand(0, $charactersLength - 1)];
    }
    return $randomString;
}


//Get link field data for any entity
//field_data_field_link
//give id, entity type, bundle, and field name 
function q2_inc_get_link( $ids, $entitytype = 'field_collection_item', 
$bundle = 'field_assets', $fieldname = 'field_link'){
  $table = 'field_data_'.$fieldname;
  $query = db_select($table, 'c');
  //SELECT the fields from node
  $query->fields('c',array($fieldname.'_url'))
    ->condition('entity_type ', $entitytype)
    ->condition('bundle ', $bundle)
    ->condition('entity_id ', $ids)
    ->range(0,100);//LIMIT to 2 records
  $result = $query->execute();
  $output = array();
  while($record = $result->fetchAssoc()) {
    $output[] = $record[$fieldname.'_url']; 
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

//get the fileID,project path filename, zip toggle and create real directory
//todo, reduce the file download and unzip
function q2_inc_move_attached_file($fid, $path,  $zip = 0 ,$file_name = ''){
  //get the file path 
  $fileuri = file_create_url(file_load($fid)->uri);
  if (0 === strpos($fileuri, 'http')) {
    //do nothing
  }
  else {
    $fileuri = 'http:'.$fileuri;
  }
  if($zip) {
    q2_inc_unzip($fileuri,$path);
  }
  else {
    //just copy
  }
  return $fileuri;
}

//download and unzip the file
function q2_inc_unzip($url, $dest_path) {
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  $data = curl_exec ($ch);
  curl_close ($ch);

  $destination = '/tmp/'.basename($url);;
  $file = fopen($destination, "w+");
  fputs($file, $data);
  fclose($file);

  $zip = new ZipArchive;  
  $res = $zip->open($destination);
  //return $res;
  if ($res === TRUE) {
    $zip->extractTo( $dest_path);
    $zip->close();
    return TRUE;
  } 
  else {
    return FALSE;
  }
}  

//get the file id and filename and path and transfer the files phycically
//todo
//dont store all these in amazon, just enough to store in drupal 
//use local copy instead of http copy
function q2_inc_move_tpl_files($fid, $file_name, $dest){
  $dest = $dest.'/'.$file_name;
  $path = pathinfo($dest);
  if(!file_prepare_directory($path['dirname'])) {
    drupal_mkdir($path['dirname'],0755, TRUE); 
  }
  $fileuri = file_create_url(file_load($fid)->uri);
  if (0 === strpos($fileuri, 'http')) {
    //do nothing
  }
  else {
    $fileuri = 'http:'.$fileuri;
  }
  
  if (copy($fileuri, $dest)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
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

//convert the string to maching name
function q2_inc_tomachine_name($string, $seperator = '_') {
  return preg_replace('@[^a-z0-9_]+@',$seperator, strtolower($string));
}

//some designes might have used same region for more than one time,
//so it is better to create unique name of arrays in the same order
function q2_inc_avoid_dup_machine_names($designs) {
  $dupdesigns = array();
  $output = array();
  foreach($designs as $design) {
    //get data for each style
    $style = q2_inc_design_machinenames($design);
    //count the duplicate styles
    if(isset($dupdesigns[$design])) {
      if($design == $style[0]->nid) {
      $dupdesigns[$design] = $dupdesigns[$design] + 1;
      }
    }
    else {
      $dupdesigns[$design] = 0;
    }
    $machine_name = '';
    if($dupdesigns[$design]) {
      $style[0]->machine_name = $style[0]->field_style_name_value.'_'.$dupdesigns[$design];
      $style[0]->human_name = $style[0]->title.' '.$dupdesigns[$design];
    }
    else {
      $style[0]->machine_name = $style[0]->field_style_name_value;
      $style[0]->human_name = $style[0]->title;
    }
    $output[] = $style[0];
  }
  return $output;
}

//get the assetID, path, storage type, filename 
//save to code to the filename
//storage : 1 = code, 2 = from file, 3 = from cdn, flow from high to low
function q2_inc_asset2file($assetID,$file_name, $path, $storage = 1 ) {
  $filename = $path.'/'.$file_name;
  $code = q2_inc_getcodes($assetID, 'field_collection_item', 'field_assets');
  if(isset($code[0])) {
    $file_content = $code[0];
  }
  return file_save_data($file_content, $filename, FILE_EXISTS_REPLACE );
}

//Get link field data for any entity
//field_data_field_link
//give id, entity type, bundle, and field name 
function q2_inc_get_doublefield( $entityids, $fieldname = 'field_folders', $entitytype = 'taxonomy_term', $bundle = 'drupal_modules'){
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


//get the designs, path, filename, and desktop/mobile and create image from that
function q2_inc_designs2screenshot($designs, $path, $file_name = "screenshot.jpg", $type = "desktop", $quality = 100) {
  $images = array();
  foreach($designs as $design) {
    $style = node_load($design);
    //add all the images
    if($type == "desktop") {
      if(isset($style->field_screenshot['und'][0])) {
        $images[] = $style->field_screenshot['und'][0];
      }
    }
    if($type == "mobile"){
      if(isset($style->field_mobile_screen['und'][0])) {
        $images[] = $style->field_mobile_screen['und'][0];
      }
    }
  }
  $height = 0;
  $width = 0;
  $count = count($images);
  foreach($images as $image) {
    $height = $height + $image['height'];
  }
  $dst_im  = imagecreatetruecolor($image['width'], $height);
  $old_file_delete = drupal_realpath($path . '/' .$file_name);
  if(file_exists ($old_file_delete)) {
    unlink ($old_file_delete);
  }
  $final_image = drupal_realpath(file_create_filename($file_name, $path));
  $height = 0;
  foreach($images as $image) {
    
    $dir_uri = file_stream_wrapper_get_instance_by_uri( $image['uri']);
    $imageurl = $dir_uri->getExternalUrl();
   
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $imageurl); 
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1); // good edit, thanks!
    curl_setopt($ch, CURLOPT_BINARYTRANSFER, 1); // also, this seems wise considering output is image.
    $data = curl_exec($ch);
    curl_close($ch); 

    $src_im = imagecreatefromstring($data); 

    imagecopy ($dst_im , $src_im , 0 , $height , 0 , 0 , $image['width'] , $image['height'] )  ;
    $height = $height + $image['height'];
    }
  $output['images'] = $images;
  $output['height'] = $height;
  if(imagejpeg ($dst_im , $final_image, $quality )) {
    return $output;
  }
  return $output;
}

//move all the content images /assets/images
//todo create image path : assets/images
function q2_inc_move_content_images($filesfcs,$theme_path){
  //get the file ids from the list of field collections
  //$theme_path = $theme_path . '/assets/images';
  //create the css/lib folder
  if(!file_prepare_directory($theme_path)) {
    drupal_mkdir($theme_path,0755, TRUE); 
  }
  //get the file ids from the list of field collections, last parameter indicates keyed array
  $fids = q2_inc_get_files('field_data_field_files', 'field_collection_item', 'field_theme_files', $filesfcs, "field_files_fid", 1 );

  $return = array();
  
  //get all the paths 
  $filepaths = q2_inc_get_text( $filesfcs,'field_collection_item', 
'field_theme_files', 'field_file_path', 1);
  
  foreach ($fids as $eid => $fid) {
    $return[$fid] = q2_inc_move_tpl_files($fid, $filepaths[$eid] ,$theme_path);
  }
  return $return;
}


//Zip a folder, get folder path and oupput file name
function q2_inc_pclzip_folder($folder ,$filename){
  $lib_path = libraries_get_path('pclzip');
  include $lib_path . '/pclzip.lib.php';
  
  $archive = new PclZip($filename);
  $v_dir = $folder; // or dirname(__FILE__);
  $v_remove = $v_dir;
  // To support windows and the C: root you need to add the 
  // following 3 lines, should be ignored on linux
  //if (substr($v_dir, 1,1) == ':') {
    //$v_remove = substr($v_dir, 2);
  //}
  $v_list = $archive->create($v_dir, PCLZIP_OPT_REMOVE_PATH, $v_remove);
  if ($v_list == 0) {
    return FALSE;
  }
  return TRUE;
}

//Zip a folder, get folder path and oupput file name
function q2_inc_folder2zip($rootPath ,$filename){
  // Initialize archive object
  $zip = new ZipArchive;
  $zip->open($filename, ZipArchive::CREATE);
  // Initialize empty "delete list"

  // Create recursive directory iterator
  $files = new RecursiveIteratorIterator(
    new RecursiveDirectoryIterator($rootPath),
    RecursiveIteratorIterator::LEAVES_ONLY
  );
  foreach ($files as $name => $file) {
    // Get real path for current file
    $filePath = $file->getRealPath();
    // Add current file to archive
    $zip->addFile($filePath);
  }
  // Zip archive will be created only after closing object
  $zip->close();
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

//get the character and number of occurance and return the string 
function _d7_inc_char2string($count, $char = " ") {
  $output = "";
  for($i = 0; $i < $count; $i++ ) {
    $output = $output.$char;
  }
  return $output;
}

// This is real screenshoot API, based
function q2_inc_designs2screenshot_realone($designs, $path = 'public://devbots/d8/adalu_d8/theme/adalu', $file_name = "screenshot.png", $output_max_height = 219, $output_max_width = 294) {
  $max_height = $output_max_height * 4.67808219;
  $images = array();
  foreach($designs as $design) {
    $style = node_load($design);
    //add all the images
    if(isset($style->field_screenshot['und'][0])) {
      $images[$design] = $style->field_screenshot['und'][0];
      $plugin_id = q2_inc_gettermreferences("field_data_field_plugin", "node", "style", $design,  "field_plugin_tid");
      $images[$design]['plugin'] = $plugin_id[0];
    }
  }

  // Add the header and footer first 
  $height_with_head_foot = 0;
  $count = count($images);
  foreach($images as &$image) {
    if ($height_with_head_foot < $max_height && ($image['plugin'] == 1 || $image['plugin'] == 6)) {
      $height_with_head_foot = $height_with_head_foot + $image['height'];
      $image['addtoscreen'] = 1;
    }
  }

  $height_for_content = $max_height - $height_with_head_foot;
  
  $content_height = $height_with_head_foot;
  if($height_for_content > 10) {
    foreach($images as &$image) {
      if ($max_height > $content_height && !isset($image['addtoscreen'])) {
        if (($content_height + $image['height']) < $max_height) {
          $content_height = $content_height + $image['height'];
          $image['addtoscreen'] = 1;
        }  
      }
    }
  }

  $height = 0;
  
  foreach($images as &$image) {
    if(isset($image['addtoscreen'])) {
      $height = $height + $image['height'];
    }
  }
  $final_height = intval($height / 4.67808219);
  $dst_im  = imagecreatetruecolor(1366, $height);
  $final_image = drupal_realpath(file_create_filename($file_name, $path));
  $height = 0;
  $rreturn = array();
  foreach($images as $style_id => &$image) {
    if(!isset($image['addtoscreen'])) {
      $rreturn['excl'][] = $style_id;
      continue;
    }
    $rreturn['incl'][] = $style_id;
    $dir_uri = file_stream_wrapper_get_instance_by_uri( $image['uri']);
    $imageurl = $dir_uri->getExternalUrl();
    
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $imageurl); 
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1); // good edit, thanks!
    curl_setopt($ch, CURLOPT_BINARYTRANSFER, 1); // also, this seems wise considering output is image.
    $data = curl_exec($ch);
    curl_close($ch);
    
    $src_im = imagecreatefromstring($data);
    
    imagecopy ($dst_im , $src_im , 0 , $height , 0 , 0 , $image['width'] , $image['height'] )  ;
    $height = $height + $image['height'];
  }
  //resize it 
  $resized_jpeg = imagecreatetruecolor($output_max_width, $final_height); 
  imagecopyresized($resized_jpeg, $dst_im, 0, 0, 0, 0, $output_max_width,$final_height, 1366, $height );
  
  if(imagepng ($resized_jpeg , $final_image )) {
    return $final_image;
  }
  return $images;
}

//This get design code and convert it to proper array 
//@todo may be eleminate unwanted design code here
//@todo make sure header in top, footer in bottom and all other stuffs together
function q2_inc_fix_designcode($design) {
  if(is_array($design)) {
    return $design;
  }
  else {
    if(trim($design) && preg_match('/^[0-9,]+$/',$design)) {
      return explode(',', $design); 
    }
    else {
      return FALSE;
    }
  }  
}

// Deletes the folder and its content 
function q2_inc_rrmdir($dir) { 
  foreach(glob($dir . '/*') as $file) { 
    if(is_dir($file)) 
      q2_inc_rrmdir($file); 
    else 
      unlink($file); 
  } 
  //rmdir($dir); // this will delete the main folder 
}

// Deletes the folder and its content 
//@ todo, supply the file url instead of file id
function q2_inc_getfc_theme_files($fcid) { 
  $output = array();
  $FCItem = field_collection_item_load($fcid);
  if(isset($FCItem->field_short_description['und'][0]['value'])) {
    $output['field_short_description'] = $FCItem->field_short_description['und'][0]['value'];
  }
  if(isset($FCItem->field_code['und'][0]['value'])) {
    $output['field_code'] = $FCItem->field_code['und'][0]['value'];
  }
  if(isset($FCItem->field_file_type['und'][0]['tid'])) {
    $output['field_file_type'] = $FCItem->field_file_type['und'][0]['tid'];
  }  
  if(isset($FCItem->field_file_path['und'][0]['value'])) {
    $output['field_file_path'] = $FCItem->field_file_path['und'][0]['value'];
  }  
  return $output;
}
