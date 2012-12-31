<?php

/**
 * 静的な資源を出力するためのお便利クラス
 *
 * MWのMVC使った時に静的なHTMLとか出す用のクラス。
 * 画像とかcssとかはとりあえず未想定
 * 資源はローカルなディスクにあることを想定
 * configファイルの base_dir を基準ディレクトリとみなす
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

require_once('config.inc');

class static_resource_localdisk {

public function __construct()
{
  //
  $this->init();
}

//
public function init()
{
  $this->set_config_obj(null);
}

//
protected function set_config_obj($o) { $this->config_ = $o; }
protected function get_config_obj() { return $this->config_; }

/**
 * configファイル名のセット
 *
 * 後でもしかしたらset_configのラッパー化するかも
 *
 * @access public
 * @param string $fn configファイル名
 * @return boolean 何か問題があればfalse
 */
public function set_config_file($fn)
{
  $obj = new config;
  $obj->set_file($fn);
  $this->set_config_obj($obj);
  //
  return true;
}

/**
 * コンテンツのファイル名取得
 *
 *
 * @access public
 * @param string $id 識別子
 * @return string 何か問題があったら空文字
 */
public function get_contents_filename($id)
{
  $ret = $this->get_config_obj()->find($id);
  return $ret;
}

//private:
private $config_;

} // end of class

