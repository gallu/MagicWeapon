<?php

// SimpleTest用基底ファイル

require_once '/opt/www/simpletest/unit_tester.php';
require_once '/opt/www/simpletest/reporter.php';

// 対象になるクラスをinclude
require_once('target_class.inc');

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
  $obj = new target_class;

  // 等しい系テスト
  $this->assertIdentical($obj->method(), false);

  // 配列は「diffって空なら一致」って方向で
  // この方法は順不同なので一応その辺注意
  $awk = 対象になる配列;
  $twk = array_diff($awk, array("value1-1", "value1-2", "value1-3"));
  $this->assertIdentical($twk, array());

  // NULL系
  $this->assertNull($obj->method());
  $this->assertNotNull($obj->method());

}
}

// 実働部分。変更不要
$test = new simple_test_TT;
$test->run(new TextReporter());

