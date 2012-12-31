<?php

// SimpleTest用基底ファイル

require_once '/opt/www/simpletest/unit_tester.php';
require_once '/opt/www/simpletest/reporter.php';

// 対象になるクラスをinclude
require_once('crypt_base64.inc');

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
  $obj  = new crypt_base64();
  $obj2 = new crypt_base64();

  //
  $data = array(
    'asoejr9873',
    'あいうえおかきくけこ',
    '漢字交じり',
    '漢字iii交じり',
  );

  // 等しい系テスト
  foreach($data as $d)
  {
    $obj->init();
    $obj->set_key('key');
    $obj->set_block_mode('cbc');
    $obj->set_plain_text($d);
    $obj->encrypt();
    $dd = $obj->get_encrypted_unit();

    $obj2->init();
    $obj2->set_key('key');
    $obj2->set_block_mode('cbc');
    $obj2->set_encrypted_unit($dd);
    $obj2->decrypt();
    $ee = $obj2->get_plain_text();

    //
    $this->assertIdentical($d, $ee);
  }


}
}

// 実働部分。変更不要
$test = new simple_test_TT;
$test->run(new TextReporter());

