<?php
/**
 * CSVを扱うクラス群
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  y_ogura
 **/
require_once("grid_data.inc"); 
require_once("file_util.inc"); 

class csv extends grid_data {

	/*
	 * CSVパスのセッター
	 */
	public function set_path($path="") {

		if (0 < strlen($path)) {
			$this->path_ = $path;
		} else {
			echo "class name is:" . get_class($this). ":file path is null!!\n";
			exit;
		}

		// パースを実行
		$this->parse();
	}
	
	/*
	 * CSVパスのゲッター
	 */
	public function get_path() {

		// ファイルパス設定してくんないと何にもできへん
		if (0 < strlen($this->path_)) {
			return $this->path_;
		} else {
			echo "class name is:" . get_class($this). ":file path is null!! please call method set_path(path)\n";
			exit;
		} 

	}
	
	/*
	 * コンストラクタ
	 */
	public function __construct() {
		$this->init();
	}

	/*
	 * init
	 */
	public function init() {
		// 親を叩く
		parent::init();
	}

	/*
	 * メイン処理
	 * 　1.CSVファイル読み込み
	 *   2.パース実行
	 */
	private function parse() {
		
		// CSVファイル読み込み
		// XXX とりあえずUTF8。あとでスイッチング
		$data = file_util::read_file_to_utf8($this->get_path());

		// CSVパーサ生成
		$obj_csv_p = new csv_parse();
		
		// CSVデータセット
		$obj_csv_p->set_data($data);
		
		// CSVパース実行！！
		$obj_csv_p->parse();
		
		// grid_dataクラスに放る
		$this->set_data($obj_csv_p->get_parsed_data());
	}

	/**
	 * CSVパス
	 */
	private $path_;

} // end of csv class

/**
 * CSVデータのパース委任クラス
 */
class csv_parse {

	/*
	 * パース前データのセッタ
	 */
	public function set_data($d) { $this->data_string_ = $d; }

	/*
	 * パース前データのゲッタ
	 */
	private function get_data() { return $this->data_string_; }

	/*
	 * パース後データのセッタ
	 */
	public function set_parsed_data($d) { $this->data_array_ = $d; } 

	/*
	 * パース後データのゲッタ
	 */
	public function get_parsed_data() { return $this->data_array_; }

	/*
	 * パース
	 */
	public function parse() {

		// CSVワークオブジェクト生成&データセット
		// 以降はCSVワークオブジェクトに実処理を委任
		// XXX set_data()でパースまで実行するようにした
		// XXX set_data()とparse()は分けた方が良い？
		$csv_wk = new csv_p_work();
		$csv_wk->set_data($this->get_data());

		// パース後データを自分に保持
		// get_parsed_data() でパース後データを取得する為に。
		$this->set_parsed_data($csv_wk->get_parsed_data());
	}

	/**
	 * パース前データ
	 */
	private $data_string_;

	/**
	 * パース後データ
	 */
	private $data_array_ = array();

} // end of csv_parse class


/**
 * CSVデータのワーククラス
 *   状態(モード)によって、パースオブジェクトをコール
 *   csv_pはcsv_parseの略
 */
class csv_p_work {

	/*
	 * コンストラクタ
	 */
	public function __construct() {
		$this->init();
	}

	/*
	 * 初期
	 */
	public function init() {
		// 状態遷移オブジェクト生成
		$this->objs_["set_normal"]  = new csv_p_set_normal();		// 通常モード
		$this->objs_["set_crlf"]    = new csv_p_set_crlf();			// 改行モード
		$this->objs_["set_esp"]     = new csv_p_set_esp();			// エスケープモード
		$this->objs_["set_esp_chk"] = new csv_p_set_esp_chk();	// エスケープチェックモード
		
		// 初期モードは通常モードをセット
		$this->set_normal_mode();
		
		// XXX ファイル末尾の改行対策
		$this->empty_flg_ = true;
	}

  /*
   * データがあるかどうか判断
   * XXX emptyメソッドなので「なければtrue」
   */
	public function is_empty() { return $this->empty_flg_; }

	/*
	 * 状態(モード)のセッタ
	 */
	public function set_mode($s) { $this->mode_ = $s; }

	/*
	 * 状態(モード)のゲッタ
	 */
	public function get_mode() { return $this->mode_; }

	/*
	 * 通常モードのセッタ
	 */
	public function set_normal_mode() { $this->set_mode("set_normal"); }

	/*
	 * 改行モードのセッタ
	 */
	public function set_crlf_mode() { $this->set_mode("set_crlf"); }

	/*
	 * エスケープモードのセッタ
	 */
	public function set_esp_mode() { $this->set_mode("set_esp"); }

	/*
	 * エスケープチェックモードのセッタ
	 */
	public function set_esp_chk_mode() { $this->set_mode("set_esp_chk"); }

	/*
	 * パース前データのセッタ
	 */
	public function set_data($d) {
		$this->data_string_ = $d;
		$this->set_length();		// データ長を保持
		$this->parse();					// パース実行
	}

	/*
	 * パース前データのゲッタ
	 */
	public function get_data($p="") {
		if (0 < strlen($p)) {
			return $this->data_string_[$p];
		} else {
			return $this->data_string_;
		}
	}

	/*
	 * パース後データのゲッタ
	 */
	public function get_parsed_data() { return $this->data_array_; }

	/*
	 * 状態遷移オブジェクトのインスタンスを返す
	 */
	public function get_parse_obj() {	return $this->objs_[$this->get_mode()];	}

	/*
	 * 1文字ゲッタ
	 */
	public function get_char() { return $this->get_data($this->pos_); }

	/*
	 * データ長を保持
	 */
	public function set_length() { $this->len_ = strlen($this->get_data()); }

	/*
	 * データ長を返す
	 */
	public function get_length() { return $this->len_; }

	/*
	 * 1文字をストック
	 */
	public function push_char() {
		$this->stac_ .= $this->get_char();
		// データが詰まれた事を保持するため、falseをセット
		$this->empty_flg_ = false;
	}	
	/*
	 * 1フィールドの文字列をストック
	 *   これが溜まっていくと、1レコード分のデータになる
	 */
	public function push_col() {

//print "push col stac is " . $this->stac_ . "\n";

		$this->col_[] = $this->stac_;	// 1フィールド分の文字列を突っ込む
		$this->stac_  = "";
	}

	/*
	 * パース後データを蓄積
	 */
	public function push_datas() {

//print "in push data " ."\n";

		$this->data_array_[] = $this->col_;	// 1レコード分のデータを突っ込む
		$this->col_ = array();
		// 行データが確定したので、データの状態をemptyに戻す
		$this->empty_flg_ = true;
	}

	/*
	 * バッファクリア
	 */
	public function buf_clear() {
		$this->push_col();
		$this->push_datas();
	}

	/*
	 * Iterate用のfetchメソッド
	 *   文字ポインタが、データ長以上なら偽を返す
	 */
	public function fetch() {

		// 文字ポインタのインクリメント
		$this->inc_pos();

		// Iterate終了判定
		if ($this->get_length() <= $this->pos_) {
			return false;
		}

		return true;
		
	}

	/*
	 * 文字ポインタを一つ進める
	 */
	public function inc_pos() { $this->pos_++ ; }

	/*
	 * 文字ポインタを一つ戻す
	 */
	public function dec_pos() { $this->pos_-- ; }

	/*
	 * パース
	 */
	public function parse() {

		// データ長だけぶん回してパース
		while ($this->fetch()) {

//print "before mode:" . $this->get_mode() . "\n";
//print "char:" . $this->get_char() . "\n";

			$this->get_parse_obj()->parse($this);
			
//print "after  mode:" . $this->get_mode() . "\n";
		}

		// XXX データが詰まれているときだけバッファクリア
		// XXX この判定が無いと、改行で終わってるときに不具合発生。
		if (! ($this->is_empty()) ) {
			$this->buf_clear();
		}
	}

	/**
	 * 状態(モード)
	 */
	private $mode_;

	/**
	 * 状態遷移オブジェクト格納配列
	 */
	private $objs_ = array();

	/**
	 * パース前データ
	 */
	private $data_string_;

	/**
	 * パース後データ
	 *   配列
	 */
	private $data_array_ = array();

	/**
	 * スタックデータ(1フィールド分の文字列を保持)
	 */
	private $stac_;

	/**
	 * 行スタックデータ(1レコード分のデータを保持)
	 */
	private $col_;

	/**
	 * 文字ポインタ
	 */
	private $pos_ = -1;
	
	/**
	 * データ長
	 */
	private $len_ = -1;
	
	/**
	 * Emptyフラグ
	 */
	private $empty_flg_;



} // end of csv_p_work class

/**
 * 通常モード
 *   最も基本的な状態を扱う。以下の挙動を行う。
 *     ','     cellの区切りを意味する文字と判断する
 *     '"'     エスケープモードへのモード切替を行う
 *     '\r'    改行だが、CR-LFの可能性を考え、CRLFモードに切り替える
 *     '\n'    改行。一行のデータの終わりと判断する
 *     any     通常のデータとみなす
 */
class csv_p_set_normal {

	/*
	 * 通常モードのパーサ
	 */
	public function parse($wk) {

		switch ($wk->get_char()) {
			// cell区切り
			case ",":
				$wk->push_col();				// 1フィールド分の文字列をストック
				$wk->set_normal_mode();	// 通常モードへ
				break;
			// エスケープモード切替え
			case "\"":
				$wk->set_esp_mode();		// エスケープモードへ
				break;
			// CRLFモード切替え
			case "\r":
				$wk->set_crlf_mode();		// CRLFモードへ
				break;
			// レコード区切り
			case "\n":
				$wk->buf_clear();				// 行データ確定＆バッファクリア
				$wk->set_normal_mode();	// 通常モードへ
				break; 
			// any(通常データ)
			default:
				$wk->push_char();				// 1文字をストック
				$wk->set_normal_mode();	// 通常モードへ
		}

	}

} // end of class

/**
 * 改行モード
 *   CR-LFを想定しているモード。以下の挙動を行う。
 *     '\n'    '\r\n'であったため、ここで一行のデータの終わりと判断
 *     any     '\r\n'ではなかったため、現在のデータを通常モードで再処理
 */
class csv_p_set_crlf {

	/*
	 * 改行モードのパーサ
	 */
	public function parse($wk) {

		switch ($wk->get_char()) {
			// レコード区切り(\r\nだから。)
			case "\n":
				$wk->buf_clear();				// 行データ確定＆バッファクリア
				$wk->set_normal_mode();	// 通常モードへ
				break; 
			// any(通常モードに差し戻し)
			default:
				$wk->buf_clear();				// (\rだったから)行データ確定＆バッファクリア
				$wk->dec_pos();					// 通常モード再処理の為、ポインタ戻す
				$wk->set_normal_mode();	// 通常モードへ
		}

	}
	
} // end of class

/**
 * エスケープモード
 *   '"'や','を入れるためのモード。','はただのデータとして扱う。
 *   以下の挙動を行う。
 *     '"'     モード切替かデータかわからないため、エスケープチェックモードへ切り替え
 *     any     通常のデータとみなす
 */
class csv_p_set_esp {

	/*
	 * エスケープモードのパーサ
	 */
	public function parse($wk) {

		switch ($wk->get_char()) {
			// エスケープチェックモードへ切り替え
			case "\"":
				$wk->set_esp_chk_mode();	// エスケープチェックモードへ
				break; 
			// any(通常データ)
			default:
				$wk->push_char();					// 1文字をストック
				$wk->set_esp_mode();			// エスケープモードへ
		}

	}
	
} // end of class

/**
 * エスケープチェックモード
 *   エスケープモード中に出てくる'"'を処理するためのモード
 *   以下の挙動を行う。
 *     '"'     '""'となっているため、データに'"'を追加、モードはエスケープ
 *     any     エスケープモードから通常モードへの切り替え指示とみなす。
 *             モードを通常に切り替え、現在のデータを通常モードで再処理
 */
class csv_p_set_esp_chk {

	/*
	 * エスケープチェックモードのパーサ
	 */
	public function parse($wk) {

		switch ($wk->get_char()) {
			// データとしての'"'をストック
			case "\"":
				$wk->push_char();							// 1文字をストック
				$wk->set_esp_mode();					// エスケープモードへ
				break; 
			// any 通常モードへの切り替え
			default:
				$wk->dec_pos();								// 通常モード再処理の為、ポインタ戻す
				$wk->set_normal_mode();				// 通常モードへ
		}

	}

} // end of class

