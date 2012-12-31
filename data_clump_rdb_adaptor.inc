<?php

/**
 * data_clumpとRDBハンドルとを紐付けるクラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 */

require_once('data_clump_adaptor_base.inc');

class data_clump_rdb_adaptor extends data_clump_adaptor_base
{

/**
 * create用処理
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean
 */
public function create($clump)
{
  // 全データの格納
  $value = array();
  foreach($clump->get_all_names() as $name) {
    $value[ $clump->get_db_name($name) ] = $clump->get_data($name);
    $data_type[ $clump->get_db_name($name) ] = $clump->get_data_type($name);
  }

  // SQLの作成
  $sql = $this->get_data_handle()->get_mw_makesql($clump->get_table_name())->make_insert($clump->get_table_name(), $value, $data_type);

  // SQLの発行依頼
  $ret = $this->_sql($sql, 'write', $clump->get_identifier());
//print "db insert is\n";
//var_dump($ret);

  // XXX ちょいと雑
  return !(is_null($ret));
}


/**
 * pkを使ったread
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean
 */
public function read_pk($clump)
{
  // WHERE句用配列の作成
  $where = $this->_make_where($clump);

  // 実働依頼
  return $this->_read($clump, $where);
}


/**
 * pkによらないread
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean
 */
public function read_nopk($clump)
{
  // WHERE句用配列の作成
  $cols = $clump->get_all_names();

  // 値があるものをandでwhereとする
  $where = array();
  foreach ($cols as $col) {
    //
    if ("" !== $clump->get_value($col)) {
      $where['where'][$col] = $clump->get_value($col);
      $where['data_type'][$col] = $clump->get_data_type($col);
    }
  }

  // 実働依頼
  return $this->_read($clump, $where);
}


/**
 * 全体update用処理
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean
 */
public function update_all($clump)
{
  // value部分の設定
  $values = array();
  foreach($clump->get_all_no_key_names() as $name) {
    $values[ $clump->get_db_name($name) ] = $clump->get_data($name);
  }

  // 後はおまかせ
  return $this->_update($clump, $values);
}


/**
 * 部分的なupdate用処理
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean
 */
public function update_mono($clump)
{
  // value部分の設定
  $values = array();
  foreach($clump->get_mono_set_db_target() as $name) {
    $values[ $clump->get_db_name($name) ] = $clump->get_data($name);
  }

  // 後はおまかせ
  return $this->_update($clump, $values);
}

/**
 * 更新処理
 *
 * insertまたはupdate
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @param array $targets 更新対象カラム名の配列
 * @return boolean
 */
public function set($clump){
  // 全データの格納
  $value = array();
  foreach($clump->get_all_names() as $name) {
    $value[ $clump->get_db_name($name) ] = $clump->get_data($name);
    $data_type[ $clump->get_db_name($name) ] = $clump->get_data_type($name);
  }

  // SQLの作成
  $sql = $this->get_data_handle()->get_mw_makesql($clump->get_table_name())->make_upsert($clump->get_table_name(), $value, $data_type);

  // SQLの発行依頼
  $ret = $this->_sql($sql, 'write', $clump->get_identifier());
//print "db insert is\n";
//var_dump($ret);

  // XXX ちょいと雑
  return !(is_null($ret));
}

/**
 * 削除用処理
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @param array $targets 更新対象カラム名の配列
 * @return boolean
 */
public function delete($clump)
{
  // deleteテーブル用のinsertを独自発行〜
  // XXX createと処理ほとんど一緒なんだけどねぇ…とりあえず共通化はしないっつか悩んでる
  // --------------------------------------------
  // 削除テーブルのテーブル名作成
  // XXX これ、後で可変にする？
  $delete_table_name = $clump->get_table_name() . '_delete';

  // もしdeleteテーブルがあったらデータを移動させる
  // XXX flgで明確に持つか？：でもミス怖いよねぇ：でも効率いいよねぇ
  if (true === $this->is_table_exists($delete_table_name)) {
    // 全データの格納
    $value = array();
    foreach($clump->get_all_names() as $name) {
      $value[ $clump->get_db_name($name) ] = $clump->get_data($name);
      $data_type[ $clump->get_db_name($name) ] = $clump->get_data_type($name);
    }
    // 削除日の格納
    $value['delete_date'] = calendar_util::get_now_string();
    $data_type['delete_date'] = data_clump::DATATYPE_STRING;

    // SQLの作成
    $sql = $this->get_data_handle()->get_mw_makesql($clump->get_table_name())->make_insert($delete_table_name, $value, $data_type);

    // SQLの発行依頼
    $this->_sql($sql, 'write', $delete_table_name);
  }

  // 実際の削除処理
  // --------------------------------------------

  // WHERE句用配列の作成
  $where = $this->_make_where($clump);

  // SQLの作成
  $sql = $this->get_data_handle()->get_mw_makesql($clump->get_table_name())->make_delete($clump->get_table_name(), $where['where'], $where['data_type']);

  // SQLの発行依頼
  $ret = $this->_sql($sql, 'write', $clump->get_identifier());

  // XXX ちょいと雑
  return !(is_null($ret));
}

/**
 * SQL発行処理
 *
 * @access protected
 * @param string $sql SQL
 * @return object エラーだったときはNULL
 */
protected function _sql($sql, $mode, $identifier_name)
{
  //
  if (true === $this->get_data_handle()->is_mono_rdb()) {
    $r = $this->get_data_handle()->query($sql);
  } else {
    $r = $this->get_data_handle()->query_to_specified_handle($sql, $mode, $identifier_name);
  }
  return $r;
}


/**
 * pkによるwhere句(用の配列)の作成
 *
 * @access protected
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return array
 */
protected function _make_where($clump)
{
  $where = array();
  foreach($clump->get_all_key_names() as $name) {
    $where['where'][ $clump->get_db_name($name) ] = $clump->get_data($name);
    $where['data_type'][ $clump->get_db_name($name) ] = $clump->get_data_type($name);
  }
  //
  return $where;
}


/**
 * readの実態
 *
 * @access protected
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @param array $where where句作成用配列
 * @return boolean
 */
protected function _read($clump, $where)
{
  // SELECT カラムの作成
  $col_array = array();
  $awk = $clump->get_all_names();
  // DB名に変更
  foreach($awk as $wk) {
    $col_array[] = $clump->get_db_name($wk);
  }

  // SQLの作成
  $sql = $this->get_data_handle()->get_mw_makesql($clump->get_table_name())->make_select($clump->get_table_name(), $col_array, $where['where'], $where['data_type']);
//print "<br>" . $sql . "<br>\n";

  // モードの確認
  if (false ===$this->get_data_handle()->is_tran()) {
    $mode = 'read';
  } else {
    // トランザクション中の場合はwriteを見に行く
    $mode = 'write';
  }
  // SQLの発行依頼
  $res = $this->_sql($sql, $mode, $clump->get_identifier());
  // SQLの発行でこけたっぽいならNG
  if (true === is_null($res)) {
    return false;
  }

  // データがなければNG
  if (false === $res->fetch()) {
    return false;
  }

  // データの設定
  $count = 0;
  foreach($clump->get_all_names() as $name) {
//print "Trap2 <br />\n";
    // データをインスタンス内に保持
    $clump->set_value($name, $res->get_data($count), false);
//print "$name is " . $res->get_data($count) . "\n";

    //
    $count ++;
  }

  //
  return true;
}


/**
 * update用処理本体
 *
 * @access protected
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @param array $values 更新対象のデータ一式
 * @return boolean
 */
protected function _update($clump, $values)
{
  // WHERE句用配列の作成
  $where = $this->_make_where($clump);

  // values用のdata_type判定
  $data_type = array();
  foreach($values as $k => $v) {
    $data_type[$k] = $clump->get_data_type($k);
  }

  // SQLの作成
  $sql = $this->get_data_handle()->get_mw_makesql($clump->get_table_name())->make_update($clump->get_table_name(), $values, $where['where'], $data_type, $where['data_type']);

  // SQLの発行依頼
  $ret = $this->_sql($sql, 'write', $clump->get_identifier());
//print "db update is\n";
//var_dump($ret);

  // XXX ちょいと雑
  return !(is_null($ret));
}

/**
 * テーブルの存在確認
 *
 * @access protected
 * @param string $table_name テーブル名
 * @return boolean
 */
protected function is_table_exists($table_name)
{
  // SQLの作成
  $sql = $this->get_data_handle()->get_mw_makesql($table_name)->make_table_exists_sql($table_name);

  // SQLの発行 XXX writeとreadで差異はないはずだから、負荷考えてread見たほうがいいでしょ、的な
  $ret = $this->_sql($sql, 'read', $table_name);

  // 存在確認
  if (false === $ret->fetch()) {
    return false;
  }
  // else
  return true;
}


/**
 * 数値の加減算
 *
 * XXX 設定したらすぐにDB等に書きに行くので留意
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean 成功したらtrue、なにか失敗があったらfalse
 */
public function calculation($clump)
{
  // WHERE句用配列の作成
  $where = $this->_make_where($clump);

  // 必要な諸条件の抜き出し
  $col = $clump->get_calculation_col_name();
  $sign = $clump->get_calculation_sign();
  $num = $clump->get_calculation_num();

  // SQLの作成
  $sql = $this->get_data_handle()->get_mw_makesql($clump->get_table_name())->make_update_calculation($clump->get_table_name(), $col, $sign, $num, $where['where'], $where['data_type']);

  // SQLの発行依頼
  $ret = $this->_sql($sql, 'write', $clump->get_identifier());
//print "db update is\n";
//var_dump($ret);

  // XXX ちょいと雑
  return !(is_null($ret));
}

} // end of class
