<?php
$phylotree = $variables['node']->phylotree; 
$phylotree = chado_expand_var($phylotree,'field','phylotree.comment'); ?>


<div class="tripal_phylotree-data-block-desc tripal-data-block-desc"> <?php

// the $headers array is an array of fields to use as the colum headers.
// additional documentation can be found here
// https://api.drupal.org/api/drupal/includes%21theme.inc/function/theme_table/7
// This table for the analysis has a vertical header (down the first column)
// so we do not provide headers here, but specify them in the $rows array below.
$headers = array();

// the $rows array contains an array of rows where each row is an array
// of values for each column of the table in that row.  Additional documentation
// can be found here:
// https://api.drupal.org/api/drupal/includes%21theme.inc/function/theme_table/7
$rows = array();

// Name row
$rows[] = array(
  array(
    'data' => 'Tree Name',
    'header' => TRUE,
  ),
  $phylotree->name
);

$leaf_type = 'N/A';
if ($phylotree->type_id) {
  $leaf_type = $phylotree->type_id->name;
}
$rows[] = array(
  array(
    'data' => 'Leaf type',
    'header' => TRUE,
  ),
  $leaf_type
);

$description = 'N/A';
if ($phylotree->comment) {
  $description = $phylotree->comment;
}
$rows[] = array(
  array(
    'data' => 'Description',
    'header' => TRUE,
  ),
  $description
);

// the $table array contains the headers and rows array as well as other
// options for controlling the display of the table.  Additional
// documentation can be found here:
// https://api.drupal.org/api/drupal/includes%21theme.inc/function/theme_table/7
$table = array(
  'header' => $headers,
  'rows' => $rows,
  'attributes' => array(
    'id' => 'tripal_phylotree-table-base',
    'class' => 'tripal-data-table'
  ),
  'sticky' => FALSE,
  'caption' => '',
  'colgroups' => array(),
  'empty' => '',
);

// once we have our table array structure defined, we call Drupal's theme_table()
// function to generate the table.
print theme_table($table);  ?>
