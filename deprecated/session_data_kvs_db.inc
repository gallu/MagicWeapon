<?php

/**
 * セッション情報をKVSとDBに書き込むクラス
 *
 * 速度が欲しいからKVSに書き込みつつ、データが消えて欲しくないのでバックアップ的にDBにも書き込みたい、なんて時に
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 */


require_once("session_data.inc");

class session_data_kvs_db extends session_data {
//public:

// 初期設定
public function __construct()
{
  //
  parent::__construct();

}

//
public function is_exist_db() { return true; }
public function is_exist_kvsb() { return true; }

/**
 * session dataの書き込み
 *
 * @access public
 * @return boolean 問題があればfalse
 */
public function write()
{
  // 両方に書き込む
  $flg = $this->write_to_kvs() && $this->write_to_db();
  return $flg;
}

/**
 * session dataへの読み込み
 *
 * @access public
 * @return boolean 問題があればfalse
 */
public function read()
{
  // KVSから読み込めたらそっち
  $ret = $this->read_from_kvs();
  if (true === $ret) {
    return true;
  }

  // 読めなかったらDBから
  $ret = $this->read_from_db();
  if (true === $ret) {
    // KVSに書いておく
    $this->write_to_kvs();
    //
    return true;
  }

  // 全滅ぽ…
  return false;
}


/**
 * session dataの削除
 *
 * メモリ上のデータを削除する場合は initメソッドで
 *
 * @access public
 * @return boolean 問題があればfalse
 */
public function del() {
  // 実態は基底クラスに
  $this->del_to_kvs();
  $this->del_to_db();
}

/**
 * セッションIDの存在チェック
 *
 * @access public
 * @return boolean 存在していたらtrue
 */
public function is_session() {
  //
  $ret = $this->is_session_from_kvs();
  if (true === $ret) {
    return true;
  }
  // else
  return $this->is_session_from_db();
}


//private:
} // end of class

