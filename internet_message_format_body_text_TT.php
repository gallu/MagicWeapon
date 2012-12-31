<?php

require_once '/opt/www/simpletest/unit_tester.php';
require_once '/opt/www/simpletest/reporter.php';

// 対象になるクラスをinclude
require_once('internet_message_format_body_text.inc');

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
  $obj = new internet_message_format_body_text;

  $s = "test\ntest\r\ntest";
  //
  $this->assertIdentical($obj->get_to_string(), "");
  $obj->set_from_string($s);
  $this->assertIdentical($obj->get_to_string(), $s);

}

}

// 実働部分。変更不要
$test = new simple_test_TT;
$test->run(new TextReporter());

