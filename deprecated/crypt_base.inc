<?php
/**
 * 暗号基底クラス
 *
 * 暗号周りの基底クラス
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/
abstract class crypt_base {

public function __construct() {
  //
  //parent::__construct();
  $this->init();
}

/**
 * オブジェクト初期化
 *
 * @access public
 */
public function init() {
  $this->set_plain_text("");
  $this->set_encrypted_unit("");
  $this->set_key("");
  $this->set_block_mode("");
}

/**
 * 平文の設定
 *
 * @access public
 * @param string $s 平文
 */
public function set_plain_text($s){ $this->plain_text_ = $s; }

/**
 * 平文の取得
 *
 * @access public
 * @return string 平文
 */
public function get_plain_text(){ return $this->plain_text_; }

/**
 * 暗号文 (IV含む) の設定
 *
 * @access public
 * @param string $s 暗号文
 */
public function set_encrypted_unit($s){ $this->encrypted_unit_ = $s; }

/**
 * 暗号文 (IV含む) の取得
 *
 * @access public
 * @return string 暗号文
 */
public function get_encrypted_unit(){ return $this->encrypted_unit_; }

/**
 * 暗号キーの設定
 *
 * @access public
 * @param string $s 暗号キー
 */
public function set_key($s){ $this->key_ = $s; }

/**
 * 暗号キーの取得
 *
 * XXX public ?
 *
 * @access public
 * @return string 暗号キー
 */
public function get_key(){ return $this->key_; }

/**
 * ブロックモードの設定
 *
 * @access public
 * @param string $s ブロックモード
 */
public function set_block_mode($s){ $this->block_mode_ = $s; }

/**
 * ブロックモードの取得
 *
 * @access public
 * @return string ブロックモード
 */
public function get_block_mode(){ return $this->block_mode_; }

/**
 * 暗号化
 *
 * あらかじめ設定された情報で暗号化を行う
 *
 * @access public
 */
abstract public function encrypt();

/**
 * 複合化
 *
 * あらかじめ設定された情報で複合化を行う
 *
 * @access public
 */
abstract public function decrypt();

// private:
// 暗号化したい文字列
private $plain_text_;

// 暗号キー
private $key_;

// 暗号化した文字列(IV含む)
private $encrypted_unit_;

// ブロックモード
private $block_mode_;

} // end of class





