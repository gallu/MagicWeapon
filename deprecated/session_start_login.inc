<?php

/**
 * セッションログインクラス
 *
 * XXX 現在非推奨
 *
 * IDとpasswordによるログインでセッションを開始するためのクラス
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once('session_base.inc');
require_once('config_util.inc');

class session_start_login extends session_base {

public function __construct()
{
  //
  //$this->init();
  parent::__construct();
}

//
public function init()
{
  parent::init();
}

/**
 * メンバーの確認
 *
 * DBにあるIDとパスワードをチェック
 * このメソッド自体は「IDとパスワードが一致していたらtrueを返す」だけである
 *
 * @access public
 * @param string $id ログインに用いられるID
 * @param string $pass ログインに用いられる秘密情報(パスワード)
 * @return boolean IDと秘密情報が一致すればtrue、それ以外ならfalse
 */
public function is_member($id, $pass)
{
  // passwordのcrypt方法を取得
  $crypt_type = $this->get_config()->get_value('password_crypt_type');
//print "Trap 1\n";
//print "crypt type is $crypt_type \n";

  // パスワードを暗号化 もしくは ハッシュ化
  if ('sha-1' === $crypt_type) {
    $pass = sha1($pass, false);
  } else
  if ('md5' === $crypt_type) {
    $pass = md5($pass, false);
  } else
  if ('none' === $crypt_type) {
    ;
  } else {
//print "crypt type '$crypt_type' is invalid!!\n";
    $this->set_error_string("crypt type '$crypt_type' is invalid");
    return false;
  }
//print "pass is $pass \n";
//print "Trap 2\n";

  // DBからパスワード情報をひっぱってくる
  // XXX あとでラッピングしてtextモードにも対応できるようにする？
  // auth_clumpの下準備
  $wk = $this->get_config()->get_value('auth_clump');
//print "$wk \n";
  $auth_obj = config_util::create_instance($wk);  
//var_dump($auth_obj);
  if (true === is_null($auth_obj)) {
//print "$wk is invalid!! \n";
    $this->set_error_string("$wk is invalid");
    return false;
  }
  // テーブル名が設定されていたら上書き
  $wk = $this->get_config()->get_value('auth_clump_table_name');
  if ("" !== $wk) {
    $auth_obj->set_table_name($wk);
//print "set table_name $wk \n";
  }
//print "Trap 3\n";

  // auth typeを取得
  $auth_type = $this->get_config()->get_value('auth_type');
  if ("0" === $auth_type) {
    // 普通に指定されたIDを突っ込む
    $auth_obj->set_value('id', $id, false);
  } else
  if ("1" === $auth_type) {
    // XXX 未実装!!
    $auth_obj->set_value('id', $id, false);
  } else {
//print "auth type '$auth_type' is invalid!!\n";
    $this->set_error_string("auth type '$auth_type' is invalid");
    return false;
  }
//print "auth type is $auth_type \n";
//print "Trap 4\n";

  // DBH設定して情報げと
//var_dump($this->get_db());
  $auth_obj->set_db($this->get_db());
  $ret = $auth_obj->get();
//var_dump($ret);
//var_dump($auth_obj);
//print "Trap 5\n";

  // 存在してなければNG
  if (false === $ret) {
    // no user
    $this->set_error_string("no user");
    return false;
  }

  // 永続的ログインロックの確認
  $ret = $this->check_locked($auth_obj->get_value('status'));
  if (false === $ret) {
    // locked
    $this->set_error_string("locked");
    return false;
  }
//print "Trap 6\n";

  // 一時的ロックの確認
  if (0 < $auth_obj->get_value('lock_time') ) {
//print "into time lock \n";
    $twk = time();
    if ($twk < $auth_obj->get_value('lock_time')) {
//print "into time lock NG\n";
      // まだロック期間なのでエラーで突っ返す
      $this->set_error_string("time locked");
      return false;
    }
    // ロック期限を越えているので、ロックをはずす
    $auth_obj->set_value('lock_time', 0);
  }

  // エラーカウントの確認
  $error_count = $this->get_config()->get_value('pass_error_count');
//print "$error_count \n";
  if ($auth_obj->get_value('err_count') > $error_count) {
//print "error count in \n";
    // 許容範囲を超えているのでロックする
    $lt = $this->get_config()->get_value('pass_error_lock_time');
    $auth_obj->set_value('err_count', 0, true);
    $auth_obj->set_value('lock_time', time() + $lt, true);

    //
    $this->set_error_string("time locked start");
    return false;
  }

  // パスワードの確認
  if ($pass !== $auth_obj->get_value('pass')) {
    // エラーカウンタをインクリメント
    $auth_obj->set_value('err_count', $auth_obj->get_value('err_count') + 1);
    //
//print "error count is invalid\n";
    $this->set_error_string("password invalid");
    return false;
  }

  // 最終的にOKっぽいのでtrue
  return true;
}

/**
 * ステータス確認
 *
 * あとで継承拡張できるようにお外に。
 * 基本は「０なら正常０以外ならロック状態」
 *
 * @access protected
 * @param int $status ステータス
 * @return boolean 正常ならtrue、ロック状態ならfalse
 */
protected function check_locked($status)
{
  if (0 == $status) {
    return true;
  }
  // else
  return false;
}


//private:

} // end of class


