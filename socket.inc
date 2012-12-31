<?php

require_once("socket_response_message.inc");		// ソケットに関する応答メッセージを扱う

/**
 * ソケットを扱うクラス
 *
 * XXX エラーハンドル系は後ほど実装。とりあえず動くものを。
 *
 * @package magicweapon
 * @access  public
 * @author  Yuuki Ogura <hiruandon2nd@gmail.com>
 * @create  2006/10/13
 * @version $Id$
 **/
class socket {

	/*
	 * コンストラクタ
	 */
	public function __construct() { $this->init(); }

	/*
	 * 初期化
	 */
	public function init() {
		// XXX デフォルトプロトコルはTCP
		$this->set_tcp();
		// socket_create()で指定するソケットタイプをセット
		$this->set_socket_type();
		// コマンド発行する上での改行コードをセット
		$this->set_crlf();
	}

	/*
	 * TCPプロトコル使用を明示
	 * 
	 * XXX ゲッターはget_protocol()
	 */
	public function set_tcp() { $this->protocol_ = "tcp"; }

	/*
	 * UDPプロトコル使用を明示
	 * 
	 * XXX ゲッターはget_protocol()
	 */
	public function set_udp() { $this->protocol_ = "udp"; }

	/*
	 * プロトコルゲッター
	 */
	public function get_protocol() { return $this->protocol_; }

	/*
	 * プロトコル番号を取得 
	 */
	public function get_protocol_num() { return getprotobyname($this->get_protocol()); }

	/*
	 * socket_createで使うソケットタイプのセッター
	 */
	public function set_socket_type() {

		switch ($this->get_protocol()) {
			case "tcp":
				$this->socket_type_ = SOCK_STREAM;
				break;
			case "udp":
				$this->socket_type_ = SOCK_DGRAM;
				break;
			default:
				$this->socket_type_ = SOCK_STREAM;
		}

	}

	/*
	 * socket_createで使うソケットタイプのゲッター
	 */
	public function get_socket_type() { return $this->socket_type_; }

	/*
	 * 接続先のポート番号をセット
	 */
	public function set_to_port($port) { $this->port_to_ = $port; }

	/*
	 * 接続先のポート番号をゲット
	 */
	public function get_to_port() { return $this->port_to_; }

	/*
	 * ソケット接続するサービス
	 */
	public function set_service($service) {		
		// サービス名からポート番号を取得
		$this->port_to_ = getservbyname($service, $this->get_protocol());
	}

	/*
	 * ソケット接続先のIPを、IPアドレス直指定またはドメイン名から正引きでセット
	 * 
	 * 
	 * @param string ip xxx.xxx.xxx.xxx or domain name 
	 */
	public function set_to_ip_addr($ip) {

		$this->to_ip_ = gethostbyname($ip);
//echo "ip:" . $this->to_ip_ . "\n";
	}

	/*
	 * ソケット接続先のIPをゲット
	 */
	 public function get_to_ip_addr() { return $this->to_ip_; }
	
	/*
	 * ソケット接続
	 */
	public function connect() {

// echo "get_socket_type:" . $this->get_socket_type() . "\n";
// echo "get_protocol_num:" . $this->get_protocol_num() . "\n";
// echo "get_to_ip_addr:" . $this->get_to_ip_addr() . "\n";
// echo "get_to_port:" . $this->get_to_port() . "\n";

		$this->_create();
		$this->_connect();

//var_dump($this->socket_);

	}

	/*
	 * ソケット（通信時の終端）を作成する
	 * XXX ソケットのゲッターは、get_socket()
	 */
	private function _create() {

		$this->socket_ = socket_create(AF_INET, $this->get_socket_type(), $this->get_protocol_num());

		if ($this->socket_) {
			return true;
		} else {
// XXX エラーハンドルどうする？
			echo "socket_create(): " . socket_strerror($this->socket_) . "\n";
			return false;
		}
	}
	private function get_socket() { return $this->socket_; }

	/*
	 * ソケット上の接続を初期化する
	 */
	private function _connect() {

		if (socket_connect($this->get_socket(), $this->get_to_ip_addr(), $this->get_to_port())) {
			return true;
		} else {
// XXX エラーハンドルどうする？
			echo "socket_connect(): " . socket_strerror(socket_last_error()) . "\n";
			return false;
		}
	}

	/*
	 * ソケット切断
	 */
	public function disconnect() { socket_close($this->get_socket()); }

	/*
	 * ソケットデータ書き込み
	 */
	public function write($data) {
		socket_write($this->get_socket(), $data);
	}

	/*
	 * ソケットデータ読み込み(writeのレスポンス)
	 * XXX $readLengthの初期値は無根拠
	 */
	public function read($readLength=2048) {
		// readLength自体は1パケット分(単なるバッファ)と仮定して、whileで回すよう修正
		$ret_buf = "";
		while( $buf = socket_read($this->get_socket(), $readLength, PHP_BINARY_READ) ) {
			$ret_buf .= $buf;
		}
		return $ret_buf;
	}

	/*
	 * ソケットデータ書き込み＆読み込み
	 * XXX $readLengthの初期値は無根拠
	 */
	public function command($data, $readLength=2048) {

		$this->write($data . $this->get_crlf());	// ソケットデータ書き込み

//print "soc class command is " . $data . "\n";

		$str = $this->read($readLength);					// ソケットからwriteに対する応答を取得
		
		// 応答メッセージを扱うクラス
		$res = new socket_response_message();
		$res->set_string($str);

		// 応答メッセージ的にエラーだったら例外を投げる
		if (true == $res->is_error()) {
			throw new Exception($data  . " : " . $res->get_message());
		}

		// XXX 本来は何を復帰？
		return $res;

	}

	public function set_crlf($d="\r\n") { $this->crlf_ = $d; } 
	private function get_crlf() { return $this->crlf_; }


	private $socket_;			// ソケットリソース
	private $socket_type_;	// ソケットタイプ(SOCK_STREAM,SOCK_DGRAM)
	private $protocol_;		// プロトコル 
	private $to_ip_;				// 接続先IP
	private $service_;			// サービス名(http,ssl,ftp etc..)
	private $port_to_;			// 接続先のポート番号

	private $crlf_;				// コマンド発行する上での改行コード

}

/*
// create
$sock_obj_ = new socket();

// 接続情報の設定

// XXX お作法的に
$sock_obj_->set_service("smtp");

// XXX 通常手順はここから？
//$sock_obj_->set_to_ip_addr("192.168.70.32");
$sock_obj_->set_to_ip_addr("localhost.localdomain");
$sock_obj_->set_to_port(25);

$sock_obj_->connect();

//$sock_obj_->write("HELO");
//echo $sock_obj_->read();

$sock_obj_->write("HELO localhost.localdomain\r\n");
echo $sock_obj_->read();

$sock_obj_->write("MAIL FROM:<y_ogura@hoge.co.jp>\r\n");
echo $sock_obj_->read();

$sock_obj_->write("RCPT TO:<y_ogura@hoge.co.jp>\r\n");
$sock_obj_->write("RCPT TO:<m_takahashi@hoge.co.jp>\r\n");
echo $sock_obj_->read();

$sock_obj_->write("DATA\r\n");
echo $sock_obj_->read();

$sock_obj_->write("from: y_ogura@hoge.co.jp\r\n");
$sock_obj_->write("to: m_takahashi@hoge.co.jp\r\n");
$sock_obj_->write("cc: y_ogura@hoge.co.jp\r\n");
$sock_obj_->write("subject: test\r\n");
$sock_obj_->write("\r\n");
$sock_obj_->write("this is socket send mail test\r\n");
$sock_obj_->write(".\r\n");
$sock_obj_->write("QUIT\r\n");
echo $sock_obj_->read();


$sock_obj_->disconnect();
*/
