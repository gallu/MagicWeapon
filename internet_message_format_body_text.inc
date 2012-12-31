<?php

/**
 * 単純なtextメール用bodyクラス
 *
 * とりあえず当面

	There are two limits that this standard places on the number of
characters in a line. Each line of characters MUST be no more than
998 characters, and SHOULD be no more than 78 characters, excluding
the CRLF.
最低でも、CRLFを除いたcharactersは998文字以下。
可能なら78文字以下。

	CR and LF MUST only occur together as CRLF; they MUST NOT appear
independently in the body.
CRとLFは、いずれかが独立して現れてはいけない。

 * という制限は未チェック
 * また厳密には、ezwebの改行コードがLFなのだが、その辺も未対応
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

require_once('internet_message_format_body_base.inc');

class internet_message_format_body_text extends internet_message_format_body_base {

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
  $this->set_from_string('');
}

/**
 * body部を一本の文字列にして取得
 *
 * 実質は未処理
 *
 * @access abstract public
 * @return string 問題があれば空文字列
 */
public function get_to_string() { return $this->body_string_; }

/**
 * 文字列の設定
 *
 * multipartだと多分パース処理とか走るんだろうなぁ
 *
 * @access abstract public
 * @param string $s 文字列
 */
public function set_from_string($s) { $this->body_string_ = $s; }

//private:
private $body_string_;

} // end of class

