<?php

/**
 * セッション用authテーブルアクセス用clumpクラス
 *
 * パスワード＆各種制限が入っているテーブルへのclump
 * 基本的なテーブルイメージは以下のとおり
create table auth_user (
   id  クライアントID
   pass
   status(ステータス
     0:正常  1:ロック(ロックおよびアンロックはアプリでよろしく)
     2以降:子Classで定義
   lock_time unsigned long
     (一時ロックが解除されるべき時間:time_t値(time()関数で取得できる値)で)
     通常は０。０以外の時には一時ロックされている状態となる
   err_count // エラーカウント
);

create table auth_user (
   id varchar(64) not null,
   pass  varchar(64) not null,
   status  varchar(8),
   lock_time bigint,
   err_count bigint,
   PRIMARY KEY (id)
);

 * ついで。sha-1でパスワード'pass'の場合のinsert文
insert into auth_user(id, pass, status, lock_time, err_count) value ( 'id', '9d4e1e23bd5b727046a9e3b4b7db57bd8d6ee684', 0, 0, 0);

 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once("data_clump.inc");

class session_auth_clump extends data_clump {

public function __construct()
{
  //
  //$this->init();
  parent::__construct();
}

//
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
 $this->set_table_name("auth_user");

 // $inname, $cginame, $tmpname, $dbname, $keyflg
 $this->push_element("id", "", "", "", "pk");
 $this->push_element("pass", "", "", "", "");
 $this->push_element("status", "", "", "", "");
 $this->push_element("lock_time", "", "", "", "");
 $this->push_element("err_count", "", "", "", "");
}


//private:


} // end of class

