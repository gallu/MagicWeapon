<?php
/**
 * コンフィグレーションファイル 解析クラス
 *
 * いわゆる name = value タイプのconfigを取り込む
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 */

class config {

//
public function __construct()
{
  $this->init();
}

//
public function init()
{
  // 配列にする
  $this->list_ = array();
}

/**
 * configファイル名設定
 *
 * 実際にはparceもやってる
 * より厳密には、parceしてるのはsetメソッドだけど
 *
 * @access public
 * @param string $filename configファイル名
 * @return boolean 何か問題があればfalse
 */
public function set_file($filename)
{
  if ("" == $filename) {
    return false;
  }

  if(!($fp = @fopen($filename, "r"))) {
    return false;
  }

  $ret = array();
  // XXX
  while($wk=fgets($fp, 10240)) {
    $wk = rtrim($wk); // 改行コードの削除
    array_push($ret, $wk);
  }

  //
  return $this->set($ret);
}

/**
 * configデータ設定
 *
 * 実際にはparceもやってる
 *
 * @access public
 * @param array $datas stringなデータの配列
 * @return boolean 何か問題があればfalse
 */
public function set(array $datas)
{
  foreach ($datas as $wk) {
    // 先頭のホワイトスペースの除去
    $wk = ltrim($wk);

    // 空改行は無視
    if ("" === $wk) {
      continue;
    }
    // コメントアウトの除去
    if ('#' === $wk[0]) {
      continue;
    }
    // １文字目が=(nameなし)も無視
    if ('=' === $wk[0]) {
      continue;
    }
    // 文字列に=が一つもなければ無視
    if (false === strpos($wk, '=')) {
      continue;
    }

    // 設定
    // XXX
    array_push($this->list_, $wk);
//print "set $wk\n";
  }
//var_dump($this->list_);

  return true;
}

////////////////////////////////////////////////
// 以下、検索系

/**
 * 単一項目の検索
 *
 * 実際にはラッパー
 *
 * @access public
 * @param string $name 検索キー
 * @return string 検索された値。ない場合は空文字
 */
public function find($name)
{
  if ($ret = $this->find_array($name)) {
    return $ret[0];
  }

  return "";
}

/**
 * 検索
 *
 * @access public
 * @param string $name 検索キー
 * @return array 検索された値。ない場合は空配列
 */
public function find_array($name)
{
//
$ret = array();
//var_dump($name);

  // ぶん回して検索
  foreach ($this->list_ as $wk) {
    if ( 1 == (preg_match( '/^[ \t]*' . $name . '[ \t]*[=][ \t]*(.*)/' , $wk, $buf)) ) {
      // 空改行の除去
      $w = $buf[1];
      $w = trim($w);
      // XXX quoted_string解除をここでやる予定：未実装
      array_push($ret , $w);
    }
  }

  return $ret;
}

/**
 * is系検索
 *
 * 指定されたnameに対応するvalueが、trueとかyesとかonとか、そんな感じならtrueを返す
 *
 * @access public
 * @param string $name 検索キー
 * @return boolean なんか肯定的な値だったらtrue
 */
public function is($name)
{
  $s = $this->find($name);
  $awk = array('true', 'ok', 'yes', '1', 'y');
  $ret = false;
  foreach($awk as $wk) {
    if (0 === strcasecmp($s, $wk)) {
      $ret = true;
      break;
    }
  }

  //
  return $ret;
}


//private:
private $list_;

} // end of class

