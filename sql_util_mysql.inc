<?php
/**
 * SQL ユーティリティクラス MySQL
 *
 * staticメンバー一式。
 * 実際にはstaticじゃないけど、扱いはそういう風に。
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/
require_once('sql_util.inc');

class sql_util_mysql extends sql_util {
//public:

// 年によるサーチ
public function eq_year($name, $year)
{
  return 'YEAR(' . $name . ') = ' . $year;
}

// 月によるサーチ
public function eq_month($name, $month)
{
  return 'MONTH(' . $name . ') = ' . $month;
}

// 日によるサーチ
public function eq_day($name, $day)
{
  return 'DAYOFMONTH(' . $name . ') = ' . $day;
}

// 時によるサーチ
public function eq_hour($name, $hour)
{
  return 'HOUR(' . $name . ') = ' . $hour;
}

// 分によるサーチ
public function eq_min($name, $min)
{
  return 'MINUTE(' . $name . ') = ' . $min;
}

// 秒によるサーチ
public function eq_sec($name, $sec)
{
  return 'SECOND(' . $name . ') = ' . $sec;
}

// 週によるサーチ
// XXX 0 = 日曜日, 1 = 月曜日, ... 6 = 土曜日
public function eq_week($name, $week_no)
{
  // MySQLは１スタートなので、インクリメント
  $week_no ++;
  return 'DAYOFWEEK(' . $name . ') = ' . $week_no;
}


/**
 * 範囲指定(LIMIT句)の作成
 *
 * 形式としては「LIMIT {$from}, {$to}」というstringを作る
 *
 * @access public
 * @param int $from LIMITの第一引数
 * @param int $to LIMITの第二引数
 * @return string SQL文
 */
public function limited_range($from, $to)
{
  return 'LIMIT ' . $from . ',' . $to;
}

} // end of class

