<?php

/**
 * マルチバイトクラス
 *
 * 文字コード変換のラッパー
 * XXX 当面日本語オンリー。適当に追加して他言語対応してください
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

/*
 * 補足
 * いくつかの観点から、あえてtypeをマジックナンバーで保有する
 * 
 * 0 : shift-jis
 * 1 : euc-jp
 * 2 : jis ( ISO-2022-JP )
 * 3 : utf-8
 * 
 */
 

class multibyte {

//public:

public function __construct()
{
  // XXX 一端日本語固定?
  mb_language("Japanese");
  // XXX 順番固定?
  //mb_detect_order('eucjp-win,sjis-win,SJIS,EUC-JP,JIS,UTF-8,ASCII');
  //mb_detect_order('sjis-win,eucjp-win,SJIS,EUC-JP,JIS,UTF-8,ASCII');

  //
  $this->init();
}

/**
 * 初期化処理
 *
 * @access public
 */
public function init()
{
  // とりあえず
  $this->set_to_encoding_type(-1);
  $this->set_from_encoding_type(-1);
}

// 外から叩かせる気ないからprotected
protected function set_from_encoding_type($i){$this->from_encoding_type_ = $i;}
protected function get_from_encoding_type(){return $this->from_encoding_type_;}
protected function set_to_encoding_type($i){$this->to_encoding_type_ = $i;}
protected function get_to_encoding_type(){return $this->to_encoding_type_;}

// XXX 修正前との互換性をかねたらっぱー
public function set_sjis() { $this->set_to_encoding_sjis(); }
public function set_euc() { $this->set_to_encoding_euc(); }
public function set_jis() { $this->set_to_encoding_jis(); }
public function set_utf8() { $this->set_to_encoding_utf8(); }

/**
 * 変換元文字エンコードの設定(不明)
 *
 * 変換元文字エンコードを「不明」として設定する
 *
 * @access public
 */
public function set_from_encoding_unknown() { $this->set_from_encoding_type(-1); }
/**
 * 変換元文字エンコードの設定(sjis)
 *
 * 変換元文字エンコードを「sjis」として設定する
 *
 * @access public
 */
public function set_from_encoding_sjis() { $this->set_from_encoding_type(0); }
/**
 * 変換元文字エンコードの設定(EUC)
 *
 * 変換元文字エンコードを「EUC」として設定する
 *
 * @access public
 */
public function set_from_encoding_euc() { $this->set_from_encoding_type(1); }
/**
 * 変換元文字エンコードの設定(jis)
 *
 * 変換元文字エンコードを「jis」として設定する
 *
 * @access public
 */
public function set_from_encoding_jis() { $this->set_from_encoding_type(2); }
/**
 * 変換元文字エンコードの設定(UTF-8)
 *
 * 変換元文字エンコードを「UTF-8」として設定する
 *
 * @access public
 */
public function set_from_encoding_utf8() { $this->set_from_encoding_type(3); }


/**
 * 変換後文字エンコードの設定(sjis)
 *
 * 変換後文字エンコードを「sjis」として設定する
 *
 * @access public
 */
public function set_to_encoding_sjis() { $this->set_to_encoding_type(0); }
/**
 * 変換後文字エンコードの設定(EUC)
 *
 * 変換後文字エンコードを「EUC」として設定する
 *
 * @access public
 */
public function set_to_encoding_euc() { $this->set_to_encoding_type(1); }
/**
 * 変換後文字エンコードの設定(jis)
 *
 * 変換後文字エンコードを「jis」として設定する
 *
 * @access public
 */
public function set_to_encoding_jis() { $this->set_to_encoding_type(2); }
/**
 * 変換後文字エンコードの設定(UTF-8)
 *
 * 変換後文字エンコードを「UTF-8」として設定する
 *
 * @access public
 */
public function set_to_encoding_utf8() { $this->set_to_encoding_type(3); }


// XXX いるのか？
protected function _is($i)
{
  if ($this->get_to_encoding_type() == $i) {
    return true;
  }
  return false;
}

// XXX
public function is_sjis() { return $this->_is(0); }
public function is_euc() { return $this->_is(1); }
public function is_jis() { return $this->_is(2); }
public function is_utf8() { return $this->_is(3); }

/**
 * 変換
 *
 * @access public
 * @param string $s 変換元文字列
 * @return string 変換後文字列
 */
public function convert($s)
{
  // XXX
  if ("" === $s) {
    return "";
  }

  // to
  if (-1 === $this->get_to_encoding_type()) {
    return "";
  } else {
    $to_encoding_type = $this->encode_type_string_[$this->get_to_encoding_type()];
  }

  // from
  if (-1 === $this->get_from_encoding_type()) {
    $from_encoding_type = 'sjis-win,eucjp-win,SJIS,EUC-JP,JIS,UTF-8,ASCII';
  } else {
    $from_encoding_type = $this->encode_type_string_[$this->get_from_encoding_type()];
  }
//print "type is " . $to_encoding_type . "<br>\n";
//print "$from_encoding_type, to $to_encoding_type\n";

  // コードを置換しぃつつ復帰
  return mb_convert_encoding($s, $to_encoding_type, $from_encoding_type);
}


/**
 * 変換先文字コードの設定
 *
 * XXX 古いので、set_guess_to_stringメソッドを使うようにしてください
 *
 * @deprecated
 * @access public
 * @param string $s 文字コードを意味する文字列("euc", "iso-2022-jp", "utf8", "utf-8"など)
 * @return int 文字コード(このクラス専用の値)
 */
public function set_guess_string($s) { return $this->set_guess_to_string($s); }


/**
 * 変換先文字コードの設定
 *
 * @access public
 * @param string $s 文字コードを意味する文字列("euc", "iso-2022-jp", "utf8", "utf-8"など)
 * @return int 文字コード(このクラス専用の値)
 */
public function set_guess_to_string($s) 
  { return $this->set_to_encoding_type($this->_set_guess_string($s)); }
/**
 * 変換元文字コードの設定
 *
 * @access public
 * @param string $s 文字コードを意味する文字列("euc", "iso-2022-jp", "utf8", "utf-8"など)
 * @return int 文字コード(このクラス専用の値)
 */
public function set_guess_from_string($s)
  { return $this->set_from_encoding_type($this->_set_guess_string($s)); }


/**
 * 文字コードの推測
 *
 * @access public
 * @param string $s 文字コードを意味する文字列("euc", "iso-2022-jp", "utf8", "utf-8"など)
 * @return int 
 * @return int 文字コード(このクラス専用の値): -1:unknown, 0:shift-jis, 1:euc-jp, 2:jis(ISO-2022-JP), 3:utf-8
 */
public function _set_guess_string($s)
{
//print "in $s \n";
  $ret = -1;
  if (preg_match("/euc/i", $s)) {
    $ret = 1;
  } else
  if (preg_match("/sjis/i", $s)) {
    $ret = 0;
  } else
  if (preg_match("/shift[-_]jis/i", $s)) {
    $ret = 0;
  } else
  if (preg_match("/iso-2022-jp/i", $s)) {
    $ret = 2;
  } else
  if (preg_match("/jis/i", $s)) {
    $ret = 2;
  } else
  if (preg_match("/utf[-_]*8/i", $s)) {
    $ret = 3;
  }

  //
  return $ret;
}

//private:
private $from_encoding_type_; // 元文字列のエンコードタイプ
private $to_encoding_type_; // convert後に期待するエンコードタイプ

// PHPの関数用の配列
//private $encode_type_string_ = array("SJIS", "EUC-JP","JIS", "UTF-8");
//private $encode_type_string_ = array("SJIS-win", "eucJP-win","JIS", "UTF-8");
private $encode_type_string_ = array("SJIS-win", "eucJP-win","ISO-2022-JP", "UTF-8");

// content-typeとかのcharset用文字列
private $charset_string_ = array("shift_jis", "euc-jp","iso-2022-jp", "utf-8");

} // end of class

