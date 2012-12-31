<?php

/**
 * controller用ってかmapファイル用のconfigファイル処理クラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

//require_once('config.inc');
require_once('config_extension.inc');
require_once('mw_config_ret.inc');

class mw_config {

public function __construct()
{
  //
  //parent::__construct();
  // XXX 使いまわしたいからここでnewする〜
  //$this->set_config_obj(new config);
  $this->set_config_obj( new config_extension() );
  // config自体がconstructerでinit callするから一旦comment out
  //$this->init();
}

//
public function init()
{
  $this->get_config_obj()->init();
}

/**
 * configクラスのsetter & getter
 *
 */
protected function set_config_obj($o) { $this->config_obj_ = $o; }
protected function get_config_obj() { return $this->config_obj_; }

/**
 * mapファイルの設定
 *
 * @access public
 * @param string $fn mapファイル名
 * @return boolean なにかあればfalse
 */
public function set_file($fn)
{
  $this->get_config_obj()->set_base_path(dirname($fn) . '/');
  return $this->get_config_obj()->set_file($fn);
}

/**
 * commandに設定されている内容を取得
 *
 * @access public
 * @param string $cmd コマンド名
 * @return mw_config_ret_obj mw_config_retクラスのインスタンス  エラーならnull
 */
public function find($cmd)
{
  // コマンド名が指定されていなければnull
  if ("" === $cmd) {
    return null;
  }

  // 存在していなければnull
  $base_line = $this->get_config_obj()->find($cmd);
//print "$cmd => $base_line \n";
  if ("" === $base_line) {
    return null;
  }

  // 復帰値用インスタンス
  $ret = new mw_config_ret;

  ////////////////////////
  // まずSPで切り分ける
  ////////////////////////
  $awkbuf = explode(' ', $base_line);
//var_dump($awk);

  // 重複した空白を削除
  $awk = array();
  foreach($awkbuf as $wk) {
    if ("" !== $wk) {
      $awk[] = $wk;
    }
  }

  ////////////////////////
  // include & class名
  ////////////////////////
  $awk2 = explode(':', $awk[0]);
//var_dump($awk2);
  // include file
  if ('' === $awk2[0]) {
    $ret->set_include_filename($cmd . '.inc');
  } else
  if ('/' === $awk2[0][strlen($awk2[0]) - 1]) {
    $ret->set_include_filename($awk2[0] . $cmd . '.inc');
  } else {
    $ret->set_include_filename($awk2[0]);
  }
  // class name
  if ('' === $awk2[1]) {
    $ret->set_classname($cmd);
  } else {
    $ret->set_classname($awk2[1]);
  }

  ////////////////////////
  // template系
  ////////////////////////
//var_dump($awk);
  // いったんデフォで設定しとく
  $ret->set_template_filename($cmd . '.tpl');
//var_dump($cmd . '.tpl');
//var_dump($awk);
  if (isset($awk[1])) {
    if ('.' != $awk[1]) {
      $awk2 = explode(',', $awk[1]);
      if ('' !== $awk2[0]) {
        $ret->set_template_filename($awk2[0]);
      }
      if (isset($awk2[1])) {
        $ret->set_error_template_filename($awk2[1]);
      }
    }
  }

  ////////////////////////
  // ssl系ってか拡張？
  ////////////////////////
  if (isset($awk[2])) {
    $ret->set_ssl_type($awk[2]);
  }

  //
//var_dump($ret);
  return $ret;
}


//private:
private $config_obj_;

} // end of class

