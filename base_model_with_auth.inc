<?php

/**
 * MVC Model部分 認証機能のある基底クラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 */

require_once('base_model.inc');
require_once('session.inc');
require_once('mobile_info.inc');
require_once('conv_util.inc');

abstract class base_model_with_auth extends base_model {

//public:
public function __construct()
{
  //
  parent::__construct();
  //$this->init();
}


//初期化
public function init(){
  // 親を叩いておく
  parent::init();
  // 基本的な初期値の設定
  $this->set_authtype_auth(); // 基本は「認証必須」
}

/**
 * 認証処理起動
 *
 * @access public
 */
public function initialize() {
  // 親を叩いておく
  parent::initialize();

  // 認証のデフォルトだけ設定しておく
  // XXX 念のため〜
  if ('' === $this->get_authorization_name()) {
    $this->set_authorization_name('authorization_cookie');
  }

  //
  return true;
}

/**
 * 認証インスタンスのゲッタ
 *
 * @access protected
 */
public function get_authorization()
{
  //
  if (null === $this->authorization_) {
    $this->authorization_ = new $this->get_authorization_name();
  }
  //
  return $this->authorization_;
}

// 認証用のクラス名
public function set_authorization_name($s) { $this->authorization_name_ = $s; }
public function get_authorization_name() { return $this->authorization_name_; }

// 認証でエラーだったときの行き先
public function set_authorization_error_command($s) { $this->authorization_error_command_ = $s; }
public function get_authorization_error_command() { return $this->authorization_error_command_; }

/*
 * modelマウントポイント
 *
 * 適切な認証チェックとかやって適切なメソッドにぶんなげる
 * 認証チェック自体は、initalize時にすでに処理が終了している
 *
 * @access public
 */
public function execute() {

  // 身もふたもなく聞く
  if (false === $this->is_bag('is_auth')) {
    $r = $this->get_authorization()->is_auth();
    // 認証状態を入れておく
    $this->set_bag('is_auth',$r);
  }
  // で、認証状態を把握
  $r = $this->get_bag('is_auth');

  // 認証できなければつっかえす
  if (false === $r) {
    // 認証が必須なのであれば
    if (true === $this->is_authtype_auth()) {
      // 認証できなかったんだから、突っ返す
      $this->recursive_on();
      $this->set_recursive_command($this->get_authorization_error_command());
      return ;
    }
  } else {
    // 認証できたらしいので
    // IDをセット
    $this->set_id( $this->get_authorization()->get_id() );
  }

  // 認証後のexecをcall
  return $this->execute_auth();
}

/**
 * 認証状態のチェック
 *
 * @access public
 * @return boolean 認証状態ならtrue、非認証状態ならfalse
 */
public function is_auth() { return $this->get_bag('is_auth'); }

// XXX セッション周り未実装〜






// -------------------------------------------

/**
 * ユーザIDを取得する
 *
 * ユーザIDとは
 * ・通常においては内部ユーザID
 * ・匿名セッションにおいてはトークン
 * ・携帯に関してはuidまたはサブスクライバIDの類
 * ・DoCoMoについては 公式フラグ が立っていればcgi requestのuid
 * を指すものとする
 *
 * @access public
 * @return string ユーザID 存在しない場合空文字
 */
public function get_id() { return $this->id_; }
protected function set_id($o) { $this->id_ = $o; }

/**
 * 認証タイプを「認証必須」にする
 *
 * @access public
 */
public function set_authtype_auth() { $this->set_authtype(1); }

/**
 * 認証タイプが「認証必須」かどうか
 *
 * @access public
 * @return boolean 「認証必須」ならtrue、そうでなければfalse
 */
public function is_authtype_auth() { return $this->_is_authtype(1); }


/**
 * 認証タイプを「認証でも非認証でも」にする
 *
 * @access public
 * @return string ユーザID 存在しない場合空文字
 */
public function set_authtype_ambiguous_auth() { $this->set_authtype(2); }

/**
 * 認証タイプが「認証でも非認証でも」かどうか
 *
 * @access public
 * @return boolean 「認証でも非認証でも」ならtrue、そうでなければfalse
 */
public function is_authtype_ambiguous_auth() { return $this->_is_authtype(2); }

// この辺は公開しないので
protected function set_authtype($i) { $this->authtype_ = $i; }
protected function get_authtype() { return $this->authtype_; }
protected function _is_authtype($i)
{
  if ($i == $this->get_authtype()) {
    return true;
  }
  // else
  return false;
}


//private:
private $authorization_name_ = '';
private $authorization_ = null;
private $authorization_error_command_ = '';
//
private $id_ = '';
private $_is_authtype;


} // end of class


