<?php

/**
 * "mw_configクラスの復帰値用クラス
 *
 * mw_configクラスと一緒に書いてもよかったんだけどねぇ…
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

class mw_config_ret {

public function __construct()
{
  //
  //parent::__construct();
  $this->init();
}

//
public function init()
{
  $this->set_include_filename('');
  $this->set_classname('');
  $this->set_template_filename('');
  $this->set_error_template_filename('');
  $this->set_ssl_type('');
}

/**
 * includeすべきファイル名の取得
 *
 * @access public
 * @return string includeすべきファイル名
 */
public function get_include_filename() { return $this->include_filename_; }
public function set_include_filename($s) { $this->include_filename_ = $s; }

/**
 * newすべきclass名の取得
 *
 * @access public
 * @return string newすべきclass名
 */
public function get_classname() { return $this->classname_; }
public function set_classname($s) { $this->classname_ = $s; }

/**
 * 通常用テンプレートファイル名の取得
 *
 * @access public
 * @return string 通常用テンプレートファイル名
 */
public function get_template_filename() { return $this->template_filename_; }
public function set_template_filename($s) { $this->template_filename_ = $s; }

/**
 * error用テンプレートファイル名の取得
 *
 * @access public
 * @return string error用テンプレートファイル名
 */
public function get_error_template_filename()
  { return $this->error_template_filename_; }
public function set_error_template_filename($s)
  { $this->error_template_filename_ = $s; }

/**
 * sslタイプの取得
 *
 * 一応、ssl、nossl、not_careの3typeを想定してるけど
 * 実際にはここは「拡張領域」扱いになると思う
 *
 * @access public
 * @return string sslタイプ
 */
public function set_ssl_type($s) { $this->ssl_type_ = $s; }
public function get_ssl_type() { return $this->ssl_type_; }


//private:
private $include_filename_;
private $classname_;
private $template_filename_;
private $error_template_filename_;
private $ssl_type_;

} // end of class

