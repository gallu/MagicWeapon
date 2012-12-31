<?php
/**
 * メール送信クラス
 *
 * sendmailコマンドでメールを送信するクラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

require_once('mail_send.inc');

class mail_send_command extends mail_send {

public function __construct()
{
  //
  parent::__construct();
  //$this->init();
}


public function init() {
  parent::init();
  $this->set_sendmail_path("/usr/sbin/sendmail");
  $this->params_ = array();
}

/**
 * メール送信を行う
 *
 * @access public
 * @param internet_message_format_obj $mobj メールデータを表すインスタンス
 * @return boolean 問題が発生したらfalse 詳細は別メソッドで。…の予定。
 */
public function send(internet_message_format $mobj) {

  // sendmailコマンド用のパラメータを生成
  $param = $this->make_param($mobj);

  // sendmailコマンド用のメールデータを生成
  $mail_string = $mobj->get_mail_string();
//var_dump($mail_string);

  // mailの出力
  $cm = $this->get_sendmail_path() . " " . $param;

  // そうしん
//print "sendmail command line is $cm <br />\n";
  $fh = popen($cm, "w");
  if (false === $fh) {
    // XXX あとでエラー内容追記すること！！
    return false;
  }
//print "file handle is $fh \n";
//print "write data is " . mb_convert_encoding($this->get_data(), "EUC-JP", "auto") . "\n";

//var_dump($mail_string);
//file_put_contents('/opt/www/MW/log', $mail_string, FILE_APPEND);
  $ret = fwrite($fh, $mail_string);
  if (false === $ret) {
    // XXX あとでエラー内容追記すること！！
    return false;
  }

  // XXX 終了コードがもうひとつ不明 orz
  $ret = pclose($fh);
  
  //
  return true;
}

/*
 * sendmailコマンド用のパラメータを生成
 */
protected function make_param(internet_message_format $mobj) {
  // まずデフォ
  $pwk = '-i';

  // envelope fromの有無
  $wk = $mobj->get_envelope_from();
  if ("" === $mobj->get_envelope_from()) {
    $pwk .= 't';
  } else {
    $pwk .= ' -f ' . $wk;
  }

  // envelope toの有無
  $wk = $mobj->get_envelope_to();
  if (empty($wk)) {
    $pwk .= ' -t';
  } else {
    $pwk .= implode(',', $wk);
  }

  // XXX 本当はここでVERP(variable envelope return path)確認する
  // postfixの場合 -XV 

  //
  return $pwk;
}


/*
 * sendmailのパス設定
 */
public function set_sendmail_path($p) { $this->sendmail_path_ = $p; }
protected function get_sendmail_path()  { return $this->sendmail_path_; }

  /*
   * Envelope Fromの設定
   */
  private function set_envelope_from() {
    if ("" != $this->get_data_obj()->get_envelope_from()) {
      $this->params_[] = "-f " . $this->get_data_obj()->get_envelope_from();
      $this->set_return_path($this->get_data_obj()->get_envelope_from());
    }
  }
  private function get_envelope_from() {}
  
  /*
   * Envelope Toの設定
   */
  private function set_envelope_to() {

    $awk = $this->get_data_obj()->get_envelope_to();
    if (0 < count($awk)) {
      $this->params_[] = implode($awk, ",") . $this->get_data_obj()->get_crlf();
    } else {
      $this->params_[] = "-t";
    }
  }
  private function get_envelope_to() {}

//private:
private $obj_mail_data_;    // メールデータオブジェクト
private $mail_data_;        // メールデータString
private $return_path_;      // 自分に戻ってくる用
private $params_;          // sendmailのパラメータ配列
private $param_;            // sendmailのパラメータ
private $sendmail_path_;    // sendmailコマンド

} // end of class

