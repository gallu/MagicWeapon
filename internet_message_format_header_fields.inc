<?php

/**
 * Internet Message Formatのheader用クラス
 *
 * とりあえずエンコードはiso-2022jp固定
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

require_once('security.inc');
require_once('multimap.inc');
require_once('multibyte.inc');
require_once('calendar.inc');

class internet_message_format_header_fields {

public function __construct()
{
  //
  $this->init();
}

//
public function init()
{
  $this->header_ = new multimap;
  $this->m_ = new multibyte;
  $this->m_->set_from_encoding_unknown();
  $this->m_->set_to_encoding_jis();

  // デフォルト設定
  // mail read考えると蛇足？
  $this->set_content_type('text/plain; charset=iso-2022-jp');
  $this->set_date_now();
  $this->make_message_id();
  $this->_set_header('Mime-Version', '1.0');
  $this->_set_header('Content-Transfer-Encoding', '7bit');
  $this->_set_header('X-Mailer', 'MagicWeapon');
}

//
protected function get_multibyte() { return $this->m_; }

/**
 * 単一ヘッダの設定
 *
 * 重複しない(はず)のヘッダの設定
 * 重複した場合、上書きされる
 *
 * @access protected
 * @param string $n フィールド名
 * @param string $v フィールド本文
 * @return boolean 何かあったらfalse
 */
protected function _set_header($n, $v)
{
  //
  if ( ("" === $n)||("" === $v) ) {
    return false;
  }
  $v = security::sanitize_header_fields_data($v);
  // キーを捜してみる
  $itr = $this->header_->find($n);
//var_dump($itr);
  if (-1 == $itr) {
    // なければinsert
    $this->header_->insert($n, $v);
//print "add_once insert\n";
  } else {
    // あれば上書き
    $this->header_->real($itr)->set_second($v);
//print "add_once update\n";
  }
  return true;
}

/**
 * 重複可能ヘッダの設定
 *
 * @access protected
 * @param string $n フィールド名
 * @param string $v フィールド本文
 * @return boolean 何かあったらfalse
 */
protected function _push_header($n, $v)
{
  if ( ("" === $n)||("" === $v) ) {
    return false;
  }
  $v = security::sanitize_header_fields_data($v);
  $this->header_->insert($n, $v);
  return true;
}

/**
 * アドレス以外が混入したケースを含むアドレスの作成
 *
 * name <email@add.ress> みたいな形式を作成する
 *
 * @access protected
 * @param string $address emailアドレス
 * @param string $sender_name 送り主の名前
 * @return string 何かあったら空文字
 */
protected function _make_internet_address($address, $sender_name = "")
{
  if ("" === $address) {
    return "";
  }
  if ("" === $sender_name) {
    //return $address;
    return '<' . $address . '>';
  }
  // お名前部分をごにょり
  $s = $this->_b_encoding($sender_name) . ' <' . $address . '>';

  //
  return $s;
}

// B-Encoding
protected function _b_encoding($base)
{
  // 一応内部コードを重視、無指定ならEUC…なのは伝統だと思いねぇ
  $m = new multibyte;
  $m->set_jis();
  $s = mb_internal_encoding();
  if ('' === $s) {
    $m->set_from_encoding_euc();
  } else {
    $m->set_guess_from_string($s);
  }
  $base = $m->convert($base);

  //
  $s = '=?ISO-2022-JP?B?' . base64_encode($base) . '?=';
  return $s;
}

// XXX
protected function _b_decoding($s)
{
  // XXX mb_encode_mimeheader不安なので未使用
  $mobj = new multibyte;
  $m->set_to_encoding_jis();
  $s = mb_internal_encoding();
  if ('' === $s) {
    $m->set_to_encoding_euc();
  } else {
    $m->set_guess_to_string($s);
  }

  // 必要部分抜いてdecodeして文字コードを変更
  // XXX すんげぇMagicNumberで抜いてる……
  $s = $mobj->convert(base64_decode(substr($s, 16, (strlen($s) - 18))));

  return $s;
}

//_get_header($n)
//_get_header_array($n)

// 各種ヘッダ設定
/**
 * Toの設定
 *
 * 複数設定はすべてつみあがる
 *
 * @access public
 * @param string $a Toに設定するメールアドレス
 * @param string $n アドレスに付記する名前表記(省略可)
 */
public function push_to($a, $n = "")
  { return $this->_push_header('To', $this->_make_internet_address($a, $n)); }

/**
 * Ccの設定
 *
 * 複数設定はすべてつみあがる
 *
 * @access public
 * @param string $a Ccに設定するメールアドレス
 * @param string $n アドレスに付記する名前表記(省略可)
 */
public function push_cc($a, $n = "")
  { return $this->_push_header('Cc', $this->_make_internet_address($a, $n)); }

/**
 * Bccの設定
 *
 * 複数設定はすべてつみあがる
 *
 * @access public
 * @param string $a Bccに設定するメールアドレス
 * @param string $n アドレスに付記する名前表記(省略可)
 */
public function push_bcc($a, $n = "")
  { return $this->_push_header('Bcc', $this->_make_internet_address($a, $n)); }

/**
 * Mail Fromの設定
 *
 * @access public
 * @param string $a Mail Fromに設定するメールアドレス
 * @param string $n アドレスに付記する名前表記(省略可)
 */
public function set_from($a, $n = "")
  { return $this->_set_header('From', $this->_make_internet_address($a, $n)); }


/**
 * Subjectの設定
 *
 * @access public
 * @param string $v Subject
 */
public function set_plain_subject($v)
  { return $this->_set_header('Subject', $this->_b_encoding($v)); }
/**
 * Subjectの設定
 *
 * set_plain_subjectへのエイリアス
 *
 * @access public
 * @param string $v Subject
 */
public function set_subject($v) { return $this->set_plain_subject($v); }
/**
 * エンコードされたSubjectの設定
 *
 * あんまり使わないはず。パース時用かな？
 *
 * @access public
 * @param string $v エンコードされているSubject
 */
public function set_encoded_subject($v)
  { return $this->_set_header('Subject', $v); }


// この辺とりあえずコメント書かないよ
public function set_sender($v) { return $this->_set_header('Sender', $v); }
public function set_return_path($v) { return $this->_set_header('Return-Path', $v); }
public function set_reply_to($v) { return $this->_set_header('Reply-to', $v); }
public function set_in_reply_to($v) { return $this->_set_header('In-reply-to', $v); }
public function set_references($v) { return $this->_set_header('References', $v); }
public function set_comments($v) { return $this->_set_header('Comment', $v); }
public function set_keywords($v) { return $this->_set_header('Keywords', $v); }

/**
 * Dateの設定
 *
 * これ使うより、set_date_nowで現在時刻設定したほうが楽
 * 時刻指定したいならcalendarインスタンス作って set_date_from_calendar かな
 *
 * @access public
 * @param string $v 日付を正しい文字列で
 */
public function set_date($v) { return $this->_set_header('Date', $v); }

/**
 * 現在時刻でDateを設定
 *
 * @access public
 */
public function set_date_now()
{
  //
  $cobj = new calendar;
  $cobj->set_now_date();
  return $this->set_date_from_calendar($cobj);
}

/**
 * 指定されたDateの設定
 *
 * calendarインスタンスで指定
 *
 * @access public
 * @param calendar_obj $cobj Dateに設定したい日付情報をもつインスタンス
 */
public function set_date_from_calendar(calendar $cobj)
{
  // 日時表記
  // Thu, 02 Aug 2001 10:45:23 +0900
  $t = sprintf("%s, %02d %s %4d %02d:%02d:%02d +0900",
                $cobj->get_week_string_short(),
                $cobj->get_day(),
                $cobj->get_month_string_short(),
                $cobj->get_year(),
                $cobj->get_hour(),
                $cobj->get_minute(),
                $cobj->get_second()
              );
  //
  $this->set_date($t);
}

/**
 * message-idの設定
 *
 * 実際にはこれよりもmake_message_idのほうが楽
 *
 * @access public
 * @param string $v 設定したいメッセージID
 */
public function set_message_id($v)
  { return $this->_set_header('Message-id', $v); }

/**
 * message-idの作成と設定
 *
 * 以下、めも
Message-Id: <20071211100546.8D46D330785@news.mixi.jp>
Message-Id: <20071211093113.694833E066D@news.mixi.jp>
Message-Id: <20071211072747.A61121C067@roccho.hatena.ne.jp>
Message-ID: <-1616970909.1197355023898.JavaMail.root@atitms02>
Message-ID: <20071211134137.595.2097.edm@emml005.nikkeibp.co.jp>
Message-Id: <475dd1fc-0007326a-ec71beaac573b087fc8f84181b5d3a01-52218@mailer08.bulk.ogk.yahoo.co.jp>
Message-Id: <200712102007.lBAK73Di026290@kym01a.rnet.or.jp>

         $message_id = '<' . $REMOTE_PORT . '.';
         if (!$hide_addr) {
            $message_id .= $REMOTE_ADDR . '.';
         }
         $message_id .= time() . '.squirrel@' . $SERVER_NAME .'>';

メッセージ識別子を作成するホストのドメイン名
(または IP アドレスのリテラル表現)を "@" の
右側に置き、左側に日付時刻とそのシステムで
利用可能なユニークな(おそらくシーケンシャルな)
識別子(例えばプロセス ID)とを置くのは良い方法
である。
 *
 * @access public
 */
public function make_message_id()
{
  // FQDN取得
  // XXX
  $domain = trim(`hostname`);

  // 時間取得
  list($usec, $sec) = explode(" ", microtime());
  $usec = (int)($usec * 1000 * 1000);

  // プロセスID
  $pid = getmypid();

  // 足しこんでみる
  $s = sprintf("<%d.%d.%d@%s>", $sec, $usec, $pid, $domain);

  //
  $this->set_message_id($s);
}

/**
 * Content-typeの設定
 *
 * @access public
 * @param string $v 設定したいcontent-type
 */
public function set_content_type($v) { return $this->_set_header('Content-type', $v); }

// これ使わせたくないから書かないよ？
public function get_all_headers()
{
  // XXX アクセサどうするかねぇ？
  $ret = "";
  for($itr = $this->header_->begin(); $itr != $this->header_->end(); $itr ++) {
    // 文字列合成
    $name = $this->header_->real($itr)->get_first();
    $value = $this->header_->real($itr)->get_second();
    // XXX 改行コード auだと違うんだとさ orz
    $ret .= $name . ': ' . $value . "\r\n";
  }
  //
  return $ret;
}

/*
*/
public function get_header_to()
{
  return $this->_get_header('To');
}

// -------
protected function _get_header($s)
{
  //
  $ret = array();

  //
  $itrs = $this->header_->find_array($s);
//var_dump($itrs);

  foreach($itrs as $itr) {
    $ret[] = $this->header_->real($itr)->get_second();
  }
  //
  return $ret;
}



//private:
private $header_;
private $m_;

} // end of class

