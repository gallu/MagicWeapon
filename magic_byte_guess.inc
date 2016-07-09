<?php

/**
 * マジックバイト(マジックナンバー)によるファイルフォーマット推測クラス
 *
 * ファイルのフォーマットを、マジックバイトから推測するクラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once('magic_byte_guess_return.inc');

class magic_byte_guess {

/**
 * ファイル名指定によるマジックバイトからの推測
 *
 * 処理実態はfor_string()メソッド
 *
 * @access public
 * @param string $fn ファイル名(できれば絶対パスが望ましい)
 * @return object magic_byte_guess_returnインスタンス。ファイルが見つからない(readできない)場合、空ファイルの場合はnull
 */
static public function for_file($fn) {
  // ファイルの存在チェック
  if (false === is_readable($fn)) {
    return null;
  }

  // ファイルのサイズの確認
  if (false === ($file_size = filesize($fn))) {
    // ないとは思うが…
    return null;
  }
  // 空の場合はそのまま突き返す
  if (0 === $file_size) {
    return null;
  }

  // 「最大20バイト」まで読み込む
  $size = ($file_size > static::MAX_LENGTH) ? static::MAX_LENGTH : $file_size;

  // とりあえず先頭20バイト(もしくはファイルサイズいっぱいまで)読み込み
  $s = file_get_contents($fn, false, NULL, 0, $size);

  // 実態をcall
  return static::for_string($s);
}


/**
 * 文字列によるマジックバイトからの推測
 *
 * @access public
 * @param string $s 判定対象の文字列
 * @return object magic_byte_guess_returnインスタンス。空文字の場合はnull
 */
static public function for_string($s) {
  // 空文字かどうかチェック
  if ('' === $s) {
    return null;
  }
  if (false === $s) {
    return null;
  }

  // 長さがstatic::MAX_LENGTHを超える場合、static::MAX_LENGTHまで切り出す
  if (static::MAX_LENGTH < strlen($s)) {
    $s = substr($s, 0, static::MAX_LENGTH);
  }

  // 文字列を16進数表記にする
  $h = bin2hex($s);

  // 判定
  // 通常(先頭からの)判定
  foreach(static::$type_ as $k => $v) {
    // key長の把握
    $key_len = strlen($k);
    // 比較
    if (0 === strncmp($h, $k, $key_len)) {
      return new magic_byte_guess_return($v);
    }
  }

  // 通常(先頭から)以外の判定：現状、tarのみだけど
  foreach(static::$far_type_ as $k => $v) {
    // 距離の把握
    $distance = $v['distance'];
    // 対象文字の切り抜き
    $target = substr($h, $distance * 2); // １バイトの文字をbin2hexで２文字にしてあるので、距離を２倍にする
    // key長の把握
    $key_len = strlen($k);
    // 比較
    if (0 === strncmp($target, $k, $key_len)) {
      return new magic_byte_guess_return($v);
    }
  }

  // else
  // 何れにも引っかからない場合
  return new magic_byte_guess_return(array());
}

// 定数
// 判定文字の最大バイト長：tarの「257バイト目からstartで7バイトサイズ」があるのでこの値に
const MAX_LENGTH = 264;

// 5.6未満だとconstで配列指定できないので…
// XXX 一応念のため「マジックバイト長い順」にしてある
private static $type_ = array (
  '526172211a070100' => array('extension' => 'rar', 'content-type' => 'application/x-rar-compressed', 'type' => 'compression', 'description' => 'RAR'),
  '89504e470d0a1a0a' => array('extension' => 'png', 'content-type' => 'image/png', 'type' => 'image', 'description' => 'PNG'),
  '526172211a0700' => array('extension' => 'rar', 'content-type' => 'application/x-rar-compressed', 'type' => 'compression', 'description' => 'RAR'),
  '474946383761' => array('extension' => 'gif', 'content-type' => 'image/gif', 'type' => 'image', 'description' => 'GIF'),
  '474946383961' => array('extension' => 'gif', 'content-type' => 'image/gif', 'type' => 'image', 'description' => 'GIF'),
  '255044462d' => array('extension' => 'pdf', 'content-type' => 'application/pdf', 'type' => 'documents', 'description' => 'PDF'),
  '52457e5e' => array('extension' => 'rar', 'content-type' => 'application/x-rar-compressed', 'type' => 'compression', 'description' => 'RAR'),
  '504b0304' => array('extension' => 'zip', 'content-type' => 'application/zip', 'type' => 'compression', 'description' => 'ZIP'),
  '504b0306' => array('extension' => 'zip', 'content-type' => 'application/zip', 'type' => 'compression', 'description' => 'ZIP'),
  '504b0308' => array('extension' => 'zip', 'content-type' => 'application/zip', 'type' => 'compression', 'description' => 'ZIP'),
  '425a68' => array('extension' => 'bz2', 'content-type' => 'application/x-bzip2', 'type' => 'compression', 'description' => 'bzip2'),
  'ff4fff' => array('extension' => 'jp2', 'content-type' => 'image/jp2', 'type' => 'image', 'description' => 'JPEG2000'),
  '1f8b' => array('extension' => 'zip', 'content-type' => 'application/gzip', 'type' => 'compression', 'description' => 'Gzip'),
  '1f9d' => array('extension' => 'Z', 'content-type' => 'application/x-compress', 'type' => 'compression', 'description' => 'compress'),
  'ffd8' => array('extension' => 'jpeg', 'content-type' => 'image/jpeg', 'type' => 'image', 'description' => 'JPEG'),
);

// 「先頭から」ではないケース用：一旦、tarくらい？
private static $far_type_ = array (
  '75737461722020' => array('distance' => 257, 'extension' => 'tar', 'content-type' => 'application/x-tar', 'type' => 'compression', 'description' => 'Tar'),
  '757374617200' => array('distance' => 257, 'extension' => 'tar', 'content-type' => 'application/x-tar', 'type' => 'compression', 'description' => 'Tar'),
);


} // end of class

