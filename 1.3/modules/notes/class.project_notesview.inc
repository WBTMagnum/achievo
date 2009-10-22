<?php

userelation("atkmanytoonerelation");
userelation("atkonetomanyrelation");
useattrib("atktextattribute");
useattrib("atkdateattribute");
useattrib("atknumberattribute");

class project_notesview extends atkNode
{
  function project_notesview()
  {
    global $ATK_VARS;

    $this->atkNode("project_notesview",NF_NO_ADD|NF_NO_DELETE);

    $this->add(new atkNumberAttribute("id"                                , AF_AUTOKEY));
    $this->add(new atkAttribute("name"                              , AF_READONLY));
    $this->add(new atkManyToOneRelation("coordinator","employee.employee", AF_READONLY|AF_HIDE_LIST));
    $this->add(new atkTextAttribute("description" ,TEXT_LARGE, AF_READONLY));
    $this->add(new atkDateAttribute("startdate","","",0,0 , AF_READONLY|AF_HIDE_LIST));
    $this->add(new atkOneToManyRelation("notes","notes.project_notes","projectid", AF_HIDE_LIST));

    $this->addFilter("project.status","active");
    $this->setTable("project","project");
    $this->setOrder("name");
    $this->setSecurityAlias("notes.project_notes");

    atkdebug("project::project()");
  }

  function descriptor_def()
  {
    return "[name]";
  }
  
  function action_admin(&$handler)
  {
    // Access control
    if (!$this->allowed("any_project"))
    {
      $user = getUser();
      $this->addFilter("project.coordinator=".$user["id"]);
    }
    return $handler->action_admin();
  }
}

?>