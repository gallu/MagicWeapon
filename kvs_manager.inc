<?php

/**
 * KVSマネージャクラス
 *
 * いわゆる ファクトリー
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  yyyy/mm/dd
 * @version $Id$
 **/

require_once('config.inc');
require_once('memcache_handle.inc');

class kvs_manager
{

/**
 * KVSハンドル取得
 *
 * @access public
 * @param string|object $param stringまたはconfigオブジェクトのパラメタ
 * @return kvs_handle_object kvsハンドルオブジェクト
 */
static public function get_handle($param)
{
  // 文字列ならおそらくファイル名であろうと見なす
  if (true === is_string($param)) {
    return self::get_handle_main($param);
  }
  // else
  // 大本のconfigからファイル名を取得して依頼する
  return self::get_handle_main($param->find('kvs_config'));
}


/**
 * KVSハンドル取得 メイン処理
 *
 * @access public
 * @param string|object $param stringまたはconfigオブジェクトのパラメタ
 * @return kvs_handle_object kvsハンドルオブジェクト
 */
static public function get_handle_main($config_filename)
{
//var_dump($config_filename);

  // 一応チェック
  if(false === file_exists($config_filename)){
    return null;
  }

  // 取り込み
  $conf = new config;
  $conf->set_file($config_filename);

  // チョイス
  $obj = null;
  switch($conf->find('type')) {
    case 'memcache':
      $obj = new memcache_handle();
      break;

  }

  //
  if (true === is_null($obj)) {
    return null;
  }

  // 設定
  // XXX この辺、共通かねぇ？？？
  foreach($conf->find_array('server') as $s) {
    // 分解
    list($ip, $port, $weight) = explode(':', $s);
    //
    $port += 0;

    //
    $weight += 0;
    if (0 >= $weight) {
      $weight = 1;
    }

    // 接続
    $obj->add_server($ip, $port, $weight);
  }
//var_dump($obj);

  //
  return $obj;
}




} // end of class


