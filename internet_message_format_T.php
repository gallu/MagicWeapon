<?php

require_once('internet_message_format.inc');

$mobj = new internet_message_format;

$mobj->set_from('furu@m-fr.net', "古庄");
$mobj->push_to('m.furusho@gmail.com');
$mobj->set_subject('テスト');
$mobj->set_body('ほげらむげら');

$s = $mobj->get_mail_string();

var_dump($s);
