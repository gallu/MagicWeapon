<?php

/**
 * RSS パーサ
 *
 * RSSパーサの基底クラス
 *
 * @deprecated
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once('multibyte.inc');
require_once('rss_data.inc');

abstract class rss_parse_base {

//
public function set_rss_string($s) { $this->base_string_ = $s; }
public function get_rss_string() { return $this->base_string_; }

abstract public function parse();

//
private $base_string_;

} // end of class

