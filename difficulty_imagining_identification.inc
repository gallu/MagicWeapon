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
 * @return string 推測困難と思われるtoken戻り値の型 簡単な説明
 */
protected function rand() {
  // XXX random_bytesはPHP7以降の関数だが、PHP5.2以降で使えるユーザランド実装( https://github.com/paragonie/random_compat )が存在する
  if(function_exists('random_bytes')) {
    $rand = random_bytes(128);
  } else if (is_readable('/dev/urandom')) {
    $rand = file_get_contents('/dev/urandom', false, NULL, 0, 128);
  } else if(function_exists('openssl_random_pseudo_bytes')) {
    $rand = openssl_random_pseudo_bytes(128);
  } else {
    throw new ErrorException('need random_bytes(), openssl_random_pseudo_bytes(), or /dev/urandom device.');
  }
  //
  return $rand;
}


/**
 * 推測困難IDの取得
 *
 * XXX あちこち固定。可変にしたいタイミングで適当に後で切り出す
 *
 * @access public
 * @return string 推測困難と思われるtoken
 */
public function get()
{
  $token = hash('sha512', $this->rand(), false);

  //
  return $token;
}

/**
 * 推測困難IDの取得(base64で文字長を短くした版)
 *
 * XXX あちこち固定。可変にしたいタイミングで適当に後で切り出す
 *
 * @access public
 * @return string 推測困難と思われるtoken
 */
public function get_base64()
{
  $token = base64_encode(hash('sha512', $this->rand(), true));
  $token = substr($token, 0, 86);

  //
  return $token;
}

//private:
} // end of class


