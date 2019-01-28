
<?php
/*Template Name: Inline Tags*/
$attributes = array();
$attributes['class'] = array('label','label-default');
$tags = array();
foreach ($data as $key => $item) {
  if(isset($item['tid'])) {
    $tags[] = l($item['name'], "taxonomy/term/" . $item['tid'], array('attributes' => $attributes));
  }
}

print implode(" ", $tags);
?>
