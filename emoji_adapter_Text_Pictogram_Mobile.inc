<?php

/**
 * 絵文字クラス : Text_Pictogram_Mobileモジュール用アダプタ
 *
 * http://blog.asial.co.jp/380
 * http://blog.asial.co.jp/351
 * 参照
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

require_once('Text/Pictogram/Mobile.php');

class emoji_adapter_Text_Pictogram_Mobile {

/**
 * インスタンス取 *
 * @access public
 * @param mobile_info_obj $mobj mobile_infoインスタンス
 * @param string $type 文字コード：当面 sjis のみ
 * @return Text_Pictogram_Mobile系インスタンス 問題がある場合null
 */
static public function get_obj(mobile_info $mobj, $type = 'sjis')
{
  //
  $carrier = 'docomo'; // デフォはとりあえずDoCoMo
  if (true === $mobj->is_ez()) {
    $carrier = 'ezweb';
  } else
  if (true === $mobj->is_softbank()) {
    $carrier = 'softbank';
  }

/*
  //
  $t = mb_detect_encoding($s, 'sjis-win,UTF-8,eucjp-win,JIS,ASCII');
  // とりあえず雑にスイッチ
  $type = 'sjis';
  if ($t === 'UTF-8') {
    $type = 'utf-8';
  }
print "type is $type <br />\n"
*/

  //
  $pobj = Text_Pictogram_Mobile::factory($carrier, $type);
  $pobj->setIntercodePrefix('[emoji');
  $pobj->setIntercodeSuffix(']');

  //
  return $pobj;
}


//private:

} // end of class

