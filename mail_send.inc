<?php
/**
 * メール送信クラス
 *
 * メール送信のためのクラス。実際にはVirtual
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

abstract class mail_send {

//public:
public function __construct()
{
  //
  //parent::__construct();
  $this->init();
}

//
public function init()
{
}

/**
 * メール送信を行う
 *
 * @access public
 * @param internet_message_format_obj $mobj メールデータを表すインスタンス
 * @return boolean 問題が発生したらfalse 詳細は別メソッドで。…の予定。
 */
abstract public function send(internet_message_format $mobj);

//private:

} // end of class
