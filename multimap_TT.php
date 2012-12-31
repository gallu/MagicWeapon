<?php

// SimpleTest用基底ファイル

require_once '/opt/www/simpletest/unit_tester.php';
require_once '/opt/www/simpletest/reporter.php';

// 対象になるクラスをinclude
require_once('multimap.inc');

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
  $obj = new multimap;

  // 先に０状態でfindチェック
  $this->assertNull($obj->find('test'));
  $this->assertIdentical($obj->find_array('test'), array());
  $this->assertIdentical($obj->isempty(), true);
  $this->assertIdentical($obj->size(), 0);
  $obj->clear();

  // データを突っ込む
  $obj->insert('test-name1', 'test-value1-1');
  $obj->insert('test-name1', 'test-value1-2');
  $obj->insert('test-name1', 'test-value1-3');
  $obj->insert('test-name2', 'test-value2-1');
  $obj->insert('test-name2', 'test-value2-2');
  $obj->insert('test-name3', 'test-value3-1');
  $obj->insert('test', 'test-value');

  //
  $this->assertIdentical($obj->isempty(), false);
  $this->assertIdentical($obj->size(), 7);

  // 全部 イテレータでぶんまわし
  $names = array();
  $values = array();
  for($itr = $obj->begin(); $itr != $obj->end(); $itr ++) {
    // 文字列合成
    $names[] = $obj->real($itr)->get_first();
    $values[] = $obj->real($itr)->get_second();
  }
//var_dump($names);
//var_dump($values);
  $twk = array_diff($names, array ( "test-name1", "test-name1", "test-name1", "test-name2", "test-name2", "test-name3", "test"));
  $this->assertIdentical($twk, array());

  $twk = array_diff($values, array ( "test-value1-1", "test-value1-2", "test-value1-3", "test-value2-1", "test-value2-2", "test-value3-1", "test-value",));
  $this->assertIdentical($twk, array());

  // key指定
  $itrs = $obj->find_array('test-name1');
//var_dump($values);
  $values = array();
  foreach($itrs as $itr) {
    // 文字列合成
    $values[] = $obj->real($itr)->get_second();
  }
//var_dump($values);
  $twk = array_diff($values, array ( "test-value1-1", "test-value1-2", "test-value1-3"));
  $this->assertIdentical($twk, array());


  // 単値
  $itr = $obj->find('test');
  $this->assertIdentical($obj->real($itr)->get_second(), 'test-value');

  // key配列の取得
  $keys = $obj->get_all_keys();
  $twk = array_diff($keys, array ( "test-name1", "test-name2", "test-name3", "test"));
  $this->assertIdentical($twk, array());
//var_dump($keys);

  // key値指定削除
  $obj->erase_key('test-name1');
  $ret = $obj->find_array('test-name1');
//var_dump($ret);

/*
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
*/

}
}

// 実働部分。変更不要
$test = new simple_test_TT;
$test->run(new TextReporter());

