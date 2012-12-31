<?php

require_once("f_pgsql.inc");

// アタッチ
  $pg = new dbh_pgsql;
  $pg->set_user("postgres");
  $pg->set_pass("");
  $pg->set_database_name("test");
  $pg->set_host_name("localhost");

  if (!($pg->connect())) {
print $pg->get_error_message() . "\n";
    exit;
  }

  if (($buf = $pg->query("SELECT * FROM aaa;")) ) {
    while( $buf->fetch() ) {
      print $buf->get_data(0) . " : " . $buf->get_data(1) . "\n";
    }
  } else {
    print "get error!!! ...\n";
    print $pg->get_error_message() . "\n";
  }


  $pg->disconnect();
?>
