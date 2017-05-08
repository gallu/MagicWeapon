<?php

/**
 * MagicWeapon用 オートローダーの登録
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

// XXX かなり念のために一端関数化。「PHP 5.3.0 以降」が前提に出来るようになったら無名関数化する
function magicweapon_spl_autoload_register($class) {
  //
  $fn = __DIR__ . DIRECTORY_SEPARATOR . $class . '.inc';
  //
  if ( (is_file($fn))&&(is_readable($fn)) ) {
    require_once($fn);
  }
}

// オートローダへの登録
spl_autoload_register('magicweapon_spl_autoload_register');

