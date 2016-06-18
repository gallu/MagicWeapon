<?php

/**
 * PHPセッションを使った認可クラス(authorization)
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/
require_once('authorization_base.inc');

class authorization_session extends authorization_base {

public function __construct()
{
  //
  parent::__construct();
  $this->set_key_name('front'); // デフォルトはfront
  $this->set_uid_key_name('uid'); // デフォルトはuid
  $this->set_user_lock_clump(null); // デフォルトはnull
}

/**
 * 設定ファイルからの設定諸々取り込み
 *
 * @param config $conf config系クラスのインスタンス
 * @access public
 */
public function set_config(config $config) {
  // 先に親をcall
  parent::set_config($config);

  // セッションの「認証情報全体を格納する」key名が設定されていたらそちらに置き換え
  if ('' !== ($s = $config->find('session_key_name'))) {
    $this->set_key_name($s);
  }
  // セッションの「uidを格納する」key名が設定されていたらそちらに置き換える
  if ('' !== ($s = $config->find('session_uid_key_name'))) {
    $this->set_uid_key_name($s);
  }
}

/**
 * $_SESSIONに格納するkey名を指定する
 *
 * $_SESSION[$v]の中にuidその他様々な情報が入る(から、これをunsetする事でログイン情報が消える)
 *
 * @access public
 * @param string $v $_SESSIONの配列key名
 */
public function set_key_name($v) { $this->key_name_ = $v; }

/**
 * $_SESSIONに格納するkey名を取得する
 *
 * $_SESSION[$v]の中にuidその他様々な情報が入る(ので、追加するならこの中に情報を追加する)
 *
 * @access public
 * @return string $_SESSIONの配列key名
 */
public function get_key_name() { return $this->key_name_; }

/**
 * $_SESSIONにユーザIDを格納するkey名を指定する
 *
 * $_SESSION[$this->set_key_name()][$v]の中にuidが入る
 *
 * @access public
 * @param string $v $_SESSIONの配列key名(uid用)
 */
public function set_uid_key_name($v) { $this->uid_key_name_ = $v; }

/**
 * $_SESSIONにユーザIDを格納するkey名を取得する
 *
 * $_SESSION[$this->set_key_name()][$v]の中にuidが入ってる
 *
 * @access public
 * @return string $_SESSIONの配列key名(uid用)
 */
public function get_uid_key_name($v) { return $this->uid_key_name_ = $v; }

/**
 * 「ロックテーブル」のclumpインスタンスを設定する
 *
 * ロックテーブルにuidが存在している場合「いかなる時にも非認可状態」とする
 * XXX 後で基底に持ち上げるかも
<pre>
DROP TABLE IF EXISTS admin_auth_lock;
CREATE TABLE admin_auth_lock (
  `id` varbinary(128) NOT NULL COMMENT 'ユーザ(管理者)を識別するためのID',
  -- 以下、authorization_系クラスの処理では使わない(けどまぁよくつかう)カラム群
  `created` datetime NOT NULL COMMENT '作成日時',
  `memo` text NOT NULL COMMENT 'lockした理由とか',
  PRIMARY KEY (`id`)
)CHARACTER SET 'utf8mb4', ENGINE=InnoDB, COMMENT='1レコードが「ロックされた1ユーザ」を意味するテーブル';
</pre>
 *
 * @access public
 * @param data_clump $v ロックテーブル(data_clump派生クラス)のインスタンス
 */
public function set_user_lock_clump($v) { $this->user_lock_clump_ = $v; }
protected function get_user_lock_clump() { return $this->user_lock_clump_; }

/**
 * 認可されてるユーザの情報一式を取り出す
 *
 * @access public
 * @return array keyにdatabaseのカラム名(ようはdata_clumpのget_all_data_to_hash()の値)
 */
public function get_all_user_data() {
  return $_SESSION[$this->get_key_name()]['clump']; // XXX magic number
}

/**
 * 認可を開始する
 *
 * $_SESSION[$this->get_key_name()][(引数の)$key]の中にuidが入る
 *
 * @access public
 * @param authentication_clump $auth_o authentication_clumpインスタンス
 * @return boolean なにかあったらfalse
 */
public function auth_on($auth_o) {

  // セッションIDを張り替える
  session_regenerate_id();
  // uidを設定する
  $_SESSION[$this->get_key_name()][$this->get_uid_key_name()] = $auth_o->get_value('id');
  // clumpのデータを一式保存しておく(roleとかがあり得るので) XXX 多分後でメソッド化する(「設定変更後のreload」が出来るように)
  $_SESSION[$this->get_key_name()]['clump'] = $auth_o->get_all_data_to_hash(); // XXX magic number
  // 寿命を設定する
  $this->set_session_expire();

  //
  return true;
}

/**
 * 認可を終了する(ログアウト)
 *
 * @access public
 * @return boolean なにかあったらfalse
 */
public function auth_off() {
  // セッションIDを張り替える
  // XXX 蛇足かなぁと思わなくもないけど念のため
  session_regenerate_id();
  // セッション情報を抹消する
  unset($_SESSION[$this->get_key_name()]);
  //
  return true;
}

/**
 * 認可を確認する
 *
 * @access public
 * @return boolean 認証できたらtrue、できなきゃfalse
 */
public function is_auth() {

  //
  $uid = (string)@$_SESSION[$this->get_key_name()][$this->get_uid_key_name()];
  if ('' === $uid) {
    // uid情報が存在しないので非認可状態
    return false;
  }
  // 寿命チェック
  if (time() > $_SESSION[$this->get_key_name()]['expire']) { // XXX magic number
    // 寿命が切れているので、セッション情報をクリアした上で非認可状態
    $this->auth_off();
    return false;
  }

  // 「ロックテーブルが存在して、かつユーザIDがそこに含まれてたら非認可状態にする」処理
  $lock_c = $this->get_user_lock_clump();
  if (null !== $lock_c) {
    $lock_c->set_value('id', $uid);
    $r = $lock_c->get(); // uidをkeyにしてレコード取得
    //
    if (true === $r) {
      // ロック対象ユーザなので、認可を取り下げた上で非認可状態
      $this->auth_off();
      return false;
    }
  }

  // 問題なさげなので
  // 寿命を再設定する
  $this->set_session_expire();
  // 認可状態でreturn
  return true;
}

/**
 * 現在認証されているuidを取得する
 *
 * @access public
 * @return string 現在認可されているユーザID(非認可の場合は空文字)
 */
public function get_uid() {
  return (string)@$_SESSION[$this->get_key_name()][$this->get_uid_key_name()];
}

/**
 * セッションの寿命を設定する
 *
 * @access protected
 */
protected function set_session_expire() {
  $_SESSION[$this->get_key_name()]['expire'] = time() + $this->get_expire(); // XXX magic number
}


//private:
private $key_name_;
private $uid_key_name_;
private $user_lock_clump_;

} // end of class

