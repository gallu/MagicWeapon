<?php

/**
 * セッションID関連クラス
 *
 *
 * XXX 現在非推奨
 *
 * セッションIDの作成、keyの作成、復号などを統括して行う
 * 以下の形式のテーブルがあることを想定している

セッションID絡みのデータ
セッションIDとユーザIDとを結びつけるざまス
create table auth_session (
   id(クライアント ID)
key sid(セッションID)
   expire(セッションIDの寿命)
);
create table auth_session (
   id varchar(64) not null,
   sid varchar(64) not null,
   expire int,
   PRIMARY KEY (sid)
);

 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once('session_base.inc');
require_once('tokenizer.inc');
require_once('crypt_util.inc');
require_once("crypt_factory.inc");
require_once("cgi_param.inc");

class session_id extends session_base {

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
  $this->set_sid("");
}

//
public function set_sid($s) { $this->sid_ = $s; }
public function get_sid() { return $this->sid_; }

public function set_id($s) { $this->id_ = $s; }
public function get_id() { return $this->id_; }

protected function get_session_clump_obj()
{
  $obj = new session_id_clump;
  $obj->set_db($this->get_db());

  $wk = $this->get_config()->get_value('session_clump_table_name');
  if ("" !== $wk) {
    $obj->set_table_name($wk);
  }
  return $obj;
}

/**
 * セッションの作成
 *
 * 新しくセッションIDを発行する
 *
 * @access public
 * @param string $id ログインセッション
 * @return boolean 成功したらtrue、失敗したらfalse
 */
public function create($id)
{
//print "in to create \n";
  // XXX 何回かリトライ。いるのか？
  $ret = false;
  $this->set_error_string('can not create session');
  $i = 10;

  $obj = $this->get_session_clump_obj();
  $obj->set_value('id', $id, false);
  $exp = $this->get_config()->get_value('session_expire');
//print $exp . "\n";
  $obj->set_value('expire', time() + $exp, false);
//var_dump($obj);
  while($i --) {
    $sid = tokenizer::get();
    $obj->set_value('sid', $sid, false);
    if ($obj->insert()) {
      $this->set_sid($sid);
      $this->set_id($id);
      $ret = true;
      $this->set_error_string('');
      break;
    }
  }

  //
  return $ret;
}

/**
 * セッションの維持
 *
 * keyから取得できるsession idをみて、認証の可不可を判断する
 *
 * @access public
 * @param string $key key string。通常hiddenかCookieが持ってる
 * @return boolean 成功したらtrue、失敗したらfalse
 */
public function auth($key)
{
//print "in to auth \n";
  // keyの確認
  if ("" === $key) {
    $this->set_error_string('key is empty.');
    return false;
  }

  // keyからsidを取得
  if (false === $this->set_key($key)) {
    $this->set_error_string('key is invalid.');
    return false;
  }
//print 'sid is ' . $this->get_sid() . "\n";

  // sidの存在チェック
  $obj = $this->get_session_clump_obj();
  $obj->set_value('sid', $this->get_sid(), false);
  if (false === $obj->get()) {
    $this->set_error_string('session id is invalid.');
    return false;
  }
//print 'id is ' . $obj->get_value('id') . "\n";
//print 'expire is ' . $obj->get_value('expire') . "\n";
//print 'time is ' . time() . "\n";

  // 寿命チェック
  if (time() > $obj->get_value('expire')) {
    $this->set_error_string('orver expire.');
    return false;
  }

  // IDを設定
  $this->set_id($obj->get_value('id'));

  // 寿命が近いときどうする？
  $flg = $this->get_config()->get_value('session_expire_flg');
  $expire = $this->get_config()->get_value('session_expire');
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
          $this->del_session_id($this->get_sid());

        case 1:
          // 新しいセッションを作成
          $this->create($this->get_id());
          break;

        case 3:
          //
          $obj->set_value('expire', time() + $expire, true);
          break;

        default:
          //
          break;
      }
    }
  }

  //
  return true;
}


/**
 * keyの作成
 *
 * セッションIDを含むkeyを作成する
 * hiddenにせよCookieにせよ、基本このkeyでやり取りすること
 * なお、事前に何らかの形(createまたはset_key)でsidを渡さないとエラー
 *
 * @access public
 * @return string 失敗したら空文字
 */
public function get_key()
{
  // 事前チェック
  $sid = $this->get_sid();
  if ("" === $sid)  {
    $this->set_error_string('sid is empty');
    return "";
  }

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

  // んでもって暗号化
  $config = $this->get_config()->get_value('session_crypt_config');
  if ("" === $config) {
    $this->set_error_string('session crypt config is not find.');
    return "";
  }
  $en_o = crypt_factory::get_instance($config);
//var_dump($en_o);
  // 暗号化したい文字列セット
  $en_o->set_plain_text($key);
  // 暗号化する。
  $en_o->encrypt();
  // 暗号文字列を受け取る
  $key = $en_o->get_encrypted_unit();

  //
  return $key;
}

/**
 * keyの取得
 *
 * keyからセッションIDとIDとを取得する
 * 基本的にはauthからcallされることしか想定してない
 *
 * @access public
 * @param string $key get_keyで作成された(はずの)key
 * @return boolean 見当たらなければfalse
 */
public function set_key($key)
{
  //
  if ("" === $key) {
    return false;
  }

  // 暗号化はずして
  $config = $this->get_config()->get_value('session_crypt_config');
  if ("" === $config) {
    $this->set_error_string('session crypt config is not find.');
    return false;
  }
  $de_o = crypt_factory::get_instance($config);
  $de_o->set_encrypted_unit($key);
  $de_o->decrypt();
  $s = $de_o->get_plain_text();
//print "cgi_param is $s \n";

  // sid取り出して
  $p = new cgi_param;
  $p->set($s);

  // 設定
  $this->set_sid($p->find('m'));

  //
  return true;
}

/**
 * セッションIDの削除
 *
 * 引数で渡されたセッションIDを削除する
 *
 * @access public
 * @param string $sid セッションID
 * @return boolean 何かあったらfalse
 */
public function del_session_id($sid)
{
  if ("" === $sid) {
    return false;
  }

  $obj = $this->get_session_clump_obj();
  $obj->set_value('sid', $sid, false);
  $obj->del();

  //
  return true;
}

//private:
private $sid_;
private $id_;
private $key_;

} // end of class

/**
 * セッション用のテーブルを制御するclump
 *
 * 基本的なテーブルイメージはセッションID関連クラス(session_id)見てくれぃ
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

require_once("data_clump.inc");

class session_id_clump extends data_clump {

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
  $this->set_element(); // ユーザ登録画面の要素を登録
}

/**
 * 基本的な情報の設定
 *
 * テーブル名およびカラム名を設定する
 *
 * @access protected
 * @return boolean falseは現状想定外。
 */
protected function set_element()
{
 //
 $this->set_table_name("auth_session");

 // $inname, $cginame, $tmpname, $dbname, $keyflg
 $this->push_element("sid", "", "", "", "pk");
 $this->push_element("id", "", "", "", "");
 $this->push_element("expire", "", "", "", "");
}

//private:

} // end of class



