<?php
/**
 * 「SQLを作る」MySQL用クラス
 *
 * SQL文(オブジェクト)を作成する
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/


require_once('mw_makesql_place_holder.inc');

class mw_makesql_place_holder_mysql extends mw_makesql_place_holder
{


/**
 * 範囲指定(LIMIT句)用文字列の作成
 *
 * 形式としては「LIMIT {$to} OFFSET {$from}」というstringを作る
 * XXX プレースホルダにしてないんだけど…まぁ「数値であることを強制してる」からいいかなぁ…どしよ…
 *
 * @deprecated
 * @access public
 * @param int $from LIMITの第一引数
 * @param int $to LIMITの第二引数
 * @return object mw_sqlオブジェクト
 */
public function limited_range($from, $to)
{
  return $this->limited_range_mysql($from, $to);
}


/**
 * 「テーブルの存在確認」用SQL作成
 *
 * XXX プレースホルダ使えないねぇ…mw_makesql_escaped_mysqlにあるロジックと一緒だ…どしよ…
 *
 * @access public
 * @param string $table_name テーブル名
 * @return object mw_sqlオブジェクト
 */
public function make_table_exists_sql($table_name)
{
  // SQL作って
  $sql = 'show tables like ' . "'{$table_name}';";

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
 * @param array $data_type 各カラムのデータ型。カラム名がkeyで、形式としてはmap<string, int>
 * @return object mw_sqlオブジェクト
 */
public function make_upsert($table_name, array $value, array $data_type)
{
  //
  $sql_obj = new mw_sql();

  //
  $vname = array();
  $place_holder = array();

  foreach ($value as $key => $val) {
    // カラム名
    $vname[] = $key;

    // バインドする場所の名前
    $s = ":{$key}";
    $place_holder[] = $s;

    // 値をバインド
    $sql_obj->bind($s, $val, $data_type[$key]);
  }

  //
  $sql =  "REPLACE INTO " . $table_name . " (" . implode($vname, ",") . ") VALUES (" . implode($place_holder, ",") . ");";
  $sql_obj->set_sql($sql);

  //
  return $sql_obj;
}

} // end of class
