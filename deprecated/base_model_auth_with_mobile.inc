<?php

/**
 * MVC Model部分認証系基底クラス
 *
 * 携帯公式サイト対応
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 */

require_once('base_model_rich.inc');

abstract class base_model_auth_with_mobile extends base_model_rich {

//public:
public function __construct()
{
  //
  parent::__construct();
  //$this->init();
}


//初期化
public function init(){
  parent::init();
  $this->set_authtype_auth();
  $this->with_mobile_on();
}

abstract public function execute_auth();

} // end of class

