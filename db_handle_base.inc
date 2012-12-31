<?php

/**
 * RDBハンドルクラス基底
 *
 * RDB系ハンドルの基底部分を扱います
 * abstractクラスなので、必ず派生させてください
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once('data_handle.inc');

abstract class db_handle_base extends data_handle{

/**
 * このハンドルがRDB系かどうか
 *
 * @access public
 * @return boolean
 */
public function is_rdb() { return true; }

/**
 * SQLからテーブル名を抜き出す
 *
 * XXX 複数テーブルには未対応なんで要注意
 *
 * @access public
 * @param string $sql SQL
 * @return string テーブル名。抜き出せなかったら空文字
 */
public function get_table_name($sql)
{
  // テーブル名の取得
$table = '';
$flg = false;
  // SQLをとりあえず分解
  $awk = explode(' ', trim($sql));
  // タイプごとに下準備
  $t = '';
  if ( (0 === strcasecmp('select', $awk[0]))||(0 === strcasecmp('delete', $awk[0])) ) {
    $t = 'from';
  } else
  if (0 === strcasecmp('insert', $awk[0])) {
    $t = 'into';
  } else
  if (0 === strcasecmp('update', $awk[0])) {
    $t = 'update';
  }

  // テーブル名の抜き出し
/*
// テストしたパターン
//
$s = ' SELECT CustomerID, CompanyName, City   FROM   table_name  WHERE City = London';
//
$s = '  DELETE FROM d_table_name;';
//
$s = 'INSERT INTO    i1_table_name(a , a ,.......)       VALUES(a , a ,.......);';
$s = 'INSERT INTO  table_name SET a=1, b=2, ...;';
//
$s = 'UPDATE u_table_name SET SET a=1, b=2, ...;';
*/
  if ('' !== $t) {
    foreach($awk as $wk) {
      //
      if ('' === $wk) { continue; }

      //
      if (true === $flg) {
        $table = $wk;
        if ( ';' === $table[strlen($table) - 1]) {
          $table[strlen($table) - 1] = ' ';
          $table = trim($table);
        }
        // insert用の対策
        $awk = explode('(', $table);
        $table = $awk[0];
        break;
      }

      //
      if (0 === strcasecmp($t, $wk)) {
        $flg = true;
      }
    }
  }
//var_dump($table);

  //
  return $table;
}



} // end of class
