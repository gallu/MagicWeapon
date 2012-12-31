<?php

require_once('cgi_param.inc');

$p = new cgi_param;

$p->init();
$p->add("test1","value1-1");
$p->add("test1","value1-2");
$p->add("test1","value1-3");
$p->add("test1","value1&=3");
$p->add("test2","value2");
$p->add("test3","value3");

$param = $p->get();
print "param is " . $param . "\n";

$v = $p->finds("test1");
print "test1 is ... ";
foreach ($v as $wk) {
  print $wk . ", ";
}
print "\n";

$s = $p->find("test1");
print "test1 is " . $s . "\n";
print "--\n";

$s = $p->find("test2");
print "test2 is " . $s . "\n";
print "--\n";

$s = $p->find("test_ng");
print "test_ng is " . $s . "\n";
print "--\n";

$p->add_once("test2","value222222222222222222222");
$s = $p->find("test2");
print "test2 is " . $s . "\n";
print "--\n";

$p->init();
$p->set("i=VSp3tBph5V&m=nCkFf68L64bltmd2");

$m = $p->find("m");
print "m is " . $m . "\n";
print "--\n";


