<?php

/**
 * パスワードストレッチ
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

class authentication_password_stretching {

public function __construct()
{
  //
  //parent::__construct();
  $this->init();
}

// XXX 一端固定
protected function get_stretching_num_base() { return 1000; }
protected function get_hash_type() { return 'sha512'; }


//
public function init()
{
}

/**
 * ストレッチ
 *
 * @access public
 * @param string $id ID
 * @param string $raw_pass 生パスワード文字列
 * @return string ストレッチされたパスワード
 */
public function stretching($id, $raw_pass)
{
  // ストレッチ回数を決定
  $snum = ((int)(md5($id) % 0xff)) + $this->get_stretching_num_base();

  // のびのびと伸び
  $alog = $this->get_hash_type();
  $pass = $raw_pass;
  for($i = 0; $i < $snum; $i++) {
    $pass = hash($alog, $id . $pass);
  }
  //
  return $pass;
}


//private:


} // end of class

