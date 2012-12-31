<?php
/**
 * error
 *
 * エラークラス
 * 
 * エラーの基底クラス
 * 
 *
 * @deprecated
 * @package magicweapon
 * @author  Yuuki Ogura <hiruandon2nd@gmail.com>
 * @create   2007/06/26
 * @updater  Yuuki Ogura <hiruandon2nd@gmail.com>
 * @update   2007/06/26
 * @version 1.0
 */
class error
{
	/**
	 * コンストラクタ
	 * 
	 * @access public
	 * @param  void
	 * @return void
	 */
	public function __construct() {
		$this->init();
	}

	/**
	 * 初期化
	 * 
	 * @access public
	 * @param  void
	 * @return void
	 */
	public function init() {
		
	}


	/**
	 * エラーコードを設定する
	 *
	 * @access public
	 * @param  integer $i エラーコード
	 * @return void
	 */
	public function set_error_code($i) {
		$this->error_code_ = $i;
	}

	/**
	 * エラーストリングを設定する
	 *
	 * @access public
	 * @param  string $s エラーストリング
	 * @return void
	 */
	public function set_error_string($s) {
		$this->error_string_ = $s;
	}


	/**
	 * エラーコードを返す
	 *
	 * @access public
	 * @param  void
	 * @return integer エラーコード
	 */
	public function get_error_code() {
		return $this->error_code_;
	}

	/**
	 * エラーストリングを返す
	 * 
	 * XXX エラーコードから文字列を返すほうがいいと思う
	 *
	 * @access public
	 * @param  void
	 * @return string エラーストリング
	 */
	public function get_error_string() {
		return $this->error_string_;
	}




	/**
	 * エラーコード格納
	 * 
	 * @access private
	 * @var    integer
	 */
	private $error_code_;

	/**
	 * エラーストリング格納
	 * 
	 * @access private
	 * @var    string
	 */
	private $error_string_;


}	// end of class


