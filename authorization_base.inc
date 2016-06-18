<?php

/**
 * 認可クラス(authorization)基底
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/


abstract class authorization_base {

public function __construct()
{
  //
  $this->set_expire(600); // デフォルトの寿命は10分
  $this->set_uid('');
}

/**
 * 設定ファイルからの設定諸々取り込み
 *
 * @param config $conf config系クラスのインスタンス
 * @access public
 */
public function set_config(config $config) {
  // 寿命が設定されていれば、そちらに設定しなおす
  if ('' !== ($i = $config->find('session_expire'))) {
    $this->set_expire($i);
  }
}

/**
 * 認可寿命の設定
 *
 * @param int $v セッションの寿命(単位：秒)
 * @access public
 */
public function set_expire($v) { $this->expire_ = $v; }
public function get_expire() { return $this->expire_; }

/**
 * ユーザIDの取得
 *
 * @access public
 * @return string ユーザID。認証状態でない場合は空文字
 */
public function get_uid() { return $this->uid_; }
protected function set_uid($v) { $this->uid_ = $v; }

/**
 * 認可を開始する
 *
 * @access public
 * @param string $uid ユーザID
 * @return boolean なにかあったらfalse
 */
abstract public function auth_on($uid);

/**
 * 認可を終了する(ログアウト)
 *
 * @access public
 * @return boolean なにかあったらfalse
 */
abstract public function auth_off();

/**
 * 認可を確認する
 *
 * @access public
 * @return boolean 認証できたらtrue、できなきゃfalse
 */
abstract public function is_auth();


//private:
private $expire_;
private $uid_;
} // end of class

