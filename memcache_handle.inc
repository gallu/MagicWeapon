<?php

/**
 *
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 */
require_once('kvs_handle.inc');


class memcache_handle extends kvs_handle {

// メムキャッシュ用ハンドルはとっとと作成しておく
public function __construct() {
  // XXX MemcacheとMemcached、いったい何が違うんだか orz
  $this->mem_ = new Memcache();
}

//
protected function get_memcache_obj() { return $this->mem_; }

/**
 * このハンドルがmemcache系かどうか
 *
 * @access public
 * @return boolean
 */
public function is_memcache() { return true; }


/**
 * データの設定
 *
 * @access public
 * @param string $key キー
 * @param string $val 値
 * @param string $exp データの寿命(Unix タイムスタンプ形式、あるいは現在からの 秒数
 * @return boolean
 */
public function set($key, $val, $exp = '')
{
  // 微妙にあつかいがわからないので…
  if ('' === $exp) {
    $ret = $this->get_memcache_obj()->set($key, $val);
  } else {
    $ret = $this->get_memcache_obj()->set($key, $val, 0, $exp);
  }

  return $ret;
}

/**
 * データの読み込み
 *
 * @access public
 * @param string $key キー
 * @return string 値。ない場合は空文字
 */
public function get($key)
{
  //
  $ret = $this->get_memcache_obj()->get($key);

  //
  if (false === $ret) {
    return '';
  }
  // else
  return $ret;
}

/**
 * データの削除
 *
 * 若干怪しい話があったので「空データを設定してから」「削除」の２段階で実装
 *
 * @access public
 * @param string $key キー
 * @return boolean
 */
public function del($key)
{
  //
  $this->set($key, '');
  //
  return $this->get_memcache_obj()->delete($key);
}


/**
 * サーバの追加
 *
 * @access public
 * @param string $host  
 * @param int $port
 * @param int $weight
 * @return boolean
 */
public function add_server($host, $port = 11211, $weight = 1)
{
  // XXX パラメタはこれでいいかねぇ？ 精査が必要かねぇ？
  return $this->get_memcache_obj()->addServer($host, $port, true, $weight);
}

//private
private $mem_;


} // end of class
