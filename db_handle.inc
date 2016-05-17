<?php

/**
 * DBハンドルクラス
 *
 * DBハンドル(単体)を扱います
 * abstractクラスなので、必ず派生させてください
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once('db_handle_base.inc');

abstract class db_handle extends db_handle_base{
//public:

  // コンストラクタで初期値設定
  public function __construct() {
    // リトライ回数。10回をデフォにしておく
    $this->set_connect_retries_num(10);
    //
    $this->connect_flg_ = false;
    //
    $this->tran_off();
  }

  // デストラクタでコネクションを切る
  public  function __destruct() {
    $this->disconnect();
  }


// virtual関数各種
abstract public function connect();
abstract public function disconnect();
abstract public function query_direct($sql);
abstract public function query_prepared($sql);
abstract public function get_inserted_id();

/**
 * クエリの発行
 *
 * 発行というか、厳密にはquery_directを呼ぶかquery_preparedを呼ぶかの選択
 *
 * @access public
 * @param string/object $sql SQL文またはmw_sqlインスタンス
 * @return object SQL成功時にmysql_dataインスタンス、SQL失敗時にnull
 */
public function query($sql) {
//var_dump($sql);
  //
  if ( true === is_string($sql) ) {
    return $this->query_direct($sql);
  }
  // else
  // オブジェクトの場合の実装
  if (false === $sql->is_prepared()) {
    return $this->query_direct($sql->get_sql());
  }
  // else
  return $this->query_prepared($sql);
}


/**
 * このハンドルが「単体のRDBハンドル」系かどうか
 *
 * @access public
 * @return boolean
 */
public function is_mono_rdb() { return true; }

/**
 * 接続確認
 *
 * @access public
 * @return boolean 接続していたらtrue、してなきゃfalse
 */
public function is_connect() { return $this->connect_flg_; }


/**
 * 接続したスイッチ
 *
 * 接続したときにポチっと
 *
 * @access protected
 */
public function on_connect() { $this->connect_flg_ = true; }

// マルチ系DBハンドルとの互換用
public function query_to_specified_handle($sql, $rw, $table)
{
  return $this->query($sql);
}
public function read_query($sql)
{
  return $this->query($sql);
}
public function write_query($sql)
{
  return $this->query($sql);
}

/**
 * リトライ付きのconnect
 *
 * 子クラスの実装メソッド「connect()」を指定回数叩く。回数指定はset_connect_retries_num()で。
 *
 * @access public
 * @return boolean connect出来たらtrue、出来なかったらfalse
 */
public function rconnect() {
  // リトライ回数の取得
  $retries_num = $this->get_connect_retries_num();

  // ぶん回す
  $flg = false;
  while( $retries_num -- ) {
//print "retry is " . $retries_num . "\n";
    if ($this->connect()) {
      $flg = true;
      break;
    }
    // 適当にsleep。１秒〜1.5秒のあいだの乱数
    // XXX 後で外に数値切り出すかな？
    time_nanosleep(1, mt_rand(0, 500000000) );
  }

  //
  return $flg;
}

/**
 * connectリトライ間隔の設定
 *
 * rconnectメソッドで利用
 *
 * @access public
 * @param int $i 接続処理を繰り返す回数
 */
public function set_connect_retries_num($i) { $this->connect_retries_num_ = $i; }
/**
 * connectリトライ間隔の取得
 *
 * rconnectメソッドで利用
 *
 * @access public
 * @return int 接続処理を繰り返す回数
 */
public function get_connect_retries_num() { return $this->connect_retries_num_; }


/**
 * エラーメッセージの設定
 *
 * @access public
 * @param string $s DBハンドル関連のエラーメッセージ
 */
public function set_error_message($s) { $this->err_msg_ = $s; }
/**
 * エラーメッセージの取得
 *
 * @access public
 * @return string DBハンドル関連のエラーメッセージ
 */
public function get_error_message() { return $this->err_msg_; }


/**
 * DB接続ユーザ名の設定
 *
 * @access public
 * @param string $s DB接続ユーザ名
 */
public function set_user($s) { $this->user_ = $s; }
/**
 * DB接続ユーザ名の取得
 *
 * @access public
 * @return string $s DB接続ユーザ名
 */
public function get_user() { return $this->user_; }


/**
 * DB接続パスワードの設定
 *
 * @access public
 * @param string $s DB接続パスワード
 */
public function set_pass($s) { $this->pass_ = $s; }
/**
 * DB接続パスワードの取得
 *
 * @access public
 * @return string $s DB接続パスワード
 */
public function get_pass() { return $this->pass_; }

/**
 * database名の設定
 *
 * @access public
 * @param string $s database名
 */
public function set_database_name($s) { $this->database_name_ = $s; }
/**
 * database名の取得
 *
 * @access public
 * @return string $s database名
 */
public function get_database_name() { return $this->database_name_; }

/**
 * DBサーバ名(ドメインまたはIPまたはlocalhost)の設定
 *
 * @access public
 * @param string $s DBサーバ名
 */
public function set_host_name($s) { $this->host_name_ = $s; }
/**
 * DBサーバ名(ドメインまたはIPまたはlocalhost)の取得
 *
 * @access public
 * @param string $s DBサーバ名
 */
public function get_host_name() { return $this->host_name_; }

/**
 * port番号等の設定
 *
 * @access public
 * @param string $s port番号等
 */
public function set_port_num($s) { $this->port_num_ = $s; }
/**
 * port番号等の取得
 *
 * @access public
 * @return string port番号等
 */
public function get_port_num() { return $this->port_num_; }

/**
 * 文字エンコードの設定
 *
 * @access public
 * @param string $s エンコード文字列
 */
public function set_encode($s) { $this->encode_ = $s; }
/**
 * port番号等の取得
 *
 * @access public
 * @return string エンコード文字列
 */
public function get_encode() { return $this->encode_; }

/**
 * sql_utilインスタンスの設定
 *
 * @deprecated
 * @access public
 * @param object $o sql_utilインスタンス
 */
public function set_sql_util($o) { $this->sql_util_obj_ = $o; }
/**
 * sql_utilインスタンスの取得
 *
 * @deprecated
 * @access public
 * @param object $o sql_utilインスタンス
 */
public function get_sql_util() { return $this->sql_util_obj_; }

/**
 * mw_makesqlインスタンスの設定
 *
 * @access public
 * @param object $o mw_makesqlインスタンス
 */
public function set_mw_makesql($o) { $this->mw_makesql_obj_ = $o; }
/**
 * mw_makesqlインスタンスの取得
 *
 * XXX 引数はdb_handle_pluralとのインタフェース合わせ用。こっちでは使わない。
 *
 * @access public
 * @param string $table_name テーブル名
 * @return object mw_makesqlインスタンス
 */
public function get_mw_makesql($table_name) { return $this->mw_makesql_obj_; }

/**
 * トランザクション状態の設定
 *
 * @access protected
 * @param boolean $b トランザクション状態ならtrue、非トランザクション状態ならfalse
 */
protected function set_tran($b) { $this->tran_flg_ = $b; }
/**
 * トランザクション状態の取得
 *
 * @access protected
 * @return boolean トランザクション状態ならtrue、非トランザクション状態ならfalse
 */
protected function get_tran() { return $this->tran_flg_; }

/**
 * トランザクション状態をonにする
 *
 * @access public
 */
public function tran_on() { $this->set_tran(true); }
/**
 * トランザクション状態をoffにする
 *
 * @access public
 */
public function tran_off() { $this->set_tran(false); }
/**
 * トランザクション状態の確認
 *
 * @access public
 * @return boolean トランザクション状態ならtrue、非トランザクション状態ならfalse
 */
public function is_tran() { return $this->get_tran(); }

/**
 * トランザクションの開始
 *
 * XXX 現状、シャーディングしてるDBには対応できてないので注意
 * XXX あと。とりあえずここに書いてるけど、RDBMSの実装依存しそうなら、依存部分切り出して実装しなおさないとねぇ。
 *
 * @access public
 * @return boolean 成功したらtrue、何か問題があればfalse
 */
public function begin()
{
  // すでにトランザクション中ならNG
  if (true === $this->is_tran()) {
    return false;
  }

  // 発行
  $r = $this->write_query('BEGIN;'); // XXX マスタ１台のマスタスレーブまでは対応させたいので
  if (null === $r) {
    return false;
  }

  // トランザクション成功なので
  $this->tran_on();
  return true;
}

/**
 * トランザクションの終了(COMMIT)
 *
 * @access public
 * @return boolean 成功したらtrue、何か問題があればfalse
 */
public function commit()
{
  // トランザクション中でないならNG
  if (false === $this->is_tran()) {
    return false;
  }

  // 発行
  $r = $this->write_query('COMMIT;'); // XXX マスタ１台のマスタスレーブまでは対応させたいので
  if (null === $r) {
    return false;
  }

  // トランザクションのCOMMIT成功なので
  $this->tran_off();
  return true;
}

/**
 * トランザクションの中断(ROLLBACK)
 *
 * @access public
 * @return boolean 成功したらtrue、何か問題があればfalse
 */
public function rollback()
{
  // トランザクション中でないならNG
  if (false === $this->is_tran()) {
    return false;
  }

  // 発行
  $r = $this->write_query('ROLLBACK;'); // XXX マスタ１台のマスタスレーブまでは対応させたいので

  // 中断なのでどっちにしてもステータス「トラン」はoffっておく。
  $this->tran_off();
  return true;
}


//private:
private $err_msg_;
private $user_;
private $pass_;
private $database_name_;
private $host_name_;
private $port_num_;
private $encode_;
//
private $sql_util_obj_;	// sql_utilインスタンス格納領域(現在非推奨)
private $mw_makesql_obj_; // mw_makesqlインスタンス格納領域
//
private $connect_retries_num_;	// connectリトライ回数
//
private $connect_flg_;		// 接続したらtrueにしておく
//
private $tran_flg_;		// トランザクションフラグ。トランザクション中ならtrue
}

