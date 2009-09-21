<?php
include_once("atk.inc");
include_once("atk/atkbrowsertools.inc");
atksession();
atksecure();
include_once("./theme.inc");
include_once("./version.inc");

$g_layout->initGUI();
$g_layout->output('<html>');
$g_layout->head($txt_app_title);
$g_layout->body();
if (!browserInfo::detectPDA())
{
  $g_layout->ui_top($txt_app_title." ".$achievo_version.($achievo_state!="stable"?" ($achievo_state)":""));

  $table = $g_layout->ret_table_simple(0,true);

  $table.= '<tr>';

  $table.= $g_layout->ret_td("&nbsp;", 'width="20%" height="40"');

  $centerpiece = $txt_logout_loggedinuser.': &nbsp;<b>'.$g_user["name"].'</b>&nbsp; &nbsp; &nbsp;';

  if ($g_user["name"]!="administrator")
  {
    $centerpiece.= href(dispatch_url("pim.pim", "pim"), text('pim'), SESSION_NEW, false, 'target="main"').'&nbsp; &nbsp; &nbsp;';
    $centerpiece.= href(dispatch_url("employee.userprefs", "edit", array("atkselector" => "person.id='".$g_user["id"]."'")), text('userprefs'), SESSION_NEW, false, 'target="main"');
  }
  else
  {
    // Administrator has a link to setup.php
    $centerpiece.= href("setup.php", text("setup"), SESSION_NEW, false, 'target="_top"');
  }
  $centerpiece.='&nbsp; &nbsp; &nbsp;';

  $centerpiece.= '<a href="index.php?atklogout=1" target="_top">'.text('logout').'</a>';

  $table.= $g_layout->ret_td($centerpiece, 'width="55%" align="center"');

  $searchnode = getNode("search.search");
  $searchpiece = $searchnode->simpleSearchForm("", "main", SESSION_NEW);

  $table.= $g_layout->ret_td($searchpiece, 'width="25%" align="right"');

  $table .= '</tr></table>';
  $g_layout->output($table);
}
else 
{
  $pdanodes = atkHarvestModules("pdaNodes");
  $menuitems = array();
  
  foreach ($pdanodes as $key => $url)
  {
    $menuitems[text("menu_$key")] = '<a href="'.$url.'" target=\'main\'>'.text("menu_$key").'</a>';
  }
  
  ksort($pdanodes);
  
  foreach ($menuitems as $url)
  {
    $g_layout->output($url);
  }
  
  $g_layout->output('<a href="index.php?atklogout=1" target="_top">'.text("logout").'</a>');
}
$g_layout->ui_bottom();
$g_layout->output('</body>');
$g_layout->output('</html>');

$g_layout->outputFlush();
?>
