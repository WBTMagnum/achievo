<?php
  $setup->installNode("scheduler.scheduler_category");
  $setup->installNode("scheduler.scheduler_userpreferences");
  $setup->installNode("scheduler.scheduler_attendees");
  $setup->installNode("scheduler.scheduler_notes");
  $setup->installNode("scheduler.scheduler_cyclus_not");
  $setup->installNode("scheduler.scheduler");
  $setup->installNode("scheduler.scheduler_holidays");
  $setup->installNode("scheduler.scheduler_userassistents");

  $setup->setVersion(2);

?>