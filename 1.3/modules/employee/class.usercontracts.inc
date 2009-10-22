<?php

// The ability to add contracts per user was sponsored by Tryllian BV, Netherlands.

userelation("atkmanytoonerelation");
useattrib("atkdateattribute");
useattrib("atktextattribute");
useattrib("atknumberattribute");
useattrib("atkmultiselectattribute");
useattrib("atknumberattribute");
useattrib("atktimeattribute");

class usercontracts extends atkNode
{
  function usercontracts()
  {
    $m_arr_minute_step = array();
    for($i=0;$i<60;$i+=5) $m_arr_minute_step[] = sprintf("%02d",$i);

    $this->atkNode("usercontracts", NF_TRACK_CHANGES); // this is a support module who's called directly

    $this->add(new atkNumberAttribute("id", AF_AUTOKEY));
    $this->add(new atkManyToOneRelation("userid","employee.employee",AF_READONLY|AF_OBLIGATORY));
    $this->add(new atkNumberAttribute("uc_hours", AF_OBLIGATORY, 4,2));
    $this->add(new atkDateAttribute("startdate","","",0,0, AF_OBLIGATORY));
    $this->add(new atkDateAttribute("enddate", "",""));
    $this->add(new atkTextAttribute("description", TEXT_SMALL, 0));
    $this->add(new atkMultiSelectAttribute("workingdays",array("mon", "tue","wed","thu","fri","sat","sun"),array(1,2,3,4,5,6,0),7,AF_HIDE_LIST));
    $this->add(new atkTimeAttribute("workstarttime"  , 0, 23, $m_arr_minute_step, "08:30", AF_OBLIGATORY));
    $this->add(new atkTimeAttribute("workendtime"    ,0, 23, $m_arr_minute_step, "17:00",AF_OBLIGATORY));

    $this->setTable("usercontract");
    $this->setOrder("usercontract.id");
    $this->setDescriptorTemplate("[uc_hours] " . $this->text("hours"));
  }

  function resetOvertimeBalance($record)
  {
    atkImport("module.utils.dateutil");
    $balancenode = &atkGetNode("timereg.overtime_balance");
    $balancenode->deleteBalance($record["userid"]["id"], dateutil::arr2str($record["startdate"], "Y-m-d"));
  }

  function postAdd($record)    { $this->resetOvertimeBalance($record); return true; }
  function postUpdate($record) { $this->resetOvertimeBalance($record); $this->resetOvertimeBalance($record["atkorgrec"]); return true; }
  function postDelete($record) { $this->resetOvertimeBalance($record); return true; }

  function initial_values()
  {
    return array("uc_hours"=>40,
                 "enddate"=>array("year"=>(date("Y")+1),
                                  "month"=>date("m"),
                                  "day"=>date("d")),
                 "workstarttime"=>array("hours"=>"08",
                                        "minutes"=>"30",
                                        "seconds"=>"00"),
                 "workendtime"=>array("hours"=>"17",
                                        "minutes"=>"00",
                                        "seconds"=>"00"),
                  );
  }

  /**
   * We override the hours edit, because it is the first field in the edit screen.
   * This override is then used to set some values for various fields, depending
   * on the previous contract of that employee (if any):
   * - uc_hours is set to the uc_hours of the previous contract
   * - workingdays is set to the workingdays of the previous contract
   * - startdate is set to the enddate of the previous contract
   * - enddate is set to startdate + 1 year.
   */
  function uc_hours_edit(&$record, $prefix, $mode)
  {
    if ($mode=="add")
    {
      if ($record["userid"]["id"]!="")
      {
       $previous = $this->selectDb("userid=".$record["userid"]["id"], "enddate DESC", 1, "", array("enddate", "workingdays", "uc_hours"));

        if (count($previous))
        {

          if (is_array($previous[0]["enddate"]))
          {
            $year = $previous[0]["enddate"]["year"];
            $month = $previous[0]["enddate"]["month"];
            $day = $previous[0]["enddate"]["day"];
            $record["startdate"] = array("year"=>$year,
                                         "month"=>$month,
                                         "day"=>$day);

            // We also set enddate now, to startdate+1 year.
            $endstamp = adodb_mktime(12, 0, 0, $month, $day, $year+1);
            $record["enddate"] = array("year"=>date("Y", $endstamp),
                                       "month"=>date("m", $endstamp),
                                       "day"=>date("d", $endstamp));
          }
          // and while we're at it, we should also set the workingdays
          // to the workingdays of the previous contract.
          if (!empty($previous[0]["workingdays"]))
          {
            $record["workingdays"] = $previous[0]["workingdays"];
          }

          // and also the uc_hours.
          if (!empty($previous[0]["uc_hours"]))
          {
            $record["uc_hours"] = $previous[0]["uc_hours"];
          }
        }
      }
    }
    // We have only implemented this method to set some values. We should
    // call the regular edit method now.
    return $this->m_attribList["uc_hours"]->edit($record, $prefix, $mode);
  }

  function getMostRecentContract($userid)
  {
    if ($userid!="")
    {
      $recs = $this->selectDb("userid=".$userid, "enddate desc", 1, "", array("startdate", "enddate", "uc_hours"));
      if (count($recs)) return $recs[0];
    }
    return array();
  }

}

?>