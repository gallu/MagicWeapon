<?php
/***************************************************
 *
 * Class Name : rss_item
 * Abstract   : RSSの個別要素
 *            : 
 * @deprecated
 * Author     : nakazawa
 * Date       : 2006/09/25
 *
 ***************************************************/

// class of rss_item start !!
class rss_item {

//  変数宣言部
private $title_;			//日本語
private $link_;				//URL
private $pubDate_;			//日付
private $guid_;				//URL？
private $description_;		//一言ど?ぞ
private $enclosure_url_;
private $enclosure_length_;
private $enclosure_type_;
// XXX 使用に当たっては細心の注意を!!
private $contents_;

	//　初期化
	public function rss_item(){
		$this->init();
	}

	public function init(){
		$this->title_ = "";
		$this->link_ = "";
		$this->pubDate_ = "";
		$this->guid_ = "";
		$this->description_ = "";
		$this->enclosure_url_ = "";
		$this->enclosure_length_ = "";
		$this->enclosure_type_ = "";
		$this->contents_ = "";
	}

	//  アクセッサー
	public function setTitle($s){ $this->title_ = $s; }
	public function getTitle(){ return $this->title_; }
	public function setLink($s){ $this->link_ = $s; }
	public function getLink(){ return $this->link_; }
	public function setPubDate($s){ $this->pubDate_ = $s; }
	public function getPubDate(){ return $this->pubDate_; }
	public function setGuid($s){ $this->guid_ = $s; }
	public function getGuid(){ return $this->guid_; }
	public function setDescription($s){ $this->description_ = $s; }
	public function getDescription(){ return $this->description_; }

	public function setEnclosureUrl($s) { $this->enclosure_url_ = $s; }
	public function getEnclosureUrl() { return $this->enclosure_url_; }
	public function setEnclosureLength($s) { $this->enclosure_length_ = $s; }
	public function getEnclosureLength() { return $this->enclosure_length_; }
	public function setEnclosureType($s) { $this->enclosure_type_ = $s; }
	public function getEnclosureType() { return $this->enclosure_type_; }

	public function setContents($s) { $this->contents_ = $s; }
	public function getContents() { return $this->contents_; }

	//  処理　　・・・ないよね？

}	// end of class !!
