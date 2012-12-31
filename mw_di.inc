<?php

/**
 * classの説明(一行)：基本的にクラス名
 *
 * classの説明(複数行)：クラスに対するコメント突っ込みの類
 * HTMLに反映されたとき改行はされない
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

require_once('validator.inc');

class mw_di {

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
 * 設定ファイルの設定
 *
 * @access publicかprivateかprotectedか
 * @param 型 名前 説明
 * @return 戻り値の型 簡単な説明
 */
public function set_config($s)
{
}

/**
 * インスタンスの取得
 *
 * @access publicかprivateかprotectedか
 * @param 型 名前 説明
 * @return 戻り値の型 簡単な説明
 */
public function new($name)
{
}


//private:


} // end of class

