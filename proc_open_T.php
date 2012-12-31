<?php

include('proc_open.inc');

$obj = new proc_open();
$obj->set_stdin_string('てすと');
$obj->set_cmd_line('nkf -w');

$ret = $obj->execute();
var_dump($ret);

var_dump($obj->get_stdout_string());
