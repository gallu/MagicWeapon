<?php
/**
 * RSS データクラス
 *
 * RSSを表す。
 * RSSチャンネルと個別要素で構成される
 *
 * @deprecated
 * @package magicweapon
 * @access  public
 * @author  nakazawa
 * @create  2006/09/25
 **/

// class of rss_data start !!
class rss_data {
  
  //　初期化
  public function rss_data(){
    $this->init();
  }

  public function init(){
    $this->channleObj_ = "";
    $this->itemObjAry_ = array();    // どうなんだろね？この初期化は。
    $this->cursor_ = -1;
  }

  //  アクセッサー
  public function setChannelObj($o){ $this->channelObj_ = $o; }
  public function getChannelObj(){ return $this->channelObj_; }

  public function putItem($o){ array_push( $this->itemObjAry_,$o ); }
  public function getAllItems(){ return $this->itemObjAry_; }
  // ↑setterはいらないよね？↑

  //  通るのか？これ。。。
  public function getItemObj(){ return $this->itemObjAry_[$this->getPos()]; }
  public function cntItems(){ return count($this->itemObjAry_); }

  public function clrPos(){ $this->cursor_ = -1; }
  public function incPos(){ $this->cursor_ += 1 ; }
  public function getPos(){ return $this->cursor_; }

  //  処理

  //  ポインタをインクリメントして次の配列要素を示せ。
  public function fetchItems(){
    //　とりあえずインクリメントして・・・
    $this->incPos();

    if($this->getPos() >= $this->cntItems()){
      //  おいおい。配列の範囲を超えちゃったよ！
      $this->clrPos();
      return false;
    }
    return true;
  }

  //  ん?、なんだろ？フェッチ系のイニシャライズ？
  //  ってことはポインターのクリアでいいのかな？
  public function fetchItemsInit(){
    $this->clrPos();
  }

  //  変数宣言部
  private $channelObj_;    //チャンネルObj
  private $itemObjAry_;    //itemObjが格納される配列
  private $cursor_;      //ポインター代わり

}  // end of class !!
