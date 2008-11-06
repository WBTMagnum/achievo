<?php

/**
 * Module Calendar
 * Class schedule_attendee
 *
 * This class makes it possible to add attendees to a schedule. These attendees can be employees and contacts.
 * That is why the relation is linked to person.
 *
 * $Id$
 * @author L. Willems  <lineke@ibuildings.nl>
 * @version $Revision$
 *
 */

userelation("atkmanytoonerelation");

class schedule_attendee extends atkNode
{
  function schedule_attendee()
  {
    $this->atkNode("schedule_attendee");

    $this->add(new atkManyToOneRelation("schedule_id", "calendar.schedule", AF_PRIMARY|AF_HIDE));    
    $this->add(new atkManyToOneRelation("person_id", "person.select_person", AF_PRIMARY|AF_LARGE));

    $this->setTable("schedule_attendee");
    $this->setSecurityAlias("calendar.schedule");
  }
}

?>