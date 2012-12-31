<?php

//
// RSSアウトプットの基底クラス
//
/**
 *
 * @deprecated
 */

require_once("conv.inc");
require_once("conv_util.inc");

abstract class rss_output_base
{
// public 

public function __construct() { $this->init(); }
public function init() {
  $this->rss_obj_  = null;
}

public function set_data(rss_data $obj) { $this->rss_obj_ = $obj; }

/**
 * get_string
 * rss_dataオブジェクトの情報からRSS作成ストリングとして返す。
 * 基本は、腹にかかえたテンプレートを置換していく。
 */
public function get_string() {

  //////
  // rss_dataがあるかチェック

  if(null == $this->_rss_obj()){  // rss_dataがnullなら終了。
//print "rss_obj is null!\n";
    return "";                    // 戻り値はストリングなので空文字。
  }

  //
  $rssdata = $this->_rss_obj();   // RSSのデータが格納されたオブジェクト
  $conv    = new f_conv();        // テンプレート置換してくれる

  //////
  // channel部分の単置換

  $channel = $rssdata->getChannelObj();

  $conv->monoDic("channel_language", $channel->getLanguage());
  //$conv->monoDic("channel_about", "固定のファイル名");
  $conv->monoDic("channel_title", $channel->getTitle());
  $conv->monoDic("channel_link", $channel->getLinkTop());
  $conv->monoDic("channel_description", $channel->getDescription());
  $conv->monoDic("channel_copyright", $channel->getCopyright());

  // CSS
  // XXX ちょいとずるいやり方
//print_r($channel);
  if ("" != $channel->getCss()) {
    $css_obj = new simple_put();
    $conv->multiDic("channel_css_put", $css_obj);
    $conv->monoDic("channel_css_url", $channel->getCss());
  }


  //////
  // image部分の単置換

  $conv->monoDic("image_resource", $channel->getImageUrl());
  $conv->monoDic("image_about", $channel->getImageUrl());
  $conv->monoDic("image_url", $channel->getImageUrl());
  $conv->monoDic("image_title", $channel->getImageTitle());
  $conv->monoDic("image_link", $channel->getImageLink());

  //////
  // textinput部分の単置換
/*
  // rss_dataオブジェクトの方で未実装？

  $conv->monoDic("textinput_resource", $channel->get());
  $conv->monoDic("textinput_about", $channel->get());
  $conv->monoDic("textinput_title", $channel->get());
  $conv->monoDic("textinput_description", $channel->get());
  $conv->monoDic("textinput_name", $channel->get());
  $conv->monoDic("textinput_link", $channel->get());
*/

  //////
  // fetchしながらmultiDicに渡すオブジェクトを作成
  // "rdf:li"部分と<item>部分を同時に。

  $liloop   = new li_loop();    // m_convがあるobj(rdf:li部分)
  $itemloop = new item_loop();  // m_convがあるobj(<item>部分)
  while($rssdata->fetchItems()) {
    $liloop->add_data($rssdata->getItemObj());
    $itemloop->add_data($rssdata->getItemObj());
  }
  $conv->multiDic("li_loop", $liloop);
  $conv->multiDic("item_loop", $itemloop);

  $ret = $conv->conv($this->rss_template());
  return $ret;
}

// protected
protected function _rss_obj() { return $this->rss_obj_; }

/**
 * RSSのテンプレート１本の文字列で返す。
 * バージョンごと、あるいはその時使用する要素などに応じて
 * 適宜変更して下さい。
 *
 * 戻り値 : string
 * 引数 : 無し
 */
abstract protected function rss_template();

//private
private $rss_obj_;   // RSSのデータを保持しているオブジェクト
                                   
};  // end of class

////////////////////////////////////////////////////////////////
// XXX multiDicに渡す用(m_conv()を持ってる)クラス
// とりあえずここに書く。これで行くと決まったら
// 別ファイルにするかも？(もしくはsimple_loop使用とか)

class li_loop
{
// public 
  public function __construct() { $this->init(); }
  public function init() { $this->data_ = array(); }

  public function add_data(rss_item $obj) { $this->data_[] = $obj; }
  public function set_data(array $ary) {
    foreach($ary as $a) { $this->add_data($a); }
  }

  public function m_conv($body, $param) {

    //////
    // データが空配列なら終了
    if (0 == count($this->data_)) { 
//print "li_loop: data is empty!\n";
      return "";
    } 
    
    //////
    // 置換しながらループ
    $ret = "";  // 結果格納用
    $c   = new f_conv();
    foreach ($this->data_ as $d) {  // $dはrss_itemオブジェクト
      $c->monoDic("li_resource", $d->getLink());
      $ret .= $c->conv($body);
    }

    //////
    // 復帰
    return $ret;
  }

// private
  private $data_ = array();  // rss_itemの配列
                             // XXX この場合無駄なデータも多いけど…
};

//
// 上のクラスとかぶりまくりだけど…
// 共通化等々はこれで行くと確定してから。
//
class item_loop
{
// public 
  public function __construct() { $this->init(); }
  public function init() { $this->data_ = array(); }

  public function add_data(rss_item $obj) { $this->data_[] = $obj; }
  public function set_data(array $ary) {
    foreach($ary as $a) { $this->add_data($a); }
  }

  public function m_conv($body, $param) {

    //////
    // データが空配列なら終了
    if (0 == count($this->data_)) {
//print "item_loop: data is empty!\n";
      return "";
    } 
    
    //////
    // 置換しながらループ
    $ret = "";  // 結果格納用
    $c   = new f_conv();
    foreach ($this->data_ as $d) {  // $dはrss_itemオブジェクト
      $c->monoDic("item_about", $d->getLink());
      $c->monoDic("item_title", $d->getTitle());
      $c->monoDic("item_link", $d->getLink());
      $c->monoDic("item_description", $d->getDescription());
      $c->monoDic("item_contents", $d->getContents());
      $c->monoDic("item_pubDate", $d->getPubDate());
      $c->monoDic("item_guid", $d->getGuid());
      $c->monoDic("enclosure_url", $d->getEnclosureUrl());
      $c->monoDic("enclosure_length", $d->getEnclosureLength());
      $c->monoDic("enclosure_type", $d->getEnclosureType());
      $ret .= $c->conv($body);
    }

    //////
    // 復帰
    return $ret;
  }

// private
  private $data_ = array();  // rss_itemの配列

};

