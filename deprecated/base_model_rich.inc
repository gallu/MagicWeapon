<?php

/**
 * MVC Model部分 色々できるよ用基底クラス
 *
 * このクラス直使うことはないと思うんだけどなぁ
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 */

require_once('base_model.inc');
require_once('session.inc');
require_once('mobile_info.inc');
require_once('conv_util.inc');

abstract class base_model_rich extends base_model {

//public:
public function __construct()
{
  //
  parent::__construct();
  //$this->init();
}


//初期化
public function init(){
  parent::init();
  $this->set_session_obj(null);
  $this->auth_state_off();
  $this->set_id('');
  $this->set_authtype_auth();
  $this->with_mobile_off();
  //
  $this->set_authentication_type(0);
  $this->set_authorization_type(0);
  //
  $this->set_login_error_command('');
  $this->set_login_ok_command('');
  //
  $this->set_authorization_cookie_name('k');
  $this->set_authorization_crypt_key('');
  $this->set_authorization_expire_flg(1);
  $this->set_authorization_expire(600);
  //
  $this->session_data_obj_ = null;
  //
  $this->set_client_clump_name('client_clump');
  $this->set_client_keyname('client_id');
}

//public:

/**
 * ログインチェックでエラーだった場合の遷移先
 *
 * @access public
 * @param string $v エラー時の遷移先コマンド名
 */
public function set_login_error_command($v){ $this->login_error_command_ = $v; }
public function get_login_error_command(){ return $this->login_error_command_; }

/**
 * ログインチェックでOKだった場合の遷移先
 *
 * @access public
 * @param string $v OK時の遷移先コマンド名
 */
public function set_login_ok_command($v) { $this->login_ok_command_ = $v; }
public function get_login_ok_command() { return $this->login_ok_command_; }

// authentication_type
public function get_authentication_type(){ return $this->authentication_type_; }
public function set_authentication_type($v){ $this->authentication_type_ = $v; }
/**
 * ID(email) + Passwordで本人認証を行う
 *
 * 概ね、フロント用
 *
 * @access public
 */
public function authentication_type_ipass(){$this->set_authentication_type(1);}

/**
 * ID(直接) + Passwordで本人認証を行う
 *
 * 概ね、管理画面用
 *
 * @access public
 */
public function authentication_type_direct_ipass()
  { $this->set_authentication_type(2); }

/**
 * 本人認証用のclumpの名前
 *
 * @access public
 * @param string $v clump クラス名
 */
public function set_authentication_user_clump_name($v)
  { $this->authentication_user_clump_name_ = $v; }
public function get_authentication_user_clump_name()
  { return $this->authentication_user_clump_name_; }


// authorization_type
public function get_authorization_type() { return $this->authorization_type_; }
public function set_authorization_type($v) { $this->authorization_type_ = $v; }
/**
 * Cookieを用いて認証継続を行う
 *
 * @access public
 */
public function authorization_type_cookie()
  { $this->set_authorization_type(1); }


/**
 * 契約者固有IDを用いて認証継続を行う
 *
 * mobile以外はerrorではねる
 *
 * @access public
 */
public function authorization_type_contractor_id()
  { $this->set_authorization_type(2); }


/**
 * 認証継続用のCookie名を設定
 *
 * defaultは 'k'
 *
 * @access public
 * @param string $v Cookie名
 */
public function set_authorization_cookie_name($v)
  { $this->authorization_cookie_name_ = $v; }
public function get_authorization_cookie_name()
  { return $this->authorization_cookie_name_; }


/**
 * 認証継続用の暗号keyの設定
 *
 * @access public
 * @param string $v key string
 */
public function get_authorization_crypt_key()
  { return $this->authorization_crypt_key_; }
public function set_authorization_crypt_key($v)
  { $this->authorization_crypt_key_ = $v; }


/**
 * 認証継続用 寿命が近いセッションの処理方法
 *
 * とりあえず下記の通り。後でラッパーメソッド追加するかなぁ？
<pre>
0:なにもしない（激しく非推奨<br>
1:新しいセッションを作成する。古いセッションも寿命までは有効<br>
2:新しいセッションを作成する。古いセッションは破棄する<br>
3:寿命そのものを伸ばす(非推奨<br>
</pre>
 *
 *
 * @access public
 * @param numeric $v 上述フラグ
 */
public function set_authorization_expire_flg($v)
  { $this->authorization_expire_flg_ = $v; }
public function get_authorization_expire_flg()
  { return $this->authorization_expire_flg_; }

/**
 * 認証継続用のセッションIDの寿命(単位:秒)
 *
 * @access public
 * @param numeric $v 寿命(秒)
 */
public function set_authorization_expire($v)
  { $this->authorization_expire_ = $v; }
public function get_authorization_expire()
  { return $this->authorization_expire_; }

/**
 * 認証継続用のclumpの名前
 *
 * @access public
 * @param string $v clump クラス名
 */
public function set_authorization_clump_name($v)
  { $this->authorization_clump_name_ = $v; }
public function get_authorization_clump_name()
  { return $this->authorization_clump_name_; }

/**
 * clientテーブルの名前が'client'以外の時に使う
 * 
 * clumpクラス名を設定
 *
 * @access public
 * @param string $v clumpクラス名
 */
public function set_client_clump_name($v) { $this->client_clump_name_ = $v; }
public function get_client_clump_name() { return $this->client_clump_name_; }

/**
 * clientテーブルのPKが'client_id'以外の時に使う
 * 
 * PKの名前を設定
 *
 * @access public
 * @param string $v PK名
 */
public function set_client_keyname($v) { $this->client_keyname_ = $v; }
public function get_client_keyname() { return $this->client_keyname_; }


////////////
/**
 * login処理を行う
 *
 * このメソッド自体は振り分け君
 *
 * @access public
 */
public function login()
{
  // ログインタイプの判定

  // 処理の振り分け
  // XXX 一端内部メソッドで抱える。理由があれば外に出す
  $ret = false;
  switch($this->get_authentication_type()) {
    case 1:
      $ret = $this->login_id_pass();
      break;

    case 2:
      $ret = $this->login_direct_id_pass();
      break;
  }
//var_dump($ret);

  // 処理
  $this->recursive_on();
  if (false === $ret) {
    // 遷移先指定
    $this->set_recursive_command($this->get_login_error_command());
    // エラー表示用
    $this->get_conv()->multiDic('is_login_error', new simple_put);
  } else {
    // 遷移先指定
    $this->set_recursive_command($this->get_login_ok_command());
  }

  //
  return ;
}

/**
 * ID(email) + Passwordによるlogin処理を行う
 *
 * XXX 一端「emailとpassword」タイプ固定
 * XXX 後でconfig値によって振り分ける
 * XXX あと、大分あちこち、決めうち。後で変更可能なメソッドを追加予定
 *
 * @access public
 * @return boolean ログインエラーなら false
 */
public function login_id_pass()
{
  // とりあえずIDとパスワードをcgi_requestから取得
  $id_name = 'id'; // XXXXXX magicnumber
  $id = $this->get_cgi_request()->find($id_name);

  // IDが空なら突っ返す
  if ('' === $id) {
//print "login_id_pass error 1 \n";
    return false;
  }
//var_dump($id);

  // ID に準拠しているclient_idをげと
  $client_clump_name = $this->get_client_clump_name();
require_once('clump/' . $client_clump_name . '.inc');
  $cobj = new $client_clump_name;
  $cobj->set_db($this->get_db());
  $cobj->set_value('email', $id, false); // XXXXXX magicnumber
//var_dump($id);
  $ret = $cobj->get_nopk();
  if (false === $ret) {
//print "login_id_pass error 2 \n";
//print $this->get_db()->get_error_message() . "\n";
    return false;
  }

  //
  return $this->__login_id_pass($cobj->get_value($this->get_client_keyname()));
}

/**
 * ID + Passwordによるlogin処理を行う
 *
 * 管理画面用想定だから多分あんまり追加はしないなぁ
 *
 * @access public
 * @return boolean ログインエラーなら false
 */
public function login_direct_id_pass()
{
  // とりあえずIDとパスワードをcgi_requestから取得
  $id_name = 'id'; // XXXXXX magicnumber
  $id = $this->get_cgi_request()->find($id_name);

  // IDが空なら突っ返す
  if ('' === $id) {
//print "login_id_pass error 1 \n";
    return false;
  }
//var_dump($id);

  //
  return $this->__login_id_pass($id);
}


// 実際の処理
protected function __login_id_pass($id)
{
//var_dump($id);
  //
  $pass_name = 'pass'; // XXXXXX magicnumber

  $pass = $this->get_cgi_request()->find($pass_name);
//var_dump($pass);
  // パスワード文字列をハッシュ化
  $pass = sha1($pass, false);

  // セッション系テーブルつかってパスワード文字列を取得
  $auth_user_clump_name = $this->get_authentication_user_clump_name();
require_once('clump/' . $auth_user_clump_name . '.inc');
  $aobj = new $auth_user_clump_name;
  $aobj->set_db($this->get_db());
  $aobj->set_value('id', $id, false); // XXXXXX magicnumber
  // 可能性は低いけど一応
  $ret = $aobj->get_nopk();
  if (false === $ret) {
//print "login_id_pass error 3 \n";
    return false;
  }

  // ステータス確認
  // XXX 一端「1のロックのみNG」としておく
  if ('1' == $aobj->get_value('status')) {
//print "login_id_pass error 4 \n";
    return false;
  }

  // 一時的ロックの確認
  // lock_time確認
  if (0 < $aobj->get_value('lock_time') ) {
//print "into time lock \n";
    $twk = time();
    if ($twk < $aobj->get_value('lock_time')) {
//print "into time lock NG\n";
      // まだロック期間なのでエラーで突っ返す
//print "login_id_pass error 5 \n";
      return false;
    }
    // ロック期限を越えているので、ロックをはずす
    $aobj->set_value('lock_time', 0, false);
  }

  // 比較
  if ($aobj->get_value('pass') === $pass) { // XXXXXX magicnumber
    // OKならいくつか処理
    $aobj->set_value('lock_time', 0, false);
    $aobj->set_value('err_count', 0, false);
    // bagつかって維持認証をスルーさせる
    $this->set_bag('__auth', $id );

    // データの更新
    $aobj->update();

    //
//print "login_id_pass OK \n";
    return true;
  }

  // エラーならいくつか処理
  // まずカウントアップ
  $wk = $aobj->get_value('err_count') + 1;
  $aobj->set_value('err_count', $wk, false);

  // エラー回数が一定を超えていたら一端ロック
  if ($wk >= 5) {
    $lt = 3600; // XXX
    $aobj->set_value('err_count', 0, false);
    $aobj->set_value('lock_time', time() + $lt, false);
  }

  // データの更新
  $aobj->update();

  //
//print "login_id_pass error 6 \n";
  return false;
}


////////////

/**
 * 認証処理起動
 *
 * @access public
 */
public function initialize() {
  // 認証タイプの取得
  $type = $this->get_authorization_type();

  // 旧ロジック用
  if (0 == $type) {
    $this->_authorization_old();
    return ;
  }

  // 新ロジック

  //--------------------------------------
  // 指定方式に従った認証継続のチェック
  //--------------------------------------
  $ret = ''; // client_idが入ってくる
  if (true === $this->is_bag('__auth')) {
    $ret = $this->get_bag('__auth');
  } else {
    $type = $this->get_authorization_type();
//var_dump($type);
    if (1 === $type) {
      $ret = $this->_authorization_cookie();
    } else
    if (2 === $type) {
      $ret = $this->_authorization_contractor_id();
    }
  }

  //--------------------------------------
  // 認証継続の状況に応じた処理
  //--------------------------------------
  if ('' === $ret) {
    // 認証継続不可
    $this->auth_state_off();
  } else {
    // 認証継続許可
    $this->auth_state_on();
    // IDを設定しておく
    $this->set_id($ret);
  }

  //
  return ;
}

/**
 * Cookieによる認証継続処理を行う
 *
 *
 * @access public
 * @return string クライアントID エラーなら空文字
 */
public function _authorization_cookie()
{
  // Cookieから「セッションIDを含む暗号文字列」を取得
  $cookie_name = $this->get_authorization_cookie_name();
  $key = $this->get_cgi_request()->find_cookie($cookie_name);

  // セッションIDを取り出す
  $crypt_obj = $this->_get_crypt_object();
  $crypt_obj->set_encrypted_unit($key);
  $crypt_obj->decrypt();
  $s = $crypt_obj->get_plain_text();
//print "cgi_param is $s \n";

  // sid取り出して
require_once('cgi_param.inc');
  $p = new cgi_param;
  $p->set($s);

  // セッションIDを抜き出す
  $sid = $p->find('m'); // XXXXXX magicnumber

  // セッションIDが有効か確認
  $cn = $this->get_authorization_clump_name();
require_once('clump/' . $cn . '.inc');
  $obj = new $cn;
  $obj->set_db($this->get_db());
  $obj->set_value('sid', $sid);
  $ret = $obj->get();
  // 存在しなければ 非認証
  if (false === $ret) {
    return '';
  }
//print 'id is ' . $obj->get_value('id') . "\n";
//print 'expire is ' . $obj->get_value('expire') . "\n";
//print 'time is ' . time() . "\n";

  // 寿命チェック
  if (time() > $obj->get_value('expire')) {
    // 消しておく
    $obj->del();
    return '';
  }

  // XXXBagに入れておく
  $this->set_bag('session_clump', $obj);

  // 処理終了
  return $obj->get_value('id');
}


/**
 * 契約者固有IDによる認証継続処理を行う
 *
 * PCは無条件でerror
 *
 * @access public
 * @return string クライアントID エラーなら空文字
 */
protected function _authorization_contractor_id()
{
  // PCチェック
  $mobj = $this->get_mobile();
  if (false === $mobj->is_mobile()) {
    return '';
  }

  //
  $clump_name = $this->get_client_clump_name();
require_once('clump/' . $clump_name . '.inc');

  //
  $aobj = new $clump_name;
  $aobj->set_db($this->get_db());

  //
  $mobile_id = 'uid'; // XXXXXX magicnumber
  $aobj->set_value($mobile_id, $mobj->get_sid(), false);
//var_dump($mobj->get_sid());

  // いればOKってことでとりあえず
  $ret = $aobj->get_nopk();
//var_dump($ret);

  if (false === $ret) {
    return '';
  }

  //
  //return $mobj->get_sid();
  return $aobj->get_value( $this->get_client_keyname() );
}

// セッションの作成
protected function create_session($id)
{
//print "in to create \n";

  $cn = $this->get_authorization_clump_name();
require_once('clump/' . $cn . '.inc');
  $obj = new $cn;
  $obj->set_db($this->get_db());
  $obj->set_value('id', $id, false); // XXXXXX magicnumber
  $exp = $this->get_authorization_expire();

//print $exp . "\n";
  $obj->set_value('expire', time() + $exp, false);
//var_dump($obj->get_value('expire'));

  // 何回かチャレンジ。いらないと思うけど…
  // XXX トークナイザがそんなに複数回NG出すとは思いにくいので
  // XXX あえて、エラーチェックなし
  $i = 10;
  while($i --) {
    $sid = tokenizer::get();
    $obj->set_value('sid', $sid, false);
    if (true === $obj->insert()) {
      break;
    }
  }

  //
  return $obj;
}

// 暗号インスタンスげと
protected function _get_crypt_object()
{
// XXX 一端べた
require_once('crypt_rijndael256.inc');
  $obj = new crypt_rijndael256;
  $obj->set_block_mode('cbc');
  $obj->set_key($this->get_authorization_crypt_key());

  //
  return $obj;
}


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
 * 認証状態のチェック
 *
 * @access public
 * @return boolean 認証状態ならtrue、非認証状態ならfalse
 */
public function is_auth() { return $this->get_auth_state(); }

// この辺は公開しないので
public function get_auth_state() { return $this->auth_state_; }
protected function set_auth_state($b) { $this->auth_state_ = $b; }
public function auth_state_on() { $this->set_auth_state(true); }
public function auth_state_off() { $this->set_auth_state(false); }


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

/*
 * modelマウントポイント
 *
 * 適切な認証チェックとかやって適切なメソッドにぶんなげる
 * 認証チェック自体は、initalize時にすでに処理が終了している
 *
 * @access public
 */
public function execute() {
  // 認証タイプの取得
  $type = $this->get_authorization_type();
  // 旧ロジック用
  if (0 == $type) {
    return $this->_old_execute();
  }

  // 認証状態の確認
  if (true === $this->is_auth()) {
    // 認証継続許可なら
    $this->get_conv()->multiDic('is_member', new simple_put);

    // 携帯認証系ならほぼスルー
    if (2 === $type) {
      // execute_authを叩く
      return $this->execute_auth();
    }
    // else
    // Cookie(1)の場合

    // session IDの取得または作成
    if (true === $this->is_bag('session_clump')) {
      // すでに存在しているセッションインスタンスをゲト
      $obj = $this->get_bag('session_clump');

      // 寿命が近いときの処理
      $flg = $this->get_authorization_expire_flg();
      $expire = $this->get_authorization_expire();
      if ($flg > 0) {
        // 残り有効時間の算出
        $twk = $obj->get_value('expire') - time();
//print "twk is $twk \n";
//print "exp is " . $obj->get_value('expire') . "\n";

        // 確認
        // XXX セッション維持時間の50%
        if (($expire / 2) > $twk) {
//print "セッション維持 50% over ( $twk ) \n";
          // 各処理を行う
          switch ($flg) {
            case 2:
              // 現在のセッションIDを削除する
              $obj->del();

            case 1:
              // 新しいセッションを作成
              $obj = $this->create_session($this->get_id());
              break;

            case 3:
              // セッション寿命を延ばす(非推奨かねぇ…
              $obj->set_value('expire', time() + $expire, true);
              break;

            default:
              //
              break;
          }
        }
      }
    } else {
      // 新しいセッションを作成
      $obj = $this->create_session($this->get_id());
    }
    // セッションIDを取得
    $sid = $obj->get_value('sid');

    // セッションIDから、keyを作成
    // -----------------------------
require_once('cgi_param.inc');
    // keyを作成
    $p = new cgi_param;
    // パラメタ列の作成
    $p->init();
    $p->add("l", crypt_util::rand_string(10,false));
    $p->add("m", $sid);
    $p->add("n", crypt_util::rand_string(10,false));
    // 作成
    $key = $p->get();
//print "cgi_param is $key\n";
    // 暗号化
    $crypt_obj = $this->_get_crypt_object();
//var_dump($en_o);
    // 暗号化したい文字列セット
    $crypt_obj->set_plain_text($key);
    // 暗号化する。
    $crypt_obj->encrypt();
    // 暗号文字列を受け取る
    $key = $crypt_obj->get_encrypted_unit();

    // Cookieでcookie発行など
    // XXX 一端Cookieで決めうち
    $name = $this->get_authorization_cookie_name();
    setcookie($name, $key);

    // XXX is_authをonに

    // execute_authを叩く
    return $this->execute_auth();
  }

  // else
  // 認証継続不許可なら
  $this->get_conv()->multiDic('is_nomember', new simple_put);
  
  if (true === $this->is_authtype_ambiguous_auth()) {
    // 曖昧認証OKの場合 execute_authを叩く
    // XXX
    if (true === method_exists($this, 'execute_auth')) {
      return $this->execute_auth();
    } else {
      // XXX こっちは非推奨
      // XXX 旧との互換性だけのためにあるので使わないでね?
      return $this->execute_ambiguous_auth();
    }
  }

  // else
  // ダメポな場合 エラーPageに
  // XXX 本当は指定できるように
  $this->recursive_on();
  $this->set_recursive_command('index'); // XXXXXX magicnumber

  //
  return ;
}

/**
 * セッションデータ保持用のインスタンスを取得
 *
 * XXX 当面、以下の機能はないものとする。
 * XXX 1: セッションデータをどこにかくか？(現状は DB固定)
 * XXX 2: keyのprefix、suffixの設定(現状は keyはユーザID固定)
 *
 * @access public
 * @return session_object エラーならnull
 */
public function get_session_data_obj($suffix = '')
{
  //
  $sid = $this->get_id() . $suffix;

  // セッションインスタンスが無ければ作成
  if ( false === isset($this->session_data_obj_[$sid])) {
    // XXX 一端DB系固定。あとでスイッチさせる

    //
    $cn = $this->get_config()->find('session_class_name');
    if ('' === $cn) {
      $cn = 'session_data_db';
    }
require_once("{$cn}.inc");

    // インスタンスつくって
    $this->session_data_obj_[$sid] = new $cn;
    // IDセットして
    $this->session_data_obj_[$sid]->set_id($sid);
    // ハンドル系セットして
    if (true === $this->session_data_obj_[$sid]->is_exist_db()) {
      $this->session_data_obj_[$sid]->set_db($this->get_db());
    }
    if (true === $this->session_data_obj_[$sid]->is_exist_kvs()) {
      $this->session_data_obj_[$sid]->set_kvs($this->get_kvs());
    }
    // XXX ほかに設定があればここで

    // で、データがあれば、読み込む
    $this->session_data_obj_[$sid]->read();
  }

  //
  return $this->session_data_obj_[$sid];
}


/* -------------------------------- */
/* -------------------------------- */
// XXXXXXXXXXXX
// 旧の、推奨されないメソッド群
// XXXXXXXXXXXX
/* -------------------------------- */
/* -------------------------------- */

//
protected function set_session_obj($o) { $this->session_obj_ = $o; }
public function get_session_obj() { return $this->session_obj_; }

//
protected function set_sconfig($o) { $this->sconfig_ = $o; }
protected function get_sconfig() { return $this->sconfig_; }

//
public function with_mobile_on() { $this->with_mobile_ = 1; }
public function with_mobile_off() { $this->with_mobile_ = 2; }
public function is_with_mobile()
{
//var_dump($this->with_mobile_);
  if (1 === $this->with_mobile_) {
    return true;
  }
  // else
  return false;
}


/**
 * 認証本体
 *
 * 結果は内部変数に格納されるため、return自体はvoid
 *
 * @access protected
 */
protected function _authorization_old()
{
//print "_authorization Trap1\n";
  // 携帯の場合
  if ($this->is_with_mobile()) {
//print "do_auth_mobile_site\n";
    $ret = $this->do_auth_mobile_site();
  } else {
  // PCの場合
//print "do_auth\n";
    $ret = $this->do_auth();
  }
  //
  if (true === $ret) {
    $this->auth_state_on();
  } else {
    $this->auth_state_off();
  }
//print "_authorization Trap99\n";

  //
  return;
}


/**
 * 認証を試みる(ID＋password系
 *
 * 通常のセッションからの認証
 * 対応するものに「公式サイト系の認証」がある
 *
 * @access public
 * @return boolean 認証状態ならtrue、非認証状態ならfalse
 */
public function do_auth() {
//print "base_model_auth execute Trap 1\n";
  //
  $req = $this->get_cgi_request();

  // セッションクラスげと
  $obj = new session;
  $obj->set_config_filename($this->get_config()->find('session_config'));
  $obj->set_db($this->get_db());
  $this->set_sconfig($obj->get_config());

  // key stringげと
  // XXX 順番は固定でpost、Cookie、get。あとで可変にしたいなぁ。
  // 現状のモードの退避
  $pm = $req->get_priority();
  $key = "";
  if (true === $this->get_sconfig()->is_value('auth_key_get')) {
    $req->set_only_get();
    $name = $this->get_sconfig()->get_value('auth_key_get_name');
    $value = $req->find($name);
    if ("" !== $value) { $key = $value; }
  }
  if (true === $this->get_sconfig()->is_value('auth_key_post')) {
    $req->set_only_post();
    $name = $this->get_sconfig()->get_value('auth_key_post_name');
    $value = $req->find($name);
    if ("" !== $value) { $key = $value; }
  }
  if (true === $this->get_sconfig()->is_value('auth_key_cookie')) {
    $name = $this->get_sconfig()->get_value('auth_key_cookie_name');
    $value = $req->find_cookie($name);
//print "cookie value is $value \n";
    if ("" !== $value) { $key = $value; }
  }
  // 戻す
  $req->set_priority($pm);
//print "key = $key \n";
 
  // keyがなければ非認証状態
  if ("" === $key) {
    $this->auth_state_off();
    return false;
  }

  // 認証エラーならやっぱり非認証
  $ret = $obj->auth($key);
  if (false === $ret) {
    $this->auth_state_off();
    return false;
  }

  // Cookieなら、keyの設定をしておく
  if (true === $this->get_sconfig()->is_value('auth_key_cookie')) {
    $name = $this->get_sconfig()->get_value('auth_key_cookie_name');
    setcookie($name, $obj->get_key());
  }

  //
  $this->set_session_obj($obj);
  $this->set_id($this->get_session_obj()->get_id());
  $this->auth_state_on();
  return true;
}

/**
 * 携帯サイトとして認証を試みる
 *
 * XXX 認証のtrue falseはユーザテーブルに存在するかどうかでみる
 * ぶっちゃけ毎回ログインしてる感じだよねぇこれって
 *
 * @access public
 * @return boolean 認証状態ならtrue、非認証状態ならfalse
 */
public function do_auth_mobile_site() {
  // 初期化
  $this->set_id('');
  $this->auth_state_off();

  // IDの取得
  $miobj = new mobile_info;

  $uid = '';
  if ( (true === $miobj->is_docomo())&&((true === $this->is_official_site())|| ( (false === empty($_SERVER['HTTPS']))&&('off' !== $_SERVER['HTTPS']) )) ) {
//print "official docomo\n";
    // DoCoMoかつ
    // 公式、またはSSLによる接続の場合
    $uid = $this->get_cgi_request()->find('uid');
  } else {
//print "any mobile\n";
    // それ以外
    $uid = $miobj->get_sid();
  }

  // XXX チョイ場当たり対応。
  // XXX PCでかつuidの類がuser-agentに設定されてなければパラメタ uid を使う
  // XXX user-agentにがんばって設定してるならそっち優先したいからelseぢゃないif
  if (('' === $uid) && (false === $miobj->is_mobile())) {
    $uid = $this->get_cgi_request()->find('uid');
  }
//var_dump($uid);
  if ("" === $uid) {
//print "uid is null\n";
    // XXX 単純なfalseぢゃなくて、なにか小細工したくなるかも
    return false;
  }

  // げとれたuidを設定
  $this->set_id($uid);

  // ユーザテーブル確認
  $cobj = $this->get_config();
  // SELECT id FROM table WHERE uid=$id and del_flg = 0;
  // XXX 後で書き直す? 重複いや orz
  if ("" !== $cobj->find('official_site_user_del_flg')) {
    $sql = sprintf('SELECT %s FROM %s WHERE %s=%s and %s=%s;',
                    $cobj->find('official_site_user_id'),
                    $cobj->find('official_site_user_table'),
                    $cobj->find('official_site_user_uid'),
                    security::sanitize_db_add_quote($uid),
                    $cobj->find('official_site_user_del_flg'),
                    security::sanitize_db_add_quote($cobj->find('official_site_user_del_flg_value'))
                  );
  } else {
    $sql = sprintf('SELECT %s FROM %s WHERE %s=%s;',
                    $cobj->find('official_site_user_id'),
                    $cobj->find('official_site_user_table'),
                    $cobj->find('official_site_user_uid'),
                    security::sanitize_db_add_quote($uid)
                  );
  }
//print $sql . "\n";
  //
//var_dump($this->get_db());
  $res = $this->get_db()->query($sql);
  if (is_null($res)) {
      return false;
  }
  if (false === $res->fetch()) {
    return false;
   }

  // 情報とれたっぽなので、処理
  $this->auth_state_on();

  //
  return true;
}


/**
 * 認証必須系においてNGだった場合の遷移処理
 *
 * @access public
 */
public function auth_ng_process() {
  //
  $this->recursive_on();
  // 設定がなければindexにつっかえす
  $command = 'index';
  if (false === is_null($this->get_sconfig())) {
    if ("" !== $this->get_sconfig()->get_value('auth_error_return_command')) {
      $command = $this->get_sconfig()->get_value('auth_error_return_command');
    }
  }
//var_dump($this->get_sconfig());

  // コマンド名設定
  $this->set_recursive_command($command);

  // テンプレートファイル情報を一旦削除
  $this->set_tmp_filename('');

  //
  return ;
}

/**
 * 公式サイトってconfigに書いてあるかどうかチェック
 *
 * XXX あとで「外から問答無用設定」できるようにしないとねぇ…
 *
 * @access public
 * @return boolean 公式サイトならtrue、非公式ならfalse
 */
public function is_official_site() {
  $s = $this->get_config()->find('official_site');
  if(preg_match("/^(yes|y|ok|true|on|1)$/i", $s)) {
    return true;
  }
  // else
  return false;
}

// 旧 マウントポイント
public function _old_execute() {
  // 一応念のためのマウントポイント
  $ret = $this->execute_rich_mount1();
  if (false === $ret) {
    $this->auth_ng_process();
    return;
  }

  // authorization チェック
  // はじくか外にってか別メソッドに
  //$this->_authorization();

  // 一応念のためのマウントポイント
  $ret = $this->execute_rich_mount2();
  if (false === $ret) {
    $this->auth_ng_process();
    return;
  }

  // OKだったりNGだったり
  // authの場合
  if ($this->is_authtype_auth()) {
    if (false === $this->is_auth()) {
      // エラー
      $this->auth_ng_process();
      return;
    }
    // else
    $ret = $this->execute_auth();
  } else {
    // XXX$this->is_authtype_ambiguous_auth() やらないよとりあえず
    $ret = $this->execute_ambiguous_auth();
    // XXX ついでにセット
    $sp = new simple_put;
    if (true === $this->is_auth()) {
      $this->get_conv()->multiDic('is_auth', $sp);
    } else {
      $this->get_conv()->multiDic('is_noauth', $sp);
    }
  }

  // 一応念のためのマウントポイント
  $ret = $this->execute_rich_mount3();
  if (false === $ret) {
    $this->auth_ng_process();
    return;
  }
  //
  return ;
}


// 何かやりたい時用
public function execute_rich_mount1() { return true; }
public function execute_rich_mount2() { return true; }
public function execute_rich_mount3() { return true; }

//private:
private $id_;			// ユーザを識別するためのID
private $auth_state_;		// 認証状態 boolean
private $authtype_;		// 1:auth必須 2:authしてもしなくても

//
private $authentication_type_;	// 本人認証の種類
				// 1: ID(email) + Password
				// 2: ID + Password
private $authentication_user_clump_name_;	// password入ってるclump

//
private $authorization_type_;	// 認証継続の種類
				// 1: Cookie
//
private $login_error_command_;	// ログインでミスったときの遷移先
private $login_ok_command_;	// ログインでOKだった時の遷移先
//
private $authorization_cookie_name_;	// 認証維持用のCookie名
private $authorization_crypt_key_;	// 認証維持用の暗号 key
private $authorization_expire_;		// 認証維持 セッション寿命
private $authorization_clump_name_;	// 認証維持 セッションDBのclump名
private $authorization_expire_flg_;	// 認証維持における処理
		// 0:なにもしない（激しく非推奨
		// 1:新しいセッションを作成する。古いセッションも寿命までは有効
		// 2:新しいセッションを作成する。古いセッションは破棄する
		// 3:寿命そのものを伸ばす(非推奨

private $session_data_obj_;	// セッション情報用オブジェクト


// ---
private $sconfig_;		// 認証configのクラスインスタンス
private $with_mobile_;		// 1:with_mobile  2:without_mobile
private $session_obj_;

// ---
private $client_clump_name_;	// ID / pass認証用 clientテーブルのclump名
private $client_keyname_;	// ID / pass認証用 clientテーブルのkey名

} // end of class


