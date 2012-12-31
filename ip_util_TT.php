<?php

// SimpleTest用基底ファイル

require_once '/opt/www/simpletest/unit_tester.php';
require_once '/opt/www/simpletest/reporter.php';

// 対象になるクラスをinclude
require_once('ip_util.inc');

class simple_test_TT extends UnitTestCase
{
//
public function __construct()
{
  $this->UnitTestCase();
}

// 
public function test()
{
  // データを用意
  // true 各種
  $tdata = array(
    '69.208.0.0/4 , 64.0.0.0',
    '69.208.0.0/4 , 70.0.0.0',
    '69.208.0.0/4 , 79.255.255.255',
    '69.208.0.0/8 , 69.0.0.0',
    '69.208.0.0/8 , 69.100.0.0',
    '69.208.0.0/8 , 69.255.255.255',
    '69.208.0.0/24 , 69.208.0.0',
    '69.208.0.0/24 , 69.208.0.100',
    '69.208.0.0/24 , 69.208.0.255',
    '69.208.0.0/28 , 69.208.0.0',
    '69.208.0.0/28 , 69.208.0.10',
    '69.208.0.0/28 , 69.208.0.15',
    '69.208.0.0/30 , 69.208.0.0',
    '69.208.0.0/30 , 69.208.0.2',
    '69.208.0.0/30 , 69.208.0.3',
    '69.208.0.0/32 , 69.208.0.0',
    '69.208.0.0 , 69.208.0.0',
  );
//var_dump($tdata);

  // false 各種
  $fdata = array(
    '69.208.0.0/4 , 63.255.255.255',
    '69.208.0.0/4 , 80.0.0.0',
    '69.208.0.0/32 , 69.208.0.1',
    '69.208.0.0 , 69.208.0.1',
  );

  //
  $data = array('true' => $tdata, 'false' => $fdata);
//var_dump($data);

  // 分回してチェック
  foreach($data as $k => $v) {
    if ('true' === $k) {
      $cbool = true;
    } else {
      $cbool = false;
    }
    foreach($v as $wk) {
      $awk = explode(',', $wk);
      $net = trim($awk[0]);
      $ip  = trim($awk[1]);
      $ret = ip_util::ip_in_network($ip, $net);
      $this->assertIdentical($ret, $cbool);
//var_dump($cbool);
if ($ret !== $cbool) { print "error? " . $wk . "\n"; }
    }

  }


}
}

// 実働部分。変更不要
$test = new simple_test_TT;
$test->run(new TextReporter());

