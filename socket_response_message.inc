<?php

/**
 * ソケットに関する応答メッセージを扱うクラス
 *
 * リターンコードとメッセージ
 *
 * @package magicweapon
 * @access  public
 * @author  Yuuki Ogura <hiruandon2nd@gmail.com>
 * @create  2006/10/13
 **/
class socket_response_message {

	/*
	 * コンストラクタ
	 */
	public function __construct() { $this->init(); }

	/*
	 * 初期化
	 */
	public function init() {}

	/*
	 * ソケットからの応答メッセージをセットして、
	 * 応答コードとメッセージ部分に分割。
	 */
	public function set_string($s) {
		$this->response_str_ = $s;
		$this->split_string();
	}
	public function get_string()   { return $this->response_str_; }

	/*
	 * 応答メッセージを、応答コードとメッセージ部分に分割してセット
	 * XXX 応答コードのゲッター：get_code()
	 * XXX 応答メッセージのゲッター：get_message()
	 */
	private function split_string() {
		$this->code_    = substr($this->get_string(), 0,3);
		$this->message_ = substr($this->get_string(), 3);
	}
	public function get_code()    { return $this->code_; }
	public function get_message() { return $this->message_; }

	/*
	 * 応答メッセージがエラーなのか判定
	 */
	public function is_error() {

		$wk = $this->get_code();
		$wk = $wk . "";
		// 先頭の数字だけ見る
		switch ($wk[0]) {
			case 2:
			case 3:
				$bol = false;
				break;
			case 4:
			case 5:
				$bol = true;
				break;
			default:
				$bol = true;
		}

//print "get_code:" . $this->get_code() . "\n";
//print "get_message:" . $this->get_message() . "\n";

		return $bol;
		
	}

	private $response_str_;	// ソケットからの応答メッセージ
	private $code_;					// 応答コード
	private $message_;				// 応答メッセージ

}

