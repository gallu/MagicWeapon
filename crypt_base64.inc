<?php
/**
 * base64による暗号化クラス
 *
 * XXX 「base64は暗号化ぢゃない」とかいう突っ込みは無視。
 * しゃぁないやん mcrypt 入ってない環境用なんだからさぁ orz
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

require_once('crypt_base.inc');
require_once('block_cipher_mode_util.inc');

class crypt_base64 extends crypt_base {

/*
 * コンストラクタ
 */
public function __construct() {
  parent::__construct();
  //$this->init();
}

/**
 * 暗号化
 *
 * あらかじめ設定された情報で暗号化を行う
 *
 * @access public
 */
public function encrypt() {
  $this->set_encrypted_unit( base64_encode($this->get_plain_text()) );
}

/**
 * 複合化
 *
 * あらかじめ設定された情報で複合化を行う
 *
 * @access public
 */
public function decrypt() {
  $this->set_plain_text( base64_decode($this->get_encrypted_unit()) );
}


// private:
} // end of class

