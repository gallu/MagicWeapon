<?php

/**
 * Pageコントロールクラス 制限版
 *
 * 最低限のデータをDBから取り出すタイプ
 * max pageなどは取れないが、メモリ的には軽い
 * 現状、主キーが１カラムのSQLにのみ対応
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  yyyy/mm/dd
 * @version $Id$
 **/

require_once('page_controll_base.inc');

class page_controll_limit extends page_controll_base {

public function __constract()
{
  parent::__constract();
  //$this->init();
}

/**
 * 初期化
 *
 * @access public
 */
public function init()
{
  parent::init();
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
  $from  = $this->get_page_num() * $this->get_par_item_num();
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
//var_dump($sql);
//var_dump($dbh);
//var_dump($res);

  // とりあえず全部データげとる
  $list = array();
  while($res->fetch()) {
    $list[] = $res->get_data(0);
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
  if ($this->get_page_num() != 0) {
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

//private:
private $dbh_;
private $sql_;

} // end of class

