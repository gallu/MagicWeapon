<?php

/**
 * CGI パラメタクラス
 *
 * いわゆる "name=value&name=value&"の形式を取り扱う
 * XXX 別にCGIに限らんのですが。
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 */

require_once('multimap.inc');

class cgi_param
{

//
public function __construct()
{
  $this->init();
}

/**
 * init
 *
 */
public function init()
{
  $this->data_ = new multimap;
}

// 念のため
protected function encode($s) { return urlencode($s); }
protected function decode($s) { return urldecode($s); }

/**
 * 追加
 *
 * @access public
 * @param string $name 項目名
 * @param string $value 値
 */
public function add($name, $value)
{
  // XXX
  if ("" === $name) {
    return true;
  }
  $this->data_->insert($name, $value);
//print "add $name <= $value \n";
  return true;
}

/**
 * 単独追加
 *
 * addと似ているが、重複したキーがある場合上書きをする
 *
 * @access public
 * @param string $name 項目名
 * @param string $value 値
 */
public function add_once($name, $value)
{
  // XXX
  if ("" === $name) {
    return true;
  }
//print "add_once $name <= $value \n";
  // キーを捜してみる
  $itr = $this->data_->find($name);
//var_dump($itr);
  //if (empty($itr)) {
  if (is_null($itr)) {
    // なければinsert
    $this->data_->insert($name, $value);
//print "add_once insert\n";
  } else {
    // あれば上書き
    $this->data_->real($itr)->set_second($value);
//print "add_once update\n";
  }
  return true;
}

 /**
 * 単体検索
 *
 * お探し君。対応データがない場合は空文字で復帰
 *
 * @access public
 * @param string $key 探したいデータのkey
 * @return string 値。keyがない場合は空文字を帰す
 */
public function find($key)
{
  //
  $buf = $this->finds($key);

  // XXX empty関数危ないけど、ここは配列onlyってわかってるので
  if (empty($buf)) {
    return "";
  }

  // else
  return $buf[0];
}

 /**
 * 複数検索ラッパー
 *
 * findsが実態だけど…つづり間違えそうだし
 *
 * @access public
 * @param string $key 探したいデータのkey
 * @return string 値。keyがない場合は空文字を帰す
 */
public function find_array($key) { return $this->finds($key); }

 /**
 * 複数検索
 *
 * お探し君。対応データがない場合は空文字で復帰
 *
 * @access public
 * @param string $key 探したいデータのkey
 * @return string 値。keyがない場合は空文字を帰す
 */
public function finds($key)
{

  //
  $ret = array();

  // まずイテレータで取得
  $itrs = $this->data_->find_array($key);

  // 空配列確認
  // 微妙危険な関数だが、戻り値が配列限定なので
  if (empty($itrs)) {
    return $ret;
  }

  // もどってきたイテレータ配列からvalueを引きずり出す
  foreach($itrs as $itr) {
    // 実態の取得
    $ret[] = $this->data_->real($itr)->get_second();
  }

  //
  return $ret;
}

 /**
 * データ設定
 *
 * 文字列を受け取って内部に展開する
 *
 * @access public
 * @param string $s name=value&形式の文字列
 */
public function set($s)
{
  // 一応念のため
  $this->init();

  // 切り分け
  $datas = explode("&", $s);

  // ぶん回しつつ設定
  foreach ($datas as $data) {
    // まず切り分け
    $kv = explode("=", $data);

    // デコード
    $key = $this->decode($kv[0]);
    $value = $this->decode($kv[1]);

    // 設定
    $this->add($key, $value);
  }

  //
  return true;
}

 /**
 * データ取得
 *
 * name=value&形式の文字列として内部情報を取り出す
 *
 * @access public
 * @return string name=value&形式の文字列
 */
public function get()
{

$datas = array();

  // イテレータでぶんまわし
  for($itr = $this->data_->begin(); $itr != $this->data_->end(); $itr ++) {
    // 文字列合成
    $wk = $this->encode($this->data_->real($itr)->get_first()) . "=" .
          $this->encode($this->data_->real($itr)->get_second());

    // 突っ込む
    $datas[] = $wk;
  }

  // つなげつつ復帰
  return implode("&", $datas);
}

 /**
 * keylist取得
 *
 * @access public
 * @return vector<string> key配列
 */
public function get_all_keys()
{
  return $this->data_->get_all_keys();
}

/**
 * 指定されたkeyを持つ要素を削除する。
 *
 * keyが空文字の場合「なにもしない」
 * 全削除したいならclearメソッドつかって。
 *
 * @access public
 * @param string $key 削除したいkey文字列
 * @return integer 削除した要素数
 */
public function erase_key($key)
{
  return $this->data_->erase_key($key);
}


//private:
private $data_;

} // end of class

