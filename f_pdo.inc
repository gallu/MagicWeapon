<?php
/**
 * DBデータクラス PDO
 *
 * SELECTによって帰ってくるDBデータを取り扱います
 *
 * @deprecated
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once("db.inc");
require_once("data_clump.inc");

class pdo_data extends db_data {
//public:

  // XXX $conn使わないねぇ…どしよ？
  public function __construct($res, $conn) {
    $this->res_ = $res;

    // XXX columnCountは「結果セットにあるカラム数」だから多分SELECT用と思われ
    // XXX 一応想定は「insert/update/delete用」なのでこっちだけど…どうしよう文字列解析してスイッチするかねぇ？
    $this->set_affected_rows($res->rowCount());
//var_dump($this->affected_rows());
  }

  // デストラクタでmysql_free_result
  public function __destruct() {
    // XXX なんか他に手ぇあるかねぇ？
    unset($this->res_);
  }

/**
 * SELECTの結果取得
 *
 * @access public
 * @return boolean falseならこれ以上のデータはなし
 */
public function fetch() {
  // typeでメソッドを切り分けておく
  // XXX なんかFETCH_BOTHでもいい気もするんだけど…どうだろう？
  if ($this->is_fetch_type_num()) {
    $type = PDO::FETCH_NUM;
  } else {
    $type = PDO::FETCH_ASSOC;
  }

  if (!($this->row_ = $this->res_->fetch($type))) {
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
 * DBハンドルクラス PDO
 *
 * DBハンドルを取り扱います
 *
 * @deprecated
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

abstract class dbh_pdo extends db_handle {

/**
 * 接続先DB用文字列
 *
 * @access protected
 * @return string DSN文字列用のRDBMS名
 */
abstract protected function get_rdb_name();

//public:
/**
 * DBへの接続
 *
 * XXX 第一引数どうしようかねぇ…普段は好まないんだけどねぇ…
 * XXX あと、接続のDB文字コードをutf8固定にしてる。後で切り出さないとねぇ…
 *
 * @access public
 * @return boolean 接続できたらtrue、出来なかったらfalse
 */
public function connect($dbtype = 'mysql') {
  // すでに接続されていたら処理しない
  if (false === (is_null($this->conn_))) {
    return true;
  }
//var_dump($this->conn_);

  // 引数を作る
  $dsn = "{$this->get_rdb_name()}:dbname={$this->get_database_name()};host={$this->get_host_name()};charset=utf8";
  // ポート番号は、指定があれば足し込む
  if ("" !== $this->get_port_num()) {
    $dsn .= ";port={$this->get_port_num()}";
  }
//print "PDO dsn is {$dsn}\n";

  // connect
  try {
    $this->conn_ = new PDO($dsn, $this->get_user(), $this->get_pass());
  } catch (PDOException $e) {
    $this->set_error_message("connect error!! ({$e->getMessage()})");
    return false;
  }
//print "connect\n";
//var_dump($this->conn_);

  // 静的プレースホルダ。これは譲る気ねぇ。
  $this->conn_->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);  // 静的プレースホルダを指定

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
  // unsetでGC様にご連絡
  unset($this->conn_);
  // 再接続の可能性に備えて、初期値であるnullにしておく
  $this->conn_ = null;

  //
  return true;
}

/**
 * 「SQL文字列」によるクエリの発行
 *
 * XXX 場合によっては警告でも出しちゃろうかしらん？
 *
 * @access public
 * @param string $sql SQL文
 * @return object SQL成功時にpdo_dataインスタンス、SQL失敗時にnull
 */
public function query_direct($sql) {
//print "pdo $sql \n";
  if (!($result = $this->conn_->query($sql))) {
//var_dump($result);
    $awk = $this->conn_->errorInfo();
    $this->set_error_message("({$awk[0]}),({$awk[1]}),({$awk[2]})");
//var_dump($this->get_error_message());
    return null;
  }

  $ret = new pdo_data($result, $this->conn_);
  return $ret;
}

/**
 * 「準備された文」によるクエリの発行
 *
 * @access public
 * @param object $sql mw_sqlインスタンス
 * @return object SQL成功時にpdo_dataインスタンス、SQL失敗時にnull
 */
public function query_prepared($sql) {

  // 文を準備する
  $p_sql = $sql->get_sql();
//print "prepared SQL: {$p_sql}\n";
  $result = $this->conn_->prepare($p_sql);
  if (false === $result) {
    $awk = $this->conn_->errorInfo();
    $this->set_error_message("({$awk[0]}),({$awk[1]}),({$awk[2]})");
    return null;
  }

  // 値をbindする
  $awk = $sql->get_bind_datas();
  foreach($awk as $wk) {
    // typeを判定
    if (data_clump::DATATYPE_INT === $wk['type']) {
      $type = PDO::PARAM_INT;
      // 値の型を整えておく
      $wk['value'] = intval($wk['value']);
    } else
    if (data_clump::DATATYPE_DATE === $wk['type']) {
      $type = PDO::PARAM_STR; // XXX 日付、ってないもんかね？
    } else
    if (data_clump::DATATYPE_FLOAT === $wk['type']) {
      $type = PDO::PARAM_INT; // INTだけど整数だけぢゃなくて小数もいけるんだねぇ…
      // 値の型を整えておく
      $wk['value'] = floatval($wk['value']);
    } else {
      $type = PDO::PARAM_STR;
    }

    // bind(念のため、bindValueののほうで紐付
    $result->bindValue($wk['name'], $wk['value'], $type);
  }

  // 実行する
  $r = $result->execute();
//print "Trap 2\n";
  if (false === $r) {
    $awk = $result->errorInfo();
    $this->set_error_message("({$awk[0]}),({$awk[1]}),({$awk[2]})");
    return null;
  }

  // dataオブジェクト作って復帰
  $ret = new pdo_data($result, $this->conn_);
  return $ret;
}


/**
 * トランザクションの開始
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
  $r = $this->conn_->beginTransaction(); // XXX マルチん時の対応想定してないなぁ…
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
  $r = $this->conn_->commit(); // XXX マルチん時の対応想定してないなぁ…
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
  $r = $this->conn_->rollBack(); // XXX マルチん時の対応想定してないなぁ…

  // 中断なのでどっちにしてもステータス「トラン」はoffっておく。
  $this->tran_off();
  return true;
}



//private:
private $conn_ = null;
}

