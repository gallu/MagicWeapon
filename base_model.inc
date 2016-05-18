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
 * 共通的な初期処理
 *
 * 実際には「レイアウト用」の初期処理。レイアウトは使ったり使わなかったりするだろうけど、重い処理ではないので共通にしておく
 *
 *
 * @access public
 */
public function initialize() {
  // 親呼んで
  $r = parent::initialize();
  // デフォルトのレイアウトだけ設定しておく
  if (false === $this->is_bag('layout')) {
    $this->set_layout_filename('layout.tpl');
  }

  //
  return $r;
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

/**
 * レイアウトファイル名の取得(view系のクラスで使う事を想定)
 *
 * @access public
 * @return string レイアウトファイル名。レイアウト未適用の場合は空文字
 */
public function get_layout_filename() {
  if (false === $this->is_bag('layout')) {
    return '';
  }
  // else
  return $this->get_bag('layout');
}

/**
 * レイアウトファイル名の設定
 *
 * @access protected
 * @param string $v レイアウトファイル名。レイアウト未適用の場合は空文字
 */
protected function set_layout_filename($v) { $this->set_bag('layout', $v); }

/**
 * CSRFトークン名の取得
 *
 * @access protected
 * @return string セッションに入れるCSRF配列のkey名($_SESSION[$this->get_csrf_token_name()]['csrf_token'])
 */
protected function get_csrf_token_name() {
  $name = $this->get_config()->find('csrf_token_name');
  if ('' === $name) {
    $name = 'csrf';
  }
  //
  return $name;
}

/**
 * CSRFトークンの登録
 *
 * セッションにCSRFトークンを一定数(csrf_token_num個)登録 ＋ テンプレートに設定(monoDic()使用)
 *
 * @access protected
 */
protected function set_csrf_token() {
  // CSRFトークン数の把握
  $num = $this->get_config()->find('csrf_token_num') + 0;
  if (0 >= $num) {
    $num = 10;
  }
//var_dump($num);

  // トークンの作成
  $diid = di::create('difficulty_imagining_identification');
  $token = $diid->get_base64();
//var_dump($token);

  // テンプレートへの登録
  $this->get_conv()->monoDic('csrf_token', $token);

  // token数の調整
  while ($num <= count(@$_SESSION[$this->get_csrf_token_name()]['csrf_token'])) {
    array_shift($_SESSION[$this->get_csrf_token_name()]['csrf_token']);
  }

  // セッションへの登録
  $_SESSION[$this->get_csrf_token_name()]['csrf_token'][$token] = time();

}

/**
 * CSRFトークンの確認
 *
 * @access protected
 * @return boolean 有効なCSRFトークンが設定されていればtrue、無効ならfalse
 */
protected function is_csrf_token() {
  // CSRFチェック
  $post_csrf_token = $this->get_cgi_request()->find('csrf_token');
  if (false === isset($_SESSION[$this->get_csrf_token_name()]['csrf_token'][$post_csrf_token])) {
    return false;
  }

  // 寿命の設定を把握
  $expire = $this->get_config()->find('csrf_token_expire') + 0;
  if (0 >= $expire) {
    $expire = 60;
  }
//var_dump($expire);


  // 寿命を把握して
  $ttl = $_SESSION[$this->get_csrf_token_name()]['csrf_token'][$post_csrf_token];
  // 先にトークンは削除(使い捨てなので)
  unset($_SESSION[$this->get_csrf_token_name()]['csrf_token'][$post_csrf_token]);
  // 寿命チェック(1分以内)
  if (time() >=  $ttl + $expire) {
    return false;
  }

  //
  return true;
}

//private:
} // end of class


