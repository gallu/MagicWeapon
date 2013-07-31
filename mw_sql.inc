<?php

/**
 * 「１インスタンスが１SQL」なクラス
 *
 * 単純に「文字列によるSQL」を扱う場合と「準備された文(プリペアドステートメント)＋bindすべき値群」を扱う場合がある
 * なお、プリペアドステートメントは、:nameのみサポート。？はサポートしない。
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once('data_clump.inc');


class mw_sql {

public function __construct()
{
  //
  $this->init();
}

/**
 * 初期化
 *
 * @access public
 */
public function init()
{
  //
  $this->set_sql('');
  //
  $this->set_mode_prepared();
  //
  $this->datas_ = array();
}

/**
 * SQL(または準備されたSQL文)の設定
 *
 * @access public
 * @param string $v SQL文(または準備されたSQL文)
 */
public function set_sql($v) { $this->sql_ = $v; }
/**
 * SQL(または準備されたSQL文)の取得
 *
 * @access public
 * @return string SQL文(または準備されたSQL文)
 */
public function get_sql() { return $this->sql_; }

/**
 * モードの設定設定
 *
 * モードは、SQLの所に何が入っているかで、「direct：直接のSQL文」「prepared：準備された文」のどちらかである
 *
 * @access protected
 * @param string $v モード用の文字列(directまたはprepared)
 */
protected function set_mode($v) { $this->mode_ = $v; }
/**
 * モードの設定取得
 *
 * モードは、SQLの所に何が入っているか。set_mode参照
 *
 * @access proetcted
 * @return string モード用の文字列(directまたはprepared)
 */
protected function get_mode() { return $this->mode_; }

/**
 * モードの設定：SQL直接
 *
 * @access public
 */
public function set_mode_direct() { $this->set_mode('direct'); }
/**
 * モードの設定：準備された文
 *
 * @access public
 */
public function set_mode_prepared() { $this->set_mode('prepared'); }

/**
 * モードの確認：準備された文か？
 *
 * @access public
 * @return boolean モードが「準備された文」ならtrueを返す
 */
public function is_prepared() { return 'prepared' === $this->get_mode(); }

/**
 * データのバインド
 *
 * 「準備された文」モード時のみ意味がある
 *
 * @access public
 * @param string $name 「準備された文」で指定した名前。通常 ':name'だと思う。:も入れること。
 * @param string $value バインドしたい値
 * @param int $type データの型。定数はdata_clump::DATATYPE_* から１つを選択
 */
public function bind($name, $value, $type = data_clump::DATATYPE_STRING) {
  $wk = array();
  $wk['name'] = $name;
  $wk['value'] = $value;
  $wk['type'] = $type;
  //
  $this->datas_[$name] = $wk;
}

/**
 * バインドされたデータ一式を取得
 *
 * XXX publicだけど、普段使うことはないと思う
 *
 * @access public
 * @return array bnidされたデータ群。name=>名前、value=>bindされたデータ、type=>データ型(data_clump参照)
 */
public function get_bind_datas() { return $this->datas_; }


//private:
private $sql_;
private $mode_;
private $datas_;
} // end of class
