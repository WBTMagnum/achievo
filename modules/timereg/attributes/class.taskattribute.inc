<?php

define("AF_JS_UPDATEACTIVITY", AF_SPECIFIC_1);
include_once(moduleDir("project")."class.project.inc"); // used for some defines

class taskAttribute extends atkNumberAttribute
{
  var $m_nested = true; // nested in a form?

  /**
   * Constructor
   * @param $name Name of the attribute
   * @param $flags Flags for this attribute
   */
  function taskAttribute($name, $flags=0)
  {
    /* base class constructor */
    $this->atkAttribute($name, $flags);
  }

  function fetchValue($postvars)
  {
    return array(
      'phaseid' => $postvars[$this->fieldName()]['phaseid'],
      'activityid' => $postvars[$this->fieldName()]['activityid']
    );
  }
   
  /**
   * Returns a piece of html code that can be used in a form to edit this
   * attribute's value.
   * @param $record array with fields
   * @return piece of html code with a textarea
   */  
  function edit($record="")
  {
    global $ATK_VARS;
    
    $selectedphaseid  = $_REQUEST["selectedphaseid"];
    $atkaction        = $_REQUEST["atkaction"];
    
    // The current phase (in case of edit of an existing record)
    $currentphaseid   = $record[$this->fieldName()]["phaseid"];
    //$currentphaseid   = $record["phaseid"];
    $currentactivity  = $record[$this->fieldname()]["activityid"];
    
    // The current value might be overriden by use of the 'search all phases' button
    if (isset($selectedphaseid)&&$selectedphaseid!="")
    {
      $currentphaseid     = $selectedphaseid;
      $currentactivity    = 0;
    }      
    
    //get phases and activities
    $phases = $this->getPhasesAndActivities($currentphaseid);
    
    $result = "";
    
    if(!$this->hasFlag(AF_JS_UPDATEACTIVITY))
    {
      $result.= '<select name="'.$this->fieldName().'[phaseid]">';
    }
    else
    {
      $result .= $this->javascript($phases);
      $result.= '<select name="'.$this->fieldName().'[phaseid]" onChange="set_activities(this.options[this.selectedIndex].value,0)">';
    }
    $result.= '<option value="">' . atktext("select", "atk");

    for ($i=0,$_i=count($phases);$i<$_i;$i++)
    {
      $result .= '<option value="'.$phases[$i]["id"].'" ';

      // de juiste default selecteren
      if ($phases[$i]["id"]==$currentphaseid) $result.="selected";

      $result .= '>'.$phases[$i]["projectname"]." - ".$phases[$i]["name"];
    }

    $result .= '</select>&nbsp';

    $return_url = dispatch_url($ATK_VARS["atknodetype"], $ATK_VARS["atkaction"], array("selectedphaseid"=>"[id]", "atklevel"=>atkLevel()));

    $targeturl_phase = dispatch_url("project.phase", "select", array("atkfilter" => "projectid=[id]", "atktarget" => atkurlencode($return_url)));
    $result .= href(dispatch_url("project.projectselector", "select", array("atktarget" => atkurlencode($targeturl_phase))), atktext("searchallprojectsphases"), SESSION_NESTED,$this->m_nested);

    // dirty trick: the dummy attribute which starts the javascript for the
    // correct activities must be notified of the currentphase.
    $GLOBALS["currentphase"]=$currentphaseid;
    $GLOBALS["currentactivity"]=$currentactivity;    
    
    return $result;    
  }
  
  
  function display($rec)
  {
    return $rec[$this->fieldName()]["projname"]." - ".$rec[$this->fieldName()]["phasename"];
  }

  /**
   * We don't support searching
   * @param $record array with fields
   * @return search field
   */
  function search($record="")
  {
    return "&nbsp;";
  }

  function db2value($rec)
  {
    return array("phaseid"=>$rec["phaseid"],
                 "activityid"=>$rec["activityid"],
                 "projectname"=>$rec["projectname"],
                 "phasename"=>$rec["phasename"],
                 "activityname"=>$rec["activityname"]
                 );
  }

  function value2db($rec)
  {
    $phaseid = $rec[$this->fieldName()]["phaseid"];    
    $activityid = $rec[$this->fieldName()]["activityid"];    
    $user = getUser();
    
    // search task or create task and set $taskid
    $db = &$this->m_ownerInstance->getDb();
    list($row) = $db->getRows(sprintf("SELECT t.id AS taskid FROM task t, activity a WHERE t.tasktype = 'activity' AND a.id = t.refid AND a.id = %d AND t.owner = %d", $activityid, $user['id']));
    // If we have no task yet, create one.
    if ($row == NULL)
    {
      atkdebug("this user does not yet have a task related to this activity.");            
      /* @var $activityNode atkNode */
      $taskNode     = &atkGetNode('task.task');
      $activityNode = &atkGetNode('project.activity');      
      
      //get activity name
      $activityRecords    = $activityNode->selectDb("activity.id='$activityid'","","","",array("name"));
      $activityname       = $activityRecords[0]["name"];
      $dt                 = date("Y-m-d");
      $datearr            = explode("-",date("Y-m-d-H-i-s"));    
      $datetime           = array();    
      $datetime["year"]   = $datearr[0];
      $datetime["month"]  = $datearr[1];
      $datetime["day"]    = $datearr[2];
      $datetime["hours"]  = $datearr[3];
      $datetime["minutes"]= $datearr[4];
      $datetime["seconds"]= $datearr[5];
      
      /* Create a task */
      $newtask = array();
      $taskid                       = $db->nextid('task');
      $newtask["id"]                = $taskid;
      $newtask["name"]              = $activityname;
      $newtask["owner"]             = array("id"=>$user["id"]);
      $newtask["createdate"]        = $dt;      
      $newtask["editdatetime"]      = $datetime;
      $newtask["refid"]             = $activityid;
      $newtask["tasktype"]          = 'activity';
      $taskNode->addDb($newtask, false);
      
      $row = array('taskid' => $taskid);
    }
    
    $taskid = $row['taskid'];
    
    atkdebug("SAVING TASKID [$taskid]");
    
    return $taskid;    
  }
  
  function addToQuery(&$query, $tablename="", $fieldaliasprefix="", $rec="", $level=0, $mode="")
  {
    if ($mode == "add" || $mode == "update")
    {
      parent::addToQuery($query, $tablename, $fieldaliasprefix, $rec, $level, $mode);
    }
    else
    {      
      /* @var $query atkQuery */
//      $query->addField($this->fieldName(),"","E",$fieldaliasprefix,!$this->hasFlag(AF_NO_QUOTES));
//      $query->addField("name","", "B", "proj_", false);
//      $query->addField("name","", "A", "phase_", false);
//      $query->addField("name","", "D", "activity_", false);
      //$query->addField($this->fieldName(),"","hours",$fieldaliasprefix,!$this->hasFlag(AF_NO_QUOTES));
      $query->addField("id","", "A", "phase", false);
      $query->addField("id","", "C", "activity", false);
      $query->addField("name","", "B", "project", false);
      $query->addField("name","", "A", "phase", false);
      $query->addField("name","", "C", "activity", false);
      $query->addJoin("task", "D", "D.id=hours.taskid", false);
      $query->addJoin("activity", "C", "C.id=D.refid", false);
      $query->addJoin("phase", "A","A.id=C.phaseid", true);
      $query->addJoin("project", "B","B.id=A.projectid", true);
      $query->addCondition("D.tasktype='activity'");
    }
  }

  function getOrderByStatement()
  {
    return "B.name, A.name";
  }

  
  /****** Data retrieving functions ******/
  
  /**
   * get phases and activities
   *
   * get phases and activities that need to be used in the hour registration.
   * currentphaseid is added if we do not already have it.
   *
   * @param int currentphaseid the id of the selected phase
   * @return array an array with phases and activities
   */
  function getPhasesAndActivities($currentphaseid)
  {        
    //first get the recent phases
    $phases = $this->getRecentPhases();
            
    atk_var_dump($phases,"<h1>recentphases</h1>");
    //check currentphaseid
    $validcurrentphaseid = (isset($currentphaseid) && $currentphaseid != "");      
    
    //if we have a legimate currentphaseid check if we already have it in our recentphases.        
    $alreadyadded = false;    //we start by assuming we don't have it
    
    if($validcurrentphaseid)
    {
      foreach($phases as $recentphase)
      {      
        if ($recentphase[$i]['id']==$currentphaseid)
        {
          $alreadyadded = true;
          break;
        }
      }      
    }
        
    //if the currentphase is not yet in the recentlist, we load the fase.
    if(!$alreadyadded && $validcurrentphaseid)
      $currentphase = $this->getPhase($currentphaseid);
    
    //if we got an empty array back, the phase must be deleted, in which case, we do not need it
    if(is_array($currentphase) && $currentphase!=array())    
      $phases[] = $currentphase;    
    else     
      $currentphaseid = "";
    
    //get all the phaseids
    foreach ($phases as $phase)
      $phaseid_arr[] = $phase["id"];
        
    //if we have any phases:
    //create an empty activity. 
    //get the activities for each phase
    if(count($phaseid_arr) > 0)
    {
      $emptyactivity = $this->createEmptyActivity();
      $allactivities = $this->getActivities($phaseid_arr);      
    
      for($i=0,$_i=count($phases);$i<$_i;$i++)
      {
        $emptyactivity["phaseid"]["id"]=$phases[$i]["id"];
        
        //Combine the activities and the emptyactivity and add them to the phases
        $phases[$i]["activities"] = array();        
        $phases[$i]["activities"][] = $emptyactivity;
                
        foreach($allactivities as $activity)
        {
          
          if($activity["phaseid"]["id"] == $phases[$i]["id"])
            $phases[$i]["activities"][] = $activity;
        }
      }      
    }
    
    return $phases;    
  }
  
  
  /**   
   * Retrieve the most recently registered phases for the current user. 
   *
   * The function retrieves a number of recent phases the currently logged
   * in user has registered time on.
   *
   * @param none
   * @return array array with records of most recent phases
   */
  function getRecentPhases()
  {
    global $g_securityManager, $ATK_VARS, $g_sessionManager;

    $user = &getUser();

    $reportnodetype = $g_sessionManager->stackVar("reportnodetype");

    if (strstr($ATK_VARS["atknodetype"],"reports") || $reportnodetype=="hoursurvey")
      $projectPhaseFilter = "phase.status<>'archived' AND project.status<>'archived'";
    else
      $projectPhaseFilter = "phase.status='active' AND project.status='active'";

    $query = "SELECT DISTINCT 
                phase.id, 
                phase.name, 
                project.name as projectname                 
              FROM 
                project,phase
              LEFT JOIN 
                project_person ON project_person.projectid = project.id
              LEFT JOIN 
                activity ON activity.phaseid=phase.id 
              LEFT JOIN 
                task ON task.refid=activity.id 
              LEFT JOIN 
                hours ON hours.taskid=task.id 
              WHERE                 
                phase.status='active' 
                AND project.status='active' 
                AND task.tasktype='activity'
                AND hours.userid='".$user["id"]."' ";
    /* @var $query atkQuery */
    $db = &atkGetDb();
    $query = $db->createQuery();
    $query->addField("id","","phase");
    $query->addField("name","","phase");
    $query->addField("name AS projectname","","project");
    $query->addTable("hours");
    $query->addJoin("task", "", "hours.taskid = task.id", true);
    $query->addJoin("activity", "", "task.refid=activity.id", true);
    $query->addJoin("phase", "", "phase.id=activity.phaseid", true);
    $query->addJoin("project", "", "project.id=phase.projectid", true);
    $query->addJoin("project_person", "", "project_person.projectid = project.id", true);
    $query->addCondition("phase.status='active'");
    $query->addCondition("project.status='active'");
    $query->addCondition("task.tasktype='activity'");
    $query->addCondition("hours.userid='".$user["id"]."'");
    
    if (!$g_securityManager->allowed("timereg.hours", "any_project"))
    {
      //$query.= "AND (project_person.personid = ".$user["id"]." OR project.timereg_limit = ".PRJ_TIMEREG_ALL_USERS.")";
      $query->addCondition("(project_person.personid = ".$user["id"]." OR project.timereg_limit = ".PRJ_TIMEREG_ALL_USERS.")");
    }

    $query->addOrderBy("hours.entrydate DESC");
    // 100 should be enough to get 5 unique recent projects.
    //$query.= "ORDER BY hours.entrydate DESC";

    //$db = &atkGetDb();
    $arr = $db->getrows($query->buildSelect(),'0','100');
    $j=0;
    $alreadyhad = Array();
    $newarr = Array();
    for ($i=0,$_i=count($arr);$i<$_i&&$j<$GLOBALS["config_numberofrecentprojects"];$i++)
    {
      if (!in_array($arr[$i]['id'],$alreadyhad))
      {
        $alreadyhad[$j]=$arr[$i]['id'];
        $newarr[$j]['id'] = $arr[$i]['id'];
        $newarr[$j]['name'] = $arr[$i]['name'];
        $newarr[$j]['projectname'] = $arr[$i]['projectname']; //update because of phases
        $j++;
      }
    }
    return $newarr;
  }

  /**
   * Retrieve a phase record by its phaseid
   *
   * Retrieve a phase record by its phaseid
   *
   * @param int phaseid a phase id
   * @return array record of the phase
   */
  function getPhase($phaseid)
  {
    $db = &atkGetDb();
    $query = "SELECT phase.id, phase.name, project.name as projectname FROM project,phase ";
    $query.= "WHERE phase.projectid=project.id AND phase.id = $phaseid";
    $phase = $db->getrows($query);
    return $phase[0];
  }

  /**
   * Retrieve activities by an array of phase ids
   *
   * Retrieve all activities that belong to one of the phases in the array 
   * phaseid_arr.
   *
   * @param array phaseid_arr an array with phase ids
   * @return array records with activities
   */
  function getActivities($phaseid_arr)
  {
    $activitynode = &atkGetNode("project.activity");
    return $activitynode->selectDb("activity.phaseid IN (".implode(",",$phaseid_arr).")","activity.name");
  }
  
  /**
   * create an empty activity element
   *
   * Create an empty activity element.
   *
   * @param type name description
   * @return type description
   */
  function createEmptyActivity()
  {
    return array (           
           "id"         => "0",
           "name"       => atktext("no_activity_selected","project","projects"),
           "phaseid"    => array("name"=>"","id"=>0),
           "activitytype" => array("name"=>"","id"=>0,"description"=>"","remarkrequired"=>false)
           );    
  }
 
  
  function javascript($phasearray)
  {
    
    $result = '<script type="text/javascript" language="javascript">';
    
    $result.= 'if (parseInt(navigator.appVersion)>=3) version="n3"; else version="n2";

               function entry(activityid,activityname) { this.actid=activityid; this.actname=activityname; }
               activityarray = new Array();
               Array.prototype.count=0;               
               Array.prototype.id=0;';

    reset($phasearray);    

    for($i=0,$_i=count($phasearray);$i<$_i;$i++)
    { 
      $phaseid = $phasearray[$i]["id"];
      $activities = $phasearray[$i]["activities"];
      
      $result.='activityarray['.$i."] = new Array();\n";
      $result.='activityarray['.$i.'].id ='.$phaseid.";\n";

      for ( $j=0; $j<count($activities); $j++)
      {
        $descrip = $activities[$j]['name'];
        if ($activities[$j]['description']!="")
        {
          $descrip.=" (".$activities[$j]['description'].")";
        }
        $result.= 'activityarray['.$i.']['.$j.'] = new entry('.$activities[$j]['id'];
        $result.= ',"'.addslashes($descrip)."\");\n";
      }
      $result.= 'activityarray['.$i.'].count = ' .count($activities).";\n";      
    }

    $result.='function set_activities(projectid, selectedactivityid)
              {                 
                if(version=="n3")
                {
                  // Find project.
                  var projectindex=0;
                  for(var i=0;i<activityarray.length;i++)
                  {
                    if (activityarray[i].id == projectid) projectindex = i;
                  }

                  with (document.entryform["'.$this->fieldName().'[activityid]"])
                  {                    
                    for (var i = options.length; i >= 0; i--) options[i] = null;
                    
                    var somethingselected=false;

                    if (projectid!=0)
                    {
                      for(var i=0;i<activityarray[projectindex].count;i++)
                      {
                        options[i] = new Option(activityarray[projectindex][i].actname,activityarray[projectindex][i].actid);
                        if (activityarray[projectindex][i].actid == selectedactivityid)
                        {
                          options[i].selected=true;
                          document.entryform["'.$this->fieldName().'[activityid]"].options.selectedIndex=i;
                          somethingselected = true;
                        }
                      }
                    }
                    else
                    {
                      options[0] = new Option("'.atktext("firstselectproject").'",0);
                    }
                    if (!somethingselected) options[0].selected=true;
                  }
                }
              }              
              </script>';
    
    return $result; 
  }
  
  function dbFieldType()
  {
    return "number";
  }

  function equal($recA, $recB)
  {
    return $recA[$this->fieldName()]["phaseid"]==$recB[$this->fieldName()]["phaseid"];
  }
  
  function validate(&$record, $mode)
  {    
        
  }
}
?>