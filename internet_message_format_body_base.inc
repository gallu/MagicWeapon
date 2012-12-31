<?php

/**
 * RFC2822によるところの Internet Message Format のBody部をあらわすクラス
 *
 * 単純なtextオンリーとmultipartがあるので、とりあえず基底クラスを作る
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/


abstract class internet_message_format_body_base {

public function __construct()
{
  //
  $this->init();
}

//
public function init()
{
}

/**
 * body部を一本の文字列にして取得
 *
 * @access abstract public
 * @return string 問題があれば空文字列
 */
abstract public function get_to_string();

/**
 * 文字列の設定
 *
 * multipartだと多分パース処理とか走るんだろうなぁ
 *
 * @access abstract public
 * @param string $s 文字列
 * @return boolean 問題があればfalse
 */
abstract public function set_from_string($s);


} // end of class

