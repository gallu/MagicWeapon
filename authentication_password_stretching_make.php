<?php

// 引数に「ID」と「パスワード」を指定すると「ストレッチされたパスワード」をreturnする
// XXX あとでconfig見たりするように変更したほうがよいかねぇ？

require_once('authentication_password_stretching.inc');


$obj = new authentication_password_stretching();
var_dump($argv[1]);
var_dump($argv[2]);

$s = $obj->stretching($argv[1], $argv[2]);
print $s . "\n";


