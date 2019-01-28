<?php
/*Template Name: Terms Checklist*/
$attributes = array();
$attributes['class'] = array('label','label-success');
$tags = array();
$_terms = zxt_helper_terms_render_checklist(8, $data);

foreach ($_terms as $_tid => $item) {
  if($item['toggle']) {
    $tags[] = '<div class="row mrg-t-5"><div class = "col-xs-5 ic-p-title">' . $item['name'] . '</div><div class = "col-xs-7 ic-p-detail">' . l('Yes', "taxonomy/term/" . $_tid, array('attributes' => $attributes)) . '</div></div>';
  }
  
}
//print_R($_terms);
print implode(PHP_EOL, $tags);
?>
