<?php

require_once('config_extension.inc');

$ob = new config_extension;
$ob->set_file("./config_extension.conf");

print $ob->find("test4") . "\n";
$ret = $ob->find_array("test");
foreach ($ret as $wk) {
  print "$wk, ";
}
print "\n";

