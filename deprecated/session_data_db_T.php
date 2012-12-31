<?php

// XXX
require_once('/opt/www/mw.conf');


require_once("f_mysql.inc");
require_once("session_data_db.inc");

  $pg = new dbh_mysql;
  $pg->set_user("root");
  $pg->set_pass("");
  $pg->set_database_name("test");
  $pg->set_host_name("localhost");
  //$pg->set_host_name("192.168.198.10");
  //$pg->set_port_num("/var/run/mysql/mysql.sock");
  if (!($pg->connect())) {
print "db connect NG\n";
    exit;
  }

$sb = new session_data_db;

  $sb->set_db($pg);
  $sb->set_id("testsid");
  //$sb->set_validity_term(1);

  if ($sb->fix_session()) {
    print "fix OK:存在しないIDだったから追加したよん\n";
  } else {
    print "fix NG:そのIDすでにあるってばよ\n";
  }


  //
  $sb->init();
  $sb->add("test1","value1-1");
  $sb->add("test1","value1-2");
  $sb->add("test1","value1-3");
  $sb->add("test2","value2");
  $sb->add("test3","value3");
  $sb->write();

sleep(2);

  if ($sb->fix_session()) {
    print "fix OK:存在しないIDだったから追加したよん\n";
  } else {
    print "fix NG:そのIDすでにあるってばよ\n";
  }

print "init()" . "\n";
  $sb->init();

  $s = $sb->find("test1");
print $s . "\n";


print "read()" . "\n";
  $sb->set_id("testsid");
  if (!($sb->read())) {
    print "read error!!\n";
  }
  $s = $sb->find("test1");
print "test1 is " . $s . "\n";


  if ($sb->is_session()) {
    print "is true...OK\n";
  } else {
    print "is false...NG\n";
  }

  $sb->del();

  if ($sb->is_session()) {
    print "is true...NG\n";
  } else {
    print "is false...OK\n";
  }

  $pg->disconnect();

