<?php

/**
 * Cookieを用いた認証クラス(authorization)
 *
 * XXX あとで、上に持ち上げるもん持ち上げないとねぇ…
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/


class authorization_cookie {

public function __construct()
{
  //
  //parent::__construct();
  $this->init();
}

//
public function init()
{
  $this->set_expire(12000); /// 寿命は10分 : XXX テスト用に200分
  $this->set_session_key_name('sid');
  $this->set_clump_class_name(''); // clump名から解決できる前提
}

  // expire
  public function get_expire() { return $this->expire_; }
  public function set_expire($v) { $this->expire_ = $v; }
  // session_key_name
  public function get_session_key_name() { return $this->session_key_name_; }
  public function set_session_key_name($v) { $this->session_key_name_ = $v; }
  // session用clump ファイル名
  public function get_clump_name() { return $this->clump_name_; }
  public function set_clump_name($v) { $this->clump_name_ = $v; }
  // session用clump class名
  public function get_clump_class_name() { return $this->clump_class_name_; }
  public function set_clump_class_name($v) { $this->clump_class_name_ = $v; }

/**
 * 認可を開始する
 *
 * @access public
 * @param string $uid ユーザID
 * @param object $model model系インスタンス：clumpげとで使う
 * @return boolean なにかあったらfalse
 */
public function auth_on($uid, $model)
{
  return $this->create_session($uid, $model);
}


/**
 * 認可を確認する
 *
 * @access public
 * @param object $model model系インスタンス：clumpげとで使う
 * @return boolean 認証できたらtrue、できなきゃfalse
 */
public function is_auth($model)
{
//print "--------\n";

  //
  $k = $this->get_session_id($model);
//var_dump($k);
  if ('' === $k) {
    return false;
  }

  //
  $as = $this->get_session_clump($model);
  $as->set_value('sid', $k);
  $r = $as->get();
//var_dump($r);

  //
  if (false === $r) {
    return false;
  }

  // とりあえずcookieの値はあるっぽいので、寿命を判断する
  $exp = $as->get_value('expire');
//var_dump($exp);
//print "now time is " . time() . "\n";
//print "exp is " . $exp . "\n";


  // 時間切れなら、NG〜
  if (time() > $exp) {
    $as->del();
    return false;
  }

  // 寿命が50％を切ってたら
  // XXX これ、あとでメソッド化して、ある程度自由度上げたいねぇ…
//var_dump($exp - time());
  if (($this->get_expire() / 2) > ($exp - time())) {
    // 新しいsessionを発行
    $this->create_session($as->get_value('id'), $model);
  } else {
    // APIのほうで必要なので、一応設定だけはしておくようにする
    $this->create_session_core($as->get_value('sid'));
  }

  // user_id設定して
  $model->set_bag('user_id', $as->get_value('id'));

  // 認可OK〜
  return true;
}



/**
 * セッションIDを新しく作る
 *
 * @access protected
 * @param string $uid ユーザID
 * @param object $model model系インスタンス：clumpげとで使う
 * @return boolean なにかあったらfalse
 */
protected function create_session($uid, $model)
{
  //
  $as = $this->get_session_clump($model);

  //
  $as->set_value('id', $uid);
  $as->set_value('expire', time() + $this->get_expire()); // XXX

  // XXX これ、後でクラス化してそっちに変更する〜
  $fp = fopen('/dev/urandom', 'r');
  $token = hash('sha512', fread($fp, 256));
  //
  $as->set_value('sid', $token);
//var_dump($as->get_all_data_to_hash());

  //
  $as->insert();

  // 設定する
  $this->create_session_core($token);

  //
  return true;
}

// cookieからセッションIDを取得
public function get_session_id($model) {
  return $model->get_cgi_request()->find_cookie($this->get_session_key_name()); // XXXX
}

// セッションIDをcookieに設定
public function create_session_core($token) {
//print "into create_session_core {$token} \n";
  setcookie($this->get_session_key_name(), $token, 0, '/'); // XXXX
}



// セッションclumpのインスタンスげと
protected function get_session_clump($model) {
  //
  $class_name = $this->get_clump_class_name();
  if ('' === $class_name) {
    $class_name = 'clump/' . $this->get_clump_name() . '.inc';
  }
//var_dump($class_name);
require_once($class_name);
  //
  $obj = $model->get_clump( $this->get_clump_name() );
  //
  return $obj;
}



//private:
private $expire_;
private $session_key_name_;
private $clump_name_;
private $clump_class_name_;

} // end of class

