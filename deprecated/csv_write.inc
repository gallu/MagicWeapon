<?php

/**
 * CSV書き出しクラス
 *
 * セットされたデータをCSV文字列として書き出す。
 *
 * @deprecated
 * @package magicweapon
 * @access  public
 * @author  Kohei Yamada <hecomark@gmail.com>
 **/

/*
// 使い方

// CSVとして吐き出すデータは、(とりあえず)１行１配列で作る
$row1 = array("column1_1", "column1_2", "column1_3");
$row2 = array("column2_1", "column2_2", "column2_3");
$row3 = array("column3_1", "column3_2", "column3_3");

// writeインスタンス作成
$writer = new csv_write;

// データをpushしていく
$writer->push_row($row1);
$writer->push_row($row2);
$writer->push_row($row3);

// セパレータと改行文字を変更する場合は使用
// デフォルトでは、セパレータ「,」、改行文字「\r\n」
//$writer->set_separator("\t");
//$writer->set_crlf("\n");

// csv_str == CSV形式の文字列
$csv_str = $writer->get_string();

*/

require_once("csv_row.inc");

class csv_write
{
//public:

public function __construct() { $this->init(); }
public function init() {
	$this->rows_ = array();
	$this->sep_ = "";
	$this->crlf_ = "";

	// 区切り文字はカンマ
	// 改行は"\r\n"で初期化
	$this->set_separator(",");
	$this->set_crlf("\r\n");
}

/**
 * １行分のデータをpushする
 *
 * @param array $row
 * @return void
 * @access public
 */
public function push_row(array $row) {
	$csv_row = new csv_row;
	foreach ($row as $column) {
		$csv_row->push($column);
	}
	$this->rows_[] = $csv_row;
}

/**
 * カラムのセパレータをセットする
 *
 * @param string $sep
 * @return void
 * @access public
 */
public function set_separator($sep) {
	$this->sep_ = $sep;
}

/**
 * 改行コードを指定する
 *
 * @param string $crlf
 * @return void
 * @access public
 */
public function set_crlf($crlf) {
	$this->crlf_ = $crlf;
}

/**
 * セットされているデータからCSV文字列を作成し返す
 *
 * @param void
 * @return string 
 * @access public
 */
public function get_string() {
	$str = "";
	foreach ($this->get_rows() as $row) {
		$row->set_separator($this->get_separator());
		$str .= $row->get_string() . $this->get_crlf();
	}
	return $str;
}

//protected:
protected function get_rows() { return $this->rows_; }
protected function get_separator() { return $this->sep_; }
protected function get_crlf() { return $this->crlf_; }

//private:

private $rows_;
private $sep_;
private $crlf_;


} // end_of_class




