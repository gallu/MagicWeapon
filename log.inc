<?php

/**
 * ログクラス
 *
 * ログを管理。
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Kohei Yamada <hecomark@gmail.com>
 * @version $Revision$
 **/

class log 
{
//public :

public function __construct() {
  $this->init();
}

/**
 * 初期化処理
 *
 * @access public
 */
public function init() {
  $this->buf_       = "";       // このあたりの変数の説明は
  $this->filename_  = "";       // 一番下を見てね。
  $this->pname_     = "";       //
  $this->flg_       = true;     //
  $this->sflg_      = true;     //
  $this->pauser_    = "\t";     //
}

/**
 * ログファイル出力フラグをonにする
 *
 * @access public
 */
public function on() { $this->flg_ = true; }

/**
 * ログファイル出力フラグをoffにする
 *
 * @access public
 */
public function off() { $this->flg_ = false; }

/**
 * ログの日時表記をyyyy/mm/dd形式にする(デフォルト)
 *
 * @access public
 */
public function spreadtime_on() { $this->sflg_ = true; }

/**
 * ログの日時表記をエポックタイムにする
 *
 * 速度的に有利なので、ログ書き込みの頻度が激しいときに。
 *
 * @access public
 */
public function spreadtime_off() { $this->sflg_= false; }

/**
 * プログラム名設定
 *
 * @access public
 * @param string $str プログラム名
 */
public function set_pname($str) {
  $this->pname_ = $str;
}

/**
 * ログファイル書き込み先設定
 *
 * @access public
 * @param string $str ログファイル名
 */
public function set_filename($str) {
  $this->filename_ = $str;
}

/**
 * サイクリック式ログファイル書き込み先設定
 *
 * ログファイル名と拡張子の間に日付が入る
 *
 * @access public
 * @param string $str ログファイル名
 */
public function set_rotate_filename($s) {
  // 分割
  $wk = file_util::split_filename_by_extension($s);
  //
  $s = $wk[0] . date('Ymd') . $wk[1];
  $this->set_filename($s);
}

/**
 * ログに文字列を追加
 *
 * flush()を呼び出す前なら何回でも使用できる。
 * その場合、ログ文字列は連結される。
 * 区切り文字が必要名場合 pauseメソッドをcallすること
 *
 * @access public
 * @param string $str ログに書き込まれる文字列
 */
public function add($str) {
  $this->buf_ .= $str;
}

/**
 * ログに区切り文字を追加。
 *
 * デフォルトでは"\t"(タブ)が追加される。
 *
 * @access public
 */
public function pause()  {
  $this->add($this->_pauser());
}

/**
 * ログ書き込み
 *
 * 書き出した後、ログのバッファはクリアされる。
 *
 * @access public
 * @return boolean 書き込みに失敗したらfalse
 */
public function flush()
{
  ///////
  // 書き込み前のチェック

  if (!($this->_is_on())) {      // 書き込み不要の場合は終了。
//print "flush off";
    $this->_clear_buf();         // XXX バッファはクリアしておく？
    return true;
  }
  
  if ("" === $this->_buf()) {    // バッファが空の場合も終了。
//print "buf empty!";
    $this->_clear_buf();         // 一応形式的にクリアしておく。
    return true;
  }

  ///////
  // 日付の取得

  $t = "";
  if ($this->_is_sptime_on()) {  // spreadtimeがonなら
    $t = date("Y/m/d H:i:s");    // 見やすく表示。
  } else {
    $t = time();                 // それ以外はエポックタイムで。
  }

  ///////
  // データ整形
  $write_str = "";
  $write_str .= $t;                     // 日時
  $write_str .= $this->_pauser();       // 区切り文字
  if ($this->_pname()) {
    $write_str .= $this->_pname();      // プログラム名
    $write_str .= $this->_pauser();     // 区切り文字
  }
  $write_str .= $this->_buf() . "\n" ;  // ログの内容
                                        // XXX 改行は\n固定？
  
  ///////
  // ファイル書き込み
  // XXX file_put_contentsでLOCK_EX,でもいいのかなぁ？

  $fp = @fopen($this->_filename(), "a");
  if(!$fp) {
//print "file open error!";
    $this->_clear_buf();      // XXX ここはクリアしていいのか…？
    return false;             // 終了
  }
  @flock($fp, LOCK_EX);        // LOCK_EX => 排他ロック
  @fwrite($fp, $write_str);
  @fclose($fp);

  ///////
  // バッファだけクリアして復帰

  $this->_clear_buf();
  return true;
}

//private:
private function _filename() { return $this->filename_; }
private function _buf() { return $this->buf_; }
private function _pname() { return $this->pname_; }
private function _pauser() { return $this->pauser_; }
private function _is_on() { return $this->flg_; }
private function _is_sptime_on() { return $this->sflg_; }
private function _clear_buf() { $this->buf_ = ""; }

//private:
private $flg_;        // true(on)   => ファイル出力する
                      // false(off) => ファイル出力しない
private $sflg_;       // true(on)   => 日時をyyyy/mm/dd形式で書き込み
                      // false(off) => 日時をエポックタイムで書き込み
private $filename_;   // ログ書き込み先ファイル名
private $buf_;        // ログの内容を溜めるバッファ
private $pname_;      // ログを出力したプログラム名
private $pauser_;     // 区切り文字

}  // end of class

