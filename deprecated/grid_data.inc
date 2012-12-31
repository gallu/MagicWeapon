<?php
/**
 * 二次元データを扱うクラス
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

class grid_data {

	/*
	 * コンストラクタ
	 */
	public function __construct() { $this->init(); }

	/*
	 * init
	 */
	public function init() {
		$this->data_array_ = array();
		$this->max_line_num_ = -1;
		$this->fetch_init();
	}

	/*
	 * パース後データのセッタ
	 *   セットされたデータの要素数も保持
	 */
	public function set_data($d) {
		$this->data_array_   = $d;
		$this->max_line_num_ = count($this->data_array_);
	}

	/*
	 * パース後データのゲッタ
	 * XXX 可能なかぎり「使うな」！！
	 */
	public function get_data() { return $this->data_array_; }

	// -----------------------------------------------------------------------

	/*
	 * 指定行のデータを返す
	 *   存在しない行を指定された場合は、falseを返す
	 */
	// array function get_line($i)
	public function get_line($i = -1) {

		// 指定がなければfetchのコンソール行を返す
		if (-1 == $i) {
			$i = $this->get_pos_row();
		}
 		// 行データを返す
		if (isset($this->data_array_[$i])) {
			return $this->data_array_[$i];
		} else {
			return false;
		}
	}

	/*
	 * 指定カラムのデータを返す
	 *   存在しないカラムを指定された場合は、falseを返す
	 * XXX メソッド名にしっくりこない。
	 */
	// string function get_pos($row, $col)
	public function get_pos($row = -1, $col = -1) {

		// 指定がなければfetchのコンソール行を返す
		if (-1 == $row) {
			$row = $this->get_pos_row();
		}
		if (-1 == $col) {
			$col = $this->get_pos_col();
		}

		// カラムのデータを返す
		if (isset($this->data_array_[$row][$col])) {
			return $this->data_array_[$row][$col];
		} else {
			return false;
		}
	}

	/*
	 * 最大行数を返す
	 */
	// integer function get_max_row()
	public function get_max_row() { return $this->max_line_num_; }

	// --------------------------------------------------
	// Fetch系
	// --------------------------------------------------

	// fetch用 init メソッド群
	public function fetch_init() {
		$this->fetch_init_row();
		$this->fetch_init_col();
	}
	public function fetch_init_row() {
		$this->pos_row_ = -1;
	}
	public function fetch_init_col() {
		$this->pos_col_ = -1;
	}

	// fetch用 ポインタ操作メソッド群
	public function inc_pos_row() { $this->pos_row_++; }
	public function inc_pos_col() { $this->pos_col_++; }
	public function get_pos_row() { return $this->pos_row_; }
	public function get_pos_col() { return $this->pos_col_; }

	// fetch メイン
	/////////////////////
	// 行のfetch
	// boolean public function fetch_row();
	public function fetch_row() {

		$this->inc_pos_row();

		// XXX 行数分ループしたら終了
		if ($this->get_pos_row() > $this->get_max_row()) { 
			return false;
		}

		// 行を移動したら、列のポインタを初期化する
		$this->fetch_init_col();
		
		return $this->get_line();
	}

	/*
	 * 列のfetch
	 */
	// string public function fetch_col()
	public function fetch_col() {

		$this->inc_pos_col();

		// XXX
		if (!(isset($this->data_array_[$this->get_pos_row()][$this->get_pos_col()]))) { 
			return false;
		}
		return $this->data_array_[$this->get_pos_row()][$this->get_pos_col()];
	}

	/**
	 * データ
	 */
	private $data_array_;

	/**
	 * データの全行数
	 */
	private $max_line_num_;

	/**
	 * 行ポインタ
	 */
	private $pos_row_;
	
	/**
	 * 列ポインタ
	 */
	private $pos_col_;

} // end of grid_data class

/*
$test_array = array(
0=>array(0=>"あああ",1=>"いいい"),
1=>array(0=>"ううう",1=>"eeee")
);

$obj = new grid_data();
$obj->set_data($test_array);

while($obj->fetch_row()) {

	$col = array();

	while($aaa = $obj->fetch_col()) {

		echo "col:$aaa\n";

	}

}

echo "------------------\n";
$obj->fetch_init();
while ($line = $obj->fetch_row()) {
	print_r($line);
}

*/



