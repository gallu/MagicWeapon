<?php

/**
 * Pageコントロールクラス 制限版
 *
 * 最低限のデータをDBから取り出すタイプ
 * max pageなどは取れないが、メモリ的には軽い
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  yyyy/mm/dd
 * @version $Id$
 **/

require_once('page_controll_base.inc');

class page_controll_limit extends page_controll_base {

public function __construct()
{
  //
  parent::__construct();
  //
  $this->get_all_flg_on(); // 「全カラム取得」をデフォルトにする
}

/**
 * DBハンドルの設定
 *
 * @access public
 * @param db_handle $o db_handleクラス(からの派生クラス)のインスタンス
 */
public function set_db($o) { $this->dbh_ = $o; }

/**
 * DBハンドルの設定
 *
 * @access public
 * @return db_handle db_handleクラス(からの派生クラス)のインスタンス
 */
public function get_db() { return $this->dbh_; }

/**
 * SQLの設定
 *
 * @access public
 * @param string $s SQL文(後ろに ; を付けない事！！)
 */
public function set_sql($s) { $this->sql_ = $s; }

/**
 * SQLの設定
 *
 * @access public
 * @return string SQL文
 */
public function get_sql() { return $this->sql_; }

/**
 * paging要素の作成
 *
 * @access public
 * @return boolean 何か問題があったらfalse
 */
public function make_list()
{
  // DBハンドルの取得
  $dbh = $this->get_db();

  // SQL文の完成
  $from  = $this->_get_page_num() * $this->get_par_item_num();
  $to = $this->get_par_item_num() + 1;
  //$sql = $this->get_sql() . ' ' . $suobj->limited_range($from, $to) . ';';

  // SQLの状況別処理
  // XXX なんか汚い。うまいこと外に切り出せないもんかねぇ？
  $sql = $this->get_sql();
  if (true === is_string($sql)) {
    // dbhハンドル内のmakesqlからlimitの文字列をとりだしてくっつける
    $sql .= $dbh->get_mw_makesql($dbh->get_table_name($sql))->limited_range($from, $to);
  } else {
    if (true === $sql->is_prepared()) {
      // オブジェクトからsqlを取り出して
      $wk = $sql->get_sql();
      // sqlにlimitをくっつけて
      $wk .= $dbh->get_mw_makesql($dbh->get_table_name($wk))->limited_range($from, $to);
//var_dump($wk);
      // sqlを入れ直して
      $sql->set_sql($wk);
    } else {
      // オブジェクトからsqlを取り出して
      $wk = $sql->get_sql();
      // sqlにlimitをくっつけて
      $wk .= $dbh->get_mw_makesql($dbh->get_table_name($wk))->limited_range($from, $to);
      // sqlを入れ直して
      $sql->set_sql($wk);
    }
  }
//print $sql . "\n";

  // データの取得
  $res = $dbh->query($sql);
  if (true === $this->is_get_all_flg()) {
    $res->set_fetch_type_hash();
  } else {
    $res->set_fetch_type_num();
  }
//var_dump($sql);
//var_dump($dbh);
//var_dump($res);

  // とりあえず全部データげとる
  // 「１カラムのみ」か「全カラム」かはモード次第
  $list = array();
  while($res->fetch()) {
    if (true === $this->is_get_all_flg()) {
      $list[] = $res->get_row();
    } else {
      $list[] = $res->get_data(0);
    }
  }

  // nextフラグ判定とデータのちょん切り
  if ($to == count($list)) {
    // ケツの要素一つ削る
    array_pop($list);
    $this->next_flg_on();
  } else {
    $this->next_flg_off();
  }

  // データを設定
  $this->set_list($list);

  // back フラグの設定
  if ($this->_get_page_num() != 0) {
    $this->back_flg_on();
  } else {
    $this->back_flg_off();
  }

  //
  return true;
}


/**
 * 最大ページ数の取得
 *
 * ０スタートである点に注意
 * XXX 未完成なので注意！
 *
 * @access public
 * @return int 最大ページ数
 */
public function get_max_page()
{
  // データの取得
  // XXX どうやって「全件の件数」取得すると重くないかねぇ？
  $res = $this->get_db()->query($this->get_sql());
//var_dump($dbh);
//var_dump($res);

  // XXX
  $count = 0; // XXX SELECTの行数取るやつ、dbハンドル側でラップして実装するかねぇ？
  // XXX これ、処理重複してるから、計算式を上に持ち上げないと…
  $ret =  floor(( $count + ($this->get_par_item_num() - 1)) / $this->get_par_item_num()) - 1;

 //
  return $ret;
}

/**
 * 「全カラム取得」モードか判定
 *
 * 旧来はfalse(pk１カラムのみ取得)。新しいフォーマットはtrue(全カラム情報取得)
 *
 * @access public
 * @return boolean 1カラムのみモードならfalse、全カラム取得モードならtrue
 */
public function is_get_all_flg() { return $this->get_all_flg_; }
/**
 * 「全カラム取得モード」に設定
 *
 * XXX 一旦こっちをデフォルトに
 *
 * @access public
 */
public function get_all_flg_on() { $this->get_all_flg_ = true; }
/**
 * 「1カラムのみ取得モード」に設定
 *
 * XXX 旧バージョンのデフォルト。今はこっちをデフォルトにしない
 *
 * @access public
 */
public function get_all_flg_off() { $this->get_all_flg_ = false; }

//private:
private $dbh_;
private $sql_;
private $get_all_flg_;

} // end of class

