<?php
require_once('multimap.inc');

$obj = new multimap;

  // データを突っ込む
  $obj->insert('test-name1', 'test-value1-1');
  $obj->insert('test-name1', 'test-value1-2');
  $obj->insert('test-name1', 'test-value1-3');
  $obj->insert('test-name2', 'test-value2-1');
  $obj->insert('test-name2', 'test-value2-2');
  $obj->insert('test-name3', 'test-value3-1');
  $obj->insert('test', 'test-value');

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

  // key指定
  $values = $obj->find_array('test-name1');
//var_dump($values);

  // 単値
  $s = $obj->find('test');
var_dump($s);
  $s = $obj->find('test-name1');
var_dump($s);


