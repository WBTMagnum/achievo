<?php
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * This file is the skeleton menu loader, which you can copy to your
   * application dir and modify if necessary. By default, it checks
   * the menu settings and loads the proper menu.
   *
   * @package atk
   *
   * @author Yury Golovnya <yury@achievo.org>
   *
   * @copyright (c)2000-2008 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision$
   * $Id$
   */

  /**
   * @internal includes
   */
  $config_atkroot = "./";
  include_once("atk.inc");

  atksession();
  atksecure();

  if(isset($_GET['parentId']))
  {
    atkimport("menu.atktabmenu");
    echo atkTabMenu::renderSubMenu($_GET['parentId'],$_GET['parentType']);
  }
?>