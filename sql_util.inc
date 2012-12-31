<?php
/**
 * SQL ユーティリティクラス 基本
 *
 * staticメンバー一式。実装は各DBMSクラスにお任せ
 * 実際にはstaticじゃないけど、扱いはそういう風に。
 *
 * @deprecated
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once('security.inc');

abstract class sql_util {
//public:


/**
 * insert構文作成
 *
 * @deprecated
 * @access public
 * @param string $table_name テーブル名
 * @param string array $value 各カラムの値。カラム名がkeyで、形式としてはmap<string, string>
 * @return string SQL文
 */
static public function make_insert($table_name, array $value)
{

  $vname = array();
  $vvalue = array();

  foreach ($value as $key => $val) {
//print $key . "\n";
    $vname[] = $key;
    $vvalue[] =  security::sanitize_db_add_quote($val);
  }

  //
  $sql =  "INSERT INTO " . $table_name . " (" . implode($vname, ",") . ") VALUES (" . implode($vvalue, ",") . ");";

  //
  return $sql;
}

/**
 * update構文作成
 *
 * @deprecated
 * @access public
 * @param string $table_name テーブル名
 * @param string array $value 各カラムの値。カラム名がkeyで、形式としてはmap<string, string>
 * @param string array $where WHERE句用の各カラムの値。カラム名がkeyで、形式としてはmap<string, string>
 * @return string SQL文
 */
static public function make_update($table_name, array $value, array $where)
{

//print "in to make_update\n";
  $v = array();
  $ret = "";

  //
  foreach ($value as $key => $val) {
    $v[] =  $key . "=" . security::sanitize_db_add_quote($val);
  }

  $sql = "UPDATE " . $table_name . " SET " . implode($v, ",");
  $s = sql_util::make_where($where);
  if ("" != $s) {
    $sql = $sql . " WHERE " . $s;
  }

  //
  $sql = $sql . ";";
  return $sql;
}

/**
 * delete構文作成
 *
 * @deprecated
 * @access public
 * @param string $table_name テーブル名
 * @param string array $where WHERE句用の各カラムの値。カラム名がkeyで、形式としてはmap<string, string>
 * @return string SQL文
 */
static public function make_delete($table_name, array $where)
{

  //
  $sql = "DELETE FROM " . $table_name;
  $s = SQL_UTIL::make_where($where);
  if ("" != $s) {
    $sql = $sql . " WHERE " . $s;
  }

  //
  $sql = $sql . ";";
  return $sql;
}

/**
 * where句作成
 *
 * @deprecated
 * @access public
 * @param string array $where WHERE句用の各カラムの値。カラム名がkeyで、形式としてはmap<string, string>
 * @return string SQL文のうち、WHERE句部分
 */
static public function make_where(array $where)
{

//print "in to make_where\n";
  $w = array();

  foreach ($where as $key => $val) {
    $w[] =  $key . "=" . security::sanitize_db_add_quote($val);
  }
  //
  $s = implode($w, " and ");

//print "out to make_where\n";
  return $s;
}


// 日付関係
/////////////////////

// 年によるサーチ
abstract public function eq_year($name, $year);
// 月によるサーチ
abstract public function eq_month($name, $month);
// 日によるサーチ
abstract public function eq_day($name, $day);
// 時によるサーチ
abstract public function eq_hour($name, $hour);
// 分によるサーチ
abstract public function eq_min($name, $min);
// 秒によるサーチ
abstract public function eq_sec($name, $sec);
// 週によるサーチ
// XXX 0 = 日曜日, 1 = 月曜日, ... 6 = 土曜日
abstract public function eq_week($name, $week_no);


// 範囲指定
// (第一引数)行目から(第二引数行)まで
// 0,1 なら、先頭１行を返すこと
abstract public function limited_range($from, $to);


// 期間指定
// CALENDER オブジェクト使用
public function term($rec_name, $from_obj, $to_obj)
{
  $ret = 
    '\'' . $from_obj->get_string() . '\' <= ' . $rec_name . ' and ' .
     $rec_name . ' <= \'' . $to_obj->get_string() . '\'';

  //
  return $ret;
}

} // end of class

