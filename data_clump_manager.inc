<?php

/**
 * data_clumpマネージャクラス
 *
 * いわゆる ファクトリー
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  yyyy/mm/dd
 * @version $Id$
 **/


class data_clump_manager
{

/**
 * データハンドルを含まないdata_clumpインスタンス取得
 *
 * @access public
 * @param 
 * @return 
*/
static public function get_instance_without_datahandle($name, $o = null)
{
  // インスタンス作る
  if (true === is_null($o)) {
    $o = new $name;
  } else {
    $o->init();
  }

  //
  return $o;
}


/**
 * data_clumpインスタンスへの、data handle系の設定
 *
 * @access public
 * @param 
 * @return 
*/
static public function set_datahandle($o)
{
  // データハンドル系を、設定に基づいてぶち込む
  // -------------------------------------------------

  // クラス名を先に把握
  $name = get_class($o);

  // まず設定を取得
  // XXX 設定がない場合「第一レイヤー：RDB」とみなす

  // 一旦デフォルト
  $layer = array('rdb');
  if (self::is_config()) { // configに設定があれば
    // 名前探して
    $v = self::$config_obj_->find($name);
    if ('' === $v) {
      // デフォルトを確認
      $v = self::$config_obj_->find('default');
    }

    // 何がしか設定があれば、従う
    if ('' !== $v) {
      //
      $layer = array();

      // タイプ(レイヤー)とモードを切り分けて
      $awk = explode(':', $v);
      // XXX 雑にエラー回避
      $awk[] = '';
      $type = $awk[0];
      $mode = $awk[1];

      // まずタイプを処理
      foreach(explode(',', $type) as $t) {
        //
        $layer[] = trim($t);
      }

      // モードに設定があれば、設定しておく
      if ('code_master' === $mode) {
        $o->set_data_access_mode_code_master();
      }
    }
  }
//var_dump($layer);

  // 必要なハンドルを、必要なレイヤーに埋め込む
  $count = count($layer);
  for($i = 0; $i < $count; $i ++) {
    if ('rdb' === $layer[$i]) {
      $o->set_n_level_layer_data_operation_handle(self::get_db(), $i);
    } else
    if ('kvs' === $layer[$i]) {
      $o->set_n_level_layer_data_operation_handle(self::get_kvs(), $i);
    } else
    if ('apc' === $layer[$i]) {
      $o->set_n_level_layer_data_operation_handle(self::get_apc(), $i);
    } else {
      // おかぴぃ…
    }
  }

  // できたよ〜
  return $o;
}

/**
 * data_clumpインスタンス取得
 *
 * @access public
 * @param
 * @return
*/
static public function get_instance($name, $o = null)
{
  // インスタンス取得
  $o = self::get_instance_without_datahandle($name, $o);
  // data handle設定
  $o = self::set_datahandle($o);
  //
  return $o;
}

/**
 * data_clumpの作成用configを設定
 *
 * @access public
 * @param string $fn configファイル名
 * @return boolean
*/
static public function set_config($fn)
{
  // 一応チェック
  if(false === file_exists($fn)){
    return false;
  }

  //
  $obj = new config();
  $obj->set_file($fn);

  //
  self::$config_obj_ = $obj;

  //
  return true;
}

/**
 * configが設定されているかどうかのチェック
 *
 * @access public
 * @return boolean 設定されていたらtrue
*/
static public function is_config()
{
  return !(is_null(self::$config_obj_));
}


/**
 * modelインスタンスの設定
 *
 * XXX びみょ〜に「modelインスタンス経由でハンドル取得ってどうよ？」とか思うけど
 * XXX とりあえずこっちつかっといて〜
 *
 * @access public
 * @return boolean 設定されていたらtrue
*/
static public function set_model($o)
{
  self::$model_obj_ = $o;
}

// 微妙に非推奨な可能性があるので書かないよ〜
static public function set_dbh($o) { self::$dbh_ = $o; }
static public function set_kvs($o) { self::$kvs_ = $o; }
static public function set_apc($o) { self::$apc_ = $o; }



/**
 * DBハンドルのゲッター
 *
 * @access protected
 * @return 
*/
static protected function get_db()
{
  if (false === is_null(self::$model_obj_)) {
    return self::$model_obj_->get_db();
  }
  // else
  return self::$dbh_;
}

/**
 * KVSハンドルのゲッター
 *
 * @access protected
 * @return 
*/
static protected function get_kvs()
{
  if (false === is_null(self::$model_obj_)) {
    return self::$model_obj_->get_kvs();
  }
  // else
  return self::$kvs_;
}

/**
 * APCハンドルのゲッター
 *
 * @access protected
 * @return 
*/
static protected function get_apc()
{
  if (false === is_null(self::$model_obj_)) {
    return self::$model_obj_->get_apc();
  }
  // else
  return self::$apc_;
}


//
private static $config_obj_ = null;	// configインスタンス
private static $model_obj_ = null;	// modelインスタンス

// 以下、データハンドル系。あんまり使う想定ないんだけど、model使わずに工場使いたいときに
private static $dbh_ = null;
private static $kvs_ = null;
private static $apc_ = null;


} // end of class


