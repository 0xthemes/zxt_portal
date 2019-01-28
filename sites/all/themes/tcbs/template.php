<?php

/**
 * @file
 * The primary PHP file for this theme.
 */

 function tcbs_preprocess_page(&$variables, $hook) {
  $path = current_path();
  $tpl = '';
  switch($path) {
    case 'user/register';
    case 'user/login';
    case 'user/password';
      $variables['theme_hook_suggestions'][] =  'page__stripped';
      
    break;
    default:
  }
}

function tcbs_menu_tree__secondary(array &$variables) {
  return '<ul class="dropdown-menu">' . $variables['tree'] . '</ul>';
}


//Function to get user name if logged in else login link
function _tcbs_hlp_user_menu_heading(&$content) {
  global $user;
  $html = '<ul class = "menu nav navbar-nav secondary nav-user_menu">';
  $base_path = base_path();
  $cart_url = $base_path . 'cart';
  $html .= '<li class = "depth-0" ><a href = "' . $cart_url . '" title = "Cart"><i class="fas fa-shopping-cart"></i></a></li>';
  if($user->uid) {
  
    $html .= '<li class = "depth-0 expanded dropdown"><a href = "#" title = "Cart"><i class="fas fa-user-circle"></i> &nbsp;';
    $html .= $user->name;    
    $html .= ' &nbsp; Earnings: $12.00 </a>' . render($content) . '</li>';
  }
  else {
    $login_url = $base_path . 'user/login';
    $html .= '<li class = "depth-0"><a href = "' . $login_url . '" title = "Signin">Sign In</a></a></li>';
  }
  $html .= '</ul>';
  return $html;
}