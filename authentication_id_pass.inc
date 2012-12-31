<?php

/**
 * IDとパスワードによる認証機構
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

class authentication_id_pass {

public function __construct()
{
  //
  //parent::__construct();
  $this->init();
}

/**
 * 初期化
 *
 * @access public
 */
public function init()
{
  $this->set_id('');
  $this->set_raw_password('');
  $this->set_authentication_clump(null);
}

/**
 * ログインID取得
 *
 * @access public
 * @return string ログインID
 */
public function get_id() { return $this->id_; }
/**
 * ログインID設定
 *
 * @access public
 * @param string $v ログインID
 */
public function set_id($v) { $this->id_ = $v; }

/**
 * raw_password(生パスワード)取得
 *
 * @access public
 * @return string 生パスワード
 */
public function get_raw_password() { return $this->raw_password_; }
/**
 * raw_password(生パスワード)設定
 *
 * @access public
 * @param string $v 生パスワード
 */
public function set_raw_password($v) { $this->raw_password_ = $v; }

/**
 * authentication_clump名取得
 *
 * @access public
 * @return string authentication_clump名
 */
public function get_authentication_clump() { return $this->authentication_clump_; }
/**
 * authentication_clump名設定
 *
 * @access public
 * @param string $v authentication_clump名
 */
public function set_authentication_clump($v) { $this->authentication_clump_ = $v; }


/**
 * ログイン処理
 *
 * loginにまつわる処理を一式
 *
 * @access public
 * @return boolean ログイン成功ならtrue、失敗ならfalse
 */
public function login()
{
  // IDの把握
  $id = $this->get_id();

  // パスワードのストレッチング
  $raw_pass = $this->get_raw_password();
  $pobj = di::create('authentication_password_stretching');
  $password_string = $pobj->stretching($id, $raw_pass);
//var_dump($password_string);

  // そのIDある？
  $obj = $this->get_authentication_clump();
  $obj->set_value('id', $id); // XXX magicnumber
  $r = $obj->get();
//var_dump($r);
  if (false === $r) {
    // ないらしいので認証失敗
    return false;
  }

  // ロック中？
/*
XXXXXXXXXXXXXXXXXX
lock_time
*/

  // パスワードあってる？
//var_dump($password_string);
//var_dump($obj->get_value('pass'));
  if ($password_string !== $obj->get_value('pass')) { // XXX magicnumber

    // エラーカウントインクリメント

    // エラーカウントが一定以上なら、一端、ロック

    //
    return false;
  }

  // 情報一通りOKっぽい

  // ロックカウントが１以上なら、クリアしておく
/*
XXXXXXXXXXXXXXXX
err_count
*/

  // ログインはOKだったよ〜 ノ
  return true;
}


//private:
private $id_;
private $raw_password_;
private $password_stretching_class_name_;
private $authentication_clump_;


} // end of class

