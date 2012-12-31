<?php

/**
 * data_clumpとデータ系ハンドルとを紐付けるクラス
 *
 * イメージがもろにCRUDなので、メソッド名もそのまんま?
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 */

abstract class data_clump_adaptor_base
{
/**
 * データハンドルの設定
 *
 * @access public
 * @param object $dh data_handleインスタンス
 * @return boolean
 */
public function set_data_handle($dh) { $this->dh_ = $dh; }
/**
 * データハンドルの取得
 *
 * @access protected
 * @return object data_handleインスタンス
 */
protected function get_data_handle() { return $this->dh_; }

/**
 * create用処理
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean
 */
abstract public function create($clump);


/**
 * pkによるread
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean
 */
abstract public function read_pk($clump);


/**
 * pkによらないread
 *
 * XXX RDB以外だとちょっと厳しいかも?
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean
 */
abstract public function read_nopk($clump);


/**
 * 全体update用処理
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean
 */
abstract public function update_all($clump);


/**
 * 部分的なupdate用処理
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean
 */
abstract public function update_mono($clump);

/**
 * 更新処理
 *
 * insertまたはupdate
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean
 */
abstract public function set($clump);


/**
 * 削除用処理
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean
 */
abstract public function delete($clump);

/**
 * 数値の加減算
 *
 * XXX 設定したらすぐにDB等に書きに行くので留意
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean 成功したらtrue、なにか失敗があったらfalse
 */
abstract public function calculation($clump);

//
private $dbh_;

} // end of class
