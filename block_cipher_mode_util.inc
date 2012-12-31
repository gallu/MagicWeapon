<?php
/**
 * ブロック暗号モード用のユーティリティ
 *
 * 暗号化された文字列にIVを下記のようなフォーマットで操作するクラス
 * [IV_SIZE(4桁)][IV][ENC_STR]
 * IV_SIZE = IVのサイズを４桁の数字で示す。 例:0032 (IVは32バイト)
 * IV = IV_SIZEで指定された長さのIVを指定
 * ENC_STR = 暗号化された文字列
 * ここの処理系でベース６４の処理を行う
 * XXX 先頭に8バイト、ランダムな文字列をつける
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

require_once('crypt_util.inc');

class block_cipher_mode_util {

public function __construct() {
  $this->init();
}

public function init() {
  $this->set_encrypted_text("");
  $this->set_iv("");
  $this->set_iv_size("");
  $this->set_encrypted_unit("");
}

//
public function set_encrypted_text($s) { $this->encrypted_text_ = $s; }
public function get_encrypted_text() { return $this->encrypted_text_; }

public function set_iv($s) { $this->iv_ = $s; }
public function get_iv() { return $this->iv_; }

public function set_iv_size($i) { $this->iv_size_ = $i; }
public function get_iv_size() { return $this->iv_size_; }

public function set_encrypted_unit($s) { $this->encrypted_unit_ = $s; }
public function get_encrypted_unit() { return $this->encrypted_unit_; }

/*
 * 処理系
 */
// 結合
public function combine() {
  $iv_size = $this->get_iv_size();
  $iv = $this->get_iv();
  $encrypted_text = $this->get_encrypted_text();

  // フォーマットを整える
  $iv_size = sprintf("%04d", $iv_size);

  // base64化する
  $encrypted_unit = base64_encode(crypt_util::rand_string(8) . $iv_size. $iv. $encrypted_text);
//print "encrypt u \n $encrypted_unit \n";

  // 結合されたデータをセット
  $this->set_encrypted_unit($encrypted_unit);
}
// 分割
public function split() {
  $encrypted_unit = $this->get_encrypted_unit();

  // base64を元に戻す
  $encrypted_unit = base64_decode($encrypted_unit);

  // ivのサイズを取得
  $length = strlen($encrypted_unit);
  //$iv_size = (int) substr($encrypted_unit, 0, 4);
  $iv_size = (int) substr($encrypted_unit, 8, 4);

  // IVを取得
  $iv = substr($encrypted_unit, 8+4, $iv_size);

  // 暗号文字列を取得
  $encrypted_text = substr($encrypted_unit, (8+4+$iv_size));

  // 分割したデータをセット
  $this->set_encrypted_text($encrypted_text);
  $this->set_iv($iv);
  $this->set_iv_size($iv_size);
}


// メンバ変数
private $iv_;
private $iv_size_;
private $encrypted_text_;
private $encrypted_unit_;
}


