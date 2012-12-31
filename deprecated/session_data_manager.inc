<?php

/**
 * セッションデータ マネージャクラス
 *
 * いわゆる ファクトリー
 * XXX 必要になってからcallして?
 *
 * @deprecated
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once('session_data_db.inc');

class session_data_manager
{

/**
 * セッションデータインスタンスの取得
 *
 * model内のconfig情報で振り分ける…予定
 *
 *
 * @access public
 * @param base_model_object base_model系のインスタンス
 * @return session_data_object セッションデータ系インスタンス
 */
static public function get_object($param)
{
  // XXX 当面、DB 固定

  // インスタンスに対して「使える」ところまで処理
  $o = new session_data_db;

  // DB handle
  $o->set_db($param->get_db());

  // XXX 保存用テーブル名の設定



  // IDの設定
  $o->set_key($param->get_id());

  // 一端読み込みをしておく
  $o->read();

  //
  return $o;
}

} // end of class
