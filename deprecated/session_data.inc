<?php

/**
 * セッションデータ保持クラス
 *
 * セッション情報を保持するクラス
 * 実際にはVirtualなクラスなので継承先じゃないとインスタンス作れない?
 *
 * XXX 多重継承が難しいので、一旦「処理周り」全部ここに集約?
 * XXX いい手が見つかったらリファクタ
 *

// 予想しているテーブル定義
create table session_data (
        id varchar(64) binary not null,
        data text,
        time datetime,
        PRIMARY KEY (id)
);

 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 */

require_once("cgi_param.inc");
require_once("sql_util.inc");
require_once("calendar.inc");
require_once("calendar_util.inc");


// XXX データ形式的にこれが便利なのでcgi_paramから継承?
abstract class session_data extends cgi_param {
//public:

public function __construct()
{
  //
  parent::__construct();

  // 有効期間は一旦15分
  $this->valid_time_ = 15 * 60;
  // テーブル名をとりあえず設定
  $this->tbl_name_ = "session_data";
}

/**
 * DBハンドルの設定
 *
 * @access public
 * @param db_handle $o db_handleクラス(からの派生クラス)のインスタンス
 */
public function set_db($o) { $this->dbh_ = $o; }
/**
 * DBハンドルの取得
 *
 * @access public
 * @return db_handle db_handleクラス(からの派生クラス)のインスタンス
 */
public function get_db() { return $this->dbh_; }

/**
 * KVSハンドルの設定
 *
 * @access public
 * @param kvs_handle $o
 */
public function set_kvs($o) { $this->kvsh_ = $o; }
/**
 * KVSハンドルの取得
 *
 * @access public
 * @return kvs_handle
 */
public function get_kvs() { return $this->kvsh_; }


/**
 * データ保存用テーブル名の設定
 *
 * @access public
 * @return string $s テーブル名
 */
public function set_tablename($s) { $this->tbl_name_ = $s; }


/**
 * セッションIDの設定
 *
 * @access public
 * @param string $id セッションID
 */
public function set_id($id) { $this->id_ = $id; }
/**
 * セッションIDの取得
 *
 * @access public
 * @return string セッションID
 */
public function get_id() { return $this->id_; }

/**
 * 有効期限(TTL)の設定
 *
 * 0または負の値を指定すると無期限とみなす
 * defaultは15分である
 *
 * @access public
 * @param int $t TTLの値を秒で
 */
public function set_validity_term($t) { $this->valid_time_ = $t; }

/**
 * 有効期限(TTL)の取得
 *
 * 0または負の値を指定すると無期限とみなす
 *
 * @access public
 * @return int TTLの値を秒で
 */
public function get_validity_term() { return $this->valid_time_; }






/**
 * 書き込み(Virtual)
 *
 * @access public
 * @return boolean 問題があればfalse
 */
abstract public function write();

/**
 * 読み込み(Virtual)
 *
 * @access public
 * @return boolean 問題があればfalse
 */
abstract public function read();

/**
 * 削除(Virtual)
 *
 * @access public
 * @return boolean 問題があればfalse
 */
abstract public function del();

/**
 * セッションIDの存在確認(Virtual)
 *
 * 書き込み先にすでにセッションIDが存在しているかの確認
 * 基本的に「trueが帰ってきたら上書きになりそうなので適宜対処」のイメージ
 *
 * @access public
 * @return boolean すでに存在していればtrue
 */
abstract public function is_session();






//
public function is_exist_db() { return false; }
public function is_exist_kvsb() { return false; }






/**
 * DBへのsession dataの書き込み
 *
 * @access protected
 * @return boolean 問題があればfalse
 */
protected function write_to_db()
{

  // データの作成
  $qwk["data"] = $this->get();
  $qwk["id"] = $this->get_id();
  $qwk["time"] = calendar_util::get_now_string();

  // SQL文の生成
  $sql = sql_util::make_insert($this->tbl_name_, $qwk);

  //
  if (true === $this->get_db()->query_to_specified_handle($sql, 'write', $this->tbl_name_)) {
    return true;
  }
//var_dump($sql);
  // NGならUPDATE
  unset ($qwk);
  $qwk["data"] = $this->get();
  $qwk["time"] = calendar_util::get_now_string();
  $where["id"] = $this->get_id();

  //
  $sql = sql_util::make_update($this->tbl_name_, $qwk, $where);
//print $sql . "\n";;
//var_dump($sql);
  if (false === $this->get_db()->query_to_specified_handle($sql, 'write', $this->tbl_name_)) {
//print "write false\n";
    return false;
  }
  // else
//print "write true\n";
  return true;
}

/**
 * DBからsession dataへの読み込み
 *
 * @access protected
 * @return boolean 問題があればfalse
 */
protected function read_from_db()
{
  $sql = "SELECT data,time FROM " . $this->tbl_name_ . " WHERE id=" . mw_security::sanitize_db_add_quote($this->get_id()) ;
//print $sql . "\n";

//print "validity term is " . $this->get_validity_term() . "\n";

  if ($this->get_validity_term() > 0) {
    // 有効期限の作成
    $cal = new calendar;
    $cal->set_now_date();
    $cal->set_epoc($cal->get_epoc() - $this->get_validity_term());

    $sql .= " and time >= '" . $cal->get_string("-") . "'";
  }
  $sql .= ";";
//print $sql . "\n";

  if (!($data = $this->get_db()->query_to_specified_handle($sql, 'read', $this->tbl_name_))) {
    return false;
  }
  if (false === $data->fetch()) {
    return false;
  }

  //
//var_dump($data->get_data(0));
  $this->set($data->get_data(0));
//var_dump($this);
//print $data->get_data(1) . "\n";
  return true;
}


/**
 * DB内にあるsession dataの削除
 *
 * メモリ上のデータを削除する場合は initメソッドで
 *
 * @access protected
 * @return boolean 問題があればfalse
 */
protected function del_to_db() {
  $where["id"] = $this->get_id();
  $sql = sql_util::make_delete($this->tbl_name_, $where);
//print $sql . "\n";
  return $this->get_db()->query_to_specified_handle($sql, 'write', $this->tbl_name_);
}


/**
 * セッションIDの存在チェック
 *
 * @access protected
 * @return boolean 存在していたらtrue
 */
protected function is_session_from_db() {
  $sql = "SELECT count(*) FROM " . $this->tbl_name_ . " WHERE id=" . security::sanitize_db_add_quote($this->get_id()) ;
//print $sql . "\n";

  if (!($data = $this->get_db()->query_to_specified_handle($sql, 'read', $this->tbl_name_))) {
//print "is_session NG 1\n";
    return false;
  }
  if (!($data->fetch())) {
//print "is_session NG 2\n";
    return false;
  }

  //
  if ( 0 == $data->get_data(0)) {
//print "is_session NG 3\n";
    return false;
  }

  //
//print "is_session OK\n";
  return true;
}







/**
 * KVS用のkeyの作成
 *
 * @access protected
 * @return boolean 問題があればfalse
 */
protected function make_kvs_key()
{
  return $this->tbl_name_ . '.' . $this->get_id();
}


/**
 * KVSへのsession dataの書き込み
 *
 * 書き込むタイミングで寿命を(一応)設定する
 *
 * @access protected
 * @return boolean 問題があればfalse
 */
protected function write_to_kvs()
{
  // 書き込む
  return $this->get_kvs()->set($this->make_kvs_key(), $this->get(), $this->get_validity_term());
}

/**
 * KVSからsession dataへの読み込み
 *
 * @access protected
 * @return boolean 問題があればfalse
 */
protected function read_from_kvs()
{
  // 読み込む
  $s = $this->get_kvs()->get($this->make_kvs_key());
  if ('' === $s) {
    return false;
  }

  //
  $this->set($s);

  //
  return true;
}


/**
 * KVS内にあるsession dataの削除
 *
 * メモリ上のデータを削除する場合は initメソッドで
 *
 * @access protected
 * @return boolean 問題があればfalse
 */
protected function del_to_kvs() {
  return $this->get_kvs()->del($this->make_kvs_key());
}


/**
 * セッションIDの存在チェック
 *
 * @access protected
 * @return boolean 存在していたらtrue
 */
protected function is_session_from_kvs() {
  $s = $this->get_kvs()->get($this->make_kvs_key());
  if ('' === $s) {
    return false;
  }
  //
  return true;
}





//private:
private $id_;
private $valid_time_;	// 有効時間：単位は秒
//
private $dbh_;		// DBハンドル
private $tbl_name_;
//
private $kvsh_;		// KVSハンドル


} // end of class

