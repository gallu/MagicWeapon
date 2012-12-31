<?php
/**
 * DBデータクラス基本
 *
 * SELECTによって帰ってくるDBデータを取り扱います
 * abstractクラスなので、必ず派生させてください
 * 
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

abstract class db_data {
//public:

/**
 * SELECTの結果取得
 *
 * @access public
 * @return boolean falseならこれ以上のデータはなし
 */
abstract public function fetch();

/**
 * fetchしたデータの取得
 *
 * 引数で指定した数字(０スタート)の位置のデータを取得。
 * set_fetch_type_hashの場合、引数と合致する名前のデータを取得。
 *
 * @access public
 * @param int $pos 取得したいデータの位置、またはhash名
 * @return string データ
 */
abstract public function get_data($pos);


/**
 * fetchデータのタイプ指定：数字
 *
 * 結果を添字配列として取得し、０からの数値でpos指定する
 *
 * @access public
 */
public function set_fetch_type_num() { $this->set_fetch_type('num'); }

/**
 * fetchデータのタイプ指定：文字
 *
 * 結果を連想配列として取得し、文字列でpos指定する
 *
 * @access public
 */
public function set_fetch_type_hash() { $this->set_fetch_type('hash'); }

/**
 * fetchデータのタイプ指定
 *
 * 結果の取得方法の指定。
 *
 * @access protected
 * @param string $s タイプを意味する文字列
 */
protected function set_fetch_type($s) { $this->fetch_type_ = $s; }

/**
 * fetchデータのタイプ取得
 *
 * @access protected
 * @return string タイプを意味する文字列
 */
protected function get_fetch_type() { return $this->fetch_type_; }

/**
 * fetchデータのタイプ判定用関数：数値か？
 *
 * @access public
 * @return boolean fetchデータのタイプ指定が数値(添え字配列)ならtrue、そうでなければfalse
 */
public function is_fetch_type_num() { return $this->is_fetch_type('num'); }

/**
 * fetchデータのタイプ判定用関数：文字か？
 *
 * @access public
 * @return boolean fetchデータのタイプ指定が文字(連想配列)ならtrue、そうでなければfalse
 */
public function is_fetch_type_hash() { return $this->is_fetch_type('hash'); }


/**
 * fetchデータのタイプ判定用関数
 *
 * @access protected
 * @param string $s タイプを意味する文字列
 * @return boolean 一致したらtrue、一致しなければfalse
 */
protected function is_fetch_type($s) { return $s === $this->get_fetch_type(); }


/**
 * fetchしたデータの取得
 *
 * 現在指定されてる１行のデータを全部取得
 *
 * @access public
 * @return array データの配列
 */
abstract public function get_row();

/**
 * insert、updateなどでの「影響行」の行数取得
 *
 * @access public
 * @return int 影響行の行数
 */
public function affected_rows() {
  return $this->affected_rows_;
}

/**
 * insert、updateなどでの「影響行」の行数設定
 *
 * @access protected
 * @param int $i 影響行の行数
 */
protected function set_affected_rows($i) {
  $this->affected_rows_ = $i;
}


//private:
private $affected_rows_;
private $fetch_type_ = 'num';
}

