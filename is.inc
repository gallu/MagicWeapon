<?php
/**
 * ISクラス
 *
 * 様々な文字列判定用クラス
 * 基本的に is::メソッド名 で使うこと
 * XXX とりあえず正規表現。重いからそのうち改修したいなぁ
 * XXX 正規表現自体も、POSIX Regex と Perl Regex と混ざってるし…
 *
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

// is_date用
require_once('calendar.inc');

class is
{


/**
 * 英文字または数字かどうかを判定
 *
 * @access public
 * @param string $s 判定する文字列
 * @return boolean 全て英数字ならtrue、異なる文字が含まれていればfalse
 */
static public function is_alphanum($s){
  // 引数が整数型なら速やかにOK
  // XXX 小数はどうすっかねぇ？
  if (true === is_int($s)) {
    return true;
  }

  //
  return ctype_alnum($s);
}



/**
 * 数字かどうかを判定
 *
 * @access public
 * @param string $s 判定する文字列
 * @return boolean 全て数字ならtrue、異なる文字が含まれていればfalse
 */
static public function is_digit($num){
  // 引数が整数型なら速やかにOK
  // XXX 小数はどうすっかねぇ？
  if (true === is_int($num)) {
    return true;
  }
  //
  return ctype_digit($num);
}


/**
 * 仮名文字かどうか判定(EUC用)
 *
 * あとで文字コード別に作り直さないと…
 *
 * @deprecated
 * @access public
 * @param string $s 判定する文字列
 * @return boolean 全て仮名文字ならtrue、異なる文字が含まれていればfalse
 */
  static public function is_kana($str){
    if(!ereg( "^(\xA5[\xA1-\xF6]|\xA1\xBC|\xA1\xA6|\xA1\xA1|\x20)+$", $str)){
      return false;
    }
    return true;
  }

/**
 * emailアドレス判定
 *
 * 
 *
 * @access public
 * @param string $s 判定する文字列
 * @return boolean emailとしてvalidな文字列ならtrue、invalidならfalse
 */
static public function is_email ($s) {
  // メアドの分解
  $awk = explode('@', $s);
  $domain_part = array_pop($awk);
  $local_part = implode('@', $awk);
  //
//var_dump($local_part);
//var_dump($domain_part);

  // ドメイン部分チェック
  $r = self::is_domain($domain_part); // XXX 本当はstatic::がよいんだが、バージョン低い所あるだろうし…弊害とりあえずないし…
  if (false === $r) {
    return false;
  }

  // local-partは雑に判定：文字があればいいよね？
  if (0 === strlen($local_part)) {
    return false;
  }

  // else
  return true;
}

/**
 * ドメイン判定
 *
 * @access public
 * @param string $s 判定する文字列
 * @return boolean ドメインとしてvalidな文字列ならtrue、invalidならfalse
 */
static public function is_domain($s){
  // 文字長取得
  $len = strlen($s);

  // XXX もうちょっとminが長い気もするんだが…とりあえず「セカンドレベル１文字＋TLD２文字」をminとして想定
  // XXX 暫定なので、後で多分書き換える
  if (4 > $len) {
    return false;
  }

  // １文字目
  if (false === ctype_alpha($s[0])) {
    return false;
  }

  // ２文字目以降
  $period_flg = false;
  for($i = 1; $i < $len; $i ++) {
    // 文字把握
    $c = $s[$i];

    // 英数またはハイフンならOK
    if (ctype_alnum($c) || ('-' === $c)) {
      $period_flg = false;
      continue;
    }
    // 「連続していない」ピリオドならOK
    if ( (false === $period_flg)&&('.' === $c) ) {
      $period_flg = true;
      continue;
    }
    // else
    return false;
  }

  // 最後の文字
  if (false === ctype_alnum($s[$len - 1])) {
    return false;
  }

  // 一通り通ったのでOK
  return true;
}


/**
 * URI判定
 *
 * XXX …微妙
 *
 * @access public
 * @param string $s 判定する文字列
 * @return boolean URIとしてvalidな文字列ならtrue、invalidならfalse
 */
  static public function is_url($url){
    if (preg_match('/^(https?|ftp)(:\/\/[-_.!~*\'()a-zA-Z0-9;\/?:\@&=+\$,%#]+)$/', $url)) {
      return true;
    }
    // else 
    return false;
  }

/**
 * ディレクトリ名判定
 *
 * XXX …微妙
 *
 * @access public
 * @param string $s 判定する文字列
 * @return boolean ディレクトリ名としてvalidな文字列ならtrue、invalidならfalse
 */
static function is_dir($s) {
  if (preg_match("/[^0-9a-zA-Z\-_]+/", $s)) {
    return false;
  }
  return true;
}

/**
 * 日付判定
 *
 * @access public
 * @param string $s 判定する文字列
 * @return boolean 日付としてvalidな文字列ならtrue、invalidならfalse
 */
static function is_date($s) {
  $cal = new calendar;
  $cal->set_string($s);

  //
  return $cal->is();
}

/**
 * カタカナかどうか判定(EUC用)
 *
 * あとで文字コード別に作り直さないと…
 *
 * @deprecated
 * @access public
 * @param string $s 判定する文字列
 * @return boolean 全てカタカナならtrue、異なる文字が含まれていればfalse
 */
static public function is_katakana($s) {
mb_regex_encoding('euc-jp');
  $ret = mb_ereg("^[アイウエオカキクケコサシスセソタチツテトナニヌネノハヒフヘホマミムメモヤユヨラリルレロワヲンガギグゲゴザジズゼゾダヂヅデドバビブベボパポピプペポァィゥェォッャュョー]*$", $s);
  // XXX 暫定なのでとりあえずざっくり
  if (false === $ret) {
    return false;
  }
  // else
  return true;
}


/**
 * カタカナかどうか判定(UTF-8用)
 *
 * @access public
 * @param string $s 判定する文字列
 * @return boolean 全てカタカナならtrue、異なる文字が含まれていればfalse
 */
static public function is_katakana_utf8($s) {
  // XXX …ど～だろ
  mb_regex_encoding('UTF-8');
  $ret = mb_ereg("\A[ァ-ヾ]+\z", $s);

  // XXX 暫定なのでとりあえずざっくり
  if (false === $ret) {
    return false;
  }
  // else
  return true;
}

/**
 * ひらがなかどうか判定(UTF-8用)
 *
 * 長音は許可するよ～
 *
 * @access public
 * @param string $s 判定する文字列
 * @return boolean 全てカタカナならtrue、異なる文字が含まれていればfalse
 */
static public function is_kana_utf8($s) {
  // XXX …ど～だろ
  mb_regex_encoding('UTF-8');
  $ret = mb_ereg("\A[ぁ-ゞー]+\z", $s);

  // XXX 暫定なのでとりあえずざっくり
  if (false === $ret) {
    return false;
  }
  // else
  return true;
}



/**
 * 郵便番号判定(日本)
 *
 * @access public
 * @param string $s 判定する文字列
 * @return boolean 郵便番号としてvalidな文字列ならtrue、invalidならfalse
 */
static public function is_zip($s)
{
  if (preg_match("/^[0-9][0-9][0-9][\- ]?[0-9][0-9][0-9][0-9]$/", $s)) {
    return true;
  }
  // else
  return false;
}

/**
 * 郵便番号判定(日本)
 *
 * @access public
 * @param string $s 判定する文字列(上３桁)
 * @param string $s 判定する文字列(下４桁)
 * @return boolean 郵便番号としてvalidな文字列ならtrue、invalidならfalse
 */
static public function is_zip2($s1, $s2)
{
  if (3 !== strlen($s1)) {
    return false;
  }
  if (4 !== strlen($s2)) {
    return false;
  }
  // XXX まとめて
  return is::is_digit($s1)&&is::is_digit($s2);
}


/**
 * 電話番号判定(日本)
 *
 * @access public
 * @param string $s 判定する文字列
 * @return boolean 電話番号としてvalidな文字列ならtrue、invalidならfalse
 */
static public function is_tel($s)
{
  if (preg_match("/^0[0-9]{1,4}[\- ]?[0-9]{1,4}[\- ]?[0-9]{4}$/", $s)) {
    return true;
  }
  // else
  return false;
}




} // end of class
