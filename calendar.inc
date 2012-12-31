<?php

/**
 * カレンダークラス
 *
 * 日付にまつわる全般を扱う
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  yyyy/mm/dd
 * @version $Id$
 **/


/*
 * ゲッター
 *
 * get_year();
 * get_month();
 * get_day();
 * get_hour();
 * get_minute();
 * get_second();
 * get_min();  // get_minuteと同じ
 * get_sec();  // get_secondと同じ
 * get_epoc(); // エポックタイム
 * get_week(); // 0:日?6:土
 * get_week_string(); // 曜日の英語名
 * get_week_string_short();  // 曜日の３文字英語名
 * get_month_string();  // 月の英語名
 * get_month_string_short(); // 月の３文字英語名
 * get_julian(); // ユリウス日
 *
 * // 'yyyy/mm/dd hh:mm:ss' 形式の文字列
 * // $separatorで "/" 部分の文字指定
 * get_string($separator); 
 * get_stringZ($separator);  // "GMT", "+900"付加
 *
 * get_string_for_cookie();  // クッキー形式の時間文字列
 * get_weeks_number_of_month(); // 日付がその月の第何週か
 *
 **********************
 * セッター
 *
 * set_year($num);
 * set_month($num);
 * set_day($num);
 * set_hour($num);
 * set_minute($num);
 * set_second($num);
 * set_min($num);
 * set_sec($num);
 * set_epoc($epoc);  // エポックタイムからセット
 * set_julian($jd);  // ユリウス日からセット
 * set_string($str); // 文字列からセット
 * set_stringApache($str); // apacheログの日付からセット
 * set_matubi(); // その月の末日に日付をセット
 * set_zone($str); // 時間帯ゾーンセット（デフォルトは "+0900"）
 * set_zoneJST();  // ゾーンを "+0900"にセット
 * set_zoneGMT();  // ゾーンを "GMT"にセット
 * set_zoneJ();  // set_zoneJSTと同じ
 *
 * set_now_date();  // 日付を現在時刻にセット
 * set_birth_day_from($age); // $age歳である一番昔の誕生日をセット
 * set_birth_day_to($age);  // $age歳である一番最近の誕生日をセット
 *
 **********************
 *
 * is();  // 日付の正当性をチェック
 * calculation_day($num);  // $num（日数）だけ日付をずらす
 * calculation_hour($num); // $num（時間）だけ時間をずらす
 *
 * // $cal(カレンダーオブジェクト)と内容比較。同じならtrue
 * equal($cal); // 全ての要素を比較
 * equal_date($cal); // 日付を比較
 * equal_time($cal); // 時間を比較
 *
*/

/**
 * calendar
 *
 * カレンダークラス
 *
 * @author
 * @package
 * @since
 */
class calendar
{

/**
 * __construct
 *
 *
 * @param 引数の説明。
 * @return  戻り値の説明
 * @access public
 * @since 1.0
 */
public function __construct()
{
  // XXX おもいっきり暫定
  date_default_timezone_set("Asia/Tokyo");
  $this->init();
}

/**
 * init
 *
 *
 * @param 引数の説明。
 * @return  戻り値の説明
 * @access public
 * @since 1.0
 */
public function init()
{
  $this->set_year(0);
  $this->set_month(0);
  $this->set_day(0);
  $this->set_hour(0);
  $this->set_minute(0);
  $this->set_second(0);
  
  // デフォルトゾーンは日本時間
  $this->set_zoneJST();
}

///////////////////////////////////////////////////
// ゲッター

/**
 * 年の取得
 *
 * @access public
 * @return int 年
 */
public function get_year()
{
  return $this->year_;
}

/**
 * 月の取得
 *
 *
 * @access public
 * @return int 月
 */
public function get_month()
{
  return $this->month_;
}

/**
 * 日の取得
 *
 * @access public
 * @return int 日
 */
public function get_day()
{
  return $this->day_;
}

/**
 * 時の取得
 *
 * @access public
 * @return int 時
 */
public function get_hour()
{
  return $this->hour_;
}

/**
 * 分の取得
 *
 * @access public
 * @return int 分
 */
public function get_minute()
{
  return $this->minute_;
}

/**
 * 秒の取得
 *
 * @access public
 * @return int 秒
 */
public function get_second()
{
  return $this->second_;
}

/**
 * 分の取得
 *
 * get_minuteへのラッパー
 * 書くのが面倒なので(苦笑
 *
 */
public function get_min()
{
  return $this->get_minute();
}

/**
 * 秒の取得
 *
 * get_secondへのラッパー
 * 書くのが面倒なので(苦笑
 */
public function get_sec()
{
  return $this->get_second();
}

/**
 * エポックタイムの取得
 *
 * @access public
 * @return int エポックタイム(秒)
 */
public function get_epoc()
{
  $t = sprintf("%4d-%02d-%02d %02d:%02d:%02d",
                $this->get_year(),
                $this->get_month(),
                $this->get_day(),
                $this->get_hour(),
                $this->get_minute(),
                $this->get_second()
              );
//print "get_epoc t is $t \n";
  return strtotime($t);
}

/**
 * 週情報の取得
 *
 * ツェラーの公式を用いて行う
 * 0:日曜日 ? 6:土曜日 まで
 *
 * @access public
 * @return int 0:日曜日 ? 6:土曜日
 */
public function get_week()
{
  $yy = $this->get_year(); 
  $mm = $this->get_month();
  $dd = $this->get_day();
  
  if($mm == 1 || $mm == 2 ){
    $yy--;
    $mm += 12;
  }

  //
  return ( $yy + floor($yy/4) - floor($yy/100) + floor($yy/400) + floor((13*$mm+8)/5) + $dd )% 7;
}

/**
 * 曜日名の文字列の取得
 *
 * このメソッドではfullスペルで返す
 * ３文字の略称がほしい場合はget_week_string_shortを用いる
 *
 * @access public
 * @return string fullスペルで曜日名の文字列
 */
public function get_week_string()
{   
  return $this->wdy_[ $this->get_week() ];
}

/**
 * 曜日名の３文字略の文字列の取得
 *
 * このメソッドでは３文字略のスペルで返す
 * fullスペルがほしい場合はget_week_stringを用いる
 *
 * @access public
 * @return string ３文字略称で曜日名の文字列
 */
public function get_week_string_short()
{
  return $this->wdy3_[ $this->get_week() ];
}

/**
 * 月名の文字列の取得
 *
 * このメソッドではfullスペルで返す
 * ３文字の略称がほしい場合はget_month_string_shortを用いる
 *
 * @access public
 * @return string fullスペルで月名の文字列
 */
public function get_month_string()
{
  return $this->mon_[ $this->get_month() -1 ];
}

/**
 * 月名の文字列の取得
 *
 * このメソッドでは３文字略のスペルで返す
 * fullスペルがほしい場合はget_month_stringを用いる
 *
 * @access public
 * @return string ３文字略称で月名の文字列
 */
public function get_month_string_short()
{   
  return $this->mon3_[ $this->get_month() -1 ];
}

/**
 * ユリウス日を返す
 *
 * 厳密にはユリウス通日から.5を引いたもの。
 *
 * @access public
 * @return int ユリウス日
 */
public function get_julian()
{
  $yy = $this->get_year();
  $mm = $this->get_month();
  $dd = $this->get_day();
  
  if ($mm <= 2) {
    $yy --;
    $mm += 12;
  }

  //
  $jd = floor($yy*365.25) + floor($yy/400) - floor($yy/100) + floor(30.59*($mm-2)) + $dd +1721088;
//print "jd = $jd\n";

  //
  return $jd;
}

/**
 * フォーマットされた日付を返す:yyyy/mm/dd hh:mm:ss
 *
 * @access public
 * @param string $sep 日付の区切り文字。デフォルトは'/'
 * @return string yyyy/mm/dd hh:mm:ssの形式にフォーマットされた文字列
 */
public function get_string($sep = "/")
{
  $t = sprintf("%4d%s%02d%s%02d %02d:%02d:%02d",
                $this->get_year(),
                $sep,
                $this->get_month(),
                $sep,
                $this->get_day(),
                $this->get_hour(),
                $this->get_minute(),
                $this->get_second()
              );
  return $t;
}

/**
 * フォーマットされた日付を返す:yyyy/mm/dd hh:mm:ss zone
 *
 * タイムゾーンが追加されたget_string
 *
 * @access public
 * @param string $sep 日付の区切り文字。デフォルトは'/'
 * @return string yyyy/mm/dd hh:mm:ss zoneの形式にフォーマットされた文字列
 */
public function get_stringZ($sep = "/")
{
  $t = $this->get_string($sep) . ' ' . $this->zone_;
  return $t;  
}

/**
 * Cookie用にフォーマットされた日付を返す
 *
 * 具体的には "Wdy, DD-Mon-YY HH:MM:SS GMT" という形式
 *
 * @access public
 * @return string Cookie形式にフォーマットされた文字列
 */
public function get_string_for_cookie()
{
  $t = sprintf("%s, %02d-%02d-%4d %02d:%02d:%02d GMT",
                $this->get_week_string_short(),
                $this->get_day(),
                $this->get_month(),
                $this->get_year(),
                $this->get_hour(),
                $this->get_minute(),
                $this->get_second()
              );
  return $t;
}

/**
 * メールヘッダ用にフォーマットされた文字列
 *
 * 実際にはget_string_rfc2822()へのエイリアス
 *
 * @access public
 * @return string メールヘッダ形式にフォーマットされた文字列
 */
public function get_string_for_mail() { return $this->get_string_for_rfc2822(); }

/**
 * RFC2822記述形式(メールヘッダ用)にフォーマットされた文字列
 *
 * フォーマットはWdy, DD short-month YYYY HH:MM:SS zone
 *
 * @access public
 * @return string メールヘッダ形式にフォーマットされた文字列
 */
public function get_string_for_rfc2822()
{
  $t = sprintf("%s, %02d %s %4d %02d:%02d:%02d %s",
                $this->get_week_string_short(),
                $this->get_day(),
                $this->get_month_string_short(),
                $this->get_year(),
                $this->get_hour(),
                $this->get_minute(),
                $this->get_second(),
                $this->zone_
              );
  return $t;
}

/**
 * 第何週目か算出
 *
 * その月の第何週目かを算出する
 * 復帰は、正常な値であれば１?５になるはず
 *
 * @access public
 * @return int 1-5。これは１週目?５週目を示す。
 */
public function get_weeks_number_of_month()
{
  $dd = $this->get_day();
  $cpy = $this->deep_copy();
  
  $cpy->set_day(1);
  // 該当月の一日の曜日を求める
  $week = $cpy->get_week();

  // 算出
  return ((int)(($dd - 1 + $week)/7))+1;
}

///////////////////////////////////////////////////
// セッター

/**
 * 年の設定
 *
 * @param int $arg 年
 * @access public
 */
public function set_year($arg)
{
  $this->year_ = $arg;
}

/**
 * 月の設定
 *
 * @param int $arg 月
 * @access public
 */
public function set_month($arg)
{
  $this->month_ = $arg;
}

/**
 * 日の設定
 *
 * @param int $arg 日
 * @access public
 */
public function set_day($arg)
{
  $this->day_ = $arg;
}

/**
 * 時の設定
 *
 * @param int $arg 時
 * @access public
 */
public function set_hour($arg)
{
  $this->hour_ = $arg;
}

/**
 * 分の設定
 *
 * @param int $arg 分
 * @access public
 */
public function set_minute($arg)
{
  $this->minute_ = $arg;
}

/**
 * 秒の設定
 *
 * @param int $arg 秒
 * @access public
 */
public function set_second($arg)
{
  $this->second_ = $arg;
}

/**
 * 分の設定
 *
 * @param int $arg 分
 * @access public
 */
public function set_min($arg)
{
  return $this->set_minute($arg);
}

/**
 * 秒の設定
 *
 * @param int $arg 秒
 * @access public
 */
public function set_sec($arg)
{
  return $this->set_second($arg);
}

/**
 * エポックタイムから日付をセット
 *
 * @param int $arg エポック秒
 * @access public
 */
public function set_epoc($arg)
{
  $t = localtime($arg, true);
//print "set_epoc t is \n";
//print_r($t);

  $this->set_year($t["tm_year"] + 1900);
  $this->set_month($t["tm_mon"] + 1);
  $this->set_day($t["tm_mday"]);
  $this->set_hour($t["tm_hour"]);
  $this->set_minute($t["tm_min"]);
  $this->set_second($t["tm_sec"]);
}

/**
 * ユリウス日からセット
 *
 * @param int $jd ユリウス日
 * @access public
 */
public function set_julian($jd)
{
  $jd += 1; // +.5に、端数を切り捨ててる分でもう.5で合計＋１
  if ($jd < 2299161 ) {
    $a = $jd;
  } else {
    $x = floor(($jd - 1867216.25) / 36524.25);
    $a = $jd + 1 + $x - floor($x/4);
  }

  $b = $a + 1524;
  $c = floor(($b-122.1)/365.25);
  $d = floor(365.25 * $c);
  $e = floor(($b - $d) / 30.6001);

  $dd = $b - $d - floor(30.6001 * $e);
  if ($e < 14) {
    $mm = $e - 1;
    $yy = $c - 4716;
  } else {
    $mm = $e - 13;
    $yy = $c - 4715;
  }

  $this->set_year($yy);
  $this->set_month($mm);
  $this->set_day($dd);
}

/**
 * 文字列から入力
 *
 * strtotimeを使っているので、対応文字列はstrtotime関数に準じる
 *
 * @param string $base_string 日付の文字列
 * @access public
 */
public function set_string($base_string)
{
  // ざっくり分解
  $time = strtotime($base_string);
  $s = date('Y n d G i s', $time);
  //
  list($yy, $mm, $dd, $h, $m, $s) = explode(' ', $s, 6);

  // 設定
  $this->set_year((int)$yy);
  $this->set_month((int)$mm);
  $this->set_day((int)$dd);
  $this->set_hour((int)$h);
  $this->set_minute((int)$m);
  $this->set_second((int)$s);
}

/**
 * Apacheログ形式文字列から入力
 *
 * @param string $base_string アパッチログの日付形式
 * @access public
 */
public function set_stringApache($base_string)
{
  $yy = $mm = $dd = $h = $m = $s = 0; // 初期化
  $matches = array();

  // 正規表現で一気に切り出し
  // print "$base_string \n";
  if(preg_match('/([0-9][0-9])\/([a-zA-Z]+)\/([0-9][0-9][0-9][0-9]):([0-9][0-9]):([0-9][0-9]):([0-9][0-9])/',
                  $base_string,
                  $matches
                )){
    $yy = $matches[3];
    $wk = $matches[2];
    $dd = $matches[1];
    $h = $matches[4];
    $m = $matches[5];
    $s = $matches[6];
  }else{
    return array($yy, $mm, $dd, $h, $m, $s);
  }

  // 月の部分を数字に変換
  if (preg_match("/^Jan/i", $wk)){
    $mm = "1";
  } else if (preg_match("/^Feb/i", $wk)) {
    $mm = "2";
  } else if (preg_match("/^Mar/i", $wk)) {
    $mm = "3";
  } else if (preg_match("/^Apr/i", $wk)) {
    $mm = "4";
  } else if (preg_match("/^May/i", $wk)) {
    $mm = "5";
  } else if (preg_match("/^Jun/i", $wk)) {
    $mm = "6";
  } else if (preg_match("/^Jul/i", $wk)) {
    $mm = "7";
  } else if (preg_match("/^Aug/i", $wk)) {
    $mm = "8";
  } else if (preg_match("/^Sep/i", $wk)) {
    $mm = "9";
  } else if (preg_match("/^Oct/i", $wk)) {
    $mm = "10";
  } else if (preg_match("/^Nov/i", $wk)) {
    $mm = "11";
  } else if (preg_match("/^Dec/i", $wk)) {
    $mm = "12";
  }

  $this->set_year($yy);
  $this->set_month($mm);
  $this->set_day($dd);
  $this->set_hour($h);
  $this->set_minute($m);
  $this->set_second($s);
}

/**
 * 日付を晦日に設定
 *
 * @access public
 */
public function set_matubi()
{
  $yy = $this->get_year();
  $mm = $this->get_month();
  
  $this->set_day($this->get_matubi());
}

/**
 * タイムゾーンの設定
 *
 * @param string $val タイムゾーンの文字列
 * @access public
 */
public function set_zone($val)
{
  $this->zone_ = $val;
}

/**
 * タイムゾーンを日本に設定
 *
 * @access public
 */
public function set_zoneJST()
{
  $this->set_zone("+0900");
}

/**
 * タイムゾーンを世界標準時に設定
 *
 * @access public
 */
public function set_zoneGMT()
{
  $this->set_zone("GMT");
}

/**
 * タイムゾーンを日本に設定
 *
 * @access public
 */
public function set_zoneJ()
{
  $this->set_zoneJST();
}

///////////////////////////////////////////////////

/**
 * 内部に現在時刻を設定
 *
 * @access public
 */
public function set_now_date()
{
  $t = localtime(time(), true);

  $this->set_year($t["tm_year"] + 1900);
  $this->set_month($t["tm_mon"] + 1);
  $this->set_day($t["tm_mday"]);
  $this->set_hour($t["tm_hour"]);
  $this->set_minute($t["tm_min"]);
  $this->set_second($t["tm_sec"]);
}

/**
 * 内部に$age歳である一番古い誕生日をセット
 *
 * @param int $age 年齢
 * @access public
 */
public function set_birth_day_from($age)
{
  $this->set_now_date();
  $this->calculation_year(-($age+1));
  $this->calculation_day(1);  // 1日プラス
}

/**
 * 内部に$age歳である一番新しい誕生日をセット
 *
 * @param int $age 年齢
 * @access public
 */
public function set_birth_day_to($age)
{
  $this->set_now_date();
  $this->calculation_year(-($age));
}

/**
 * 内部に設定された数値の正当性チェック
 *
 * @return  boolean 正当ならtrue
 * @access public
 */
public function is()
{
  $yy = $this->get_year();
  $mm = $this->get_month();
  $dd = $this->get_day();

  // 年チェック
  // 負でなければよしとする
  if (0 > $yy) { return false; }

  // 月チェック
  if (1 > $mm) { return false; }
  if (12 < $mm) { return false; }

  // 日チェック
  if (1 > $dd) { return false; }
  $misoka = $this->get_matubi();
  if ($misoka < $dd) {
    return false;
  }

  return true;
}


////////////////////////////////
/**
 * 晦日の取得
 *
 * @return  int 晦日。エラーの場合は０
 * @access public
 */
// 晦日日の判定
public function get_matubi()
{
  $yy = $this->get_year();
  $mm = $this->get_month();

  // 一応
  if ($mm < 1) { return 0; }
  if ($mm > 12) { return 0; }

  // 配列を使って簡単に判定
  $data = array("31", "", "31", "30", "31", "30", 
                "31", "31", "30", "31", "30", "31");

  if ($mm != 2) {
    $ret = $data[$mm - 1];
  } else {
    if (($yy%400)==0) {
      $ret = 29;
    } else if (($yy%100)==0) {
      $ret = 28;
    } else if (($yy%4)==0) {
      $ret = 29;
    } else {
      $ret = 28;
    }
  }

  return $ret;
}


/**
 * 日付計算
 *
 * 引数分だけ変更
 * 第一引数で単位を指定
 *
 * @param string $unit 単位。'year', 'month', 'day', 'hour' のいずれか
 * @param int $arg 増減させる数値
 * @access public
 */
public function calculation($unit, $arg)
{
  // 単位ごとに振り分けを行う
  if (!(strcasecmp("year", $unit))) {
    $this->calculation_year($arg);
  } else
  if (!(strcasecmp("month", $unit))) {
    $this->calculation_month($arg);
  } else
  if (!(strcasecmp("day", $unit))) {
    $this->calculation_day($arg);
  } else
  if (!(strcasecmp("hour", $unit))) {
    $this->calculation_hour($arg);
  }
}

/**
 * 引数分だけ年を変更（マイナスなら戻る）
 *
 * @param int $arg 変更する年数
 * @access public
 */
public function calculation_year($arg)
{
  $this->set_year($this->get_year()+($arg));
}

/**
 * 引数分だけ月を変更（マイナスなら戻る）
 *
 * @param int $arg 変更する月数
 * @access public
 */
public function calculation_month($arg)
{
  // とりあえず現状の月に足しこむ
  $wk = $this->get_month()+($arg);
  //
  $ywk = $this->get_year();

  // マイナスの場合の処理
  while($wk < 1) {
    $wk += 12;
    $ywk --;
  }
  // 13月以上の場合の処理
  while($wk > 12) {
    $wk -= 12;
    $ywk ++;
  }

  // 設定
  $this->set_year($ywk);
  $this->set_month($wk);
}


/**
 * 引数分だけ日数を変更（マイナスなら戻る）
 *
 * @param int $arg 変更する日数
 * @access public
 */
public function calculation_day($arg)
{
  // ユリウス日に加算してセット
  $this->set_julian($this->get_julian()+($arg));
}

/**
 * 引数分だけ時間を変更（マイナスなら戻る）
 *
 * @param int $arg 変更する時間
 * @access public
 */
public function calculation_hour($arg)
{
  // 加える日付のエポックタイムの算出
  $arg_epoc = 60 * 60 * $arg;
  
  // 現在のエポックタイムに加算
  $new_epoc = $arg_epoc + $this->get_epoc();
  
  // セット
  $this->set_epoc($new_epoc);
}

/**
 * 日時が同じならtrue;
 *
 * 日付も時間も一致した場合にtrue
 *
 * @param calendar $cal 比べるカレンダーオブジェクト
 * @return  boolean 同じならtrue
 * @access public
 */
public function equal($cal)
{
  if(!$this->equal_date($cal) || !$this->equal_time($cal)){
    return false;
  }
  
  return true;
}
/**
 * 日付が同じならtrue
 *
 * @param calendar $cal 比べるカレンダーオブジェクト
 * @return  boolean 同じならtrue
 * @access public
 */
public function equal_date($cal)
{
  // それぞれ違うならfalse
  if($this->get_year() != $cal->get_year()){
    return false;
  }
  
  if($this->get_month() != $cal->get_month()){
    return false;
  }
  
  if($this->get_day() != $cal->get_day()){
    return false;
  }
  
  return true;
}

/**
 * 時間が同じならtrue
 *
 * @param calendar $cal 比べるカレンダーオブジェクト
 * @return  boolean 同じならtrue
 * @access public
 */
public function equal_time($cal)
{
  // それぞれ違うならfalse
  if($this->get_hour() != $cal->get_hour()){
    return false;
  }
  
  if($this->get_minute() != $cal->get_minute()){
    return false;
  }
  
  if($this->get_second() != $cal->get_second()){
    return false;
  }
  
  return true;
}

/**
 * 自身のコピーを作成
 *
 * @return calendar copyされたインスタンス
 * @access public
 */
public function deep_copy()
{
  // とりあえずライトcopy
  return clone $this;
}

/**
 * インスタンス内のymdから満年齢を計算
 *
 * XXX 誕生日はインスタンス内にあるとする
 *
 * @access public
 */
public function make_age(){

  // 現在時刻の取得
  $now_time = new calendar;
  $now_time->set_now_date();

  // とりあえず大雑把な年齢算出
  $age = $now_time->get_year() - $this->get_year();

  // 1歳分の微調整
  if ($now_time->get_month() < $this->get_month()) {
    $age --;
  } else
  if ($now_time->get_month() == $this->get_month()) {
    if ($now_time->get_day() < $this->get_day()) {
      $age --;
    }
  }

  //
  return $age;
}

//private:
private $year_;
private $month_;
private $day_;
private $hour_;
private $minute_;
private $second_;
private $zone_;

private $mon_ = array("January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"); 

private $mon3_ = array("Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul" , "Aug", "Sep ", "Oct", "Nov", "Dec");

private $wdy_ = array("Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"); 

private $wdy3_ = array("Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat");

};
?>
