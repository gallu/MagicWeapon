<?php

/**
 * 単純な出力用model 認証型
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @deprecated
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 */


require_once('base_model_auth.inc');

class simple_put_model_auth extends base_model_auth {

//処理
public function execute_auth() {
  ///////////////////////////////
  // 出力処理
  ///////////////////////////////
  $this->make_body_with_conv();
}

} // end of class
