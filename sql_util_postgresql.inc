<?php
/**
 * SQL ユーティリティクラス PostgreSQL
 *
 * staticメンバー一式。
 * 実際にはstaticじゃないけど、扱いはそういう風に。
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/
require_once('sql_util.inc');

class sql_util_postgresql extends sql_util {
//public:

// 日付関係
/////////////////////

// 年によるサーチ
public function eq_year($name, $year)
{
  return 'EXTRACT (' . $name . ' FROM year) = ' . $year ;
}

// 月によるサーチ
public function eq_month($name, $month)
{
  return 'EXTRACT (' . $name . ' FROM month) = ' . $month ;
}

// 日によるサーチ
public function eq_day($name, $day)
{
  return 'EXTRACT (' . $name . ' FROM day) = ' . $day ;
}

// 時によるサーチ
public function eq_hour($name, $hour)
{
  return 'EXTRACT (' . $name . ' FROM hour) = ' . $hour ;
}

// 分によるサーチ
public function eq_min($name, $min)
{
  return 'EXTRACT (' . $name . ' FROM minute) = ' . $min ;
}

// 秒によるサーチ
public function eq_sec($name, $sec)
{
  return 'EXTRACT (' . $name . ' FROM second) = ' . $sec ;
}

// 週によるサーチ
// XXX 0 = 日曜日, 1 = 月曜日, ... 6 = 土曜日
public function eq_week($name, $week_no)
{
  return 'EXTRACT (' . $name . ' FROM dow) = ' . $week_no ;
}


/**
 * 範囲指定(LIMIT句)の作成
 *
 * 形式としては「LIMIT {$to} OFFSET {$from}」というstringを作る
 *
 * @access public
 * @param int $from LIMITの第一引数
 * @param int $to LIMITの第二引数
 * @return string SQL文
 */
public function limited_range($from, $to)
{
  return 'LIMIT ' . $to . ' OFFSET ' . $from;
}

} // end of class

