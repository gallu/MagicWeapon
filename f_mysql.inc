<?php
/**
 * DBデータクラス Mysql
 *
 * SELECTによって帰ってくるDBデータを取り扱います
 *
 * @deprecated
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once("db.inc");

class mysql_data extends db_data {
//public:

  //
  public function __construct($res, $conn) {
    //parent::__construct();
    $this->res_ = $res;

    //
    $this->set_affected_rows(mysql_affected_rows($conn));
//var_dump($this->affected_rows());
  }

  // デストラクタでmysql_free_result
  public function __destruct() {
    @mysql_free_result($this->res_);
    //parent::__destruct();
  }

/**
 * SELECTの結果取得
 *
 * @access public
 * @return boolean falseならこれ以上のデータはなし
 */
public function fetch() {
  // typeでメソッドを切り分けておく
  if ($this->is_fetch_type_num()) {
    $m = 'mysql_fetch_row';
  } else {
    $m = 'mysql_fetch_assoc';
  }

  if (!($this->row_ = $m($this->res_))) {
    return false;
  }
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
 * DBハンドルクラス Mysql
 *
 * DBハンドルを取り扱います
 *
 * @deprecated
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/
require_once('sql_util_mysql.inc');
require_once("mw_makesql_escaped_mysql.inc");

class dbh_mysql extends db_handle {
//public:


  // コンストラクタ
  public function __construct() {
    //
    parent::__construct();

    //
    $this->set_sql_util(new sql_util_mysql);
    //
    $o = new mw_makesql_escaped_mysql();
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
  // すでに接続されていたら処理しない
  if (false === (is_null($this->conn_))) {
    return true;
  }
//var_dump($this->conn_);

  // 引数を作る
  $param = "";
  if ("" != $this->get_host_name()) {
    $param = $this->get_host_name();
    if ("" != $this->get_port_num()) {
      $param .= ":" . $this->get_port_num();
    }
  }
//print "mysql param is $param \n";

  // connect
  if (!($this->conn_ = mysql_connect($param, $this->get_user(), $this->get_pass(), true))) {
//print $this->conn_->mysql_error() . "\n";
    $this->set_error_message("connect error!!");
    return false;
  }
//print "connect\n";
//var_dump($this->conn_);

  // db set
  $ret = mysql_select_db($this->get_database_name(), $this->conn_);
//print 'mysql select db ret is ';
//var_dump($ret);
//print mysql_error();
//print "database is " . $this->get_database_name() . "\n";
//var_dump($this->conn_);

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
//print "run disconnect \n";
  return @mysql_close($this->conn_);
}

/**
 * 「SQL文字列」によるクエリの発行
 *
 * @access public
 * @param string $sql SQL文
 * @return object SQL成功時にmysql_dataインスタンス、SQL失敗時にnull
 */
public function query_direct($sql) {
//print "mysql $sql \n";
  if (!($result = mysql_query($sql, $this->conn_))) {
//var_dump($result);
    $this->set_error_message(mysql_error($this->conn_));
//var_dump($this->get_error_message());
    return null;
  }

  $ret = new mysql_data($result, $this->conn_);
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
  return mysql_real_escape_string($s, $this->conn_);
}


//private:
private $conn_ = null;
}

