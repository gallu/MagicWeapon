<?php

/**
 * アクセス分析/解析用のutil
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once('mw_area_setting.inc');
require_once('cgi_param.inc');

class access_analysis_util {

/**
 * 検索エンジンで検索された文字列の取得
 *
 * refererのuriから抜き出す
 * とりあえず一端雑に
 * mw_area_settingに設定があれば、文字コードを変更しておく
 *
 * @access publicかprivateかprotectedか
 * @param string $uri refererで取得できたURI
 * @return array ['word'] = 検索文字  ['engine'] = 検索エンジン名
 */
static public function get_retrieval_word($uri)
{
  //
  $ret = array();
  if ('' === $uri) {
    return $ret;
  }

  // 設定
  static $conf = array(
    'www.google.co.jp' => array('param' => 'q', 'name' =>'Google'),
    'beta.search.yahoo.co.jp' => array('param' => 'p', 'name' =>'Yahoo!(サーチベータ)'),
    'search.yahoo.co.jp' => array('param' => 'p', 'name' =>'Yahoo!(サーチベータ) dir'),
    'search.msn.co.jp' => array('param' => 'q', 'name' =>'msn'),
    'search.goo.ne.jp' => array('param' => 'MT', 'name' =>'goo'),
    'dir.goo.ne.jp' => array('param' => 'MT', 'name' =>'goo dir'),
    'www.infoseek.co.jp' => array('param' => 'qt', 'name' =>'infoseek'),
    'cgi.search.biglobe.ne.jp' => array('param' => 'q', 'name' =>'Biglobe'),
    'www.excite.co.jp' => array('param' => 'search', 'name' =>'Excite'),
    'search.livedoor.com' => array('param' => 'q', 'name' =>'livedoor'),
    'dir.livedoor.com' => array('param' => 'q', 'name' =>'livedoor dir'),
    'search.nifty.com' => array('param' => 'Text', 'name' =>'nifty'),
    'ask.jp' => array('param' => 'q', 'name' =>'Ask.jp'),
    'allabout.co.jp' => array('param' => 'qs', 'name' =>'AllAbout'),
    'joyjoy.com' => array('param' => 'key', 'name' =>'JOY'),
    'www.bing.com' => array('param' => 'q', 'name' =>'bing'),
  );
//var_dump($conf);

  // uriの分解
  $awk = explode('/', $uri, 4);
//var_dump($awk);

  // ある？
  if (true === isset($conf[$awk[2]])) {
//print "true\n";
    // 取得
    $awk2 = explode('?', $awk[3], 2);
//var_dump($awk2);
    $p = new cgi_param;
    $p->set($awk2[1]);
    //
    $ret['word'] = $p->find($conf[$awk[2]]['param']);
    $ret['engine'] = $conf[$awk[2]]['name'];
  }
//var_dump($ret);
  return $ret;
}


//private:


} // end of class

