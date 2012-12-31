<?php
/**
 * DBハンドルクラス PDO mysql
 *
 * DBハンドルを取り扱います
 *
 * @deprecated
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/
require_once('f_pdo.inc');
require_once('sql_util_mysql.inc');
require_once("mw_makesql_place_holder_mysql.inc");

class dbh_pdo_mysql extends dbh_pdo {
//public:

  // コンストラクタ
  public function __construct() {
    //
    parent::__construct();

    // XXX これそろそろ…
    $this->set_sql_util(new sql_util_mysql);
    //
    $o = new mw_makesql_place_holder_mysql();
    $o->set_db($this);
    $this->set_mw_makesql($o);
  }

/**
 * 接続先DB用文字列
 *
 * @access protected
 * @return string 'mysql'
 */
protected function get_rdb_name() { return 'mysql'; }

}

