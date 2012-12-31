<?php

/**
 * ストップウォッチクラス
 *
 * プログラム内の時間計測用。多分チューニングにしか使わない。
 * ざっくり系なので、一端、雪駄下駄なしで作成。
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

class stopwatch {

public function __construct()
{
  //
  //parent::__construct();
  $this->init();
}

// XXX startでどの道初期化するから、initいらないと思う
public function init()
{
}


/**
 * 計測の開始
 *
 * @access public
 */
public function start()
{
  // 開始時間の把握
  $this->time_ = $this->lap_ = microtime(true);
}

/**
 * 計測の終了
 *
 * @access public
 * @return float startからの経過秒
 */
public function stop()
{
  return microtime(true) - $this->time_;
}

/**
 * スプリットタイムの取得
 *
 * 「スプリットタイム」というのは、スタートからの経過時間
 *
 * @access public
 * @return float startからの経過秒
 */
public function split()
{
  return microtime(true) - $this->time_;
}

/**
 * ラップタイムの取得
 *
 * 「ラップタイム」というのは、「前回のラップタイムの取得」からの経過時間。初回は「startからの経過時間」になる。
 * 「ある一定区間にかかった時間」が計測できる、って考えると分かりやすいかも。
 *
 * @access public
 * @return float 前回lapからの経過秒
 */
public function lap()
{
  // 時間把握して
  $m = microtime(true);
  // lap差分計算して
  $t = $m - $this->lap_;
  // 新しいlap基準値を設定して
  $this->lap_ = $m;
  // 返却
  return $t;
}


//private:
private $time_;
private $lap_;


} // end of class

