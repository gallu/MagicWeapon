<?php

/**
 * データハンドルクラス
 *
 * すべての「データウェアハウス系のハンドルクラス」の基底
 * 取り合えずとしては data_clump_adaptor系クラス用
 *
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  yyyy/mm/dd
 * @version $Id$
 **/

abstract class data_handle {
//public:

/**
 * このハンドルがRDB系かどうか
 *
 * @access public
 * @return boolean
 */
public function is_rdb() { return false; }

/**
 * このハンドルが「単体のRDBハンドル」系かどうか
 *
 * @access public
 * @return boolean
 */
public function is_mono_rdb() { return false; }

/**
 * このハンドルが「複数のRDBハンドルs」系かどうか
 *
 * @access public
 * @return boolean
 */
public function is_multi_rdb() { return false; }


/**
 * このハンドルがmemcache系かどうか
 *
 * @access public
 * @return boolean
 */
public function is_memcache() { return false; }

/**
 * このハンドルがAPC系かどうか
 *
 * @access public
 * @return boolean
 */
public function is_apc() { return false; }


}

