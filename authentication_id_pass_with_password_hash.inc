<?php

/**
 * IDとパスワードによる認証機構
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once('authentication_id_pass.inc');

class authentication_id_pass_with_password_hash extends authentication_id_pass {

/**
 * パスワードの比較
 *
 * @access public
 * @return boolean パスワード一致ならtrue、不一致ならfalse
 */
protected function password_verify($obj) {
  // デバッグしやすいように一端変数で受け取る
  $raw_pass = $this->get_raw_password();
  $db_pass = $obj->get_value('pass');

  // 比較
  return password_verify($raw_pass, $db_pass) ;
}

} // end of class
