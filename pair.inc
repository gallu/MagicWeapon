<?php

/**
 * ペアクラス
 *
 * ２つの要素を持つ単純なクラス
 * 概ね、各コンテナークラス用の下っ端
 *
 * @package magicweapon
 * @access  public
 * @author  Kohei Yamada <hecomark@gmail.com>
 **/

class pair
{
//public:
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
  unset($this->first_);
  unset($this->second_);
}

/**
 * 第一要素(普通はkey)の取得
 *
 * @access public
 * @return object key情報。一応「何でも入る」けど、stringを推奨
 */
public function get_first() { return $this->first_; }
/**
 * 第二要素(普通はvalue)の取得
 *
 * @access public
 * @return object value情報。
 */
public function get_second() { return $this->second_; }
/**
 * 第一要素(普通はkey)の設定
 *
 * @deprecated
 * @access public
 * @param object $x key情報。一応「何でも入る」けど、stringを推奨
 */
public function set_first($x) { $this->first_ = $x; }
/**
 * 第二要素(普通はvalue)の設定
 *
 * @deprecated
 * @access public
 * @param object $x value情報。
 */
public function set_second($x) { $this->second_ = $x; }

/**
 * ペア情報の設定
 *
 * @access public
 * @param object $fir key情報。一応「何でも入る」けど、stringを推奨
 * @param object $sec value情報。
 */
public function set($fir, $sec) { $this->set_first($fir); $this->set_second($sec); }

//private:
private $first_;
private $second_;

} // end of class

