<?php
require_once('db_util.inc');
require_once("f_mysql.inc");

    // とりあえずDBハンドルはねぇ
    $dbh = new dbh_mysql;
    $dbh->set_user("root");
    $dbh->set_pass("");
    $dbh->set_database_name("test");
    $dbh->set_host_name("localhost");
    $dbh->connect(); // ここのエラーチェックは省略するよ？

$sql = 'select * from session_data;';

$s = db_util::get_mono_data($dbh, $sql);
var_dump($s);

$s = db_util::get_row_data($dbh, $sql);
var_dump($s);

$s = db_util::get_row_data_array($dbh, $sql);
var_dump($s);
