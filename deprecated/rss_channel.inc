<?php
/**
 * RSS チャンネルクラス
 *
 * RSSのヘッダ部分の管理
 *
 * @deprecated
 * @package magicweapon
 * @access  public
 * @author  nakazawa
 * @create  2006/09/25
 **/


// class of rss_channel start !!
class rss_channel {
  //　初期化
  public function rss_channel(){
    $this->init();
  }

  public function init(){
    $this->title_ = "";
    $this->linkTop_ = "";
    $this->description_ = "";
    $this->language_ = "";
    $this->copyright_ = "";
    $this->image_url_ = "";
    $this->image_link_ = "";
    $this->image_title_ = "";
  }

  //  アクセッサー
  public function setTitle($s){ $this->title_ = $s; }
  public function getTitle(){ return $this->title_; }
  public function setLinkTop($s){ $this->linkTop_ = $s; }
  public function getLinkTop(){ return $this->linkTop_; }
  public function setDescription($s){ $this->description_ = $s; }
  public function getDescription(){ return $this->description_; }
  public function setLanguage($s){ $this->language_ = $s; }
  public function getLanguage(){ return $this->language_; }
  public function setCopyright($s){ $this->copyright_ = $s; } 
  public function getCopyright(){ return $this->copyright_; }
  public function setImageLink($s){ $this->image_link_ = $s; }
  public function getImageLink(){ return $this->image_link_; }
  public function setImageUrl($s){ $this->image_url_ = $s; }
  public function getImageUrl(){ return $this->image_url_; }
  public function setImageTitle($s){ $this->image_title_ = $s; }
  public function getImageTitle(){ return $this->image_title_; }

  public function setCss($s){ $this->css_ = $s; }
  public function getCss(){ return $this->css_; }

  //  処理  ・・・あるのか？


  //　変数宣言部
  private $title_;    //全体タイトル
  private $linkTop_;    //Page URL
  private $description_;  //一言ど?ぞ
  private $language_;    //言語。jaだけっぽ
  private $copyright_;  //こぴぃらいと
  private $image_url_;  //チャンネルのサムネイルイメージURL
  private $image_link_;  //チャンネルのサムネイルイメージリンク先
  private $image_title_;  //チャンネルのサムネイルイメージタイトル

  //
  private $css_;	// CSS

}  // end of class!!
