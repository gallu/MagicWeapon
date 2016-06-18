<?php

/**
 * Pageコントロールクラス
 *
 *
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  yyyy/mm/dd
 * @version $Id$
 **/

abstract class page_controll_base {

public function __construct()
{
  $this->set_page_num(0);
  $this->set_par_item_num(1);
  $this->next_flg_off();
  $this->back_flg_off();
}

/**
 * 現在Page数の指定
 *
 * @access public
 * @param int $i 現在Page数
 */
public function set_page_num($i) { $this->page_num_ = (($i+0) > 0)? $i: 0; }
/**
 * 現在Page数の取得
 *
 * @access public
 * @return int 現在Page数
 */
public function get_page_num() { return $this->page_num_; }

/**
 * 1Pageあたりの要素数の指定
 *
 * @access public
 * @param int $i 1Pageあたりの要素数
 */
public function set_par_item_num($i) { $this->par_item_num_ = $i; }
/**
 * 1Pageあたりの要素数の取得
 *
 * @access public
 * @return int 1Pageあたりの要素数
 */
public function get_par_item_num() { return $this->par_item_num_; }


/**
 * Paging処理結果の設定
 *
 * @access protected
 * @param array $a Paging結果情報の配列
 */
protected function set_list($a) { $this->list_ = $a; }
/**
 * Paging処理結果の取得
 *
 * @access public
 * @return array Paging結果情報の配列
 */
public function get_list() { return $this->list_; }


/**
 * 「次Pageに遷移出来るかどうか」フラグの設定
 *
 * @access protected
 * @param boolean $b 遷移できるならtrue、出来ないならfalse
 */
protected function set_next_flg($b) { $this->next_flg_ = $b; }
/**
 * 「次Pageに遷移出来るかどうか」フラグの取得
 *
 * @access protected
 * @return boolean 遷移できるならtrue、出来ないならfalse
 */
protected function get_next_flg() { return $this->next_flg_; }
/**
 * 「次Pageに遷移出来るかどうか」フラグを「遷移出来る」に設定
 *
 * @access public
 */
public function next_flg_on() { $this->set_next_flg(true); }
/**
 * 「次Pageに遷移出来るかどうか」フラグを「遷移出来ない」に設定
 *
 * @access public
 */
public function next_flg_off() { $this->set_next_flg(false); }
/**
 * 「次Pageに遷移出来るかどうか」判定
 *
 * @access public
 * @return boolean 遷移できるならtrue、出来ないならfalse
 */
public function is_next() { return $this->get_next_flg(); }


/**
 * 「前Pageに遷移出来るかどうか」フラグの設定
 *
 * @access protected
 * @param boolean $b 遷移できるならtrue、出来ないならfalse
 */
protected function set_back_flg($b) { $this->back_flg_ = $b; }
/**
 * 「前Pageに遷移出来るかどうか」フラグの取得
 *
 * @access protected
 * @return boolean 遷移できるならtrue、出来ないならfalse
 */
protected function get_back_flg() { return $this->back_flg_; }
/**
 * 「前Pageに遷移出来るかどうか」フラグを「遷移出来る」に設定
 *
 * @access public
 */
public function back_flg_on() { $this->set_back_flg(true); }
/**
 * 「前Pageに遷移出来るかどうか」フラグを「遷移出来ない」に設定
 *
 * @access public
 */
public function back_flg_off() { $this->set_back_flg(false); }
/**
 * 「前Pageに遷移出来るかどうか」判定
 *
 * @access public
 * @return boolean 遷移できるならtrue、出来ないならfalse
 */
public function is_back() { return $this->get_back_flg(); }


/**
 * 「特定の幅でPage数を表示したい時」の幅取得
 *
 * 全Pageじゃなくて特定の幅でPage数を表示したい時用
 * 表示するPage数を「現在に±ｎPageまで」とかいう時用
 * XXX 1 2 3 4 5 6 7 8 9 10 ではなくて、 .. [3] [4] 5 [6] [7] ... みたいな感じ
 * XXX 上述の場合「現在5page目で、±２page」って設定。引数に２が入る。
 *
 * @access public
 * @param int $i 前後に振りたいPage数の幅。説明参照。
 * @return array ページ数の配列
 */
public function make_range($i)
{
  //
  $ret = new max_min;
  $now = $this->get_page_num();

  // 設定
  $ret->set_max( ($now + $i > $this->get_max_page())? $this->get_max_page() : $now + $i );
  $ret->set_min( ($now - $i < 0)? 0 : $now - $i );

  //
  return $ret;
}

//
abstract public function make_list();
abstract public function get_max_page();


/*
 * convへの設定用util
 *
 * 概ね以下の書式を想定
 * 
<pre>
現在 %%%now_page%%% Pageです。
$$$is_next$$$
<a href="?c=hogehoge&id=iddd&p=%%%next_page%%%">次を見る</a><br />
$$$/is_next$$$
$$$is_back$$$
<a href="?c=hogehoge&id=iddd&p=%%%back_page%%%">前に戻る</a><br />
$$$/is_back$$$
</pre>
 *
 * @access public
 * @param conv $conv テンプレートエンジンインスタンス
 */
public function set_to_conv($conv)
{
  // pagingまわり
  $sp = new simple_put;
  if ($this->is_next()) {
    $conv->multiDic('is_next', $sp);
  }
  if ($this->is_back()) {
    $conv->multiDic('is_back', $sp);
  }
  $conv->monoDic('now_page',  $this->get_page_num());
  $conv->monoDic('next_page', $this->get_page_num() + 1);
  $conv->monoDic('back_page', $this->get_page_num() - 1);

  //
  return ;
}


//private:
private $page_num_;	// 現在のPage数
private $par_item_num_;	// 1Pageあたりの要素数
private $next_flg_;	//
private $back_flg_;	//
private $list_;		// 出力に用いるList : Pageingされた内容

} // end of class

// maxとminを持つクラス
class max_min
{
//public:
public function set_max($i) { $this->max_ = $i; }
public function get_max() { return $this->max_; }

public function set_min($i) { $this->min_ = $i; }
public function get_min() { return $this->min_; }

//private:
private $max_;
private $min_;
}
