<?php

/**
 * なんか掲示板で使いそうなユーティリティ各種
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

class bbs_util {

/**
 * NGワードとか警告ワードとかをはじき出すエトセトラ
 *
 * メソッドの説明(複数行)：詳細な説明その他
 *
 * @access publicかprivateかprotectedか
 * @param string $s 調査対象の文字列
 * @param array $ng NGなりwarningなりの文言の配列
 * @return array 引っかかった問題語句のリスト。無ければ空配列
 */
static public function ng_check($s, array &$ng)
{
$ret = array();

  //
  if ("" === $s) { return $ret; }
  //
  $s = str_replace( array("\r", "\n", " ", "\t"), array("","","",""), $s);

  //
  foreach($ng as $w) {
    //
    if ("" === $w) { continue; }
    // XXX ここでやるのもどうかとは思うけど
    //$w = mb_convert_encoding($w, 'eucjp-win', 'sjis-win,eucjp-win,EUC-JP,SJIS,JIS,UTF-8,ASCII');

    // チェック
    $b = strpos($s, $w);
    if (false !== $b) {
      $ret[] = $w;
    }
  }
//var_dump($ret);
  //
  return $ret;
}



//private:


} // end of class

