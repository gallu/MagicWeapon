<?php

/**
 * 空メールからfromと「Toのトークン部分」を取り出すクラス
 *
 * おおむねユーティリティclass
 * 後でmailクラス群と結合
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

//require_once('security.inc');
//require_once('sql_util.inc');
//require_once('tokenizer.inc');
require_once('mail_util.inc');

class empty_mail_analysis {

public function __construct()
{
  //
  $this->init();
}

//
public function init()
{
  $this->set_mail_array( array() );
  $this->set_mail_from('');
  $this->set_mail_to('');
  $this->set_mail_to_token('');
  $this->set_token_separator('+'); // デフォはpostfixの+
}

// XXX 元データがどうなるか不明なのであえて参照にはしない
public function get_mail_array() { return $this->mail_array_; }
public function set_mail_array($a) { $this->mail_array_ = $a; }

public function get_mail_from() { return $this->mail_from_; }
public function set_mail_from($s) { $this->mail_from_ = $s; }

public function get_mail_to() { return $this->mail_to_; }
public function set_mail_to($s) { $this->mail_to_ = $s; }

public function get_mail_to_token() { return $this->mail_to_token_; }
public function set_mail_to_token($s) { $this->mail_to_token_ = $s; }

public function get_token_separator() { return $this->token_separator_; }
public function set_token_separator($s) { $this->token_separator_ = $s; }

public function get_mail_body() { return $this->mail_body_; }
protected function set_mail_body($a) { $this->mail_body_ = $a; }

// 使うなら使うとき実装して
//public function get_mail_string() { return $this->mail_string_; }

/**
 * mail stringを配列にする
 *
 * CRLFとLFの双方に対応
 * 復帰値の配列の後ろの改行はtrimする
 * XXX 使うなら使うとき使う人が実装して
 *
 * @access public
 * @return boolean 何か問題があればfalse
 */
public function set_mail_string($s, $crlf="\r\n")
{
}

/**
 * stdinからの入力を取得してmail arrayにする
 *
 * データの格納はクラス変数に
 *
 * @access public
 * @return boolean 何か問題があればfalse
 */
public function make_mail_array_from_stdin()
{
  // 標準入力からメールを取得
  $fp = fopen('php://stdin', 'r');

  $ret_array = array();
  while(!feof($fp)) {
    $buf = fgets($fp);
    $buf = rtrim($buf);
    $ret_array[] = $buf;
  }
  // 
  $this->set_mail_array($ret_array);

  //
  return true;
}

//
public function analysis_from_stdin()
{
  $this->make_mail_array_from_stdin();
  return $this->analysis();
}

/**
 * 分析
 *
 * mailstringを分析して必要な情報を返す
 *
 * @access public
 * @return boolean 何か問題があればfalse
 */
public function analysis()
{

//
$mail_header = array();
$mail_body = array(); // auの時に改行をLFにしたいのでいったん配列で
$i = -1; // 初手にインクリするので
$header_flg = true;

  foreach($this->get_mail_array() as $buf) {
    // ヘッダモードの場合
    if (true === $header_flg) {
      // ヘッダから抜ける場合
      if ('' === $buf) {
        $header_flg = false;
      } else {
        if ( (' ' === $buf[0])||("\t" === $buf[0]) ) {
          $mail_header[$i] = $mail_header[$i] . "\t" .  trim($buf);
        } else {
          $mail_header[++$i] = trim($buf);
        }
      }
    } else {
      $mail_body[] = $buf;
    }
  }
//var_dump($mail_header);
//var_dump($mail_body);

  // body格納
  $this->set_mail_body($mail_body);

  // from行からメアドをげと
  // to行からトークンをげと
  foreach($mail_header as $wk) {
//print $wk . "\n"
    if (1 === preg_match('/(.*?)[ \t]*[:][ \t]*(.*)/', $wk, $mt)) {
//var_dump($mt);
      if (0 === strcasecmp("from", $mt[1])) {
//print $mt[2] . "\n";
        $from = $mt[2];
      } else
      if (0 === strcasecmp("to", $mt[1])) {
//print $mt[2] . "\n";
        $to = $mt[2];
      }
    }
  }

  $from = mail_util::carve_address($from);
  $to = mail_util::carve_address($to);

//print "from $from \n";
//print "to $to \n";

  // トークンをげとる
  $token = "";
  $patarn = '/[\\' . $this->get_token_separator() . '](.*?)[\@]/';
  if (1 === preg_match($patarn, $to, $mt)) {
//var_dump($mt);
    $token = $mt[1];
  }
//print "token $token \n";

  // 
  $this->set_mail_from($from);
  $this->set_mail_to($to);
  $this->set_mail_to_token($token);

  //
  return true;
} // end of analysis



//private:
private $mail_array_;
private $mail_from_;
private $mail_to_;
private $mail_to_token_;
private $token_separator_;
private $mail_body_;

} // end of class

