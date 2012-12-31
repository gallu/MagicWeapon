<?php

/**
 * セキュアなCONVクラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

require_once('conv.inc');
require_once('security.inc');

class secure_conv extends f_conv {

public function __construct()
{
  // XXX 親クラスにinitがないからとりあえず消しとく
  parent::__construct();
  //$this->init();
}

//
public function init()
{
  parent::init();
}

/**
 * セキュアな単置換登録
 *
 * @access public
 * @param string $name 名称
 * @param string $value 値
 * @return boolean 何か問題があればfalse
 */
public function monoDic($name, $value) 
{
  //
  return parent::monoDic($name, security::sanitize_html($value));
}

/**
 * サニタイズしたくないとき用の単置換設定
 *
 * @access public
 * @param string $name 名称
 * @param string $value 値
 * @return boolean 何か問題があればfalse
 */
public function monoDic_unsecure_raw($name, $value)
{
  //
  return parent::monoDic($name, $value);
}



//private:


} // end of class

