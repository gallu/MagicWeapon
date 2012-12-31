<?php

/**
 * configにまつわるユーティリティ
 *
 * util系なので基本的にstaticなメソッドのみ
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

require_once('load_manager.inc');

class config_util {

/**
 * config書式からインスタンス生成
 *
 * includeファイル名:クラス名 の書式から、includeしてnewしてまでを一気
 *
 * @access static public
 * @param string $s "includeファイル名:クラス名"書式の文字列
 * @return object 失敗した場合null
 */
static public function create_instance($s)
{
  // クラス名とincludeファイル名に切り分ける
  $awk = preg_split("/:/", $s);
  $include_filename = trim($awk[0]);
  $classname = trim($awk[1]);
//print "include is " . $include_filename . "<BR>\n";
//print "class is " . $classname . "<BR>\n";

  // include処理
  if (false === load_manager::load_file($include_filename)) {
    return null;
  }

  // class存在チェック
  // XXX 一応ね
  if (false === class_exists($classname, false) ) {
    return null;
  }

  // オブジェクト作成
  return new $classname;
}

} // end of class

