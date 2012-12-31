<?php

/**
 * IPアドレス用ユーティリティ
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

class ip_util {

/**
 * IPアドレスの比較
 *
 * 指定されたネットワーク内のIPならtrueを返す
 *
 * @access public
 * @param string $ip IPアドレス
 * @param string $net ネットワークアドレス
 * @return boolena ネットワーク内ならtrue, ネットワーク外ならfalse
 */
static public function ip_in_network($ip, $network)
{
//print "$ip ? $network \n";
  // ipを分解
  // XXX サブネットがあっても無視する
  $awk = explode('/', $ip);
  $ips = explode('.', $awk[0]);
//var_dump($ips);

  // netoworkを分解
  $awk = explode('/', $network);
  $awk[] = '32'; // 付いてない時用雑対応
//var_dump($awk);
  $awk2 = explode('.', $awk[0]);
//var_dump($awk2);
  $mask = explode('.', self::$mask_[$awk[1]]); // XXX これ、厳密にはコスト高い
//var_dump($mask);

  // ネットワークアドレスを求める
  // &
  // 比較を４回
  //$net = array()
  //$bcast = array();
  for($i = 0; $i < 4; $i ++) {
//print "-------------\n";
//print "mask " . $mask[$i] . "\n";

    // 指定されたネットワークのネットワークアドレス
    $net   = ($awk2[$i]+0) & ($mask[$i]+0);
//var_dump($awk2[$i]);
//var_dump($mask[$i]);
//print $awk2[$i] . ' & ' . $mask[$i] . "\n";
//print "net $net \n";

/*
    // broadcastアドレスの求め方ってことで残しておく〜
    $bcast = ($awk2[$i]+0) | (($mask[$i]+0) ^ 255);
//print "bcas $bcast \n";
//print "ip " . $ips[$i] . "\n";
*/
    // 指定されたIPのネットワークアドレス
    $ip_net = ($ips[$i]+0) & ($mask[$i]+0);

    // 指定されたIPのネットワークアドレスと
    // 指定されたネットワークのネットワークアドレスと
    // を比較する〜
    if ($net !== $ip_net) {
      return false;
    }

  }
  //
  return true;
}


//private:
private static $mask_ = array(
  0 => "0.0.0.0",
  1 => "128.0.0.0",
  2 => "192.0.0.0",
  3 => "224.0.0.0",
  4 => "240.0.0.0",
  5 => "248.0.0.0",
  6 => "252.0.0.0",
  7 => "254.0.0.0",
  8 => "255.0.0.0",
  9 => "255.128.0.0",
  10 => "255.192.0.0",
  11 => "255.224.0.0",
  12 => "255.240.0.0",
  13 => "255.248.0.0",
  14 => "255.252.0.0",
  15 => "255.254.0.0",
  16 => "255.255.0.0",
  17 => "255.255.128.0",
  18 => "255.255.192.0",
  19 => "255.255.224.0",
  20 => "255.255.240.0",
  21 => "255.255.248.0",
  22 => "255.255.252.0",
  23 => "255.255.254.0",
  24 => "255.255.255.0",
  25 => "255.255.255.128",
  26 => "255.255.255.192",
  27 => "255.255.255.224",
  28 => "255.255.255.240",
  29 => "255.255.255.248",
  30 => "255.255.255.252",
  31 => "255.255.255.254",
  32 => "255.255.255.255"
);


} // end of class

