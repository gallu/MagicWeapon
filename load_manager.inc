<?php

/**
 * ロードマネージャクラス
 *
 * 「エラートラップのできる」requireを実現するためのクラス
 * XXX Perlだと結構意味があるか実現できないクラスになりそうだが…
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  yyyy/mm/dd
 * @version $Id$
 **/


class load_manager {
//public:

/**
 * staticに使われることを前提にしている
 *
 * @access	public
 * @param	string	$filename	reuqireしたいファイル名
 * @return	boolean	成功したらtrue、失敗したらfalse
 */
static public function load_file($filename)
{
/*
  // ファイル存在チェック
  // ファイルが実在ファイルではなく
  if (!(is_file($filename))) {
    // かつリンクファイルでもない場合
    if (!(is_link($filename))) {
      return false;
    }
  }
*/

  // XXX requireだとエラー捕捉できないのであえてのinclude
//print "in to load_file $filename \n";
  $ret = @include_once($filename);
//print "ret is \n";
//var_dump($ret);
  if (false === $ret) {
    return false;
  }
  // else
  return true;
}

} // end of class
