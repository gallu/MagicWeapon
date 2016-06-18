<?php

/**
 * Pageコントロールクラス
 *
 * 「全List(通常データの主キー)を取得してからページング処理」なロジック
 * max page数などが簡単に捕捉できる反面、ぶっちゃけメモリを大量に食らう
 * あんまり配列がでかいときはお勧めしません。
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once('page_controll_base.inc');

class page_controll extends page_controll_base {

public function __construct()
{
  //
  parent::__construct();
  //
  $this->clear_datas();
}

/**
 * 「全体の要素配列」の設定
 *
 * XXX とはいえ、pkだけの配列持って「clumpぶん回す」ほうが色々安いです
 *
 * @access public
 * @param array $a 全体の要素配列
 */
public function set_datas($a) { $this->datas_ = $a; }
/**
 * 「全体の要素配列」の取得
 *
 * @access public
 * @return array $a 全体の要素配列
 */
public function get_datas() { return $this->datas_; }
/**
 * 「全体の要素配列」のクリア
 *
 * @access public
 */
public function clear_datas() { $this->datas_ = array(); }


/**
 * paging要素の作成
 *
 * @access public
 * @return boolean 何か問題があったらfalse
 */
public function make_list()
{
  // とりあえず先に初期化
  $this->set_list(array());

  // とりあえず現在の配列の件数をチェック
  $count = count($this->get_datas());
  if ( 0 == $count) {
    return false;
  }

  // 現在指定されているPageが範疇内かどうか決定
  // XXX 範疇内でない場合、最大Pageに無理やり調整
  $min = $this->get_page_num() * $this->get_par_item_num() + 1;
  if ( $count < $min ) {
    $this->set_page_num($this->get_max_page());
  }

  // 出力用のListを抽出
  $base = $this->get_page_num() * $this->get_par_item_num();
  $datas = $this->get_datas();
  $list = array();
  for($i=0; $i < $this->get_par_item_num(); $i ++) {
    if ($count > ($base + $i) ) {
      $list[] = $datas[$base + $i];
    }
  }
  //
  $this->set_list($list);

  // フラグの設定
  if ($this->get_page_num() != 0) {
    $this->back_flg_on();
  } else {
    $this->back_flg_off();
  }

  $max = $this->get_max_page();
  if ($this->get_page_num() < $max) {
    $this->next_flg_on();
  } else {
    $this->next_flg_off();
  }

  //
  return true;
}


/**
 * 最大ページ数の取得
 *
 * ０スタートである点に注意
 *
 * @access public
 * @return int 最大ページ数
 */
public function get_max_page()
{
  $count = count($this->get_datas());
  $ret =  floor(( $count + ($this->get_par_item_num() - 1)) / $this->get_par_item_num()) - 1;

 //
  return $ret;
}

//private:
private $datas_;	// 全体の要素配列

} // end of class

