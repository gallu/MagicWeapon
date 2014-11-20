<?php

/**
 * mail用ユーティリティクラス
 *
 * mail用のユーティリティクラス
 * ユーティリティ系クラスなので、全て
 * mail_util::メソッド
 * の形式でのcallを想定している
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/
require_once('mw_area_setting.inc');
require_once('internet_message_format.inc');

class mail_util {


/**
 * メールアドレスの抜き出し
 *
 * 名前 <メールアドレス> の形式から、メールアドレスだけを抜き出す
 *
 * @access public
 * @param string $s メールアドレス
 * @return string 抜き出したアドレス部分。エラーは空文字復帰
 */
static public function carve_address($s)
{
  if (1 === preg_match('/[<](.*)[>]/', $s, $mt)) {
    return $mt[1];
  }
  // else
  return trim($s);
}

/**
 * 可変エンベロープリターンパス (VERP : variable envelope return path) 作成
 *
 * エラーメールを取得する用のごにょり
 * 例)
 * mail_util::make_verp('error@test.com', 'm.furusho@gmail.com');
 * を
 * error+m.furusho=gmail.com@test.com
 * にする。
 *
 * @access public
 * @param string $e エラー到達用メールアドレス
 * @param string $t toメールアドレス(中に埋め込む
 * @param string $d メールアドレスのデリミタ : とりあえずPostfixのデフォ＋を
 * @return string return-pathに設定すべきメールアドレス
 */
static public function make_verp($e, $t, $d = '+')
{
  // まず分解
  $wk = explode('@', $e);

  // toアドレスの＠を置換
  $t = strtr($t, '@', '=');

  // 足し込んで復帰
  $s = $wk[0] . $d . $t . '@' . $wk[1];
  return $s;
}

/**
 * テンプレートからのinternet_message_formatインスタンスの作成 modelからcall用
 *
 * mail headerをテンプレートにまとめて書けるのが便利
 *
 * @access public
 * @param model $model model系インスタンス
 * @param string $filename テンプレートファイル名
 * @return object internet_message_formatインスタンス
 */
static public function make_mailobj_from_template_with_model($model, $filename)
{
  // 呼び出す
  $fn = $model->get_config()->find('template_dir') . '/' . $filename;
//var_dump($fn);
  $template_encode = $model->get_config()->find('template_encoding_type');
//var_dump($template_encode);

  //
  return self::make_mailobj_from_template($fn, $template_encode);
}


/**
 * テンプレートからのinternet_message_formatインスタンスの作成 本体
 *
 * mail headerをテンプレートにまとめて書けるのが便利
 *
 * @access public
 * @param string $filename テンプレートファイル名
 * @param string $file_encode ファイルのエンコード
 * @return object internet_message_formatインスタンス
 */
static public function make_mailobj_from_template($filename, $file_encode = '')
{
//var_dump($filename);
  // 文字コードの確定
  $eobj = new multibyte;
  // fromは引数次第
  if ('' === $file_encode) {
    // デフォルトは、まだ当分 sjis かねぇ？
    $eobj->set_from_encoding_sjis();
  } else {
    // 仰せのままに パパラパ〜
    $eobj->set_guess_from_string($file_encode);
  }
  // toは内部エンコードにする
  $eobj->set_guess_to_string(mw_area_setting::get_inside_encoding_type());

  // ファイル見つけて
  $s = file_get_contents($filename);
  if (true === empty($s)) {
    return null;
  }

  // 文字コードを変換しておく
  $s = $eobj->convert($s);
//var_dump($s);

  // 実処理はむこうさんにお願い〜
  $mobj = new internet_message_format;
  $r = $mobj->set_mail_string($s);
//var_dump($r);
//var_dump($mobj);

  //
  return $mobj;
}

/**
 * 可変エンベロープリターンパス (VERP : variable envelope return path) からアド>レスを戻す
 *
 * エラーメールを取得する用のごにょり
 * 例)
 * error+m.furusho=gmail.com@test.com
 * を
 * m.furusho@gmail.com
 * にする。
 *
 * @access public
 * @param string $e エラー到達用メールアドレスの先頭(上述だと'error'を指定する
 * @param string $t toメールアドレス(中に埋め込む
 * @param string $d メールアドレスのデリミタ : とりあえずPostfixのデフォ＋を
 * @return string 復元したメールアドレス。問題があったら空文字
 */
static public function back_verp($e, $t, $d = '+')
{
  // 元のアドレスを合成する
  $to = mail_util::carve_address($t);
//var_dump($to);
  // XXX errormail以外なら除外
  if (0 !== strncmp($e . $d, $to, strlen($e . $d))) {
    return '';
  }
//print "Trap 1\n";
  // 前後をぶった切る
  list($dummy, $to) = explode('+', $to, 2);
  list($to, $dummy) = explode('@', $to, 2);
  // =を@に戻す
  $to = str_replace('=', '@', $to);
//var_dump($to);

  //
  return $to;
}



} // end of class
