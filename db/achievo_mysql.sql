CREATE TABLE accessright (
   node varchar(25) NOT NULL,
   action varchar(25) NOT NULL,
   entity int(10) DEFAULT '0' NOT NULL,
   PRIMARY KEY (node, action, entity)
);

CREATE TABLE activity (
   id int(10) unsigned DEFAULT '0' NOT NULL,
   name varchar(50) NOT NULL,
   description varchar(50),
   remarkrequired tinyint(1) DEFAULT '0',
   PRIMARY KEY (id)
);

CREATE TABLE customer (
   id int(10) unsigned DEFAULT '0' NOT NULL,
   name varchar(100) NOT NULL,
   address varchar(100),
   zipcode varchar(20),
   city varchar(100),
   country varchar(100),
   phone varchar(20),
   fax varchar(20),
   email varchar(50),
   website varchar(100),
   bankaccount varchar(30),
   remark text,
   currency varchar(10),
   PRIMARY KEY (id)
);

CREATE TABLE contact (
   id int(10) unsigned DEFAULT '0' NOT NULL,
   lastname varchar(50),
   firstname varchar(50),
   address varchar(100),
   zipcode varchar(20),
   city varchar(100),
   country varchar(100),
   phone varchar(20),
   cellular varchar(20),
   fax varchar(20),
   email varchar(50),
   remark text,
   company int(10) DEFAULT '0',
   owner varchar(15),
   PRIMARY KEY (id)
);

CREATE TABLE contract (
   id int(10) unsigned DEFAULT '0' NOT NULL,
   billing_period varchar(30) NOT NULL,
   period_price decimal(13,5) DEFAULT '0.00' NOT NULL,
   contracttype int(4) DEFAULT '0' NOT NULL,
   customer int(10) DEFAULT '0' NOT NULL,
   startdate date DEFAULT '0000-00-00' NOT NULL,
   enddate date DEFAULT '0000-00-00' NOT NULL,
   description text,
   PRIMARY KEY (id)
);

CREATE TABLE contracttype (
   id int(10) unsigned DEFAULT '0' NOT NULL,
   description varchar(100) NOT NULL,
   PRIMARY KEY (id)
);

CREATE TABLE dependency (
   phaseid_row int(10) DEFAULT '0' NOT NULL,
   phaseid_col int(10) DEFAULT '0' NOT NULL,
   PRIMARY KEY (phaseid_row, phaseid_col)
);

CREATE TABLE employee (
   userid varchar(15) NOT NULL,
   supervisor varchar(15),
   name varchar(40),   
   email varchar(100),
   password varchar(40),
   status varchar(15) NOT NULL,
   theme varchar(50),
   entity int(10) DEFAULT '0',
   PRIMARY KEY (userid)
);


CREATE TABLE hours (
   id int(10) unsigned DEFAULT '0' NOT NULL,
   entrydate date DEFAULT '0000-00-00' NOT NULL,
   activitydate date DEFAULT '0000-00-00' NOT NULL,
   phaseid int(10) unsigned DEFAULT '0' NOT NULL,
   activityid int(10) unsigned DEFAULT '0' NOT NULL,
   remark text,
   time int(4) DEFAULT '0' NOT NULL,
   userid varchar(15) NOT NULL,
   PRIMARY KEY (id)
);

CREATE TABLE hours_lock (
   week varchar(6) NOT NULL,
   userid varchar(15) NOT NULL,
   PRIMARY KEY (week, userid)
);

CREATE TABLE phase (
   id int(10) unsigned DEFAULT '0' NOT NULL,
   name varchar(50) NOT NULL,
   projectid int(10) DEFAULT '0' NOT NULL,
   status varchar(15) DEFAULT 'active' NOT NULL,
   description text,
   max_phasetime int(10) DEFAULT '0',
   max_hours int(10) DEFAULT '0',
   PRIMARY KEY (id)
);

CREATE TABLE phase_activity (
   phaseid int(10) unsigned DEFAULT '0' NOT NULL,
   activityid int(10) unsigned DEFAULT '0' NOT NULL,
   billable tinyint(1) DEFAULT '0' NOT NULL,
   PRIMARY KEY (phaseid, activityid)
);

CREATE TABLE profile (
   id int(10) unsigned DEFAULT '0' NOT NULL,
   name varchar(50) NOT NULL,
   PRIMARY KEY (id)
);

CREATE TABLE project (
   id int(10) unsigned DEFAULT '0' NOT NULL,
   name varchar(50) NOT NULL,
   coordinator varchar(20),
   status varchar(15) NOT NULL,
   description text,
   startdate date NULL,
   enddate date NULL,
   customer int(10) DEFAULT '0' NULL,
   fixed_price decimal(13,5) DEFAULT '0.00000' NULL,
   contact INT(10) NOT NULL,
   PRIMARY KEY (id)
);

CREATE TABLE project_notes (
   id int(10) unsigned DEFAULT '0' NOT NULL,
   projectid int(10) unsigned DEFAULT '0' NOT NULL,
   owner varchar(15) NOT NULL,
   entrydate date DEFAULT '0000-00-00' NOT NULL,
   description text,
   title varchar(50) NOT NULL,
   PRIMARY KEY (id)
);

CREATE TABLE tpl_dependency (
   phaseid_row int(10) DEFAULT '0' NOT NULL,
   phaseid_col int(10) DEFAULT '0' NOT NULL,
   projectid int(10) DEFAULT '0' NOT NULL,
   PRIMARY KEY (phaseid_row, phaseid_col, projectid)
);

CREATE TABLE tpl_phase (
   id int(10) unsigned DEFAULT '0' NOT NULL,
   name varchar(50) NOT NULL,
   description text,
   PRIMARY KEY (id)
);

CREATE TABLE tpl_phase_activity (
   phaseid int(10) unsigned DEFAULT '0' NOT NULL,
   activityid int(10) unsigned DEFAULT '0' NOT NULL,
   PRIMARY KEY (phaseid, activityid)
);

CREATE TABLE tpl_project (
   id int(10) unsigned DEFAULT '0' NOT NULL,
   name varchar(50) NOT NULL,
   description text,
   PRIMARY KEY (id)
);

CREATE TABLE tpl_project_phase (
   projectid int(10) DEFAULT '0' NOT NULL,
   phaseid int(10) DEFAULT '0' NOT NULL,
   PRIMARY KEY (projectid, phaseid)
);

CREATE TABLE todo (
   id int(10) unsigned DEFAULT '0' NOT NULL,
   projectid int(10) unsigned DEFAULT '0' NOT NULL,
   owner varchar(15) NOT NULL,
   assigned_to varchar(100) NOT NULL,
   entrydate date DEFAULT '0000-00-00' NOT NULL,
   duedate date DEFAULT '0000-00-00' NOT NULL,
   description text,
   title varchar(50) NOT NULL,
   status tinyint(4) DEFAULT '0' NOT NULL,
   priority tinyint(4) DEFAULT '0' NOT NULL, 
   PRIMARY KEY (id)
);

CREATE TABLE usercontract (
   id int(10) unsigned DEFAULT '0' NOT NULL,
   description text,
   startdate date DEFAULT '0000-00-00' NOT NULL,
   enddate date DEFAULT '0000-00-00' NOT NULL,
   userid varchar(15) NOT NULL,
   uc_hours int(3) DEFAULT '0' NOT NULL,
   PRIMARY KEY (id)
);

CREATE TABLE db_sequence (
   seq_name varchar(20) NOT NULL,
   nextid int(10) unsigned DEFAULT '0' NOT NULL,
   PRIMARY KEY (seq_name)
);

