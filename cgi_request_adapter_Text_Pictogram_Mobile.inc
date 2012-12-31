<?php

/**
 * CGI リクエストクラス 絵文字ラッパー
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  yyyy/mm/dd
 * @version $Id$
 **/

require_once('cgi_request.inc');
require_once('emoji_adapter_Text_Pictogram_Mobile.inc');

//
class cgi_request_adapter_Text_Pictogram_Mobile extends cgi_request {

/**
 * Text_Pictogram_Mobile 使用
 *
 * @access public
 * @param string $s データ
 * @return string 出力
 */
public function interruption_emoji($s)
{
  // 絵文字変換
  $pobj = emoji_adapter_Text_Pictogram_Mobile::get_obj( new mobile_info );

  //
  $s = $pobj->convert($s);

  //
  return $s;
}

//private:

} // end of class

