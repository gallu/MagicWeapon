<?php

/**
 * difficulty imagining ID(推測困難ID)の作成
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

class difficulty_imagining_identification {


/**
 * 推測困難IDの取得
 *
 * XXX あちこち固定。可変にしたいタイミングで適当に後で切り出す
 *
 * @access public
 * @return 戻り値の型 簡単な説明
 */
public function get()
{
  // XXX hash方式、ランダムの場所、文字数あたりが「可変になりうる」要素？
  $token = hash('sha512', file_get_contents('/dev/urandom', false, NULL, 0, 128), false);

  //
  return $token;
}

/**
 * 推測困難IDの取得(base64で文字長を短くした版)
 *
 * XXX あちこち固定。可変にしたいタイミングで適当に後で切り出す
 *
 * @access public
 * @return 戻り値の型 簡単な説明
 */
public function get_base64()
{
  $token = base64_encode(hash('sha512', file_get_contents('/dev/urandom', false, NULL, 0, 128), true));
  $token = substr($token, 0, 86);

  //
  return $token;
}

//private:
} // end of class


