<?php

/**
 * APC用のハンドルクラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 */

require_once('data_handle.inc');

class apc_handle extends data_handle{

/**
 * このハンドルがAPC系かどうか
 *
 * @access public
 * @return boolean
 */
public function is_apc() { return true; }


/**
 * データの設定
 *
 * @access public
 * @param string $key キー
 * @param string $val 値
 * @return boolean
 */
public function set($key, $val)
{
  return apc_store($key, $val);
}

/**
 * データの読み込み
 *
 * @access public
 * @param string $key キー
 * @return string 値。ない場合は空文字
 */
public function get($key)
{
  // エラーハンドリングだけ丁寧に
  $ret = apc_fetch($key, $state);
  if (false === $state) {
    return '';
  }
  // else
  return $ret;
}


/**
 * データの削除
 *
 * @access public
 * @param string $key キー
 * @return boolean
 */
public function del($key)
{
  return apc_delete($key);
}



} // end of class
