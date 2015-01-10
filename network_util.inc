<?php

/**
 * ネットワーク系ユーティリティ
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

class network_util {

/**
 * localなIPアドレスの取得
 *
 * XXX UNIX系専用だねぇどうしよう？
 *
 * @access public
 * @param string $eth IPを取得したいNICの指定
 * @return string IPアドレス
 */
static public function get_local_ip($eth = 'eth0')
{
  // 複数回callの可能性があるので、チューニング用に
  static $ip = array();
  if (true === isset($ip[$eth])) {
    return $ip[$eth];
  }
  // else

  // 情報げとって
  $base = `/sbin/ifconfig {$eth}`;
//var_dump($base);

  // 大雑把に文字の場所を把握して
  $pos = strpos($base, 'inet addr');
  $pos2 = strpos($base, ':', $pos + 9); // XXX +9ってのは'inet addr'の文字数
  $pos3 = strpos($base, ' ', $pos + 9);
//var_dump($pos);
//var_dump($pos2);

  // 文字列かるく切り抜いて
  $s = substr($base, $pos2 + 1, ($pos3 - $pos2 - 1));
//var_dump($s);

  // IPっぽい場所を取得
  $ip[$eth] = $s;
  return $ip[$eth];
}


} // end of class

