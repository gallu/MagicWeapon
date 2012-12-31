<?php

/**
 * 絵文字取得用基底クラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

class emoji_base {

public function __construct()
{
  //
  //parent::__construct();
  $this->init();
}

//
public function init()
{
  $this->emoji_ = array();
}

//
protected function set_emoji(array $a) { $this->emoji_ = $a; }
//protected function get_emoji() { return $this->emoji_; }

/**
 * 絵文字の取得
 *
 * [e]{0,1}[0-9]{1,3} の文字列を必要な文字列に変換
 *
 * @access public
 * @param string $s [e]{0,1}[0-9]{1,3} の文字列
 * @return string 変換後の絵文字。対応がない場合は空文字
 */
public function encode($s)
{
  // XXX 配列だしねぇ参照にする？
  if (true === isset($this->emoji_[$s])) {
    return $this->emoji_[$s];
  }
  // else
  return "";
}

// XXX
//abstract public function decode($s);


//private:
private $emoji_;

} // end of class

