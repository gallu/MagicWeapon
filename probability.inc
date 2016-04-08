<?php

/**
 * (がちゃとか用)確率クラス
 *
 * ざっくりとした「指定された確率に基づいて１つを選ぶ」系クラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

class probability {
  // コンストラクタ(なんできっちりしたコメントなし)
  public function __construct() {
    $this->init();
  }
  // 初期化メソッド(普段は特にcallする必要もないんできっちりしたコメントなし)
  public function init() {
    $this->data_ = new arrayObject();
    $this->total_ = 0;
  }

  // 要素をpush
/**
 * 要素をpush
 *
 * 絶対ではないですが「確率の大きい(高い)もの」から順番にpushしたほうが効率はよいです
 *
 * @access public
 * @param string $item_id アイテムとかカードとかのID
 * @param int $probability_num 確率
 */
  public function push($item_id, $probability_num) {
    // トータルを加算
    $this->total_ += $probability_num;
    // ぶちこむ
    $this->data_[$this->total_] = $item_id;
  }

/**
 * 選択
 *
 * 確率に基づいて、anyから１つを返します
 *
 * @access public
 * @return string アイテムとかカードとかのID
 */
  public function choice() {
    // XXX 乱数はmtでいいかねぇ？
    $i = mt_rand(1, $this->total_);
//var_dump($this->total_);
//var_dump($i);

    // 選び出す
    foreach($this->data_ as $probability_num => $item_id) {
      if ($i <= $probability_num) {
        return $item_id;
      }
    }
    // XXX ここにはこないはずだが…
    return '';
  }

//private:
  private $data_;
  private $total_;
} // end of class
