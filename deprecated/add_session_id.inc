<?php

/**
 * セッションIDをURIに付与するクラス
 *
 * 基本的にセキュリティ上脆弱だから使うときは要注意
 * でも携帯だとCookieないから結構必須なのよね(DoCoMoの馬鹿...
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/


class add_session_id {

public function __construct()
{
  //
  $this->init();
}

//
public function init()
{
  // 一部びみょ?にdefaultあり
  $this->set_body("");
  $this->set_session_id("");
  $this->set_session_name("s");
  //
  $this->clear_target_suffix();
  $this->push_target_suffix("php");
  $this->push_target_suffix("cgi");
}

/**
 * 変換元のHTMLを設定
 *
 * @access public
 * @param string $s 基本的には元HTML
 * @return boolean なにかあったらfalse
 */
public function set_body($s) { $this->body_ = $s; }
public function get_body() { return $this->body_; }

/**
 * 埋め込みたいsession idを設定
 *
 * @access public
 * @param string $s セッションID
 * @return boolean なにかあったらfalse
 */
public function set_session_id($s) { $this->session_id_ = $s; }
public function get_session_id() { return $this->session_id_; }

/**
 * 埋め込みたいsession idのname部分を設定
 *
 * @access public
 * @param string $s セッションIDのname
 * @return boolean なにかあったらfalse
 */
public function set_session_name($s) { $this->session_name_ = $s; }
public function get_session_name() { return $this->session_name_; }

/**
 * 対象になる拡張子の設定
 *
 * .phpと.cgiはデフォルトで入ってる
 *
 * @access public
 * @param string $s セッションIDのname
 * @return boolean なにかあったらfalse
 */
public function push_target_suffix($s) { $this->target_suffix_[] = $s; }

public function clear_target_suffix() { $this->target_suffix_ = array(); }
public function get_target_suffix() { return $this->target_suffix_; }

/**
 * URIにセッションIDを追加する処理
 *
 * @access public
 * @return boolean なにかあったらfalse
 */
public function run()
{
  $base = $this->get_body();
  $ret = "";

  // 正規表現による分解
  // XXX 重くない？
  $pattern = "/(.*?)([<])(.*?)([>])(.*)/s";
  while(0 < preg_match($pattern, $base, $wk)) {
    // まず該当「以外」の文字列を切り出す
    $ret .= $wk[1] . $wk[2];
    $base = $wk[4] . $wk[5];

    // この時点で、扱う対象は$wk[3]
//print "do is $wk[3]\n";
    // 拡張子がphp、cgi、html以外ならとりあえず省く
    $pattern2 =
      '/(.*\.)(' . implode('|', $this->get_target_suffix()) . ')(.*)/si';
    if (0 === preg_match($pattern2, $wk[3], $wk2)) {
      $ret .= $wk[3];
      continue;
    }
    // else 
//var_dump($wk2);
    // まずファイル名までは貼り付ける
    $ret .= $wk2[1] .  $wk2[2];

    if ('?' === $wk2[3][0]) {
      $wk2[3][0] = '&';
    }
    $ret .= '?' . $this->get_session_name() . '=' . $this->get_session_id() . $wk2[3];
//print "add $buf \n";
    $ret .= $buf;
//print "\n-------------\n";
//print $ret;
//print "\n-------------\n\n";
  }
  // 後処理
  if ("" != $base) {
    $ret .= $base;
  }

  //
  return $ret;
}


//private:
private $body_;
private $session_id_;
private $session_name_;
private $target_suffix_;

} // end of class

?>
