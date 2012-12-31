<?php
/**
 * 「SQLを作る」MySQL用クラス
 *
 * SQL文(文字列)を作成する
 * XXX エスケープ系なので非推奨
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/


require_once('mw_makesql_escaped.inc');

class mw_makesql_escaped_mysql extends mw_makesql_escaped
{


/**
 * 範囲指定(LIMIT句)用文字列の作成
 *
 * 形式としては「LIMIT {$to} OFFSET {$from}」というstringを作る
 *
 * @deprecated
 * @access public
 * @param int $from LIMITの第一引数
 * @param int $to LIMITの第二引数
 * @return string SQL文
 */
public function limited_range($from, $to)
{
  return $this->limited_range_mysql($from, $to);
}


/**
 * 「テーブルの存在確認」用SQL作成
 *
 * @access public
 * @param string $table_name テーブル名
 * @return string/object SQL文またはmw_sqlオブジェクト
 */
public function make_table_exists_sql($table_name, array $where)
{
  // SQL作って
  $sql = 'show tables like ' . $this->escape_add_quote($table_name);

  // return
  return $sql;
}


/**
 * upsert構文作成
 *
 * upsertは「PKでレコードがあればupdate、なければinsert」な動きをするもの
 *
 * @access public
 * @param string $table_name テーブル名
 * @param array $value 各カラムの値。カラム名がkeyで、形式としてはmap<string, string>
 * @return string SQL文
 */
public function make_upsert($table_name, array $value)
{
  $vname = array();
  $vvalue = array();

  foreach ($value as $key => $val) {
//print $key . "\n";
    $vname[] = $key;
    $vvalue[] =  $this->escape_add_quote($val);
  }

  //
  $sql =  "REPLACE INTO " . $table_name . " (" . implode($vname, ",") . ") VALUES (" . implode($vvalue, ",") . ");";

  //
  return $sql;
}



}
