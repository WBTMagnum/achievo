<?php
  useattrib("atknumberattribute");
  class scheduler_cyclus_not extends atkNode
  {
     function scheduler_cyclus_not()
     {
       $this->atkNode("scheduler_cyclus_not",NF_NO_ADD|NF_NO_EDIT|NF_NO_VIEW|NF_MRA);
       $this->add(new atkNumberAttribute("scheduler_id",AF_PRIMARY));
       $this->add(new atkNumberAttribute("date",AF_PRIMARY));
       $this->setTable("scheduler_cyclus_not");
       $this->setSecurityAlias("scheduler.scheduler");
     }

     function date_display($rec)
     {
       return date("d-m-Y H:i",$rec["date"]);
     }
     
     function descriptor_def()
     {
       return "[date]";
     }
  }
?>