<?php
/**
 * Implements hook_preprocess_hook()
 *
 * @param $variables
 */
function tripal_phylotree_preprocess_tripal_phylotree_phylogram(&$variables) {
  $phylotree = $variables['node']->phylotree;
  $module_path = drupal_get_path('module', 'tripal_phylotree');

  drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/d3/3.4.8/d3.min.js', 'external');

  drupal_add_js("$module_path/theme/js/d3.phylogram.js");
  drupal_add_js("$module_path/theme/js/organism-bubble-plot.js");
  drupal_add_js("$module_path/theme/js/tripal_phylotree.js");
  drupal_add_css("$module_path/theme/css/tripal_phylotree.css");

  drupal_add_library('system', 'ui.dialog');

  // Get the tree options as set by the administrator.
  $options = json_encode(array(
    'phylogram_width' => variable_get('tripal_phylotree_default_phylogram_width', 350),
    'dendrogram_width' => variable_get('tripal_phylotree_default_dendrogram_width', 500),
    'bubble_width' => variable_get('tripal_phylotree_default_bubble_width', 500),
    'root_node_size' => variable_get('tripal_phylotree_default_root_node_size', 6),
    'interior_node_size' => variable_get('tripal_phylotree_default_interior_node_size', 6),
    'leaf_node_size' => variable_get('tripal_phylotree_default_leaf_node_size', 6),
  ));

  // Get the node colors as set by the administrator.
  $colors = array();
  $color_defaults = variable_get("tripal_phylotree_org_colors", array('1' => array('organism' => '', 'color' => '')));
  foreach ($color_defaults as $i => $details) {
    if ($details['organism']) {
      $colors[$details['organism']] =  $details['color'];
    }
  }
  $colors = json_encode($colors);

  // Add javascript data needed for this tree.
  drupal_add_js("
      // var having URL of json data source for charting
      var phylotreeDataURL = \"/chado_phylotree/$phylotree->phylotree_id/json\";
      // var with path to our theme, for use by javascript functions.
      var pathToTheme = \"/$module_path/theme\";
      // var with custom options
      var treeOptions = $options;
      // var with the organism colors
      var organismColors = $colors;
    ",
    'inline'
  );
  tripal_phylotree_set_tree_vars($phylotree);
}

/**
 * Implements hook_preprocess_hook()
 *
 * @param $variables
 */
function tripal_phylotree_preprocess_tripal_phylotree_taxonomic_tree(&$variables) {
  $phylotree = $variables['node']->phylotree;
  $module_path = drupal_get_path('module', 'tripal_phylotree');

  drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/d3/3.4.8/d3.min.js', 'external');

  drupal_add_js("$module_path/theme/js/d3.phylogram.js");
  drupal_add_js("$module_path/theme/js/organism-bubble-plot.js");
  drupal_add_js("$module_path/theme/js/tripal_phylotree.js");
  drupal_add_css("$module_path/theme/css/tripal_phylotree.css");

  drupal_add_library('system', 'ui.dialog');

  drupal_add_js("
    // var having URL of json data source for charting
    var phylotreeDataURL = \"/chado_phylotree/$phylotree->phylotree_id/json\";
    // var with path to our theme, for use by javascript functions.
    var pathToTheme = \"/$module_path/theme\";
    ",
    'inline'
  );
  tripal_phylotree_set_tree_vars($phylotree);
}
/**
 * Implements hook_preprocess_hook();
 *
 * @param $variables
 */
function tripal_phylotree_preprocess_tripal_phylotree_radial(&$variables) {
  $phylotree = $variables['node']->phylotree;
  tripal_phylotree_set_tree_vars($phylotree);
}

/**
 * Implements hook_preprocess_hook();
 *
 * @param $variables
 */
function tripal_phylotree_preprocess_tripal_phylotree_organisms(&$variables) {
  $phylotree = $variables['node']->phylotree;
  tripal_phylotree_set_tree_vars($phylotree);
}

/**
 * Helper function for the preprocess hooks that adds a 'has_nodes' to the phylotree object.
 */
function tripal_phylotree_set_tree_vars(&$phylotree) {
  if (!property_exists($phylotree, 'has_nodes')) {
    // If the nodes haven't loaded then set a value so the template can
    // choose not to show the phylogram.
    $values  = array('phylotree_id' => $phylotree->phylotree_id);
    $options = array('limit' => 1, 'offset' => 0, 'has_record' => 1);
    $phylotree->has_nodes = chado_select_record('phylonode', array('phylonode_id'), $values, $options);
  }
  if (!property_exists($phylotree, 'has_features')) {
    // If the nodes haven't loaded then set a value so the template can
    // choose not to show the circular dendrogram. The chado_select_record()
    // API call can't do this query so we have to do it manually.
    $sql = "
      SELECT count(*) as num_features
      FROM {phylonode}
      WHERE NOT feature_id IS NULL and phylotree_id = :phylotree_id
      LIMIT 1 OFFSET 0
    ";
    $phylotree->has_features = chado_query($sql, array(':phylotree_id' => $phylotree->phylotree_id))->fetchField();
  }
}
