<?php
/**
 * dbにまつわるユーティリティ
 *
 * おおむね「シンプルなSQLでざっくりデータがほしい」時用
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

class db_util {

/**
 * データ一式取得
 *
 * SQLで指定されたデータを一式取得
 * データは配列の配列になる
 *
 * @access public
 * @param db_handle $dbh db_handleクラス(からの派生クラス)のインスタンス
 * @param string $sql SQL文
 * @return array 正確には「array(array))」。情報がない場合空配列
 */
static public function get_row_data_array($dbh, $sql)
{
  $ret = array();
  $res = $dbh->query($sql);
  if (empty($res)) {
    return $ret;
  }
  while($res->fetch()) {
    $ret[] = $res->get_row();
  }
  //
  return $ret;
}

/**
 * データ一行
 *
 * SQLで指定されたデータを一行分取得
 * データは配列になる
 *
 * @access public
 * @param db_handle $dbh db_handleクラス(からの派生クラス)のインスタンス
 * @param string $sql SQL文
 * @return array 1行分を配列で。情報がない場合空配列
 */
static public function get_row_data($dbh, $sql)
{
  $ret = array();
  $res = $dbh->query($sql);
  if (empty($res)) {
    return $ret;
  }
  if($res->fetch()) {
    $ret = $res->get_row();
  }
  //
  return $ret;
}

/**
 * データ取得
 *
 * SQLで指定されたデータを取得
 *
 * @access public
 * @param db_handle $dbh db_handleクラス(からの派生クラス)のインスタンス
 * @param string $sql SQL文
 * @return string 取得したデータ。ない場合は空文字列
 */
static public function get_mono_data($dbh, $sql)
{
  $ret = "";
  $res = $dbh->query($sql);
  if (empty($res)) {
    return $ret;
  }
  if($res->fetch()) {
    $ret = $res->get_data(0);
  }
  //
  return $ret;
}


} // end of class

