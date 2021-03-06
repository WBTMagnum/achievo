<?php
/**
 * This file is part of the Achievo distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package achievo
 * @subpackage pim
 *
 * @copyright (c)2008 Ibuildings B.V.
 * @license http://www.achievo.org/licensing Achievo Open Source License
 *
 * @version $Revision$
 * $Id$
 */
/**
 * @internal Include the parent class file
 */
useattrib("atklistattribute");

/**
 * pimItemAttribute class extends the atkListAttribute class with a routine
 * to automatically populate it with the available pim items when needed.
 *
 * @author guido <guido@ibuildings.nl>
 * @package achievo
 * @subpackage  pim
 */
class pimItemAttribute extends atkListAttribute
{
    /**
     * Loaded state which is used to prevent loading the pim items multiple times
     *
     * @access protected
     * @var boolean
     */
    var $m_pimitemsloaded = false;

    /**
     * pimItemAttribute constructor
     *
     * @access private
     * @param string $name Template file to parse
     * @param string $flags Template file to parse
     * @param string $size Template file to parse
     */
    function pimItemAttribute($name, $flags = 0, $size = 255)
    {
        $this->atkListAttribute($name, array(), array(), $flags, $size);
        $this->m_dbfieldtype = "string";
    }

    /**
     * Populates the internal m_options, m_values and m_lookup arrays with pim items
     */
    function loadpimitems($record, $mode)
    {
        $useritems = array();
        if ($mode == "edit" || $mode == "add") {
            $user = getUser();
            $userpimitemsnode = &atkGetNode("pim.userpimitems");
            $pimitems = $userpimitemsnode->selectDb("userpimitems.userid = '" . $user["id"] . "'");
            foreach ($pimitems as $key => $item) {
                if ($mode == "add" || ($record["pimitem"] != $item["pimitem"] && $mode = "edit"))
                    $useritems[] = $item['pimitem'];
            }
        }
        // Do not continue if pim items already loaded
        if ($this->m_pimitemsloaded)
            return;

        // Harvest the modules for pim items and populate m_lookup, m_options
        // and m_values
        $pimItems = atkHarvestModules("getPimItems", "", true);
        $pimRights = atkHarvestModules("getPimRights", "", true);
        $this->m_lookup = array();
        foreach ($pimItems as $module => $items) {
            foreach ($items as $itemName => $itemFunc) {
                $allowed = true;
                //Check, if access right is set for this item
                if (isset($pimRights[$module][$itemName])) {
                    list($nodename, $action) = $pimRights[$module][$itemName];
                    $node = &atkGetNode($nodename);
                    if (!$node->allowed($action)) {
                        $allowed = false;
                    }
                }
                if ($allowed) {
                    $item = $module . '_' . $itemName;
                    if (!in_array($item, $useritems))
                        $this->m_lookup[$item] = atkText($module, $module) . ' - ' . atkText(array("pim_$itemName", $itemName), $module);
                }
            }
        }

        // Remove disallowed pimitems from the list
        $disallowedpimitems = atkConfig::get("pim", "disallowedpimitems", array());
        foreach ($disallowedpimitems as $disallowedpimitem)
            if (array_key_exists($disallowedpimitem, $this->m_lookup))
                unset($this->m_lookup[$disallowedpimitem]);

        $this->m_options = array_values($this->m_lookup);
        $this->m_values = array_keys($this->m_lookup);

        // Set m_pimitemsloaded to true to prevent loading pim items multiple times
        $this->m_pimitemsloaded = true;
    }

    /**
     * Calls loadpimitems before calling the parents display function
     *
     * @param array $record Array with fields
     * @return string text string of $record
     */
    function display($record, $mode = "")
    {
        $this->loadpimitems($record, $mode);
        return parent::display($record);
    }

    /**
     * Calls loadpimitems before calling the parents edit function
     *
     * @param array $record Array with fields
     * @param String $fieldprefix The fieldprefix to put in front of the name
     *                            of any html form element for this attribute.
     * @return String Piece of html code containing the select control
     */
    function edit($record = "", $fieldprefix = "", $mode = "")
    {
        $this->loadpimitems($record, $mode);
        return parent::edit($record, $fieldprefix);
    }

}

?>