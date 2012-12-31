<?php

/**
 * session.confのラッパークラス
 *
 * XXX 現在非推奨
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once('config.inc');

class session_config {

public function __construct()
{
  //
  $this->init();
}

//
public function init()
{
  $this->set_config_obj(new config);
  // デフォルト値の設定
  $this->default_ = array(
    'password_crypt_type' => 'sha-1',
    'auth_type' => '0',
    'auth_clump' => 'session_auth_clump.inc:session_auth_clump',
    'auth_clump_table_name' => 'auth_user',
    'session_clump_table_name' => 'auth_session',
    'user_clump' => 'session_user_clump.inc:user',
    'foreign_key_auth' => 'id',
    'foreign_key_user' => 'id',
    'id_tuple_name' => 'email',
    'pass_error_count' => '5',
    'pass_error_lock_time' => '3600',
    'session_expire' => '600',
    'session_expire_flg' => '1',
    'login_id_form_name' => 'id',
    'login_pass_form_name' => 'pass',
    'login_error_return_command' => 'index',
    'login_return_command' => 'top',
    'auth_key_cookie' => 'on',
    'auth_key_cookie_name' => 's',
    'auth_key_post' => 'off',
    'auth_key_post_name' => 's',
    'auth_key_get' => 'off',
    'auth_error_return_command' => 'index',

  );
}

//
protected function set_config_obj($o) { $this->config_obj_ = $o; }
protected function get_config_obj() { return $this->config_obj_; }


/**
 * ファイル名設定
 *
 * config設定が書かれているファイル名を設定
 *
 * @access public
 * @param string $filename 設定ファイルのファイル名
 * @return boolean ファイルがないなどの何らかの異常がある場合false
 */
public function set_filename($filename)
{
  //
  return $this->get_config_obj()->set_file($filename);
}

/**
 * 設定情報の取得各種
 *
 * defaultもあるので別に切り出す
 *
 * @access public
 * @param string $name 設定名
 * @return string 問題がある場合は空文字復帰
 */
public function get_value($name)
{
  // まずげと
  $s = $this->get_config_obj()->find($name);
  if ("" === $s) {
    if (true === array_key_exists($name, $this->default_)) {
      $s = $this->default_[$name];
    }
  }
  //
  return $s;
}

/**
 * 設定情報の取得各種
 *
 * is系なので復帰がboolean
 * on | ok | true | y[es] | alloy を認識してtrueを、それ以外でfalseを返す
 *
 * @access public
 * @param string $name 設定名
 * @return string 問題がある場合は空文字復帰
 */
public function is_value($name)
{
  // とりあえず普通に値をげと
  $v = trim($this->get_value($name));
//print "is_value $name -> $v \n";

  // 値を判定
  // XXX たぶん、速度的にif連打のほうが早い
  // switchぢゃcasecmp使えないし :-p
  $ret = false;
  if (0 === strcasecmp($v, "on")) {
    $ret = true;
  } else
  if (0 === strcasecmp($v, "ok")) {
    $ret = true;
  } else
  if (0 === strcasecmp($v, "y")) {
    $ret = true;
  } else
  if (0 === strcasecmp($v, "yes")) {
    $ret = true;
  } else
  if (0 === strcasecmp($v, "allow")) {
    $ret = true;
  }

//var_dump($ret);
  return $ret;
}


//private:
private $config_obj_;
private $default_;

} // end of class

