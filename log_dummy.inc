<?php

/**
 * ダミーログクラス
 *
 * 実際には何もしないクラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @version $Revision$
 **/

class log_dummy
{
//public :

/**
 * 初期化処理
 *
 * @access public
 */
public function init() {
}

public function on() { ; }
public function off() { ; }

public function spreadtime_on() { ; }
public function spreadtime_off() { ; }

public function set_pname($str) { ; }
public function set_filename($str) { ; }
public function set_rotate_filename($s) { ; }

public function add($str) { ; }

public function pause()  { ; }

public function flush() { ; }

}  // end of class

