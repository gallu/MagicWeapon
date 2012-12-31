<?php

require_once('mobile_info.inc');

$obj = new mobile_info;

//$fp = fopen('./user_agent', "r");
//$fp = fopen('./mobile_info_data.user_agent', "r");
$fp = fopen('./a', "r");
while($s = fgets($fp)) {
  $s = 'HTTP_USER_AGENT=' . trim($s);
  putenv($s);
  //print "\n$s\n";
  $obj->init();

/*
  //
  if ($obj->is_docomo()) {
    print "docomo\t";
  } else
  if ($obj->is_ez()) {
    print "ez\t";
  } else
  if ($obj->is_softbank()) {
    print "softb\t";
  }
  print $obj->get_type();
  print "\t";
  print $obj->get_sid();
  print "\n";
*/
  if (true === $obj->is_hdml()) {
    print "$s \n";
  }
}


