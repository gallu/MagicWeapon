<?php
/**
 * 「SQLを作る」PostgreSQL用クラス
 *
 * SQL文(文字列)を作成する
 * XXX エスケープ系なので非推奨
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/


require_once('mw_makesql_escaped.inc');

class mw_makesql_escaped_postgresql extends mw_makesql_escaped
{


/**
 * 範囲指定(LIMIT句)用文字列の作成
 *
 * 形式としては「LIMIT {$to} OFFSET {$from}」というstringを作る
 *
 * @deprecated
 * @access public
 * @param int $from LIMITの第一引数
 * @param int $to LIMITの第二引数
 * @return string SQL文
 */
public function limited_range($from, $to)
{
  return $this->limited_range_postgresql($from, $to);
}


}
