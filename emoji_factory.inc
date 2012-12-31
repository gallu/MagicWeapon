<?php

/**
 * emoji_baseに連なるクラスのファクトリー
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

require_once('emoji_docomo.inc');
require_once('emoji_ez.inc');
require_once('emoji_softbank_binary.inc');

class emoji_factory {

/**
 * 指定に合わせたオブジェクトをreturn
 *
 * @access publicかprivateかprotectedか
 * @param string $type 機種情報。docomo、ez、softbankのいずれか(あとで変更
 * @return emoji_base_obj 必要なインスタンス。エラーならnull
 */
static public function get_instance($type)
{
  $o = null;

  if (0 === strcasecmp($type, 'docomo')) {
    $o = new emoji_docomo;
  } else
  if (0 === strcasecmp($type, 'au')) {
    $o = new emoji_ez;
  } else
  if (0 === strcasecmp($type, 'ez')) {
    $o = new emoji_ez;
  } else
  if (0 === strcasecmp($type, 'softbank')) {
    $o = new emoji_softbank_binary;
  }
  //
  return $o;
}


} // end of class

