<?php
/**
 * カレンダー作成クラス
 *
 * カレンダーを出力する
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

/*
 概ね以下のようなテンプレートで出力を行う

  <tr id="day">
    <td id="txtred">日</td>
    <td>月</td>
    <td>火</td>
    <td>水</td>
    <td>木</td>
    <td>金</td>
    <td>土</td>
    <td id="empty">&nbsp;</td>
  </tr>
$$$loop$$$
    <tr>
        <td>%%%d0%%%
        <td>%%%d1%%%
        <td>%%%d2%%%
        <td>%%%d3%%%
        <td>%%%d4%%%
        <td>%%%d5%%%
        <td>%%%d6%%%
    </tr>
$$$/loop$$$

  もしボタンにしたい場合、

$$$loop$$$
        <td><input type="submit" name="btnDay" value="%%%d0%%%" %%%opt0%%%>
        <td><input type="submit" name="btnDay" value="%%%d1%%%" %%%opt1%%%>
        <td><input type="submit" name="btnDay" value="%%%d2%%%" %%%opt2%%%>
        <td><input type="submit" name="btnDay" value="%%%d3%%%" %%%opt3%%%>
        <td><input type="submit" name="btnDay" value="%%%d4%%%" %%%opt4%%%>
        <td><input type="submit" name="btnDay" value="%%%d5%%%" %%%opt5%%%>
        <td><input type="submit" name="btnDay" value="%%%d6%%%" %%%opt6%%%>
$$$/loop$$$
とすることで「日付のないボタンは押せなくなる」

*/

require_once('calender.inc');
require_once('conv.inc');

class make_calendar {


function set_year($s) { $this->yy_ = $s; }
function set_month($s) { $this->mm_ = $s; }

function get_year() { return $this->yy_;  }
function get_month() { return $this->mm_; }

function m_conv($s_base,$p) {

  // データリスト
  $list = array();

  $cal_obj = new calender;

  $cal_obj->set_year($this->get_year());
  $cal_obj->set_month($this->get_month());

  // 晦日の取得
  $cal_obj->set_matubi();

  $misoka = $cal_obj->get_day();

  // 指定年月の１日の曜日の取得
  $cal_obj->set_day(1);

  //  戻り値は０?６まで。０->日。あとはてきとーに。
  $fw = $cal_obj->get_week();
//print $obj->get_year() . $obj->get_month() . "\n";
//print "fw is $fw \n";

  // メイン処理
  $dd = 1;         // 日付
  $label = 0;       // 「第ｎ週」用ラベル。無論もっぱら０スタート
  $swk = "";         // 作業用文字列

  // １日から晦日までぶん回す
  while($misoka >= $dd) {
    // 一週間単位で処理：途中でループから抜ける可能性があるので注意!!
    for( $j = 0; $j < 7; $j++ ) {
      if ($fw > 0) {
        // 第一週の「出力しない部分」の算出
        $swk .= '0,';
        $fw --;
      } else {
        // 通常の「日付のpush」
        $swk .= $dd . ',';
        $dd ++;
        if ($misoka < $dd) {
          break;
        }
      }
    }

    // 第ｎ週の日付配列の格納
    $list[$label] = $swk;
    $swk = "";
    $label ++;
  }

  // 後始末
  if ("" != $swk) {
    $list[$label] = $swk;
  }

  // 出力文字列置換
  foreach($list as $wk) {

    $c = new f_conv;

    $dd = split("\,", $wk);

    for($i = 0; $i < 7; $i++) {
          if(!isset($dd[$i])) {
        $dd[$i] = 0;
      }

      // XXX とりあえず０なら「未出力」だけど。後で指定できるようにする？

      if (0 == $dd[$i]) {
        $c->monoDic(('d' . $i), "");
        $c->monoDic("opt". $i,"disabled");
      } else {
        $c->monoDic(('d' . $i), $dd[$i]);
        $c->monoDic("opt". $i,"");
      }
    }
    $ret .= $c->conv($s_base);
  }

  //
  return $ret;
}

//private:
private $yy_;    // 年
private $mm_;    // 月

} // end of class


