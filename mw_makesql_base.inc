<?php
/**
 * 「SQLを作る」基底クラス
 *
 * SQL文(文字列)またはSQLオブジェクト(mw_sql：プリペアドステートメント用オブジェクト)を作成する
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

abstract class mw_makesql_base
{
/**
 * DBハンドルの設定
 *
 * @access public
 * @param db_handle $o db_handleクラス(からの派生クラス)のインスタンス
 */
public function set_db($o) { $this->dbh_ = $o; }

/**
 * DBハンドルの取得
 *
 * @access public
 * @return db_handle db_handleクラス(からの派生クラス)のインスタンス
 */
public function get_db() { return $this->dbh_; }



/**
 * limit句用文字列：MySQL用
 *
 * …あとで「trait」使えるようになったらもうちょっとマシな実装するんだ……
 *
 * @access public
 * @param int/string $from LIMITの第一引数
 * @param int/string $to LIMITの第二引数
 * @return string MySQL用
 */
public function limited_range_mysql($from, $to)
{
  // XXX ざっくりしたvalidate
  $from = abs( $from + 0 );
  $to = abs( $to + 0 );

  // 復帰
  return ' LIMIT ' . $from . ',' . $to;
}

/**
 * limit句用文字列：PostgreSQL用
 *
 * …あとで「trait」使えるようになったらもうちょっとマシな実装するんだ……
 *
 * @access public
 * @param int/string $from LIMITの第一引数
 * @param int/string $to LIMITの第二引数
 * @return string PostgreSQL用
 */
public function limited_range_postgresql($from, $to)
{
  // XXX ざっくりしたvalidate
  $from = abs( $from + 0 );
  $to = abs( $to + 0 );

  return ' LIMIT ' . $to . ' OFFSET ' . $from;
}

//private:
private $dbh_ = NULL;

} // end of class
