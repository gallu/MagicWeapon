<?php

require_once('/opt/new/wwwroot/cron_base.inc');
require_once('empty_mail_cushion_db.inc');

$obj = new empty_mail_cushion;
$obj->set_db($dbh);
$obj->set_config_filename('/opt/new/conf/registration.conf');

// 処理(笑
$obj->run();

// XXX メアド取得側
// model継承クラス内で実行
  // メアドげとって
  $obj = new empty_mail_cushion;
  $obj->set_db($dbh);
  $obj->set_config_filename($this->get_config()->find('add_config'));

  // 処理
  // 多分本当はこう
  //$obj->get_なんりゃら($req->find('i'));
  //$email = $obj->get_email();
  //
  $email = $obj->get_email($req->find('i'));
//var_dump($email);

  // XXX 念のため？
  if ("" === $email) {
//print "error (email is empty) \n";
//exit;
    $this->make_body_with_conv('member/add_invalid.tpl');
    return ;
  }

  // iを無効にして
  $obj->delete($req->find('i'));


