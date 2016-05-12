<?php

/**
 * 暗号化クラス(openssl使用)
 *
 * opensslを使用する暗号化クラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once('mw_crypt_base.inc');
//require_once('debug_util.inc');

class mw_crypt_openssl extends mw_crypt_base{

public function __construct()
{
  // opensslが入ってなかったら例外はき出して終了
  if (false == function_exists('openssl_encrypt')) {
    throw new ErrorException('openssl_encrypt is not find!!');
  }

  //
  parent::__construct();
}

/**
 * rijndael_128で暗号化(厳密にはAES)
 *
 * @access public
 */
  public function set_cipher_rijndael_128() {
    $this->set_cipher('AES-128');
  }

/**
 * rijndael_192で暗号化(厳密にはAES)
 *
 * @access public
 */
  public function set_cipher_rijndael_192() {
    $this->set_cipher('AES-192');
  }

/**
 * rijndael_256で暗号化(厳密にはAES)
 *
 * @access public
 */
  public function set_cipher_rijndael_256() {
    $this->set_cipher('AES-256');
  }

/**
 * blowfishで暗号化
 *
 * @access public
 */
  public function set_cipher_blowfish() {
    $this->set_cipher('BF');
  }


/**
 * ブロック暗号モードをCBCに設定
 *
 * @access public
 */
  public function set_mode_cbc() {
    $this->set_mode('CBC');
  }

/**
 * ブロック暗号モードをECBに設定(非推奨)
 *
 * @access public
 */
  public function set_mode_ecb() {
    $this->set_mode('ECB');
  }

/**
 * メソッド(暗号方式＋ブロック暗号モード)の取得
 *
 * @access protected
 * @return int 指定された暗号タイプのkey長
 */
protected function get_method() {
  return $this->get_cipher() . '-' . $this->get_mode();
}

/**
 * key長の取得
 *
 * XXX 関数がないっぽいので、一旦固定で持つ
 *
 * @access protected
 * @return int 指定された暗号タイプのkey長
 */
protected function get_key_length() {
  $keylen = 0;
  switch($this->get_cipher()) {
    case 'AES-128':
    case 'AES-192':
    case 'AES-256':
      $keylen = 32;
      break;
      
    case 'BF':
      $keylen = 56;
      break;
  }
  //
  return $keylen;
}

/**
 * IV必須チェック
 *
 * @access protected
 * @return bool IVが必要ならtrue、不用ならfalse
 */
protected function is_need_iv() {
  // XXX 一旦「ECBなら不用」ってロジックにしとく
  if ('ECB' === $this->get_mode()) {
    return false;
  }
  // else
  return true;
}

/**
 * IV作成
 *
 * @access protected
 * @return string IV
 */
protected function create_iv() {
  return openssl_random_pseudo_bytes($this->get_iv_size());
}

/**
 * ブロックサイズの取得
 *
 * XXX 関数がないっぽいので、一旦固定で持つ
 *
 * @access protected
 * @return int ブロックサイズ
 */
protected function get_block_size() {
  $blocklen = 0;
  switch($this->get_cipher()) {
    case 'AES-128':
      $blocklen = 16;
      break;
      
    case 'AES-192':
      $blocklen = 24;
      break;
      
    case 'AES-256':
      $blocklen = 32;
      break;
      
    case 'BF':
      $blocklen = 8;
      break;
  }
  //
  return $blocklen;
}

/**
 * IVサイズの取得
 *
 * @access protected
 * @return int IVのサイズ
 */
protected function get_iv_size() {
  return openssl_cipher_iv_length($this->get_method());
}

/**
 * 暗号化メイン処理
 *
 * @access protected
 * @param string $data パディングされた平文
 * @param string $key 秘密鍵
 * @param string $iv IV。不用な場合は空文字
 * @return string 暗号化された暗号文(バイナリ)
 */
protected function encrypt_main($data, $key, $iv = '') {
//echo 'encrypted is ';
//debug_util::dump_string_put($data, false);
//echo 'iv is ';
//debug_util::dump_string_put($iv, false);
  if ('' === $iv) {
    $r = openssl_encrypt($data, $this->get_method(), $key);
  } else {
    $r = openssl_encrypt($data, $this->get_method(), $key, OPENSSL_RAW_DATA, $iv);
  }
  //
//echo 'encrypt is ';
//debug_util::dump_string_put($r, false);
  return $r;
}

/**
 * 復号メイン処理
 *
 * @access protected
 * @param string $data 暗号文
 * @param string $key 秘密鍵
 * @param string $iv IV。不用な場合は空文字
 * @return string 復号された文字列
 */
protected function decrypt_main($data, $key, $iv = '') {
//echo 'iv is ';
//debug_util::dump_string_put($iv, false);
//echo 'decrypt is ';
//debug_util::dump_string_put($data, false);
  if ('' === $iv) {
    $r = openssl_decrypt($data, $this->get_method(), $key);
  } else {
    $r = openssl_decrypt($data, $this->get_method(), $key, OPENSSL_RAW_DATA, $iv);
  }
//echo 'decrypted is ';
//debug_util::dump_string_put($r, false);
  //
  return $r;
}

} // end of class

