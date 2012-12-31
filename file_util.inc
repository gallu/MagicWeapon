<?php

/**
 * ファイル用ユーティリティクラス
 *
 * ファイル操作にまつわるユーティリティ系クラス
 * ユーティリティ系クラスなので、全て
 * file_util::メソッド
 * の形式でのcallを想定している
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

//
class file_util {

/**
 * ファイルパスを「拡張子と」「それ以外」に分解
 *
 * @access public
 * @param $s ファイル名(ファイルパス)
 * @return array [0]がそれ以外(ファイル名本体)、[1]が拡張子(.付き) 拡張子はなければ空文字
 */
static public function split_filename_by_extension($s)
{
$ret = -1;
$wk = 0;

  //
  while( false !== ($wk = strpos($s, '.', $wk))) {
    $ret = $wk;
    $wk ++;
  }

  //
  if (-1 !== $ret) {
    $a[] = substr($s, 0, $ret);
    $a[] = substr($s, $ret);
  } else {
    $a[] = $s;
    $a[] = '';
  }
//var_dump($a);

  //
  return $a;
}

} // end of class

