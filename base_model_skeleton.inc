<?php

/**
 * MVC Model部分基底クラス骨組み
 *
 * かな〜りすかすかに作る
 * 基本的にはこれではなく、これの継承クラスであるbase_modelを継承されたし
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 */

require_once('db_manager.inc');
require_once('debug_util.inc');

abstract class base_model_skeleton {

//public:

//
public function __construct() { $this->init(); }
//
public function init()
{
  // ここはoffる
  $this->recursive_off();
  $this->direct_output_off();
  //
  $this->set_body("");
  $this->dbh_ = null;
}


//
public function set_config($o) { $this->cfobj_ = $o; }
public function get_config() { return $this->cfobj_; }
//
public function set_mobile($o) { $this->mobobj_ = $o; }
public function get_mobile() { return $this->mobobj_; }
//
public function set_cgi_request($o) { $this->reqobj_ = $o; }
public function get_cgi_request() { return $this->reqobj_; }
//
public function set_cgi_response($o) { $this->resobj_ = $o; }
public function get_cgi_response() { return $this->resobj_; }

//
public function set_my_name($s) { $this->my_name_ = $s; }
public function get_my_name() { return $this->my_name_; }

//
public function set_body($s) { $this->body_ = $s; }
public function get_body() { return $this->body_; }

//
protected function recursive_on() { $this->recursive_flg_ = true; $this->set_tmp_filename(''); }
protected function recursive_off() { $this->recursive_flg_ = false; }
public function is_recursive_on() { return $this->recursive_flg_; }
protected function set_recursive_command($s) { $this->recursive_command_ = $s; }
public function get_recursive_command() { return $this->recursive_command_; }

/*
*/
public function direct_output_on() { $this->direct_output_flg_ = true; }
public function direct_output_off() { $this->direct_output_flg_ = false; }
public function is_direct_output_on() { return $this->direct_output_flg_; }


//
//
public function set_inside_encoding_obj($o)
  { $this->inside_encoding_obj_ = $o; }
public function get_inside_encoding_obj()
  { return $this->inside_encoding_obj_; }

// XXX Skeltonでいいのか？
public function get_template_dir() { return $this->get_config()->find('tmplate_base_dir'); }

public function set_tmp_filename($s) { $this->set_bag('tmp_filename', $s); }
public function get_tmp_filename() { return $this->get_bag('tmp_filename'); }
public function is_tmp_filename()
{
  if (false === $this->is_bag('tmp_filename')) {
    return false;
  }
  if ("" === $this->get_tmp_filename()) {
    return false;
  }
  return true;
}

public function set_error_tmp_filename($s)
  { $this->set_bag('error_tmp_filename', $s); }
public function get_error_tmp_filename()
  { return $this->get_bag('error_tmp_filename'); }
public function is_error_tmp_filename()
{
  if (false === $this->is_bag('error_tmp_filename')) {
    return false;
  }
  if ("" === $this->get_error_tmp_filename()) {
    return false;
  }
  return true;
}

// 鞄系
// 鞄インスタンスそのもののセッター
// XXX getterいる？ 参照だしねぇ実態はcontroller知ってるしねぇ…
// XXX とりあえずprotectedで。後で必要ならpublicに直して〜
public function set_bag_obj($o) { $this->bag_ = $o; }
protected function get_bag_obj() { return $this->bag_; }
// XXX 内部用いんちきmethod w
protected function bag() { return $this->get_bag_obj(); }

// 以下普段使い用
// 単独：重複は上書き
public function set_bag($name, $o) {
  // isでtrueなら一旦削除する
  if (true === $this->is_bag($name)) {
    // まずiterator取得して
    $itr = $this->bag()->find($name);
    // さく〜じょ
    $this->bag()->erase($itr);
  }

  // 追加
  $this->push_bag($name, $o);
}
// 配列のように追加
public function push_bag($name, $o) { $this->get_bag_obj()->insert($name, $o); }
// 単値のget
public function get_bag($name) { return $this->bag()->real($this->bag()->find($name))->get_second(); }

// 配列のget
public function get_array_bag($name)
{
  // まずiterator配列げと
  $itr_array = $this->bag()->find_array($name);
  // ぶん回しつつ要素を取得
  $ret = array();
  foreach($itr_array as $itr) {
    $ret[] = $this->bag()->real($itr)->get_second();
  }
  return $ret;
}
// 存在確認
public function is_bag($name) {
//print debug_util::backtrace_string();
//print "\n";

//var_dump($this->get_bag_obj());

  $ret = $this->get_bag_obj()->find_array($name);
  if (empty($ret)) {
    return false;
  }
  // else
  return true;
}

// 再帰の深さ(０ start)
public function get_recursive_depth() {
  return $this->get_bag('recursive_counter');
}

//
// abstract
public function initialize() { return true; }
abstract public function execute();
public function finalize() {}

//private:
private $my_name_;	// 自分自身のコマンドの名前

private $cfobj_;	// サイト固有のconfig。configクラスインスタンス
private $mobobj_;	// mobile_info インスタンス
private $reqobj_;	// CGI REQUEST インスタンス
private $resobj_;	// CGI RESPONSE インスタンス

private $body_;		// HTML body部分

private $recursive_flg_;	// 再帰用フラグ。trueで再帰する
private $recursive_command_;	// 再帰するときのモジュール名
private $direct_output_flg_;	// 直接出力フラグ

private $inside_encoding_obj_;  // 内部エンコード用 multibyteインスタンス

private $bag_;  // 持ち回り用かばんインスタンス：multimap

//private $tmp_filename_;
//private $error_tmp_filename_;

} // end of class

