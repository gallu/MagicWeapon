<?php

require_once('sql_util.inc');

// テスト用にダミークラス

class sql_util_t extends sql_util {
// 年によるサーチ
public function eq_year($name, $year){}
// 月によるサーチ
public function eq_month($name, $month){}
// 日によるサーチ
public function eq_day($name, $day){}
// 時によるサーチ
public function eq_hour($name, $hour){}
// 分によるサーチ
public function eq_min($name, $min){}
// 秒によるサーチ
public function eq_sec($name, $sec){}
// 週によるサーチ
// XXX 0 = 日曜日, 1 = 月曜日, ... 6 = 土曜日
public function eq_week($name, $week_no){}


public function limited_range($from, $to){}

}

$qwk = array();
$where = array();

  $qwk["name1"] = "data1";
  $qwk["name2"] = "da'ta2";
  $qwk["name3"] = "da\\ta3";
  $qwk["name4"] = "40";
  $qwk["name5"] = "50";

  $where["name_key"] = "data_key";
  $where["name_key2"] = "data_key2";

print "insert\n";
print sql_util_t::make_insert("table1", $qwk) . "\n";
print "\nupdate\n";
print sql_util_t::make_update("table1", $qwk, $where) . "\n";
print "\ndelete\n";
print sql_util_t::make_delete("table1", $where) . "\n";



?>
