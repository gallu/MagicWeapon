<?php
/**
 * CGI レスポンスクラス
 *
 * CGI ヘッダを設定、出力(viewで)するクラス
 * XXX 一端Cookieは実装しないが、近日中にこっちに取り込みたいなぁ
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 */



class cgi_response
{

public function __construct()
{
  $this->init();
}

// 初期化
public function init()
{
  $this->header_ = array();
  $this->put_flg_off();
}

/**
 *  ヘッダ追加
 *
 * とりあえず「改行は問答無用で削除」
 * 後でもうちょい丁寧に処理したいなぁ
 *
 * @access public
 * @param string $n headerの名前
 * @param string $v headerの値
 */
public function add_header($n, $v)
{
  // 処理
  $this->header_[$n] = $v;
  // 出力フラグを寝かせる
  $this->put_flg_off();
  return true;
}
protected function get_all_header() { return $this->header_; }
protected function get_header($n)
{
  if (isset($this->header_[$n])) {
    return $this->header_[$n];
  }
//print "get $n is empty \n";
  return "";
}

private function _set_put_flg($b) { $this->put_flg_ = $b; }
public function put_flg_on() { $this->_set_put_flg(true); }
public function put_flg_off() { $this->_set_put_flg(false); }
public function is_put_flg() { return $this->put_flg_; }


/**
 * 出力
 *
 * stdoutに出力
 * すでに出力した場合、returnでfalseを返す
 *
 * @access public
 * @return boolean 出力済みの場合false
 */
public function put()
{
  // 出力チェック
  if (true == $this->is_put_flg()) {
    return false;
  }

  // 出力
  foreach ($this->get_all_header() as $key => $value) {
//print $key . ": " . $value . "\n";
    // 先に改行の取り除き
    $n = preg_replace('/[\r\n]/', '', $key);
    $v = preg_replace('/[\r\n]/', '', $value);
    // 設定
    header($n . ": " . $v);
  }
  
  // フラグを「出力済み」にして、終了
  $this->put_flg_on();
  return true;
}

/**
 * content-typeがHTMLな時のヘッダ設定
 *
 * @access publicかprivateかprotectedか
 * @param string $jcode_type エンコードタイプ。デフォルトはeuc
 */
public function set_content_html($jcode_type = "euc")
{
  
  // 基本部分
  $n = "Content-type";
  $s = "text/html";

  // 各コード判定
  // XXX ある程度いろいろと対応
  if (preg_match('/euc/i', $jcode_type)) {
    $s .= "; charset=euc-jp";
  } else
  if (preg_match('/sjis/i', $jcode_type)) {
    $s .= "; charset=shift_jis";
  } else
  if (preg_match('/shift[-_]*jis/i', $jcode_type)) {
    $s .= "; charset=shift_jis";
  } else
  if (preg_match('/utf[-_]*8/i', $jcode_type)) {
    $s .= "; charset=utf-8";
  } else
  if (preg_match('/iso-2022-jp/i', $jcode_type)) {
    $s .= "; charset=iso-2022-jp";
  } else
  if (preg_match('/jis/i', $jcode_type)) {
    $s .= "; charset=iso-2022-jp";
  }
  // もし引っかからなければ文字コードは指定しない

  // ヘッダbufferに登録
  return $this->add_header($n, $s);
}

/**
 * Content-type設定
 *
 * さまざまな、Content-typeを自由に設定する
 *
 * @access public
 * @param string $type type
 */

public function set_content($type = "")
{
  if ("" === $type) {
    return false;
  }
  
  // 追加
  return $this->add_header("Content-type", $type);
}

/**
 * Content-lengthの設定
 *
 * @access public
 * @param int $len コンテンツの長さ
 */

public function set_length($len = 0)
{
  if (0 >= $len) {
    return false;
  }
  
  // 追加
  return $this->add_header("Content-Length", $len);
}

/**
 * plain text用
 *
 * @access public
 */
public function set_content_plain() { return $this->set_content("text/plain"); }

/**
 * Jpeg画像用
 *
 * @access public
 */
public function set_content_jpeg() { return $this->set_content("image/jpeg"); }

/**
 * GIF画像用
 *
 * @access public
 */
public function set_content_gif() { return $this->set_content("image/gif"); }

/**
 * PNG画像用
 *
 * @access public
 */
public function set_content_png() { return $this->set_content("image/png"); }

/**
 * CSVファイル用
 *
 * @access public
 * @param string $filename ファイル名
 */
public function set_content_csv($filename)
{
  $this->set_content("application/octet-stream");
  $this->add_header('Content-Disposition', 'attachment; filename=' . $filename . ';');

  return true;
}

/**
 * PDFファイル用
 *
 * @access public
 * @param string $filename ファイル名
 */
public function set_content_pdf($filename)
{
  $this->set_content("application/pdf");
  $this->add_header('Content-Disposition', 'attachment; filename=' . $filename . ';');

  return true;
}

/**
 * json用
 *
 * XXX 一旦、文字エンコードはUTF-8固定
 *
 * @access public
 */
public function set_content_json()
{
  $this->set_content("application/json; charset=utf-8");

  return true;
}

/**
 * Location設定
 *
 * これを設定するとほかの結構なheader設定が無意味になるので注意すること
 *
 * @access public
 * @param string $loc 遷移させたいURI
 * @param int $http_status 302(Moved Temporarily)か301(Moved Permanently)か。デフォは302。
 */
public function set_location($loc, $http_status=302) {
  // 301の時の処理：結構アバウトかも
  if (301 == $http_status) {
    // XXX これでいけるかし？ もうちょっとちゃんと実装しないと駄目かし？
    $this->add_header("Status",  "301 Moved Permanently");
  }

  return $this->add_header("Location", $loc);
}

/**
 * status204(画面を変化させない)の出力
 *
 * データダウンロードなんかのときにあわせてどうぞ
 *
 * @access public
 */
public function set_status204()
{
  return $this->add_header("Status",  "204 No Response");
}

/**
 * 画面キャッシュ無効化
 *
 * ブラウザによって大分違うので、一通りまとめて
 * 動的Pageなら発行して損はなし
 *
 * @access public
 */
public function set_nocache()
{
  $this->add_header("Expires", "0");
  $this->add_header("Pragma", "no-cache");
  $this->add_header("Cache-Control", "no-cache");
  // XXX Cache-Control: private, no-store, no-cache, must-revalidat

  return true;
}

// Content-type判定
// 文字コードが絡むtypeならtrueを復帰
// XXX 内部用っぽいのでPHPDoc形式のコメントをあえて省略
public function is_need_convert()
{
  // まず取り出す
  $type = $this->get_header("Content-type");
//print "type is $type \n";
  // からっぽならデフォルトはtext系だろうからtrue復帰
  if ("" == $type) {
//print "return true\n";
    return true;
  }
  // textならtrue
  //if (!(strpos($type, "text") === false)) {
  if (0 === (strncmp($type, 'text', 4))) {
//print "return true\n";
    return true;
  }
  // XXX DoCoMo用かなぁ？
  if (0 === (strncmp($type, 'application/xhtml+xml', 21))) {
    return true;
  }

  // 上述以外ならエラー
//print "return false\n";
  return false;
}


// 出力するヘッダ一式
private $header_;

// 出力したかどうか
// falseなら未出力 trueなら出力済み
private $put_flg_ = false;

} // end of class

