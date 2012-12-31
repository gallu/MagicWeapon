<?php

/**
 * MVC Model部分認証系基底クラス
 *
 * session.incを基準にしたsessionクラス郡を使うことを前提にしたお便利class
 * auth系クラスの基底として使うことを前提にしている。
 * 設定ファイル適切に設定すれば、ほぼそのまま使えるとおもう。
 * 無理なら適宜作り直すなり継承するなりしてちょ。
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 */

require_once('base_model_rich.inc');

abstract class base_model_auth extends base_model_rich {

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
  $this->with_mobile_off();
}

//
abstract public function execute_auth();

} // end of class

