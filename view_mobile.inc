<?php

/**
 * 携帯用Viewクラス
 *
 * 絵文字変換入れた
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  yyyy/mm/dd
 * @version $Id$
 **/

require_once('emoji_convert.inc');
require_once('view.inc');
require_once('mobile_info.inc');

class view_mobile extends view {

//
public function __construct() { parent::__construct(); }

public function output_extra($s)
{
  // まず上位処理
  $s = parent::output_extra($s);

  // 絵文字変換処理
  if ($this->get_res_obj()->is_need_convert()) {
//print "Trap\n";
    $obj = new emoji_convert;
    $s = $obj->convert($s);
  }

  // XXX 場当たり的uid処理
  $uid = "";
  if (isset($_POST['uid'])) {
      $uid = $_POST['uid'];
  } else
  if (isset($_GET['uid'])) {
    $uid = $_GET['uid'];
  }
  // PCでのデバッグ & DocomoのSSL 用
  $wk = $this->get_model_obj()->get_mobile();
  if ( (false === $wk->is_mobile()) ||
       ( (true === $wk->is_docomo()) && ((false === empty($_SERVER['HTTPS']))&&('off' !== $_SERVER['HTTPS'])) )
     ) {
    if ("" !== $uid) {
      $s = str_replace('NULLGWDOCOMO', $uid, $s);
      $s = str_replace('guid=on', 'uid=' . $uid, $s);
    }
  }
//var_dump($uid);

  //
  // NULLGWDOCOMO削除処理…ここかなぁ？
  // 一緒にguid=onも削除
  // XXX 多分本当は「docomoならadd」のが良いんだろうけどねぇ orz
  $wk = $this->get_model_obj()->get_mobile();
  if (false === $wk->is_docomo()) {
    //$s = str_replace('?uid=NULLGWDOCOMO&', '?', $s);
    //$s = str_replace('?uid=NULLGWDOCOMO', '', $s);
    //$s = str_replace('&uid=NULLGWDOCOMO&', '&', $s);
    //$s = str_replace('&uid=NULLGWDOCOMO', '', $s);
    //$s = str_replace('?guid=on&', '?', $s);
    //$s = str_replace('?guid=on', '', $s);
    //$s = str_replace('&guid=on&', '&', $s);
    //$s = str_replace('&guid=on', '', $s);

    $s = str_replace(array('?uid=NULLGWDOCOMO&',
                           '?uid=NULLGWDOCOMO',
                           '&uid=NULLGWDOCOMO&',
                           '&uid=NULLGWDOCOMO',
                           '?guid=on&',
                           '?guid=on',
                           '&guid=on&',
                           '&guid=on' ) ,
                      array('?', '', '&', '', '?', '', '&', '' ) ,
                      $s);
  }

  //
  return $s;
}

/**
 * 指定されたファイル名に基づいたファイルを読み込んで文字列を返す
 *
 * 結構複雑な処理をしてる。
 * ・とりあえずXML優先
 * ・一端、拡張子をxtpl、tplに固定してる(後で直すと思うけど
 * ・拡張子がxtplならXML(XHTML)と判断。ファイル先頭のXML宣言を切り替える
 * ・拡張子がxtplでキャリアがdocomoの場合ヘッダを変える
 * ・テンプレートディレクトリを「機種名考慮」にする
 *   検索順番としては
 *   template_dir/機種固有名/tfile.tpl
 *   template_dir/キャリア(i | s | e)/tfile.tpl
 *   template_dir/mobile/tfile.tpl
 *   template_dir/tfile.tpl
 *
 * @access public
 * @return string ファイルがそも存在しない場合は例外を投げる
 */
public function get_template_string()
{
  // 必要なインスタンスげと
  $mobj = $this->get_model_obj();
//var_dump($mobj);


  // まずファイル確定
  // 可能性のあるパスを一式くみ上げる
  // XXX 多分あとでメソッド外だし

  // 必要なインスタンスげと
  $mobj = $this->get_model_obj();
//var_dump($mobj);

  // テンプレートディレクトリげと
  $tdir = $mobj->get_config()->find('template_dir');

//var_dump( $mobj->get_tmp_filename() );
  // ファイル名から拡張子省いたのげと
  if (!(false === strpos($mobj->get_tmp_filename(), '.tpl'))) {
    $base_tmp_fn =  $mobj->get_tmp_filename();
    $base_tmp_xfn =  substr($base_tmp_fn, 0, (strlen($base_tmp_fn)-3)) . 'xtpl';
  } else {
    $base_tmp_xfn =  $mobj->get_tmp_filename();
    $base_tmp_fn =  substr($base_tmp_xfn, 0, (strlen($base_tmp_xfn)-4)) . 'tpl';
  }
//var_dump($base_tmp_fn);
//var_dump($base_tmp_xfn);

  // mobile_infoげと
  $mobobj = $mobj->get_mobile();

  // 組み立て
  $guess_paths = array();

  // template_dir/機種固有名/tfile.tpl
  $guess_paths[] = $tdir . '/' . $mobobj->get_type() . '/' . $base_tmp_xfn;
  $guess_paths[] = $tdir . '/' . $mobobj->get_type() . '/' . $base_tmp_fn;

  // template_dir/キャリア(i | s | e)/tfile.tpl
  if (true === $mobobj->is_ez()) {
    $c = 'e';
  } else
  if (true === $mobobj->is_softbank()) {
    $c = 's';
  } else {
    // デフォルトはとりあえずdocomoで
    $c = 'i';
  }
  $guess_paths[] = $tdir . '/' . $c . '/' . $base_tmp_xfn;
  $guess_paths[] = $tdir . '/' . $c . '/' . $base_tmp_fn;

  // template_dir/mobile/tfile.tpl
  $guess_paths[] = $tdir . '/mobile/' . $base_tmp_xfn;
  $guess_paths[] = $tdir . '/mobile/' . $base_tmp_fn;

  // template_dir/tfile.tpl
  $guess_paths[] = $tdir . '/' . $base_tmp_xfn;
  $guess_paths[] = $tdir . '/' . $base_tmp_fn;
//var_dump($guess_paths);

  // 文字列取得
  $flg = false;
  $template_file = "";

  // 存在しているファイルを探し出す
  foreach($guess_paths as $p) {
    if (true === is_file($p)) {
      $flg = true;
      $template_file = $p;
      break;
    }
  }
  // ファイルが存在しなければはじく
  if (false === $flg) {
    throw new Exception("template file not find!! ( $p ) ");
  }

  // ファイル文字列を取得
  $ret = file_util::read_file($template_file);

  // XMLなら…
  if (!(false === strpos($template_file, '.xtpl'))) {
    // 先頭にXML宣言があったら取り除く
    // <?xml
    $pt = '/[\r\n]*<\?xml(.*)?>[\r\n]*/';
    $ret = preg_replace($pt, '', $ret);
    // <!DOCTYPE
    $pt = '/[\r\n]*<!DOCTYPE(.*)?>[\r\n]*/';
    $ret = preg_replace($pt, '', $ret);
    // <html
    $pt = '/[\r\n]*<html(.*)?>[\r\n]*/';
    $ret = preg_replace($pt, '', $ret);

    // XML宣言を個別にadd
    if ($mobobj->is_docomo()) {
      $wk = '<?xml version="1.0" encoding="Shift_JIS" ?>
<!DOCTYPE html PUBLIC "-//i-mode group (ja)//DTD XHTML i-XHTML(Locale/Ver.=ja/2.2) 1.0//EN" "i-xhtml_4ja_10.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="ja">';
      // ついでにheaderを切り替える
      $mobj->get_cgi_response()->set_content('application/xhtml+xml; charset=Shift_JIS');
    } else
    if ($mobobj->is_ez()) {
      $wk = '<?xml version="1.0" encoding="Shift_JIS" ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.0//EN" "http://www.w3.org/TR/xhtml-basic/xhtml-basic10.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="ja">';
    } else
    if ($mobobj->is_softbank()) {
      $wk = '<?xml version="1.0" encoding="Shift_JIS" ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.0//EN" "http://www.w3.org/TR/xhtml-basic/xhtml-basic10.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="ja">';
    } else {
      // PCとか用
      $wk = '<?xml version="1.0" encoding="Shift_JIS" ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.0//EN" "http://www.w3.org/TR/xhtml-basic/xhtml-basic10.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="ja">';
    }
    // 宣言を付け足しておく
    $ret = $wk . $ret;
  }
//var_dump($ret);

  // XXX is_docomoとかuriとか、ここで足しておく?
/*
  $uri_ = $this->get_model_obj()->get_config()->find("uri");
  $uri_ssl_ = $this->get_model_obj()->get_config()->find("uri_ssl");
  $mx_domain_ = $this->get_model_obj()->get_config()->find("mx_domain");
  $conv->monoDic("uri", $uri_);
  $conv->monoDic("uri_ssl", $uri_ssl_);
  $conv->monoDic('mx_domain', $mx_domain_);
*/

  //
  return $ret;
}

//private:

} // end of class

