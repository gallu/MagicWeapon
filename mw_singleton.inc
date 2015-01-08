<?php

/**
 * シングルトン基底クラス
 *
 * 最低限程度、の、シングルトン用基底クラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

class mw_singleton {

/**
 * なにはともあれ、外部からのnew禁止！
 *
 * @access protected
 */
protected function __construct() {
}

/**
 * インスタンス取得口
 *
 * @access public
 * @return object mw_singleton又はその継承クラスのインスタンス
 */
static public function get_instance() {
  static $obj = null;
  if (null === $obj) {
    $obj = new static; // ここをselfにすると大変！！ ただPHP5.3.0以降なんで微妙に注意！
  }
  //
  return $obj;
}



} // end of class


