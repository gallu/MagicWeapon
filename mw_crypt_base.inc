<?php

/**
 * 暗号化基底クラス
 *
 * 暗号化クラスの基底クラス。
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

//require_once('debug_util.inc');

abstract class mw_crypt_base {

public function __construct()
{
  //
  $this->init();
}

//
public function init()
{
  //
  $this->set_cipher(''); // 暗号化方式は空にしておく
  $this->set_mode_cbc(); // ブロック暗号モードはCBCがベース
  $this->set_key_base(''); // 鍵ベース文字列は空にしておく
  $this->set_key_make_class_name('mw_crypt_make_secret_key'); // ベーシック(であると想定している)クラスをデフォで指定
}

//public:
  // cipher(protectedに限りなく近いpublic：基本は下記のラッパーメソッド経由でcallされる想定)
  public function get_cipher() { return $this->cipher_; }
  public function set_cipher($v) { $this->cipher_ = $v; }

  // XXX 必要に応じて適宜追加
/**
 * rijndael_128で暗号化
 *
 * @access public
 */
  abstract public function set_cipher_rijndael_128();

/**
 * rijndael_192で暗号化
 *
 * @access public
 */
  abstract public function set_cipher_rijndael_192();

/**
 * rijndael_256で暗号化
 *
 * @access public
 */
  abstract public function set_cipher_rijndael_256();

/**
 * blowfishで暗号化
 *
 * @access public
 */
  abstract public function set_cipher_blowfish();

  // mode (protectedに限りなく近いpublic：基本は下記のラッパーメソッド経由でcallされる想定)
  public function get_mode() { return $this->mode_; }
  public function set_mode($v) { $this->mode_ = $v; }

  // XXX 必要に応じて適宜追加
/**
 * ブロック暗号モードをCBCに設定
 *
 * @access public
 */
  abstract public function set_mode_cbc();

/**
 * ブロック暗号モードをECBに設定(非推奨)
 *
 * @access public
 */
  abstract public function set_mode_ecb();

/**
 * 秘密鍵のベースになる文字列を設定
 *
 * 実際には、ここで設定した文字列を「set_key_make_class_nameで指定したクラス」で処理して実際の秘密鍵を作る
 *
 * @access public
 * @param string $v 秘密鍵用ベース文字列
 */
  public function set_key_base($v) { $this->key_base_ = $v; }
  public function get_key_base() { return $this->key_base_; }

/**
 * 秘密鍵生成用クラス名を設定
 *
 * set_key_base()で設定されるベース文字列から「実際の秘密鍵を作る」処理が書かれたクラスを指定
 *
 * @access public
 * @param string $v 「実際の秘密鍵を作る」処理が書かれたクラス名
 * @param string $p 「実際の秘密鍵を作る」処理が書かれたクラスがあるpath名。デフォルト空文字の時は「クラス名.inc」がinclude_path上にある想定
 */
  public function set_key_make_class_name($v, $p = '') { $this->key_make_class_name_ = $v; $this->key_make_class_path_ = $p; }
  public function get_key_make_class_name() { return $this->key_make_class_name_; }
  public function get_key_make_class_path() { return $this->key_make_class_path_; }

/*
とりあえず未実装：後で必要そうなら追加実装
//$obj->set_padding_type_XXX(); // PKCS5Padding以外あんまり想定してないんだよなぁ…
//$obj->set_iv($iv); // ivは指定させたくない
 */

/**
 * 暗号化 / 復号前のvalidate
 *
 * XXX NGなら例外をぶん投げるので、returnなし
 *
 * @access protected
 */
protected function validate() {
  if ('' === $this->get_key_base()) {
    throw new ErrorException('key base is empty!!');
  }
  if ('' === $this->get_cipher()) {
    throw new ErrorException('cipher is empty!!');
  }
  if ('' === $this->get_mode()) {
    throw new ErrorException('mode is empty!!');
  }
  if ('' === $this->get_key_make_class_name()) {
    throw new ErrorException('key_make_class_name is empty!!');
  }
}

/**
 * 秘密鍵作成
 *
 * @access protected
 * @return string 秘密鍵
 */
protected function make_key() {
  // 鍵を把握
  $key_make_class_name = $this->get_key_make_class_name();
  $key_make_class_path = $this->get_key_make_class_path();
  if ('' === $key_make_class_path) {
    $key_make_class_path = $key_make_class_name . '.inc';
  }
  require_once($key_make_class_path);
  $key = $key_make_class_name::make_key($this->get_key_base(), $this->get_key_length());

  //
  return $key;
}

/**
 * 暗号化
 *
 * @access public
 * @param string $data 暗号化対象の平文データ
 * @param bool $raw trueなら戻り値は生バイナリ、false(default)ならbase64_encode文字列
 * @return string 暗号化文字列
 */
public function encrypt($data, $raw = false) {
  // データを軽くvalidate
  $this->validate();

  // 鍵を把握
  $key = $this->make_key();

  // ivが必要なら作成
  $iv = '';
  if (true === $this->is_need_iv()) {
    $iv = $this->create_iv();
  }

  // データをパディング
  // XXX 一旦ベタ。必要なら後で切り出す
//echo 'encrypt base 1 is ';
//debug_util::dump_string_put($data, false);
  $blocksize = $this->get_block_size();
  $pkcs = $blocksize - (strlen($data) % $blocksize);
  $data .= str_repeat(chr($pkcs), $pkcs);
//echo 'encrypt base 2 is ';
//debug_util::dump_string_put($data, false);

  // 暗号化
  $ciphertext = $this->encrypt_main($data, $key, $iv);

  // ivがあるなら付与
  if ('' !== $iv) {
    $ciphertext = $iv . $ciphertext;
  }

  // base64エンコードが必要ならエンコード
  if (false === $raw) {
    $ciphertext = base64_encode($ciphertext);
  }

  //
  return $ciphertext;
}

/**
 * 復号
 *
 * @access public
 * @param string $ciphertext 復号対象の暗号文(バイナリ)
 * @return string 復号した平文
 */
public function decrypt($ciphertext) {
  // データを軽くvalidate
  $this->validate();

  // 鍵を把握
  $key = $this->make_key();

  // ivが必要なモードなら切り出す
  $iv = '';
  if (true === $this->is_need_iv()) {
    $iv = substr($ciphertext, 0, $this->get_iv_size());
    $ciphertext = substr($ciphertext, $this->get_iv_size());
  }

  // 復号
  $plaintext = $this->decrypt_main($ciphertext, $key, $iv);

  // パディングの切り取り
//echo 'decrypt base 1 is ';
//debug_util::dump_string_put($plaintext, false);
  $pad = ord($plaintext[strlen($plaintext)-1]);
//var_dump($pad);
  $plaintext = substr($plaintext, 0, -1 * $pad);
//echo 'decrypt base 1 is ';
//debug_util::dump_string_put($plaintext, false);

  //
  return $plaintext;
}

/**
 * base64文字列からの復号
 *
 * 単純なラッパー
 *
 * @access public
 * @param string $ciphertext 復号対象の暗号文(base64文字列)
 * @return string 復号した平文
 */
public function decrypt_base64_decodde($ciphertext) {
  return $this->decrypt( base64_decode($ciphertext) );
}

/**
 * key長の取得
 *
 * @access protected
 * @return int 指定された暗号タイプのkey長
 */
abstract protected function get_key_length();

/**
 * IV必須チェック
 *
 * @access protected
 * @return bool IVが必要ならtrue、不用ならfalse
 */
abstract protected function is_need_iv();

/**
 * IV作成
 *
 * @access protected
 * @return string IV
 */
abstract protected function create_iv();

/**
 * ブロックサイズの取得
 *
 * @access protected
 * @return int ブロックサイズ
 */
abstract protected function get_block_size();

/**
 * IVサイズの取得
 *
 * @access protected
 * @return int IVのサイズ
 */
abstract protected function get_iv_size();

/**
 * 暗号化メイン処理
 *
 * @access protected
 * @param string $data パディングされた平文
 * @param string $key 秘密鍵
 * @param string $iv IV。不用な場合は空文字
 * @return string 暗号化された暗号文(バイナリ)
 */
abstract protected function encrypt_main($data, $key, $iv = '');

/**
 * 復号メイン処理
 *
 * @access protected
 * @param string $data 暗号文
 * @param string $key 秘密鍵
 * @param string $iv IV。不用な場合は空文字
 * @return string 復号された文字列
 */
abstract protected function decrypt_main($data, $key, $iv = '');


//private:
private $cipher_;
private $mode_;
private $key_base_;
private $key_make_class_name_;
private $key_make_class_path_;

} // end of class

