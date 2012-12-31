<?php

/**
 * MW用広域設定領域
 *
 * MW全体にかかるような設定を所持する singleton
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

require_once('multibyte.inc');

class mw_area_setting {

/**
 * 内部文字コードの取得
 *
 * @access public
 * @return string 文字コードタイプ JIS, eucjp-win, UTF-8,s jis-win
 */
static public function get_inside_encoding_type()
{
  return self::$etype_;
}

/**
 * 内部文字コードの設定
 *
 * @access public
 * @param string $s 文字コードタイプ JIS, eucjp-win, UTF-8,s jis-win
 */
static public function set_inside_encoding_type($s)
{
  //
  $obj = new multibyte();
  $ret = $obj->_set_guess_string($s);

  //
  if (-1 == $ret) {
    self::$etype_ = '';
  } else
  if (0 == $ret) {
    self::$etype_ = 'sjis-win';
  } else
  if (1 == $ret) {
    self::$etype_ = 'eucjp-win';
  } else
  if (2 == $ret) {
    self::$etype_ = 'jis';
  } else
  if (3 == $ret) {
    self::$etype_ = 'UTF-8';
  }
}


//private:
private static $etype_ = 'UTF-8';	// 内部文字エンコード defaultはutf-8


} // end of class

