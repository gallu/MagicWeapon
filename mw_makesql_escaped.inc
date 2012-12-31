<?php
/**
 * 「SQLを作る」基底クラス
 *
 * SQL文(文字列)を作成する
 * XXX エスケープ系なので非推奨
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/


require_once('mw_makesql_base.inc');
require_once('security.inc');

abstract class mw_makesql_escaped extends mw_makesql_base
{

/**
 * select構文作成
 *
 * 後ろに付け足す可能性もあるので、SQLの最後の;は付けずに返す
 *
 * @deprecated
 * @access public
 * @param string $table_name テーブル名
 * @param array $cols 各カラムの名前(＊でもいいけど、あんまり推奨しない)。形式としてはvector<string>
 * @param array $where WHERE句用の各カラムの値。カラム名がkeyで、形式としてはmap<string, string>
 * @return string SQL文
 */
public function make_select($table_name, array $cols, array $where)
{
  // トランザクション確認
  // -----------------------------
  $tran_flg = false;
  // monoDBハンドルの取得
  $dbh = $this->get_db();
  if ((NULL !== $dbh)and(true === $dbh->is_multi_rdb())) {
    $dbh = $dbh->choose_dbh('write', $table_name); // tran切ってる可能性があるのはwriteなので
  }
  // 確認
  if (true === $dbh->is_tran()) {
    $tran_flg = true;
  }

  // カラム名文字列の作成
  $col = implode(",", $cols);

  // SQLの作成
  $sql = "SELECT " . $col . " FROM " . $table_name . " WHERE " . $this->make_where($where);
  if (true === $tran_flg) {
    // トランザクション用ロックの付与
    // XXX 外から「問答無用でoffる」オプションとかいるかなぁ？ 必要になったら考えよう
    $sql .= " FOR UPDATE";
  }
  $sql .= ';';
//var_dump($sql);

  //
  return $sql;
}


/**
 * insert構文作成
 *
 * @deprecated
 * @access public
 * @param string $table_name テーブル名
 * @param array $value 各カラムの値。カラム名がkeyで、形式としてはmap<string, string>
 * @return string SQL文
 */
public function make_insert($table_name, array $value)
{

  $vname = array();
  $vvalue = array();

  foreach ($value as $key => $val) {
//print $key . "\n";
    $vname[] = $key;
    $vvalue[] =  $this->escape_add_quote($val);
  }

  //
  $sql =  "INSERT INTO " . $table_name . " (" . implode($vname, ",") . ") VALUES (" . implode($vvalue, ",") . ");";

  //
  return $sql;
}

/**
 * update構文作成
 *
 * @deprecated
 * @access public
 * @param string $table_name テーブル名
 * @param array $value 各カラムの値。カラム名がkeyで、形式としてはmap<string, string>
 * @param array $where WHERE句用の各カラムの値。カラム名がkeyで、形式としてはmap<string, string>
 * @return string SQL文
 */
public function make_update($table_name, array $value, array $where)
{

//print "in to make_update\n";
  $v = array();
  $ret = "";

  //
  foreach ($value as $key => $val) {
    $v[] =  $key . "=" . $this->escape_add_quote($val);
  }

  $sql = "UPDATE " . $table_name . " SET " . implode($v, ",");
  $s = $this->make_where($where);
  if ("" != $s) {
    $sql = $sql . " WHERE " . $s;
  }

  //
  $sql = $sql . ";";
  return $sql;
}

/**
 * 数値計算系update構文作成
 *
 * １カラムに対して足したり引いたり、っての専用
 *
 * @access public
 * @param string $table_name テーブル名
 * @param string $col カラム名
 * @param string $sign 演算子。とりあえず＋と−を想定
 * @param int $num 加減算する値
 * @param array $where WHERE句用の各カラムの値。カラム名がkeyで、形式としてはmap<string, string>
 * @return string SQL文
 */
public function make_update_calculation($table_name, $col, $sign, $num, array $where)
{
//print "in to make_update_calculation\n";
  $v = array();
  $ret = "";

  // numはざっくりとvalidate
  $num = $num + 0;
  // signは外部入力を用いないはずなのだけど…一応念のため
  if ('-' !== $sign) {
    $sign = '+';
  }
  // XXX カラム名は内部もちだと思うので、いったんvalidateはしない

  //
  $sql = "UPDATE {$table_name} SET {$col} = {$col} {$sign} {$num} ";
  $s = $this->make_where($where);
  if ("" != $s) {
    $sql = $sql . " WHERE " . $s;
  }

  //
  $sql = $sql . ";";
  return $sql;
}

/**
 * delete構文作成
 *
 * @deprecated
 * @access public
 * @param string $table_name テーブル名
 * @param array $where WHERE句用の各カラムの値。カラム名がkeyで、形式としてはmap<string, string>
 * @return string SQL文
 */
public function make_delete($table_name, array $where)
{

  //
  $sql = "DELETE FROM " . $table_name;
  $s = $this->make_where($where);
  if ("" != $s) {
    $sql = $sql . " WHERE " . $s;
  }

  //
  $sql = $sql . ";";
  return $sql;
}

/**
 * where句作成
 *
 * @deprecated
 * @access public
 * @param array $where WHERE句用の各カラムの値。カラム名がkeyで、形式としてはmap<string, string>
 * @return string SQL文のうち、WHERE句部分
 */
public function make_where(array $where)
{

//print "in to make_where\n";
  $w = array();

  foreach ($where as $key => $val) {
    $w[] =  $key . "=" . $this->escape_add_quote($val);
  }
  //
  $s = implode($w, " and ");

//print "out to make_where\n";
  return $s;
}

/**
 * エスケープ処理メソッド
 *
 * DBハンドルが存在する場合はdb毎のエスケープ関数を、DBハンドルがない場合はsecurity::sanitizeを使う
 *
 * @deprecated
 * @access public
 * @param string $s エスケープしたい文字列
 * @return string エスケープされた文字列
 */
public function escape($s)
{
  $dbh = $this->get_db();
  if (NULL === $dbh) {
    return security::sanitize($s);
  }
  // else
  return $dbh->escape($s);
}

/**
 * 両端にクォートを付けたエスケープ処理メソッド
 *
 * 処理本体はescapeメソッドに依存
 *
 * @deprecated
 * @access public
 * @param string $s エスケープしたい文字列
 * @return string クォート付きでエスケープされた文字列
 */
public function escape_add_quote($s)
{
  return "'" . $this->escape($s) . "'";
}


/**
 * 範囲指定(LIMIT句)用文字列の作成
 *
 * 形式としては「LIMIT {$to} OFFSET {$from}」というstringを作る
 *
 * @deprecated
 * @access public
 * @param int $from LIMITの第一引数
 * @param int $to LIMITの第二引数
 * @return string SQL文
 */
abstract public function limited_range($from, $to);

} // end of class

