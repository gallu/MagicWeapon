<?php
require_once('add_session_id.inc');

$obj = new add_session_id;
$obj->set_body(file_get_contents('./add_session_id_test_text.txt'));
$obj->set_session_id('session');
$obj->set_session_name('s');
$s = $obj->run();
print $s;

$obj->init();
$obj->set_body(file_get_contents('./add_session_id_test_text.txt'));
$obj->set_session_id('session2');
$obj->push_target_suffix('html');
$obj->push_target_suffix('htm');
$s = $obj->run();
print $s;
