<?php

/**
 * 既製品クラス：login用
 *
 * XXX 現在非推奨
 *
 * session.incを基準にしたsessionクラス郡を使うことを前提にしたお便利class
 * 設定ファイル適切に設定すれば、ほぼそのまま使えるとおもう。
 * mapファイルでこのファイル指定すればしゅ?りょ?
 * 無理なら適宜作り直すなり継承するなりしてちょ。
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 */

require_once('base_model.inc');
require_once('file_util.inc');
require_once('session.inc');


class session_module_login extends base_model {

//
public function initialize() {  }
public function finalize() {  }

//処理
public function execute() {

  // cgiリクエストが詰まったオブジェクト取ってくる
  $req = $this->get_cgi_request();

  // config設定?
  $obj = new session;
  $obj->set_config_filename($this->get_config()->find('session_config'));
  $obj->set_db($this->get_db());
  $sconfig = $obj->get_config();
//var_dump($sconfig);

  // IDとパスワードを取得
  $id = $req->find($sconfig->get_value('login_id_form_name'));
  $pass = $req->find($sconfig->get_value('login_pass_form_name'));
//print "id is " . $id . "\n";
//print "pass is " . $pass . "\n";

  $ret = $obj->login($id, $pass);
//var_dump($ret);
//print $obj->get_error_string() . "\n";

  //
  $conv = $this->get_conv();

  // エラー処理：とりあえずindexに突っ返す？
  if (false === $ret) {
    $this->recursive_on();
    $this->set_recursive_command($sconfig->get_value('login_error_return_command'));
    $conv->monoDic('login_error', $obj->get_error_string());
    $sp = new simple_put;
    $conv->multiDic('is_login_error', $sp);
    // XXX IDはいれとくかとりあえず
    $conv->monoDic($sconfig->get_value('login_id_form_name'), $id);
    return ;
  }
  // else
  // ちと場当たり的にだが情報を格納
  if (true === $sconfig->is_value('auth_key_cookie')) {
    $name = $sconfig->get_value('auth_key_cookie_name');
    // XXX 場当たり的対応
    $_COOKIE[$name] = $obj->get_key();
//print "add on Cookie\n";
//print_r($_COOKIE);
  }
  if (true === $sconfig->is_value('auth_key_post')) {
    $name = $sconfig->get_value('auth_key_post_name');
    // XXX 場当たり的対応
    $_POST[$name] = $obj->get_key();
//print "add on post\n";
//print_r($_POST);
  }
  if (true === $sconfig->is_value('auth_key_get')) {
    $name = $sconfig->get_value('auth_key_get_name');
    // XXX 場当たり的対応
    $_GET[$name] = $obj->get_key();
//print "add on get\n";
//print_r($_GET);
  }

  // topに再帰
  $this->recursive_on();
  $this->set_recursive_command($sconfig->get_value('login_return_command'));
  return ;
}

} // end of class
