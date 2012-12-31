<?php

/**
 * セッション情報をKVSに書き込むクラス
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 */


require_once("session_data.inc");

class session_data_kvs extends session_data {
//public:

// 初期設定
public function __construct()
{
  //
  parent::__construct();

}

//
pubilc function is_exist_kvsb() { return true; }


/**
 * KVSへのsession dataの書き込み
 *
 * @access public
 * @return boolean 問題があればfalse
 */
public function write()
{
  // 実態は基底クラスに
  return $this->write_to_kvs();
}

/**
 * KVSからsession dataへの読み込み
 *
 * @access public
 * @return boolean 問題があればfalse
 */
public function read()
{
  // 実態は基底クラスに
  $this->read_from_kvs();
}


/**
 * KVS内にあるsession dataの削除
 *
 * メモリ上のデータを削除する場合は initメソッドで
 *
 * @access public
 * @return boolean 問題があればfalse
 */
public function del() {
  // 実態は基底クラスに
  $this->del_to_kvs();
}

/**
 * セッションIDの存在チェック
 *
 * @access public
 * @return boolean 存在していたらtrue
 */
public function is_session() {
  // 実態は基底クラスに
  return $this->is_session_from_kvs();
}


//private:
} // end of class

