<?php

/**
 * 暗号系のユーティリティ
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

class crypt_util {


/**
 * 指定された文字列長のランダムな文字列の復帰
 *
 * 引数次第で長さもランダムになる
 *
 * @access static public
 * @param int $num 文字列のlength
 * @param boolean $flg ここがtrueだと$numが$num以内でランダムになる
 * @return string ランダムな文字列
 */
static public function rand_string($num, $flg = false)
{
  //
  $data = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789';

  // ランダムな文字列長の設定
  if (true === $flg) {
    $num = mt_rand(1, $num);
  }

  $ret = "";
  $max = strlen($data);
  $max --; // 0 startのため

  for($i=0; $i < $num; $i ++) {
    $j = mt_rand(0,$max);
    $ret .= $data[$j];
  }

  return $ret;
}

//private:


} // end of class

