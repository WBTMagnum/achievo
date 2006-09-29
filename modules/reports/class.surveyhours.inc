<?php
atkimport("module.timereg.hours");
class surveyhours extends hours 
{
  function surveyhours()
  {
    $this->hours('surveyhours');
    foreach (array_keys($this->m_attribList) as $attrName)
    {
      $attr = &$this->getAttribute($attrName);
      if (is_a($attr, 'atkmanytoonerelation'))
      {
        $attr->removeFlag(AF_RELATION_AUTOCOMPLETE);
      }
        
      if($attrName == "phaseid")
      {
        $attr->m_dependees = array();
        $attr->removeFlag(AF_OBLIGATORY);
      }        
    }
    $this->setOrder("");    
    $this->setSecurityAlias('timereg.hours');    
  }
  
  /**
   * This filter is only needed in the hoursnode, where activityid
   * depends on the phaseid.
   *
   * @param array $record The record
   * @param String $mode The mode
   * @return result of function getSelectableRecords
   */
  function activityid_selection($record, $mode="")
  {
    $attr = &$this->getAttribute('activityid');
    return $attr->getSelectableRecords($record, $mode);    
  }
  
  
}
?>