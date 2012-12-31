<?php

/**
 * 複数用DBハンドルクラス
 *
 * クラスタリング環境下における、DBハンドルの集合を保持するクラス
 * 単数なら db_handle class使って
 * db_handle classは「DB接続の実態」を(継承先で)もってるけど、このクラスは「複数のDBHに対するアダプタ」なので、実態はもってない点に注意
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 */

require_once('db_handle_base.inc');
require_once('db_manager.inc');

class db_handle_plural extends db_handle_base{

// コンストラクタ
public function __construct() {
}



/**
 * このハンドルが「複数のRDBハンドルs」系かどうか
 *
 * @access public
 * @return boolean
 */
public function is_multi_rdb() { return true; }


/**
 * 指定されたハンドルに対するクエリーの発行
 *
 * @access public
 * @param string $sql SQL
 * @param string $rw 'read'または'write'または''
 * @param string $table テーブル名
 * @return db_data_object DBデータオブジェクト または null
 */
public function query_to_specified_handle($sql, $rw, $table)
{
  // ハンドルを取得
  $dbh = $this->choose_dbh($rw, $table);
  if (null === $dbh) {
    return null;
  }

  // 接続してなきゃ接続する
  if (false === $dbh->is_connect()) {
    // つなげてみる
    $ret = $dbh->rconnect();
    // ど?してもむずがったら
    if (false === $ret) {
      // つっかえす
      return null;
    }
  }

  // 投げる！！
  return $dbh->query($sql);
}

/**
 * read handleに対するSQL
 *
 * @access public
 * @param string $sql SQL
 * @return db_data_object DBデータオブジェクト または null
 */
public function read_query($sql)
{
  return $this->query_to_specified_handle($sql, 'read', $this->get_table_name($sql));
}

/**
 * write handleに対するSQL
 *
 * @access public
 * @param string $sql SQL
 * @return db_data_object DBデータオブジェクト または null
 */
public function write_query($sql)
{
  return $this->query_to_specified_handle($sql, 'write', $this->get_table_name($sql));
}

/**
 * queryメソッドのラッパー
 *
 * 現状とりあえずread/writeの区別のみ。
 * あとでテーブル名見たりするかねぇ:着手した? ノ
 *
 * @access public
 * @param string $sql SQL
 * @return db_data_object DBデータオブジェクト または null
 */
public function query($sql)
{
  //
  $sql = trim($sql);

  // XXX 未想定なSQLの場合、read側に向いてたほうが被害が少ないはずなので
  $type = 'read';

  // タイプ判定
  if (0 === strncasecmp($sql, 'UPDATE', strlen('UPDATE'))) {
    $type = 'write';
  } else
  if (0 === strncasecmp($sql, 'INSERT', strlen('INSERT'))) {
    $type = 'write';
  } else
  if (0 === strncasecmp($sql, 'DELETE', strlen('DELETE'))) {
    $type = 'write';
  }

/*
// XXX この辺もうちょっと色々必要な気がするので、一端コメントアウト
 else
  if (0 === strncasecmp($sql, 'BEGIN', strlen('BEGIN'))) {
    $type = 'write';
  } else
  if (0 === strncasecmp($sql, 'ROLLBACK', strlen('ROLLBACK'))) {
    $type = 'write';
  } else
  if (0 === strncasecmp($sql, 'COMMIT', strlen('COMMIT'))) {
    $type = 'write';
  }
*/

  // テーブル名の取得
  $table = $this->get_table_name($sql);

  // SQLの発行
  return $this->query_to_specified_handle($sql, $type, $table);
}

// ------------------------------------------------------------
// ハンドルをぶち込むあたり
// ------------------------------------------------------------
/**
 * 情報一式のsetter
 *
 * XXX 後でもうチョイちゃんとしたパーサなりを経由させる?
 *
 * $dbh[name]['type']
 *           ['database']
 *           ['user']
 *           ['pass']
 *           ['host']
 *           ['port']
 *
 * $des[table名]['read']  = array(name, name, ...);
 *              ['write'] = array(name, name, ...);
 *
 *
 * @access public
 * @param string $sql SQL
 * @return db_data_object DBデータオブジェクト または null
 */
public function set_config_by_variable($dbh, $des)
{

  // db_objectを作る
  foreach($dbh as $name => $data) {
    // タイプに沿ったdb_handleオブジェクトを取得
    $obj = db_manager::get_db_handle_object($data['type']);
    if (true === is_null($obj)) {
      // XXX …どうすっかねぇ
      return false;
    }

    // 情報をぶち込む。接続はしにゃい！！
    $obj->set_user($data['user']);
    $obj->set_pass($data['pass']);
    $obj->set_database_name($data['database']);
    $obj->set_host_name($data['host']);
    $obj->set_port_num($data['port']);

    // 格納?
    $this->dbh_[$name] = $obj;
  }


  // 「どれ使う？」を作る
  $this->destination_ = array();
  foreach($des as $key => $val) {
    $awk = explode(',', $key);
    foreach($awk as $v) {
      $this->destination_[trim($v)] = $val;
    }
  }
//var_dump($this->destination_);

  //
  return true;
}

/**
 * DBハンドルの選択
 *
 * テーブル名とread/write情報から、DBハンドルを１つ選び出す
 *
 * @access protected
 * @param string $rw readまたはwrite
 * @param string $table テーブル名
 * @return object mono_dbなDBハンドル
 */
protected function choose_dbh($rw, $table)
{
  // 必要なDB接続を把握してから
  $db_names = array();
  if (true === isset($this->destination_[$table][$rw]) ) {
    $db_names = $this->destination_[$table][$rw];
  } else
  // テーブル名なしのパターンを一応想定
  if (true === isset($this->destination_[''][$rw]) ) {
    $db_names = $this->destination_[''][$rw];
  } else
  // read/writeなしのパターンも一応想定
  if (true === isset($this->destination_[$table]['']) ) {
    $db_names = $this->destination_[$table][''];
  } else
  // XXX デフォルトはどうするかねぇ？
  // ＆
  // XXX テーブル名とr/w両方なし…は、ないか。それなら普通のハンドル使えばいいんだし
  // XXX 一応想定しておくか
  if (true === isset($this->destination_['']['']) ) {
    $db_names = $this->destination_[''][''];
  }
//var_dump($db_names);

  // 見つからなきゃNG?
  if (true === empty($db_names)) {
    return null;
  }

  // 雑に分散
  // XXX そのうち「接続できなきゃ別のハンドル、全部NGならだめぽ?」のあたりも実装せなねぇ…
  $s = $db_names[ mt_rand(0, count($db_names) - 1) ];

  // ハンドルを取得
  // XXX エラーチェックしないよ？ 開発中だけっしょンなもん出るの
  $dbh = $this->dbh_[$s];

  //
  return $dbh;
}

// XXX is_checkとか作って、nameの対応とか確認する？？


/**
 * SQL_UTILインスタンスの取得
 *
 * XXX 場当たりなんてもんじゃない実装 orz
 *
 * @deprecated
 * @access public
 * @return object sql_utilインスタンス
 */
public function get_sql_util()
{
  return $this->dbh_[key($this->dbh_)]->get_sql_util();
}

/**
 * mw_makesqlインスタンスの取得
 *
 * DBはwriteを使うと負荷が気になるので、read側を使うようにする
 *
 * @access public
 * @param string $table_name テーブル名
 * @return object mw_makesqlインスタンス
 */
public function get_mw_makesql($table_name)
{
  return $this->choose_dbh('read', $table_name)->get_mw_makesql();
}


// DBハンドルの実態
private $dbh_;		// [DBハンドル名]  = db_object

// DBハンドルのどれ使うの？
private $destination_;	// [テーブル名][read/write] = array(DBハンドル名, ...)

} // end of class
