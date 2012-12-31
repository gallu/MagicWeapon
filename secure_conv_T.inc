<?php
require_once("secure_conv.inc");
require_once("file_util.inc");

$c = new secure_conv;
$c->monoDic("name", '<A href="./test">te&st</A>');
//$c->monoDic_unsecure_raw("name", '<A href="./test">te&st</A>');

$c->T_put();
print "\n";
print "----  CONV Start ---------------------------------------\n";
print $c->conv(file_get_contents("./conv_T.tmp"));


