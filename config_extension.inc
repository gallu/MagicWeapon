<?php

/**
 * config拡張クラス
 *
 * 機能を拡張したコンフィグクラス
 * - @includeによる「別ファイルの取り込み」をサポート
 *   書式例:
 *     @include('/opt/www/products/conf/extension.conf')
 *     @include_wbp('conf/extension.conf')
 *
 * - base_path設定と名前に'_wbp'添加による、基準ディレクトリのサポート
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

require_once('config.inc');

class config_extension extends config {

public function __construct()
{
  //
  parent::__construct();
  //$this->init();
}

//
public function init()
{
  parent::init();
  $this->set_base_path('');
}

//
public function set_base_path($s) { $this->base_path_ = $s; }
public function get_base_path() { return $this->base_path_; }



/**
 * configデータ設定
 *
 * @access public
 * @param array $datas stringなデータの配列
 * @return boolean 何か問題があればfalse
 */
public function set(array $datas)
{
//print "Trap 1\n";
  // まず親処理をやる
  $ret = parent::set($datas);

  /////////////////
  // で、拡張処理
  /////////////////
  // base_pathの取得(内部優先)
  $s = parent::find('base_path');
  if ("" !== $s) {
    $this->set_base_path($s);
//var_dump($s);
  }
//var_dump($this->get_base_path());

  // @includeの処理
  foreach ($datas as $wk) {
    // 先頭のホワイトスペースの除去
    $wk = ltrim($wk);

    // XXX とりあえずベタに作成
    if ( 0 === strncmp('@include', $wk, 8)) {
      $awk = explode('\'', $wk);
      $path = $awk[1];
      if (("" !== $this->get_base_path())&&(0 === strncmp('@include_wbp', $wk, 12))) {
        $path = $this->get_base_path() . $path;
      }
//var_dump($path);
      //
      $this->set_file($path);
    }
  }

  //
  return true;
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
$ret2 = array();

  // まず通常キーの検索
  $ret = parent::find_array($name);

  // base_path設定がある場合拡張検索
  $bp = $this->get_base_path();
//var_dump($bp);
  if ("" !== $bp) {
    $ret2 = parent::find_array($name . '_wbp');
    foreach($ret2 as $wk) {
      $ret[] = $bp . $wk;
    }
  }

  //
  return $ret;
}


//private:
private $base_path_;

} // end of class

