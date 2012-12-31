<?php

/**
 * Internet Message Formatを意味するクラス
 *
 * RFC2822準拠…予定のメールクラス(笑
 * 基本facadeだけど、面倒なのでheader_fieldsを継承してる
 * 文字エンコードが 内部エンコーディング(デフォEUC) -> jis で固定になってるので注意
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

require_once('internet_message_format_header_fields.inc');
require_once('internet_message_format_body_text.inc');
require_once('multibyte.inc');
require_once('mw_area_setting.inc');

class internet_message_format extends internet_message_format_header_fields {

public function __construct()
{
  //
  parent::__construct();
  //$this->init();
}

//
public function init()
{
  parent::init();
  $this->set_envelope_from('');
  $this->envelope_to_ = array();
  //$this->verp_flg_off();
  // XXX 当面場当たり。後でfactory予定
  $this->internet_message_body_obj_ = new internet_message_format_body_text;
}

/**
 * envelope fromの設定
 *
 * わからなければ設定しないこと!!
 *
 * @access public
 * @param string $s envelope用のfromアドレス
 */
public function set_envelope_from($s) { $this->envelope_from_ = $s; }
// 表には出さないからDocument形式コメントは省略
public function get_envelope_from() { return $this->envelope_from_; }


/**
 * envelope toの設定
 *
 * わからなければ設定しないこと!!
 *
 * @access public
 * @param string $s envelope用のtoアドレス
 */
public function push_envelope_to($s) { $this->envelope_to_[] = $s; }
// 表には出さないからDocument形式コメントは省略
public function get_envelope_to() { return $this->envelope_to_; }

//
protected function get_body_obj() { return $this->internet_message_body_obj_; }

/**
 * mail bodyの設定
 *
 * @access public
 * @param string $s mailそのものをあらわす文字列。問題があれば空文字
 */
public function set_body($s)
  { $this->get_body_obj()->set_from_string($s); }

/**
 * mail bodyの取得
 *
 * @access public
 * @return string mailそのもの(本文)をあらわす文字列。問題があれば空文字
 */
public function get_body()
  { return $this->get_body_obj()->get_to_string(); }

/**
 * mail stringの取得
 *
 * internet messageすべてを１本の文字列にして復帰
 *
 * @access public
 * @return string mailそのものをあらわす文字列。問題があれば空文字
 */
public function get_mail_string()
{
$ret = "";
  // まずヘッダの取り出し
  $ret = $this->get_all_headers();
  $ret .= "\r\n";

  // XXX いったんiso-2022jp固定
  $m = new multibyte;
  $m->set_jis();
  $s = mw_area_setting::get_inside_encoding_type();
  if ('' === $s) {
    $m->set_from_encoding_euc();
  } else {
    $m->set_guess_from_string($s);
  }

  //
  $ret .= $m->convert($this->get_body_obj()->get_to_string());
//var_dump($this->get_body_obj()->get_to_string());
//var_dump($ret);

  //
  return $ret;
}

/**
 * mail stringの取り込み
 *
 * @access public
 * @param string $s mail stringそのもの
 * @return boolean なにかあったらfalse
 */
public function set_mail_string($s)
{
// 一端ざっくりと
//
$mail_header = array();
$mail_body = array(); // auの時に改行をLFにしたいのでいったん配列で
$i = -1; // 初手にインクリするので
$header_flg = true;

  // 分離
  // XXX …悪手だなぁ
  // \r\n を \n に置換
  $s = str_replace("\r\n", "\n", $s);
  // \rを\nに置換
  $s = str_replace("\r", "\n", $s);
  // で、分解
  $awk = explode("\n", $s);
//var_dump($awk);

  // 各行ごとに処理
  foreach($awk as $buf) {
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
  $this->set_body(implode("\r\n", $mail_body));

  // ヘッダ格納処理
  // ここは丁寧に
  // XXX とか言いつつけっこう雑
  foreach($mail_header as $wk) {
    // まずは分離
    list($h_name, $value) = explode(':', $wk);
    $h_name = strtolower(trim($h_name));
    $value = trim($value);

    // で、ヘッダごとにvalueを処理
    switch($h_name) {
      case 'from':
      case 'to':
      case 'cc':
      case 'bcc':
//print "Trap 1\n";
        // 名前とアドレスを分離して
        $name = '';
        $address = '';
        if (false === strpos($value, '<')) { //  < がない
          // valueをメアドとして扱う
          $address = trim($value);
        } else {
          // 分解
          list($name, $address) = explode('<', $value, 2);
          // >を削除して
          $address = str_replace('>', '', $address);
          // 前後のホワイトスペースを切って
          $name = trim($name);
          $address = trim($address);
        }
//print " {$name} => {$address} \n";

        // 設定
        switch($h_name) {
          case 'from':
            $this->set_from($address, $name);
            break;

          case 'to':
            $this->push_to($address, $name);
            break;

          case 'cc':
            $this->push_cc($address, $name);
            break;

          case 'bcc':
            $this->push_bcc($address, $name);
            break;
        }
        break;

      case 'subject':
        $this->set_subject($value);
        break;

      default:
        // valueを設定
        $this->_set_header($h_name, $value);
        break;
    }
  }
  //
  return true;
}

//private:
private $envelope_from_;
private $envelope_to_;
//private $verp_flg_;
private $internet_message_body_obj_;

} // end of class
