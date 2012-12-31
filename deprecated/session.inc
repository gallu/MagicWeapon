<?php

/**
 * セッションクラス
 *
 * 実際には、このクラスはGoFによるところのfacadeパターンである。
 * このクラスはインタフェースのみの提供となる。
 * XXX 現在非推奨
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

//require_once('session_auth_add.inc');
require_once('session_id.inc');
require_once('session_start_login.inc');
require_once('session_config.inc');

class session {

public function __construct()
{
  //
  $this->init();
}

//
public function init()
{
  $this->set_config(null);
  $this->set_login(null);
}

//
protected function set_config($o) { $this->config_ = $o; }
public function get_config() { return $this->config_; }

protected function set_login($o) { $this->login_ = $o; }
public function get_login() { return $this->login_; }

protected function set_session_id($s) { $this->sid_ = $s; }
public function get_session_id() { return $this->sid_; }

protected function set_id($s) { $this->id_ = $s; }
public function get_id() { return $this->id_; }

public function set_db($o) { $this->dbh_ = $o; }
protected function get_db() { return $this->dbh_; }

//
protected function set_error_string($s) { $this->error_string_ = $s; }
public function get_error_string() { return $this->error_string_; }


/**
 * インスタンス自動生成付きgetter
 *
 * すでにnewされてればなにもしない。nullなら必要なclassをnewする
 * で、指定されたobjectインスタンス(参照)を返す
 *
 * @access protected
 * @param string $class_name newすべきクラス名
 * @param string $val_name setter/getterで使う名前
 * @return object 必要なインスタンス参照。エラーの場合null
 */
protected function get_object($class_name, $val_name = '')
{
  //
  if ('' === $val_name) {
    $val_name = $class_name;
  }

  //
  $o = call_user_func(array($this, 'get_' . $val_name));
  if (is_null($o)) {
    $o = new $class_name;
    call_user_func(array($this, 'set_' . $val_name), $o);
//print "new $class_name \n";
  }
// XXX
//$o = call_user_func(array($this, 'get_' . $val_name));
//var_dump($o);

  //
  return $o;
}

/**
 * configファイル名設定
 *
 * 実際には、class newしてファイル名設定してparceまで
 *
 * @access public
 * @param string $filename configファイル名
 * @return boolean なんか問題あればfalse
 */
public function set_config_filename($filename)
{
  // インスタンスげと
  $o = $this->get_object('session_config', 'config');
//var_dump($o);

  // ファイル設定
  $o->set_filename($filename);
//var_dump($o);

  //
  return true;
}

/**
 * ログイン
 *
 * IDとパスワードチェックして、OKならセッションをスタートさせる
 *
 * @access public
 * @param string $id ログイン用のID。e-mailってパターンもよくあるよねぇ。
 * @param string $pass ログイン用の秘密情報。いわゆるパスワード
 * @return boolean ログインできたらtrue。できなきゃfalse。
 */
public function login($id, $pass)
{
//print "$id $pass \n";
  // インスタンスげと
  $o = $this->get_object('session_start_login', 'login');

  // XXX エラーチェックどうしよ？
  $o->set_config($this->get_config());
  $o->set_db($this->get_db());
//var_dump($this->get_db());
  $ret = $o->is_member($id, $pass);
//var_dump($ret);

  //
  if (false === $ret) {
    $this->set_error_string('login:' . $o->get_error_string());
    return false;
  }

  // OKっぽいのでセッションを作成する
  $o = $this->get_object('session_id');
  $o->set_config($this->get_config());
  $o->set_db($this->get_db());
  $ret = $o->create($id);

  //
  if (false === $ret) {
    $this->set_error_string('create session:' . $o->get_error_string());
  } else {
    $this->set_id($id);
  }

  //
  return $ret;
}

/**
 * keyの取得
 *
 * かならず、loginまたはauthの後にcallすること
 *
 * @access public
 * @return string エラーの場合は空文字
 */
public function get_key()
{
  //
  $o = $this->get_object('session_id');
  // XXX 既存のインスタンスがあるはずなので以下二つは省略
  //$o->set_config($this->get_config());
  //$o->set_db($this->get_db());

  //
  return $o->get_key();
}

/**
 * 認証
 *
 * @access public
 * @param string $key key情報
 * @return boolean 認証OKならtrue。できなきゃfalse。
 */
public function auth($key)
{
  //
  $o = $this->get_object('session_id');
  $o->set_config($this->get_config());
  $o->set_db($this->get_db());

  //
  $ret = $o->auth($key);
  //
  if (false === $ret) {
    $this->set_error_string('auth:' . $o->get_error_string());
  } else {
    $this->set_id($o->get_id());
  }

  //
  return $ret;
}


//private:
private $config_; // config
private $login_; // session_start_login
private $sid_; // session_id
private $id_; // session_id

private $dbh_;
private $error_string_;

} // end of class

