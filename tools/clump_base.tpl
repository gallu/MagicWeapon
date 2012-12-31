<?php
/*
 * %%%table_comment%%% 用 clump_base
 * XXX 自動生成用クラスなので、ここに追記などはしないこと！！
 *
 */


require_once("data_clump.inc");

class %%%clump_base_name%%% extends data_clump {

public function __construct()
{
  parent::__construct();
}

public function init()
{
  parent::init();
  $this->set_element();	// ユーザ登録画面の要素を登録
}

/**
 * 基本的な情報の設定
 *
 * テーブル名およびカラム名を設定する
 *
 * @access protected
 * @return boolean falseは現状想定外。
 */
protected function set_element()
{
  //
  $this->set_table_name('%%%table_name%%%');

  // $inname, $cginame, $tmpname, $dbname, $keyflg, $data_type(db)
$$$recodes$$$  $this->push_element('%%%Field%%%', '', '', '', '%%%Key%%%', %%%TypeString%%%); // %%%Comment%%%(%%%Type%%%)
$$$/recodes$$$

}

//private:


} // end of class



