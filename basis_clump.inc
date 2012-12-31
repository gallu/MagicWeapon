<?php

/**
 * clump系最基底クラス
 *
 * 設定ファイルなどで動的に変化する「構造体クラス」
 * XXX struct clumpとかいう名前でもよかったのかなぁ？
 * multimapはとりあえず想定していないので、data、PHP的にはハッシュでよいかなぁ？
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

class basis_clump {

public function __construct()
{
  //
  $this->init();
}

/**
 * 初期化
 *
 * @access public
 */
public function init()
{
  // 要素を一式clear
  $this->init_data();
  $this->allname_ = array();
}

/**
 * 値のみを全クリアする
 *
 * @access public
 */
public function init_data()
{
  // 要素を一式clear
  $this->data_ = array();
}

//
public function get_all_names() { return $this->allname_; }


/**
 * setter
 *
 * 値を設定する。すでに存在するnameを指定した場合は上書き
 *
 * @access public
 * @param string $n 設定するデータの名前
 * @param string $v 設定するデータの値
 */
public function set_data($n, $v) { $this->data_[$n] = $v; }

/**
 * getter
 *
 * 値を取得する。設定されていない値の場合は空文字が返る。
 * 存在チェックはis_nameをしようすること。
 *
 * @access public
 * @param string $n 取得したいデータの名前
 * @return string 引数で指定したnameに対応する値。存在しない場合は空文字
 */
public function get_data($n)
{
  if (false === $this->is_name($n)) {
    return "";
  }
  if (false === isset($this->data_[$n]) ) {
    return "";
  }
  return $this->data_[$n];
}

/**
 * 存在チェック
 *
 * nameで指定された値が存在しているかどうかチェック
 * 実際には「設定ファイルで指定されていたかどうか」のチェック。
 *
 * @access public
 * @param string $n データの名前
 * @return boolean 存在してたらtrue、存在していなければfalse
 */
public function is_name($n)
{
  if(empty($this->allname_)) {
    return false;
  }
 return in_array($n, $this->allname_);
}

/**
 * データの設定
 *
 * 要素を一つ一つ積み上げるようにaddしていく
 * XXX このタイミングにおけるdataの中身は不定とする
 *
 * @access public
 * @param string $n 要素の名前
 * @return boolean すでに存在してたらfalse
 */
public function push_element($n)
{
  if (true === $this->is_name($n)) {
    return false;
  }
  $this->allname_[] = $n;
  return true;
}

//private:
private $data_;		// データ一式
private $allname_;	// 論理名一式全部

} // end of class

