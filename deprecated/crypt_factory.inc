<?php
/**
 * 暗号クラス系のファクトリ
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once('config.inc');
require_once('crypt_factory.inc');

class crypt_factory {

/**
 * コンフィグファイルからの暗号オブジェクトの取得
 *
 * @access public
 * @param string $f 暗号ファイル用のconfigファイル名
 */
static public function get_instance($f) {
  // 取り込み
  $conf = new config;
  $conf->set_file($f);

  // 暗号化アルゴリズムのチョイス
  $crypt_type = $conf->find('type');
  if (strcasecmp($crypt_type, "MCRYPT_RIJNDAEL_256") === 0) {
    require_once('crypt_rijndael256.inc');
    $conn = new crypt_rijndael256;
  } else 
  if (strcasecmp($crypt_type, "base64") === 0) {
    require_once('crypt_base64.inc');
    $conn = new crypt_base64;
  } else {
    // 判別不能なのでエラー復帰
    return null;
  }

  // 設定
  $conn->set_key($conf->find('key'));
  $conn->set_block_mode($conf->find('block_mode'));

  return $conn;
}

} // end of class

