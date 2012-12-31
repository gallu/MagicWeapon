<?php

// MW config
require_once('/opt/www/mw.conf');

//
require_once('db_manager.inc');
require_once('session.inc');

// とりあえずDBハンドルげと
$dbh = db_manager::get_handle('./db.conf');
//var_dump($dbh);

// config設定〜
$obj = new session;
$obj->set_config_filename('./session.conf');
$obj->set_db($dbh);

// ログインしてみる
$id = 'id';
$pass = 'pass';
$ret = $obj->login($id, $pass);
var_dump($ret);
print $obj->get_error_string() . "\n";

$key = $obj->get_key();
//print "key is $key \n";

// 匿名で発行してみる

//--

// セッション維持
$obj2 = new session;
$obj2->set_config_filename('./session.conf');
$obj2->set_db($dbh);

$ret = $obj2->auth($key);
var_dump($ret);
print $obj2->get_error_string() . "\n";

// ユーザの追加：とりあえず問答無用系
$obj3 = new session;
$obj3->set_config_filename('./session.conf');
$obj3->set_db($dbh);
$obj->add_auth($id, $pass [, $status]);
