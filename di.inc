<?php

/**
 * DIコンテナ
 *
 * イメージする書式
 * hoge = hoge.inc:hoge
 * hoge = hoge.inc:
 * hoge = :hoge
 * hoge = :
 * hoge2 = hogera/hoge2.inc:hoge2
 * hoge2 = hogera/hoge2.inc:
 * hoge2 = hogera/:hoge2
 * hoge2 = hogera/:
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  yyyy/mm/dd
 * @version $Id$
 **/

require_once('config.inc');

class di
{

/**
 * 設定ファイルのぶち込み
 *
 * @access public
 * @param string $fn 設定ファイル名
 * @return boolean なにかあったらfalse
 */
static public function set_config($fn)
{
//print "set_config Trap 1\n";

	//
	self::$config_obj_ = new config();

	//
	return self::$config_obj_->set_file($fn);
}

/**
 * クラス名の取得
 *
 * 概ね、staticなクラスのDI用。…っても5.3＋じゃないと使えないんだけどねぇ…
 * 当面(5.2-)は call_user_func つかっといて〜
 * createでも使う〜
 *
 * @access public
 * @param string $virtual_class_name 仮想的なクラス名
 * @return string 解決されたクラス名。設定がなければ virtual_class_name と同じ文字列が返る
 */
static public function get_name($virtual_class_name)
{
  // 処理的になにもできなければそのまま復帰させたいので
  $class_name = $virtual_class_name;
  $inc_name = $virtual_class_name . '.inc';

  //
  if (null !== self::$config_obj_) {
    // 探してみる
    $s = self::$config_obj_->find($virtual_class_name);

    //
    if ('' !== $s) {
      // まず分解
      list($inc_name, $class_name) = explode(':', $s);

      // 補完
      // クラス名の補完
      if ('' === $class_name) {
        $class_name = $virtual_class_name;
      }
      //includeファイル名の補完
      if ('' === $inc_name) {
        $inc_name = $class_name . '.inc';
      } else
      if ('/' === $inc_name[strlen($inc_name) - 1]) {
        $inc_name = $inc_name . $class_name . '.inc';
      }
    }
  }
//var_dump($class_name);
//var_dump($inc_name);

  // includeしておいて
  require_once($inc_name);

  //
  return $class_name;
}


/**
 * クラスインスタンスの取得
 *
 * 「イニシャライズに引数がある」パターンは考慮してないよ？ 気をつけてね〜
 *
 * @access public
 * @param string $virtual_class_name 仮想的なクラス名
 * @return object 解決されたクラス名をnewしたインスタンス。
 */
static public function create($virtual_class_name)
{
  // まずクラス名を取得
  $cn = self::get_name($virtual_class_name);

  // で、にゅ〜
  return new $cn;
}

//
static private $config_obj_ = null;

} // end of class


