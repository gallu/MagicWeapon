<?php

require_once("f_mysql.inc");

// アタッチ
  $pg = new dbh_mysql;
  $pg->set_user("root");
  $pg->set_pass("");
  $pg->set_databaseName("test");
  //$pg->set_hostName("localhost");

  if (!($pg->connect())) {
    exit;
  }

  if (($buf = $pg->query("SELECT * FROM night;")) ) {
    while( $buf->fetch() ) {
      print $buf->getData(0) . " : " . $buf->getData(1) . "\n";
    }
  } else {
    print "get error!!! ...\n";
    print $pg->get_errMessage() . "\n";
  }


  $pg->deconnect();

