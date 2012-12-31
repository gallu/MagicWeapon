<?php
/**
 * DBデータクラス PostgreSQL
 *
 * SELECTによって帰ってくるDBデータを取り扱います
 *
 *
 * @deprecated
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once("db.inc");

class pg_data extends db_data {
//public:

//
public function __construct($res, $conn) {
  $this->res_ = $res;
  $this->row_int_ = 0;
  //
  $this->set_affected_rows(pg_affected_rows($conn));
}

/**
 * SELECTの結果取得
 *
 * @access public
 * @return boolean falseならこれ以上のデータはなし
 */
public function fetch() {
  //
  if ($this->is_fetch_type_num()) {
    $this->row_ = pg_fetch_row($this->res_);
  } else {
    $this->row_ = pg_fetch_array($this->res_, NULL, PGSQL_ASSOC);
  }

  //
  if (!($this->row_)) {
    return false;
  }
   //
  return true;
}

/**
 * fetchしたデータの取得
 *
 * 引数で指定した数字(０スタート)の位置のデータを取得。
 * set_fetch_type_hashの場合、引数と合致する名前のデータを取得。
 *
 * @access public
 * @param int $pos 取得したいデータの位置、またはhash名
 * @return string データ
 */
public function get_data($pos) {
  return $this->row_[$pos];
}

/**
 * fetchしたデータの取得
 *
 * 現在指定されてる１行のデータを全部取得
 *
 * @access public
 * @return array データの配列
 */
public function get_row() {
  return $this->row_;
}

//private:
private $res_;
private $row_;
}


/**
 * DBハンドルクラス PostgreSQL
 *
 * DBハンドルを取り扱います
 *
 * @deprecated
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  yyyy/mm/dd
 * @version $Id$
 **/
require_once("sql_util_postgresql.inc");
require_once("mw_makesql_escaped_postgresql.inc");

class dbh_pgsql extends db_handle {
//public:

  // コンストラクタ
  public function __construct() {
    //
    parent::__construct();

    //
    $this->set_sql_util(new sql_util_postgresql);
    //
    $o = new mw_makesql_escaped_postgresql();
    $o->set_db($this);
    $this->set_mw_makesql($o);
  }

/**
 * DBへの接続
 *
 * @access public
 * @return boolean 接続できたらtrue、出来なかったらfalse
 */
  public function connect() {
    // 引数を作る
    $wk = array();
    if ("" != $this->get_host_name()) {
      $buf = "host=" . $this->get_host_name();
      array_push($wk, $buf);
    }
    if ("" != $this->get_port_num()) {
      $buf = "port=" . $this->get_port_num();
      array_push($wk, $buf);
    }
    if ("" != $this->get_database_name()) {
      $buf = "dbname=" . $this->get_database_name();
      array_push($wk, $buf);
    }
    if ("" != $this->get_user()) {
      $buf = "user=" . $this->get_user();
      array_push($wk, $buf);
    }
    if ("" != $this->get_pass()) {
      $buf = "password=" . $this->get_pass();
      array_push($wk, $buf);
    }

    //
    $param = implode(" ", $wk);
//print "param is " . $param . "\n";

    // connect
    if (!($this->conn_ = pg_connect($param))) {
      $this->set_error_message("Connect error!!");
      return false;
    }

    //
    $this->on_connect();
    return true;
  }

/**
 * DB接続の切断
 *
 * @access public
 */
public function disconnect() {
  @pg_close($this->conn_);
}

/**
 * 「SQL文字列」によるクエリの発行
 *
 * @access public
 * @param string $sql SQL文
 * @return object SQL成功時にpg_dataインスタンス、SQL失敗時にnull
 */
public function query_direct($sql) {
  if (!($result = @pg_query($this->conn_, $sql))) {
    $this->set_error_message(@pg_last_error($this->conn_));
    return false;
  }

  $ret = new pg_data($result, $this->conn_);
  return $ret;
}

/**
 * 「準備された文」によるクエリの発行
 *
 * XXX ここではこの実装は使えないので常に失敗になります
 *
 * @access public
 * @param object $sql mw_sqlインスタンス
 * @return object 常に失敗なので、null
 */
public function query_prepared($sql) {
  return NULL;
}


/**
 * エスケープ処理メソッド
 *
 * @deprecated
 * @access public
 * @param string $s エスケープしたい文字列
 * @return string エスケープされた文字列
 */
public function escape($s)
{
  return pg_escape_string($this->conn_, $s);
}


//private:
private $conn_;
}


