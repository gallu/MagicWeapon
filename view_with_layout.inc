<?php

/**
 * layoutファイルを適用するviewクラス
 *
 * デフォルトでlayout.tplを充てる系viewクラス拡張。テンプレートファイル名はmodelで指定可能。
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once('view.inc');

class view_with_layout extends view {

// レイアウトをあてる
protected function output_extra($s) {
  // 必要なインスタンスげと
  $mobj = $this->get_model_obj();
//var_dump( get_class($mobj) );

  // simple_put_modelクラスを使ってたら「静的なHTMLだろう」ってんでレイアウト適用はしない
  // XXX 「レイアウト適用したい静的なHTML」なら、クラス継承してクラス名だけ変えちゃって
  if ('simple_put_model' === get_class($mobj) ) {
    return $s;
  }

  // レイアウト不要時の処理
  if ('' === $mobj->get_layout_filename()) {
    return $s;
  }

  // テンプレートファイルのfullディレクトリ作成
  $fn = $mobj->get_config()->find('template_dir') . $mobj->get_layout_filename();

  // main_contentsをあてる
  // XXX mw_smarty系、secure_conv系以外は一端未想定
  if (true === is_a($mobj->get_conv(), 'mw_smarty')) {
    $mobj->get_conv()->assign_unsecure_raw('main_contents', $s);
    $s = $mobj->get_conv()->fetch($fn);
  } else if (true === is_a($mobj->get_conv(), 'secure_conv')) {
    $mobj->get_conv()->monoDic_unsecure_raw('main_contents', $s);
    $s = $mobj->get_conv()->conv(file_get_contents($fn));
  }

  // レイアウトの中にmain_contentsを当てはめて復帰
  return $s;
}



} // end of class

