<?php

/**
 * MVC Model部分基底クラス
 *
 * 少しだけお便利げに作る。MVCのMを担当
 * skeltonからの追加は以下の通り
 * ・DB handle
 * ・Logクラス各種
 * ・convインスタンス取得
 * ・その他お便利関数いくつか
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 */

require_once('base_model_skeleton.inc');
require_once('data_clump_manager.inc');
require_once('log.inc');
require_once('log_dummy.inc');
require_once('file_util.inc');
require_once('conv_util.inc');
require_once('is.inc');
require_once('secure_conv.inc');

abstract class base_model extends base_model_skeleton {

//public:

//
public function __construct() {
  parent::__construct();
  // $this->init(); 上位で叩いているはずなので
}
//
public function init()
{
  parent::init();
}

/**
 * 共通的な終了処理
 *
 * テンプレートに、configから取得した「uri、ssl_uri、mx_domain」を設定
 * XXX ここでいいのか？ 場所は悩み中。
 *
 * @access public
 */
public function finalize(){
  // 常に表示するもの、の追加
  $this->get_conv()->monoDic('uri', $this->get_config()->find('uri'));
  $this->get_conv()->monoDic('ssl_uri', $this->get_config()->find('ssl_uri'));
  $this->get_conv()->monoDic('mx_domain', $this->get_config()->find('mx_domain'));

  // close系なのでこっちの処理が終わってからcallする
  parent::finalize();
}

/**
 * DBハンドル取得
 *
 * @access public
 * @return object DBハンドル
 */
public function get_db()
{
require_once('db_manager.inc');

  // 未接続なら接続しとけや
  if ( false === $this->is_bag('dbh') ) {
    $this->set_bag('dbh', db_manager::get_handle( $this->get_config() ));
  }

  // ハンドルを復帰
  return $this->get_bag('dbh');
}

/**
 * KVSハンドル取得
 *
 * @access public
 * @return object KVSハンドル
 */
public function get_kvs()
{
require_once('kvs_manager.inc');

  // 未接続なら接続しとけや
  if ( false === $this->is_bag('kvs') ) {
    $this->set_bag('kvs', kvs_manager::get_handle( $this->get_config() ));
  }

  // ハンドルを復帰
  return $this->get_bag('kvs');
}

/**
 * APCハンドル取得
 *
 * XXXハンドルっていうのかなぁ？
 *
 * @access public
 * @return object APCハンドル
 */
public function get_apc()
{
require_once('apc_handle.inc');

  // 未接続なら接続しとけや
  if ( false === $this->is_bag('apc') ) {
    $this->set_bag('apc', new apc_handle());
  }

  // ハンドルを復帰
  return $this->get_bag('apc');
}



/**
 * CONVインスタンス取得
 *
 * @access public
 * @return object convインスタンス
 */
public function get_conv() { return $this->get_bag('conv'); }

/**
 * body自動作成 with conv
 *
 * $thisのconvインスタンスと設定ファイルのtemplate_dir内にある
 * 引数テンプレートファイルを使ってbodyを自動作成する
 * 引数なし(又は空文字)の場合、mapファイルに設定されたtemplateが有効になる
 * 「ぢつはこのメソッドcallしなくてもいいじゃん」とか気づいちゃだめｗ
 *
 * @access protected
 * @param string $tmp_filename テンプレートファイル名
 * @return boolean なにか問題あったらfalse
 */
public function make_body_with_conv($tmp_filename = "")
{
  if ("" !== $tmp_filename) {
    $this->set_tmp_filename($tmp_filename);
  }
//var_dump($this->get_tmp_filename());
  //$this->set_body($this->get_conv()->conv(file_util::read_file($this->get_config()->find("template_dir") . $tmp_filename)));

  return true;
}

/**
 * エラーログ用logクラスインスタンス取得
 *
 * @access public
 * @return object logインスタンス エラーの場合はnull
 */
public function get_error_log() { return $this->_get_log('error'); }

/**
 * warningログ用logクラスインスタンス取得
 *
 * @access public
 * @return object logインスタンス エラーの場合はnull
 */
public function get_warning_log() { return $this->_get_log('warning'); }

/**
 * debugログ用logクラスインスタンス取得
 *
 * @access public
 * @return object logインスタンス エラーの場合はlog_dummyインスタンス
 */
public function get_debug_log() { return $this->_get_log('debug'); }

//
protected function _get_log($s)
{
  // 設定が足りなきゃdummy return 
  $s = $this->get_config()->find($s . '_log_path');
  if ("" === $s) {
    $obj = new log_dummy;
    $this->set_bag($s . '_log', $obj);
    return $this->get_bag($s . '_log');
  }

  //
  if (false === $this->is_bag($s . '_log')) {
//print "new create error_log \n";
    $obj = new log;
    //$obj->set_filename($s);
    $obj->set_rotate_filename($s);
    $obj->set_pname($this->get_my_name());
    $this->set_bag($s . '_log', $obj);
  }
  //
  return $this->get_bag($s . '_log');

}

/**
 * clumpインスタンス取得
 *
 * new してset_dbするか、或いはinit()してset_dbする
 *
 * @access public
 * @param string $s newしたいclumpの名称
 * @param object $o 再利用したい場合、ここにインスタンスをいれておく
 * @return object clumpインスタンス
 */
public function get_clump($s, $o = null)
{
// XXX 汚いかなぁ？
static $flg = false;
  //
  if (false === $flg) {
    // これはやっとかないと動かないしねぇ
    data_clump_manager::set_model($this);

    //
    $flg = true;
  }

  //
  return data_clump_manager::get_instance($s, $o);
}

/**
 * フォワード(プロセス内部で、別のmodelを呼び出す)
 *
 * XXX recursive_on＋set_recursive_commandでいいんだけど、こっちの方がわかりやすいとのことなので
 *
 * @access public
 * @param string $model_name forward先のモデル名
 */
public function forward($model_name) {
  $this->recursive_on();
  $this->set_recursive_command($model_name);
}

/**
 * リダイレクト(いわゆる、HTTPでlocationヘッダをぶちかます系)
 *
 * XXX 自力でやってもいいけど、使用頻度高めなので
 *
 * @access public
 * @param string $path 遷移させたいURI(相対でも絶対でも)
 * @param int $http_status 302(Moved Temporarily)か301(Moved Permanently)か。デフォは302。
 */
public function redirect($path, $http_status=302) {
  $this->get_cgi_response()->set_location($path, $http_status);
  $this->set_body(' ');
  $this->direct_output_on();
}

/**
 * cgi requestからの情報取得＋get_convでとれるインスタンスへの情報のセット
 *
 * XXX 所謂お便利メソッド
 *
 * @access public
 * @param string $k key名
 * @return string 得られた値(詳細はcgi_requestクラスのfindを参照
 */
public function get_from_cgi($k) {
  $s = $this->get_cgi_request()->find($k);
  $this->get_conv()->set($k, $s);
  //
  return $s;
}

/**
 * cgi requestからの情報取得＋get_convでとれるインスタンスへの情報のセット、の配列版(checkbox用)
 *
 * XXX 所謂お便利メソッド
 *
 * @access public
 * @param string $k key名
 * @return array 得られた値(詳細はcgi_requestクラスのfind_arrayを参照
 */
public function get_from_cgi_array($k) {
  $s = $this->get_cgi_request()->find_array($k);
  $this->get_conv()->set($k, $s);
  //
  return $s;
}




//private:
} // end of class


