<?php

/**
 * 秘密鍵暗号用の秘密鍵を生成するクラス
 *
 * ベース文字列をsha-512して必要な長さを切り出す、程度、これくらいでいいと思うのだが…
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

class mw_crypt_make_secret_key {

// 一旦sha-512固定。後々の想定で一応外に出しておく
protected static function get_hash_type() {
  return 'sha512';
}

/**
 * 秘密鍵の作成
 *
 * XXX hashタイプ(とりあえずsha-512)を超える長さは一旦未想定なので幾分注意
 *
 * @access public
 * @param string $base_string baseになる文字列
 * @param int $length 秘密鍵の長さ
 * @return string 秘密鍵(バイナリ)
 */
public static function make_key($base_string, $length)
{
  // base文字列をhash
  $s = hash(static::get_hash_type(), $base_string, true);

  // 切り取って復帰
  return substr($s, 0, $length);
}

//private:

} // end of class

