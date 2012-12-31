<?php

/**
 * KVS系ハンドル基底クラス
 *
 * なんか微妙に「interfaceでもよくね？」とか思ったりしてるのは内緒
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 */

require_once('data_handle.inc');

abstract class kvs_handle extends data_handle{

/**
 * データの設定
 *
 * @access public
 * @param string $key キー
 * @param string $val 値
 * @return boolean
 */
abstract public function set($key, $val);

/**
 * データの読み込み
 *
 * @access public
 * @param string $key キー
 * @return string 値。ない場合は空文字
 */
abstract public function get($key);


/**
 * データの削除
 *
 * @access public
 * @param string $key キー
 * @return boolean
 */
abstract public function del($key);



} // end of class
