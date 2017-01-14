<?php
/*
 * multimap
 */

//
use PHPUnit\Framework\TestCase;

// 対象になるクラスをinclude
require_once('multimap.inc');

//
class multimapTest extends TestCase {
  /*
   * テスト前処理
   */
  public static function setUpBeforeClass(){
    static::$obj = new multimap();
  }

  /*
   * ０状態でfindチェック
   * @test
   */
  public function test_zero_find() {
    static::$obj->clear();
    $this->assertSame(static::$obj->find('test'), -1);
    $this->assertSame(static::$obj->find_array('test'), array());
    $this->assertSame(static::$obj->isempty(), true);
    $this->assertSame(static::$obj->size(), 0);
    static::$obj->clear();
  }

  /*
   * データ挿入
   * @test
   * @depends test_zero_find
   */
  public function test_insert() {
    // データを突っ込む
    static::$obj->insert('test-name1', 'test-value1-1');
    static::$obj->insert('test-name1', 'test-value1-2');
    static::$obj->insert('test-name1', 'test-value1-3');
    static::$obj->insert('test-name2', 'test-value2-1');
    static::$obj->insert('test-name2', 'test-value2-2');
    static::$obj->insert('test-name3', 'test-value3-1');
    static::$obj->insert('test', 'test-value');

    //
    $this->assertSame(static::$obj->isempty(), false);
    $this->assertSame(static::$obj->size(), 7);
  }

  /*
   * データ挿入確認
   * @test
   * @depends test_insert
   */
  public function test_insert_check() {
    // 全部 イテレータでぶんまわし
    $names = array();
    $values = array();
    for($itr = static::$obj->begin(); $itr != static::$obj->end(); $itr ++) {
      // 文字列合成
      $names[] = static::$obj->real($itr)->get_first();
      $values[] = static::$obj->real($itr)->get_second();
    }
//var_dump($names);
//var_dump($values);
    $twk = array_diff($names, array ( "test-name1", "test-name1", "test-name1", "test-name2", "test-name2", "test-name3", "test"));
    $this->assertSame($twk, array());

    $twk = array_diff($values, array ( "test-value1-1", "test-value1-2", "test-value1-3", "test-value2-1", "test-value2-2", "test-value3-1", "test-value",));
    $this->assertSame($twk, array());
  }

  /*
   * Key指定確認
   * @test
   * @depends test_insert_check
   */
  public function test_find_key() {
    // 複数
    $itrs = static::$obj->find_array('test-name1');
//var_dump($values);
    $values = array();
    foreach($itrs as $itr) {
      // 文字列合成
      $values[] = static::$obj->real($itr)->get_second();
    }
//var_dump($values);
    $twk = array_diff($values, array ( "test-value1-1", "test-value1-2", "test-value1-3"));
    $this->assertSame($twk, array());

    // 単数
    $itr = static::$obj->find('test');
    $this->assertSame(static::$obj->real($itr)->get_second(), 'test-value');
  }

  /*
   * Key配列の取得
   * @test
   * @depends test_find_key
   */
  public function test_key_array() {
    $keys = static::$obj->get_all_keys();
    $twk = array_diff($keys, array ( "test-name1", "test-name2", "test-name3", "test"));
    $this->assertSame($twk, array());
  }

  /*
   * Key指定削除
   * @test
   * @depends test_key_array
   */
  public function test_key_delete() {
    $key_name = 'test-name1';
    $ret = static::$obj->find($key_name);
    $this->assertNotSame($ret, -1);
    static::$obj->erase_key($key_name);
    $ret = static::$obj->find($key_name);
    $this->assertSame($ret, -1);
  }

//
private static $obj;
}
