<?php
/**
 * @file
 * API documentation for Query Parameters to URL module.
 */

/**
 * Given path and contextual data, decide if path rewriting should be done.
 *
 * Might be called from hook_url_inbound_alter() or hook_url_outbound_alter().
 * Depending on that, passed contextual data can differ.
 *
 * @param $path String The path to rewrite.
 * @param $options Array An array of url options in outbound alter.
 * @param $original_path String|Null The original path before altering.
 * @param $path_language String|Null The path language in inbound alter.
 *
 * @return string
 */
function zxt_urls_query_parameters_to_url_rewrite_access($path, $options, $original_path, $path_language) {
  if($path === 'items') {
    $output = array(
      'path' => $path,
      'options' => $options,
      'original_path' => $original_path,
      'path_language' => $path_language,

    );

  
    $message = '<pre>' . print_R($output, TRUE)  . '</pre>';
    //watchdog('zxt_urls_query_parameters_to_url_rewrite_access', $message);    
    return QUERY_PARAMETERS_TO_URL_REWRITE_ALLOW;
  }
  return QUERY_PARAMETERS_TO_URL_REWRITE_IGNORE;
  


  return QUERY_PARAMETERS_TO_URL_REWRITE_DENY;

  // Allow path rewriting on the special dashboard page.
  if ($path == 'special-dashboard') {
    return QUERY_PARAMETERS_TO_URL_REWRITE_ALLOW;
  }

  // Deny rewriting on the events page, if a special query parameter is set.
  if ($path == 'events' && isset($options['query']['special_parameter']) && $options['query']['special_argument'] == TRUE) {
    return QUERY_PARAMETERS_TO_URL_REWRITE_DENY;
  }

  // Ignore otherwise.
  return QUERY_PARAMETERS_TO_URL_REWRITE_IGNORE;
}

/**
 * Allow rewriting the final URLs containing encoded query parameters.
 *
 * Called from hook_url_inbound_alter() and from hook_url_outbound_alter().
 * If you want to modify the final urls and query parameters, make sure to
 * check for both inbound and outbound direction.
 *
 * In the outbound case you will most probably want to modify the
 * $context['options']['query'] array, which contains the query parameters
 * that will be encoded into the URL. You can alter both the values and the key.
 *
 * In the inbound case you will probably want to modify the
 * $context['extracted_query_parameters'] array, which contains the decoded
 * query parameters from the initial inbound URL.
 *
 * @param $path String The path that is being rewritten depending on the hook:
 *  - hook_url_outbound_alter(): The original path before it will have the query
 *    parameters appended to it in encoded form.
 *  - hook_url_inbound_alter(): The final decoded path, which does not contain
 *    the encoded query parameters.
 * @param $context Array An associative array with additional options depending
 *  on the called hook:
 *  - called from hook_url_outbound_alter(), will have the following keys:
 *    - direction - the string 'outbound' so you can use it in conditionals.
 *    - options - the options array passed to the outbound hook. This contains
 *      the 'query' key with the parameters which you will want to change.
 *    - original_path - the original_path passed to the outbound hook.
 *    - new_path_without_parameters - the new path prefix that will be used
 *      together with the encoded parameters string.
 *  - called from hook_url_inbound_alter(), will have the following keys:
 *    - direction - the string 'inbound' so you can use it in conditionals.
 *    - original_path - the original_path passed to the inbound hook.
 *    - path_language - the path_language passed to the inbound hook.
 *    - extracted_query_parameters - the array of query parameters that were
 *      decoded from the inbound url.
 *    - request_uri_path - the request uri path which will be assigned to the
 *      $SERVER['REQUEST_URI'] variable.
 *    - initial_url_components - an array of url components exploded by '/',
 *      of the original inbound url, for informational purposes.
 */
function zxt_urls_query_parameters_to_url_rewrite_alter(&$path, &$context) {
  //$output = func_num_args();
  $message = '<pre>' . print_R($path, TRUE)  . '</pre>';
  watchdog('zxt_urls_query_parameters_to_url_rewrite_alter', $message);


}

function zasdxt_urls_url_inbound_alter(&$path, $original_path, $path_language) {
    $output = array(
      'path' => $path,
      'original_path' => $original_path,
      'path_language' => $path_language,

    );

    $message = '<pre>' . print_R($output, TRUE)  . '</pre>';
    watchdog('zxt_urls_query_parameters_to_url_rewrite_access', $message);     
}