<?php

/**
 * formデータ用 文字エンコーディング推測クラス
 *
 * とりあえず日本語限定
 * 後でもうちょいかっつり作りたいってのがあるので、staticにはしない
 * とりあえず暫定仕様は以下のとおり
・ヒンティング文字列のデフォルト名は encoding_hint
・ヒンティング文字列が設定されている場合その文字列を解析対象にする
・ヒンティングがGETとPOSTと両方ある場合POST優先
・設定されていない場合すべてのPOST文字列を対象にする
・POST文字列が空の場合に限り、すべてのGET文字列を対象にする
・ど〜せWin系が多いだろうから判定順番はsjisに比重を置く
 *
 * hinting文字列の推奨としては「美乳」とか「雀の往来」とか「有朋自遠方来」とか。
<INPUT type="hidden" name="encoding_hint" value="雀の往来">
 *
 * 予定では、もうチョイまともなguessルーチンつくって確率で文字コード判定したい
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

class guess_from_data_encoding {

public function __construct()
{
  //
  //parent::__construct();
  $this->init();
}

//
public function init()
{
  $this->set_hinting_name('encoding_hint');
  $this->set_encoding_string('');
}

/**
 * hinting文字列が格納されているnameの設定
 *
 * @access public
 * @param string $s hinting用文字列が格納されているname
 */
public function set_hinting_name($s) { $this->hinting_name_ = $s; }
// XXX protectedでもよくない？
public function get_hinting_name() { return $this->hinting_name_; }

/**
 * 推測される文字コード
 *
 * @access public
 * @return string PHPのmb_detect_encoding関数が返す文字列
 */
public function get_encoding_string() { return $this->encoding_string_; }
//
protected function set_encoding_string($s) { $this->encoding_string_ = $s; }


/**
 * 推測
 *
 * 文字コードの推測を行う
 * XXX 本当は確率にしたいんだけどねぇ…
 *
 * @access public
 * @return boolean 何かあったらfalse
 */
public function guess()
{
  // 対象stringの確定
  $target_string = "";

  // hintingがあったらそっちつかう
  if ("" !== $this->get_hinting_name()) {
    if (true === isset($_POST[$this->get_hinting_name()])) {
      $target_string = $_POST[$this->get_hinting_name()];
    } else
    if (true === isset($_GET[$this->get_hinting_name()])) {
      $target_string = $_GET[$this->get_hinting_name()];
    }
  }
  // なかったらPOST
  if ("" === $target_string) {
    // XXX stdin使おうかと思ったけどファイルuploadが怖いので一端やらん
    $target_string = @implode('', $_POST);
  }
  // POSTすらもなけりゃGET
  if ("" === $target_string) {
    //$target_string = @implode('', $_GET);
    $target_string = urldecode(getenv('QUERY_STRING'));
  }

  // XXX …ここでなけりゃエラーだよねぇ？
  if ("" === $target_string) {
    return false;
  }
//var_dump($target_string);

  // 推測
  // XXX とりあえず推測順番きめうち。後でメソッドにはじき出す？
  // XXX あと第三引数の「厳格なエンコーディング検出」がもうひとつ不明
  //$ec = mb_detect_encoding($target_string, 'sjis-win,eucjp-win,SJIS,EUC-JP,JIS,UTF-8,ASCII');
  //$ec = mb_detect_encoding($target_string,'JIS,eucjp-win,UTF-8,sjis-win,ASCII');
  $ec = mb_detect_encoding($target_string,'JIS,UTF-8,sjis-win,eucjp-win,ASCII');

  // 推測結果を格納
  $this->set_encoding_string($ec);

  //
  return true;
}


//private:
private $hinting_name_;
private $encoding_string_;

} // end of class

