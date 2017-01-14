<?php
/**
 *
 * multimapクラス
 *
 * キーの重複を許すmap(ようはハッシュ配列だ)
 * 2005/2/24 イテレータは単に配列の添字なので、
 *           中身を参照する場合は$ob->real($itr)のように
 *           使用してください。
 *           要素は基本的にPairオブジェクトなので、
 *           $key = $ob->real($itr)->get_first();
 *           $value = $ob->real($itr)->get_second();
 *           のように参照します。
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Kohei Yamada <hecomark@gmail.com>
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/
require_once("pair.inc");

class multimap
{

public function __construct()
{
  $this->init();
}


/**
 * 初期化
 *
 * @access public
 */
public function init()
{
  $this->data_ = array();
}

/**
 * 最初の要素を指すイテレータを返す
 *
 * @access public
 * @return int イテレータの値
 */
public function begin()
{
  // 今のところは配列の添字を返す
  // いちおうイテレータ扱い
  return 0;
}

/**
 * 最後の要素の次を指すイテレータを返す
 * 
 * 最後の要素の「次」である点に注意
 *
 * @access public
 * @return int イテレータの値
 */
public function end()
{
  return $this->size();
}

/**
 * 先頭の要素を指す逆イテレータを返す。
 *
 * @access public
 * @return int イテレータの値
 */
public function rbegin()
{
  // 配列の添字になるので+1
  return abs(-($this->size()) + 1);
}

/**
 * 最後の要素を指すイテレータを返す
 *
 * @access public
 * @return int イテレータの値
 */
public function rend()
{
  // 最後の次の要素を指すように+1
  return 0 + 1;
}

/**
 * $key, $valueを表すPairオブジェクトを追加する
 *
 * 追加されるのは最後尾
 *
 * @access public
 * @param string $key キー
 * @param string $value 値
 * @return int 挿入された位置を示すイテレータを返す
 */
public function insert($key, $value)
{
  // とりあえず最低限の動き

  $obj = new pair();
  $obj->set($key, $value);
  $this->data_[] = $obj;

  // 常に最後尾に入るはずなので
  return $this->size() - 1;  
}

/**
 * $keyをキーに持つ最初の要素を探して、その要素を指すイテレータを返す。
 *
 * iterator自体は数値…ってのはPHP時の制約
 * キーが見つからない場合は-1
 *
 * @access public
 * @param string $key キー
 * @return int イテレータを返す。複数見つかった場合はどれが返るか未定。見つからない場合は-1
 */
public function find($key)
{
  $ret = $this->find_array($key);

  // 微妙ご法度な関数だが、配列限定なので
  if (empty($ret)) {
    return -1; 
  }
  //
  return $ret[0];
}

/**
 * $keyをキーに持つ最初の要素を探して、
 * その要素を指すイテレータを返す。
 * キーが見つからない場合は空配列を返す
 *
 * @access public
 * @param string $key キー
 * @return array イテレータの配列を返す。見つからない場合は空配列
 */
public function find_array($key)
{
  $ret = array();
  //
  for($itr = $this->begin(); $itr != $this->end(); $itr++){
    if(strcmp($this->real($itr)->get_first(), $key)==0){
      $ret[] = $itr;
    }
  }

  //
  return $ret; 
}

/**
 * multimapの要素が空だとtrue
 *
 * @access public
 * @return boolean 中身が空ならtrue、中身があればfalse
 */
public function isempty()
{
  return empty($this->data_); 
}

/**
 * 要素の数を返す
 *
 * @access public
 * @return int 要素の数
 */
public function size()
{
  return count($this->data_);
}

/**
 * firstの要素を削除する。あるいは、firstからlastまでの要素を削除する
 *
 * @access public
 * @param int $first 削除する要素の最初を指すイテレータ
 * @param int $last 削除する要素の最後の次を指すイテレータ
 * @return int 削除した要素の次を指すイテレータ
 */
public function erase($first, $last = false)
{
  if(false !== $last){
    // last指定があれば、first,lastの間の要素を削除
    array_splice($this->data_, $first, $last-$first);
  }else{
    array_splice($this->data_, $first, 1);
  }
  
  return $first;
}

/**
 * 全ての要素を削除
 *
 * @access public
 */
public function clear()
{
  $this->data_ = array();
}

/**
 * イテレータの指す要素のオブジェクトを返す（ポインタもどき）
 *
 * @access public
 * @param int $itr 取得したい要素のイテレータ
 * @return string valueの値。存在しないイテレータの場合は空文字を返す
 */
public function real($itr)
{
  if($itr < 0){
    return "";
  }
  if (false === $this->data_[$itr]) {
    return "";
  }

  return $this->data_[$itr];
}

/*
 * 要素を足しこむ
 *
 * 足すというか、merge
 *
 * @access public
 * @param multimap $o マージしたいmultimapオブジェクト
 * @return boolean 現状つねにtrue
 */
public function add(multimap $o)
{
  for($itr = $o->begin(); $itr != $o->end(); $itr ++) {
    $this->insert($o->real($itr)->get_first(), $o->real($itr)->get_second());
  }
  return true;
}

/**
 * キー一覧取得
 *
 * キー名の一覧なので、重複しているキー名は１つにまとめる
 *
 * @access public
 * @return array キー名の一覧。形式はvector<string>
 */
public function get_all_keys()
{
  // とりあえず雑にげと
  $names = array();
  for($itr = $this->begin(); $itr != $this->end(); $itr ++) {
    // 文字列合成
    $names[$this->real($itr)->get_first()] = 1;
  }

  $keys = array_keys($names);
  return $keys;
}

/**
 * 指定されたkeyを持つ要素を削除する。
 *
 * keyが空文字の場合「なにもしない」
 * 全削除したいならclearメソッドつかって。
 *
 * @access public
 * @param string $key 削除したいkey文字列
 * @return int 削除した要素数
 */
public function erase_key($key)
{
  //
  if ("" === $key) {
    return 0;
  }

  // 対象を検索 & 削除
  $ret = 0;
  while(-1 !== ($itr = $this->find($key))) {
    // 削除対象数取得
    $ret ++;
    $this->erase($itr);
  }
//var_dump($this->data_);

  //
  return $ret;
}


//
private $data_;
} // end of class


