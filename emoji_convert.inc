<?php

/**
 * 絵文字変換クラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

require_once('mobile_info.inc');
require_once('emoji_factory.inc');

class emoji_convert {

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
 * メソッドの説明(一行)：基本的にメソッド名
 *
 * メソッドの説明(複数行)：詳細な説明その他
 *
 * @access publicかprivateかprotectedか
 * @param 型 名前 説明
 * @return 戻り値の型 簡単な説明
 */
public function convert($base_string, $type="")
{
  // user_agentから適宜機種名等を取得
  $obj = new mobile_info;
  if ("" === $type) {
    if ($obj->is_docomo()) {
      $type = 'docomo';
    } else
    if ($obj->is_ez()) {
      $type = 'ez';
    } else
    if ($obj->is_softbank()) {
      $type = 'softbank';
    } else {
      // XXX 本当はPCのインスタンス〜
      $type = 'docomo';
    }
  }
//var_dump($type);

  // インスタンス取得
  $eobj = emoji_factory::get_instance($type);
  
  // 置換
  // XXX 後で可変に
  $prefix = '[emoji:';
  $suffix = ']';

  $prefix_len = strlen($prefix);
  $suffix_len = strlen($suffix);

  $s = $base_string;

  $ret = "";
  while(1) {
    // 先頭
    $pos = strpos($s, $prefix);
    if (false === $pos) { break; }
    $ret .= substr($s, 0, $pos);
    $s = substr($s, $pos + $prefix_len);

    // 置換と続き
    $pos = strpos($s, $suffix);
    if (false === $pos) { break; }
    $no = substr($s, 0, $pos);
    $ret .= $eobj->encode($no);
    $s = substr($s, $pos + $suffix_len);
  }
  if ("" != $s) {
    $ret .= $s;
  }


  //
  return $ret;
}


//private:
//prefix
//suffix

} // end of class

