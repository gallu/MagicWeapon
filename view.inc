<?php

/**
 * Viewクラス
 *
 * 基本的には出力して終わり
 * 全体のcallの流れは以下のとおり

get_contents_body
	model::convertでget_bodyの結果を文字コード変換
	または
	get_bodyで直接取得
		get_body_from_convert
			get_template_filename
				make_template_filename

output_extra
put_contents_header
(echo)

 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  yyyy/mm/dd
 * @version $Id$
 **/

class view {

//
public function __construct() { $this->init(); }

/**
 * 初期化
 *
 * @access public
 */
public function init() { }

/**
 * 出力データの設定
 *
 * 変換とかの必要がない時用のメソッド
 *
 * @access public
 * @param string $s 出力データ
 */
public function set_body($s) { $this->body_ = $s; }

/**
 * httpレスポンスオブジェクトの設定
 *
 * @access public
 * @param object $o cgi_response系インスタンス
 */
public function set_res_obj($o) { $this->res_obj_ = $o; }
/**
 * httpレスポンスオブジェクトの取得
 *
 * @access public
 * @return object cgi_response系インスタンス
 */
public function get_res_obj() { return $this->res_obj_; }

/**
 * 文字コード変換系クラス(multibyte)のインスタンスの設定
 *
 * @access public
 * @param object $o multibyte系インスタンス
 */
public function set_multi_obj($o) { $this->multi_obj_ = $o; }
/**
 * 文字コード変換系クラス(multibyte)のインスタンスの取得
 *
 * @access public
 * @retrun object  multibyte系インスタンス
 */
public function get_multi_obj() { return $this->multi_obj_; }

public function set_model_obj($o) { $this->model_obj_ = $o; }
public function get_model_obj() { return $this->model_obj_; }

/**
 * view系クラスの中核を担うメソッド。あるいは入り口
 *
 * 設定された情報を基準にして「テンプレートファイルを見つけて」「置換処理して」「文字コードそろえて」「http header出力して」「http body出力して」までを一通り。
 *
 * @access public
 * @return boolean 何かがあったらfalse
 */
public function output()
{
  // まず作成
//print $this->get_contents_body();
  $out = $this->get_contents_body();
  if ("" === $out) {
    throw new Exception("body is empty!!");
  }
  //
  $out = $this->output_extra($out);

  // ヘッダ出力
  $this->put_contents_header();

  // body出力
  // echoが早いとか言う話なので…まぁ一応
  //print $out;
  echo $out;

  //
  return true;
}


/**
 * コンテンツ本体を取得する：文字コード変換のラッパー
 *
 * @access protected
 * @return boolean 何かがあったらfalse
 */
protected function get_contents_body()
{
  //
  $res = $this->get_res_obj();
  $mobj = $this->get_multi_obj();

  // bodyの出力
  if ($res->is_need_convert()) {
    // 文字系っぽいので置換して出力〜
    return $mobj->convert( $this->get_body() );
  }
  // else
  // 文字コード置換必要なさそうなのでそのまま〜
  return $this->get_body();
}

/**
 * コンテンツ本体を取得する：本体
 *
 * @access protected
 * @return boolean 何かがあったらfalse
 */
protected function get_body() {
 if ("" != $this->body_) {
   return $this->body_;
  }
  // else
  return $this->get_body_from_convert();
}

/**
 * bodyが空のときにmodelからごりごり作成
 *
 * XXX 多分、本当は「かつダイレクトフラグが云々」とかやったほうがいいんだろうなぁ、っと
 *
 * @access protected
 * @return string 何かがあったら空文字
 */
protected function get_body_from_convert()
{
//print "get_body_from_convert \n";
  // 必要なインスタンスげと
  $mobj = $this->get_model_obj();
  $incode = $mobj->get_inside_encoding_obj();

  // テンプレートの文字コードチェック
  $s = $this->get_model_obj()->get_config()->find('template_encoding_type');
  if ('' === $s) {
    // XXX 微妙に怖いなぁここ…
    $incode->set_from_encoding_unknown();
  } else {
    $incode->set_guess_from_string($s);
  }

  // テンプレートエンジンインスタンスげと
  // XXX 判定微妙だねぇ…
  $conv = $mobj->get_conv();
  if (true === is_subclass_of($conv, 'Smarty')) {
    // テンプレートファイル名を取得
    $fn = $this->get_template_filename();
    // smarty的処理
    $out = $conv->fetch($fn);
  } else { // elseだとconv想定
    // テンプレートファイルを文字列として取得
    $ts = file_get_contents($this->get_template_filename());
    //
    $out = $conv->conv($incode->convert($ts));
  }

  //
  return $out;
}


/**
 * テンプレートファイル名の取得
 *
 * 実処理はmake_template_filename。ここは「ファイルの存在チェック」が追加されてるくらい
 * XXX 別クラスにしなくてもいいかなぁ？
 *
 * @access protected
 * @return string 何かがあったら空文字
 */
protected function get_template_filename()
{
  $tf = $this->make_template_filename();
//var_dump($tf);

  // ファイルが存在しなければはじく
  if (false === is_file($tf)) {
    throw new Exception("template file not find!! ($tf)");
  }

  //
  return $tf;
}

/**
 * テンプレートファイル名の取得：実処理
 *
 * 「user-agentでテンプレートファイルを切り替えたい」とかはこの子を上書きする形で継承クラスを作成する
 *
 * @access protected
 * @return string 何かがあったら空文字
 */
protected function make_template_filename()
{
  // 必要なインスタンスげと
  $mobj = $this->get_model_obj();
//var_dump($mobj);

  // テンプレートファイルのfullディレクトリ作成
//var_dump($mobj);
//var_dump($mobj->get_tmp_filename());
//var_dump($tf);
  return $mobj->get_config()->find('template_dir') . $mobj->get_tmp_filename();
}

/**
 * 出力直前での処理：置換とか全部終わった後
 *
 * 特別な処理をするときはここに上書いて
 *
 * @access protected
 * @return string 何かがあったら空文字
 */
protected function output_extra($s) {
  return $s;
}


/**
 * HTTP response headerの出力
 *
 * @access protected
 * @return string 何かがあったら空文字
 */
protected function put_contents_header()
{
  //
  $res = $this->get_res_obj();
//var_dump($res);
  // ヘッダの出力
  $res->set_nocache();
  $res->put();
}




//private:
private $body_;
private $res_obj_;
private $multi_obj_;
private $model_obj_; // XXX 場当たり？

} // end of class

