<?php

require_once('empty_mail_analysis.inc');

$obj = new empty_mail_analysis;

$obj->set_token_separator('-');
//$obj->analysis();
$obj->analysis_from_stdin();

$from = $obj->get_mail_from();
$to = $obj->get_mail_to();
$tkn = $obj->get_mail_to_token();

print "from: $from \n";
print "to  : $to \n";
print "tkn : $tkn \n";
