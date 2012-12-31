<?php

/**
 * サイコロを振るクラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

class dice {

public function __construct()
{
  //
  //parent::__construct();
  $this->init();
}

//
public function init()
{
  $this->set("");
}

/**
 * ダイス用のstringセット
 *
 * TRPGでおなじみの、例えば 1d4+2d6+1d8+3 といった書式
 * XXX 変な文字列とかのチェックはしてないからね!!
 *
 * @access public
 * @param string $s ダイス文字列
 */
public function set($s) {
  // とりあえずカット
  $awk = explode('+', $s);

  // trimしつつDをdにしつつ格納
  $this->ds_ = array();
  if (false === empty($awk)) {
    foreach($awk as $wk) {
      $this->ds_[] = strtolower(trim($wk));
    }
  }
}

/**
 * サイコロを振る
 *
 * @access public
 * @return int サイコロを振った結果
 */
public function get()
{
  // 先にはじくもんはじく
  if (true === empty($this->ds_)) {
    return 0;
  }
  // さいころふるぞ〜
  $ret = 0;
  foreach($this->ds_ as $d) {
    // dが無いときの処理〜
    if (false === strstr($d, 'd')) {
      $ret += $d;
      continue;
    }
    // dがあるときはダイスふるにゅ〜
//print "$d \n";
    $wk = explode('d', $d);
    for($i = 0; $i < $wk[0]; $i ++) {
//print 'dice roll 1 to ' . $wk[1] . "\n";
      $ret += mt_rand(1, $wk[1]);
    }
  }

  return $ret;
}


//private:
private $ds_;

} // end of class

