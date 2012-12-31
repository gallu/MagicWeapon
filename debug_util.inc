<?php

/**
 * debug用utilクラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

class debug_util {

/**
 * 文字列のcharコード出力
 *
 * @access public
 * @param string& $s 出力したい文字列への参照
 * @param boolean $flg ここがtrueなら元文字列も出力、falseなら未出力
 * @return string 出力用文字列
 */
static public function dump_string(&$s, $flg = true)
{
//var_dump($s);
  $len = strlen($s);
  if (true === $flg) {
    $ret = $s . '(';
  } else {
    $ret = '(';
  }
  for($i = 0; $i < $len; $i ++) {
    $ret .= sprintf("(%02x)", ord($s[$i]));
  }
  $ret .= ')';
  return $ret;
}
/**
 * 文字列のcharコード出力:stdoutらっぱー
 *
 * @access public
 * @param string& $s 出力したい文字列への参照
 * @param boolean $flg ここがtrueなら元文字列も出力、falseなら未出力
 */
static public function dump_string_put(&$s, $flg = true)
{
  print debug_util::dump_string($s, $flg) . "\n";
}

/**
 * var_dumpを文字列で取得する
 *
 * @access public
 * @param object $o 出力したいObject
 * @return string 出力用文字列
 */
static public function var_dump_string($o)
{
  ob_start();
  var_dump($o);
  $s = ob_get_contents();
  ob_end_clean();
  return $s;
}

/**
 * debug_backtrace関数の整形版
 *
 * @access public
 * @return string 出力用文字列
 */
static public function backtrace_string()
{
  //
  $awk = debug_backtrace();

  // 一つ目は自分をcallしてるのがわかってるから削除
  array_shift($awk);

  // 逆積みしてるっぽいので配列を反転
  // XXX リバースイテレートとかありゃいいのに orz
  $awk = array_reverse($awk);

  $ret_array = array();
  foreach($awk as $wk) {
//var_dump($wk['class']);
//var_dump($wk['function']);
//var_dump($wk['args']);
    //
    $ret = $wk['class'] . $wk['type'] . $wk['function'] . '(';

    // XXX empty好かんのやけどなぁ
    if (false === empty($wk['args'])) {
      $type_awk = array();
      foreach($wk['args'] as $wk_args) {
        $type_awk[] = gettype($wk_args) . ' ' . $wk_args;
      }
      $ret .= implode(', ', $type_awk);
    }

    //
    $ret .= ')';

    //
    $ret_array[] = $ret;
  }
  //
  $ret = implode(' => ', $ret_array);

  //
  return $ret;
}

//private:


} // end of class

