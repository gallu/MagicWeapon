<?php

/**
 * data_clumpとKVS系ハンドルとを紐付けるクラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 */

require_once('cgi_param.inc');
require_once('data_clump_adaptor_base.inc');

class data_clump_kvs_adaptor_base extends data_clump_adaptor_base
{
/**
 * create用処理
 *
 * XXX 実態はsetに
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean
 */
public function create($clump)
{
  return $this->set($clump);
}

/**
 * 全体update用処理
 *
 * XXX 実態はsetに
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean
 */
public function update_all($clump)
{
  return $this->set($clump);
}

/**
 * 部分的なupdate用処理
 *
 * XXX 実態はsetに & 部分的にってのはKVS的には無理だべさ
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean
 */
public function update_mono($clump)
{
  return $this->set($clump);
}


/**
 * pkによらないread
 *
 * XXX RDB以外だとちょっと厳しいかも〜なので、一旦はねる
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean pk以外の指定がある場合、KVS的にはfalseなのでfalseを返す
 */
public function read_nopk($clump)
{
  // whereしたいっぽい値のげと
  $where = array();
  foreach ($clump->get_all_names() as $col) {
    //
    if ("" !== $clump->get_value($col)) {
      $where[$col] = 1;
    }
  }

  // pkと比較
  foreach($clump->get_all_key_names() as $pk_name) {
    //
    if (false === isset($where[$pk_name])) {
      // だめっぽいのでとっとと突っ返す
      return false;
    }
    // pk listにあるので、消す
    unset($where[$pk_name]);
  }

  // 残り物があるってことはなんか余分があるってことだ〜
  if (false === empty($where)) {
    return false;
  }

  // 配列が一致してればread_pkでcallしたげる〜
  return $this->read_pk($clump);
}

/**
 * pkによるread
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean
 */
public function read_pk($clump)
{
//print "in to kvs read\n";
  // get
  $s = $this->get_data_handle()->get( $this->_make_key($clump) );

  //
  if (false === $s) {
    return false;
  }
  // XXX memcacheでNULLが帰ってきたよ？
  if (true === is_null($s)) {
    return false;
  }
  // XXX 念のため〜
  if ('' === $s) {
    return false;
  }

  // 格納処理
  $this->_store_read_data_to_clump($clump, $s);
//print "      kvs read ok\n";

  //
  return true;
}

/**
 * 更新処理
 *
 * insertまたはupdate
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @param array $targets 更新対象カラム名の配列
 * @return boolean
 */
public function set($clump)
{
  // 格納
  return $this->get_data_handle()->set( $this->_make_key($clump), $this->_clump_to_string($clump) );
}

/**
 * 削除用処理
 *
 * XXX 微妙に不安な話があるので「空valueで上書きしつつ」「削除を試みる」の２段階
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @param array $targets 更新対象カラム名の配列
 * @return boolean
 */
public function delete($clump)
{
  // 空valueによる上書き
  $this->get_data_handle()->set( $this->_make_key($clump), '' );

  // 削除処理
  return $this->get_data_handle()->del( $this->_make_key($clump) );
}



/**
 * key作成処理
 *
 * テーブル名＋pkの集まりとする
 * XXX 使える文字の制限とかあるのかなぁ？
 *
 * @access protected
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean
 */
protected function _make_key($clump)
{

  // pkの群れを取得
  $where = array();
  foreach($clump->get_all_key_names() as $name) {
    $where[] = $clump->get_db_name($name) . ':' . $clump->get_data($name);
  }

  //
  $key = $clump->get_identifier() . '.' . implode('.', $where);

  //
  return $key;
}

/**
 * read処理ラッパー
 *
 * 一本の文字列をclumpに格納してまわる
 *
 * @access protected
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @param string $base_string 一本の文字列に展開した格納情報
 * @return boolean
 */
protected function _store_read_data_to_clump($clump, $base_string)
{
  // 分解して
  $obj = new cgi_param();
  $obj->set($base_string);

  // 格納
  foreach($clump->get_all_names() as $name) {
    $clump->set_value($name, $obj->find($name), false);
  }

  //
  return true;
}

/**
 * set処理ラッパー
 *
 * clumpの情報を一本の文字列にする
 *
 * @access protected
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return string 一本化したstring
 */
protected function _clump_to_string($clump)
{
  // value作って
  $obj = new cgi_param();
  foreach($clump->get_all_names() as $col) {
    // 値を格納して
    $obj->add($col, $clump->get_value($col));
  }

  //
  return $obj->get();
}

/**
 * 数値の加減算
 *
 * XXX 設定したらすぐにDB等に書きに行くので留意
 *
 * @access public
 * @param data_clump_obj $clump 対象data_clumpインスタンス
 * @return boolean 成功したらtrue、なにか失敗があったらfalse
 */
public function calculation($clump)
{
  // 必要な諸条件の抜き出し
  $col = $clump->get_calculation_col_name();
  $sign = $clump->get_calculation_sign();
  $num = $clump->get_calculation_num();

  // 元データ保持して
  $base_i = $i = $clump->get_value($col);

  // 計算して
  if ('-' === $sign) {
    $i -= $num;
  } else {
    $i += $num;
  }
  // 設定して
  $clump->set_value($col, $i);

  // writeして
  $this->set($clump);

  // 元に戻して(上(data_clump本体)でちゃんと帳尻合わせるので)
  $clump->set_value($col, $base_i);
}

} // end of class
