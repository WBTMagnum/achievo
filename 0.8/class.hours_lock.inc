<?php

class hours_lock extends atkNode
{
  function hours_lock()
  {
    $this->atkNode("hours_lock",NF_NO_EDIT|NF_NO_EXTENDED_SEARCH|NF_NO_VIEW);

    $this->add(new atkDummyAttribute("label",text("hourlock_week_explanation"),AF_HIDE_LIST));
    $this->add(new atkAttribute("week",AF_PRIMARY|AF_SEARCHABLE));
    $this->add(new atkAttribute("userid",AF_PRIMARY|AF_SEARCHABLE));
    
    $this->setOrder("week DESC");
    $this->setTable("hours_lock");
  }
  
  function userid_edit($record)
  {
    $res = '<select name="userid">';
    $res.= $this->get_employees($record["userid"]);
    $res.= '</select>';
    return $res;
  }
  
  function get_employees($userid)
  {
    global $g_db;
  
    $sql = "SELECT name,userid 
            FROM employee
            WHERE status='active'
            ORDER BY name
           ";

    $records = $g_db->getrows($sql);
  
    // add the 'select all users' option.
    array_unshift($records,array("userid"=>"*","name"=>text("all_users")));
    
    for($i=0;$i<count($records);$i++)
    { 
      if ($userid==$records[$i]["userid"]) $selected="selected"; 
      else $selected="";
      $employee_code.='<OPTION VALUE="'.$records[$i]["userid"].'" '.$selected.'>'.$records[$i]["name"].'</OPTION>';
    }
    return $employee_code;
  }
  
  function postAdd($record)
  {
    global $g_db;
    atkdebug("postAdd");
    // After adding a lock for all users, we delete individual locks for that same week.
    if ($record["userid"]=="*")
    {
      $query = "DELETE FROM hours_lock WHERE week='".$record['week']."' AND userid <> '*'";
      $g_db->query($query);
    }    
  }
}

?>