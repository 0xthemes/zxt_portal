<?php

//get list of all terms of given voc ID
function zxt_helper_terms_by_voc($voc_id) {
  $conditions = array(
    'vid' => $voc_id,
  );
  $orderby = array(
    'weight' => 'ASC',
  ); 
  $fields = array('tid', 'name');

  $terms = zxt_helper_db_single_table_query('taxonomy_term_data', $fields, $conditions, $orderby, 0, 'fetchAllAssoc', 'tid');
  $output = array();
  if($terms && count($terms)) {
    foreach($terms as $tid => $term) {
      $output[$tid] = $term->name;
    }
  }
  return $output;
}

function zxt_helper_terms_extract_fft_data(&$terms) {
  //$output = array();

}

function zxt_helper_terms_render_checklist($voc_id, &$terms, $options = NULL) {
  $all_terms = zxt_helper_terms_by_voc($voc_id);
  $tids = array();
  foreach($terms as $delta => &$term) {
    $tids[] = $term['tid'];
  }
  $output = array();
  foreach($all_terms as $_tid => $_name) {
    $output[$_tid] = array('name' => $_name);
    if(in_array($_tid, $tids)) {
      $output[$_tid]['toggle'] = true;
    }
    else {
      $output[$_tid]['toggle'] = false;
    }
  }
  return $output;
}