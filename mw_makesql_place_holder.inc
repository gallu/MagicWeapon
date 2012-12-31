<?php
/**
 * 「SQLを作る」基底クラス
 *
 * SQL文(オブジェクト)を作成する
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once('mw_sql.inc');
require_once('mw_makesql_base.inc');
require_once('security.inc');

abstract class mw_makesql_place_holder extends mw_makesql_base
{

/**
 * select構文作成
 *
 * 後ろに付け足す可能性もあるので、SQLの最後の;は付けずに返す
 *
 * @access public
 * @param string $table_name テーブル名
 * @param array $cols 各カラムの名前(＊でもいいけど、あんまり推奨しない)。形式としてはvector<string>
 * @param array $where WHERE句用の各カラムの値。カラム名がkeyで、形式としてはmap<string, string>
 * @param array $where_data_type WHERE句用の各カラムのデータ型。カラム名がkeyで、形式としてはmap<string, int>
 * @return object mw_sqlオブジェクト
 */
public function make_select($table_name, array $cols, array $where, array $where_data_type)
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
  // XXX ここはチェックしねぇけどいいだろ別に外部入力ひっぱってこねぇし
  $col = implode(",", $cols);

  // WHERE部分の作成
  $sql_obj = $this->make_where($where, $where_data_type);

  // SQL準備文の合成
  $sql = "SELECT " . $col . " FROM " . $table_name . " WHERE " . $sql_obj->get_sql();
  if (true === $tran_flg) {
    // トランザクション用ロックの付与
    // XXX 外から「問答無用でoffる」オプションとかいるかなぁ？ 必要になったら考えよう
    $sql .= " FOR UPDATE";
  }
  $sql .= ';';

  // 設定
  $sql_obj->set_sql($sql);

  //
  return $sql_obj;
}


/**
 * insert構文作成
 *
 * @access public
 * @param string $table_name テーブル名
 * @param array $value 各カラムの値。カラム名がkeyで、形式としてはmap<string, string>
 * @param array $data_type 各カラムのデータ型。カラム名がkeyで、形式としてはmap<string, int>
 * @return object mw_sqlオブジェクト
 */
public function make_insert($table_name, array $value, array $data_type)
{
  //
  $sql_obj = new mw_sql();

  //
  $vname = array();
  $place_holder = array();

  foreach ($value as $key => $val) {
    // カラム名
    $vname[] = $key;

    // バインドする場所の名前
    $s = ":{$key}";
    $place_holder[] = $s;

    // 値をバインド
    $sql_obj->bind($s, $val, $data_type[$key]);
  }

  //
  $sql =  "INSERT INTO " . $table_name . " (" . implode($vname, ",") . ") VALUES (" . implode($place_holder, ",") . ");";
  $sql_obj->set_sql($sql);

  //
  return $sql_obj;
}

/**
 * update構文作成
 *
 * XXX 引数多いのと順番がちょいと気になり。ちゃんとしたやり方に変えた方がいいかなぁとか思案中
 *
 * @access public
 * @param string $table_name テーブル名
 * @param array $value 各カラムの値。カラム名がkeyで、形式としてはmap<string, string>
 * @param array $where WHERE句用の各カラムの値。カラム名がkeyで、形式としてはmap<string, string>
 * @param array $data_type 各カラムのvalue用のデータ型。カラム名がkeyで、形式としてはmap<string, int>
 * @param array $where_data_type WHERE句用の各カラムのデータ型。カラム名がkeyで、形式としてはmap<string, int>
 * @return object mw_sqlオブジェクト
 */
public function make_update($table_name, array $value, array $where, array $data_type, array $where_data_type)
{
//print "in to make_update\n";

  // 先にWHERE部分の作成
  $sql_obj = $this->make_where($where, $where_data_type);

  // 「name = value」部分の作成
  $awk = array();
  foreach ($value as $key => $val) {
    // プレースホルダ名
    $s = ":{$key}";

    // SQL用素材
    $awk[] = "{$key} = {$s}";

    // 値のbind
    $sql_obj->bind($key, $val, $data_type[$key]);
  }

  // sql準備文の組み立て
  $sql = "UPDATE {$table_name} SET " . implode($awk, ",");
  // WHERE句の付与
  if ('' !== $sql_obj->get_sql()) {
    $sql .= " WHERE " . $sql_obj->get_sql();
  }
  $sql .= ";";

  // sql準備文の設定
  $sql_obj->set_sql($sql);

  //
  return $sql_obj;
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
 * @param array $where_data_type WHERE句用の各カラムのデータ型。カラム名がkeyで、形式としてはmap<string, int>
 * @return object mw_sqlオブジェクト
 */
public function make_update_calculation($table_name, $col, $sign, $num, array $where, array $where_data_type)
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

  // 先にWHERE部分の作成
  $sql_obj = $this->make_where($where, $where_data_type);

  // SQLの作成
  $s = ":{$col}_calculation"; // XXX プレースホルダ名は重複を避けるためにsuffix付けて
  $sql = "UPDATE {$table_name} SET {$col} = {$col} {$sign} {$s}";
  // bindは数値固定
  $sql_obj->bind($s, $num, data_clump::DATATYPE_INT);
  // WHERE句の付与
  if ('' !== $sql_obj->get_sql()) {
    $sql .= " WHERE " . $sql_obj->get_sql();
  }
  $sql .= ";";

  // sql準備文の設定
  $sql_obj->set_sql($sql);

  //
  return $sql_obj;
}

/**
 * delete構文作成
 *
 * @access public
 * @param string $table_name テーブル名
 * @param array $where WHERE句用の各カラムの値。カラム名がkeyで、形式としてはmap<string, string>
 * @param array $where_data_type WHERE句用の各カラムのデータ型。カラム名がkeyで、形式としてはmap<string, int>
 * @return object mw_sqlオブジェクト
 */
public function make_delete($table_name, array $where, array $where_data_type)
{
  // 先にWHERE部分の作成
  $sql_obj = $this->make_where($where, $where_data_type);
  //
  $sql = "DELETE FROM {$table_name}";
  // WHERE句の付与
  if ('' !== $sql_obj->get_sql()) {
    $sql .= " WHERE " . $sql_obj->get_sql();
  }
  $sql .= ";";

  // sql準備文の設定
  $sql_obj->set_sql($sql);

  //
  return $sql_obj;
}

/**
 * where句作成
 *
 * @access public
 * @param array $where WHERE句用の各カラムの値。カラム名がkeyで、形式としてはmap<string, string>
 * @param array $where_data_type WHERE句用の各カラムのデータ型。カラム名がkeyで、形式としてはmap<string, int>
 * @return object mw_sqlオブジェクト(WHERE句部分のみが含まれている。WHEREキーワード自体は入ってない)
 */
public function make_where(array $where, array $where_data_type)
{

//print "in to make_where\n";
  $sql_obj = new mw_sql();
  $w = array();

  foreach ($where as $key => $val) {
    // place_holder用の文字列
    $s = ":{$key}";

    // 準備文用文字列の作成
    $w[] =  "{$key} = {$s}";
    // 値のバインド
    $sql_obj->bind($s, $val, $where_data_type[$key]);
  }
  //
  $sql_obj->set_sql(implode($w, " and "));

//print "out to make_where\n";
  return $sql_obj;
}

/**
 * 範囲指定(LIMIT句)用文字列の作成
 *
 * @deprecated
 * @access public
 * @param int $from LIMITの第一引数
 * @param int $to LIMITの第二引数
 * @return object mw_sqlオブジェクト
 */
abstract public function limited_range($from, $to);

} // end of class

