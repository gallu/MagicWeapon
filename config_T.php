<?php

require_once('config.inc');

$ob = new config;
$ob->set_file("./config.conf");

print $ob->find("test4") . "\n";
$ret = $ob->find_array("test");
foreach ($ret as $wk) {
  print "$wk, ";
}
print "\n";

?>
