<?php

/**
 * トークナイザー
 *
 * ユニークなトークンを生成する
 * XXX まず大抵の状況でユニークになると思います
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 */
require_once('code_util.inc');
require_once('network_util.inc');

class tokenizer 
{

/**
 * token作成メインロジック
 *
 * 一応publicだけどほとんどprotectedに近いので、あんまり直接はcallしないほうが、面倒くさくない。
 *
 * @access public
 * @param string $delimiter 各値をつなげるデリミタ文字
 * @param boolean $flg_ran trueなら、tokenの１要素に乱数を用いる
 * @param boolean $flg_pid trueなら、tokenの１要素にプロセスIDを用いる
 * @param boolean $flg_ip trueなら、tokenの１要素にIPアドレスを用いる
 * @return string 一意のトークン
 */
static public function _get($delimiter, $flg_ran, $flg_pid, $flg_ip)
{
$tokens = array();
  // 秒＆マイクロ秒を取得
  // XXX …関数の復帰値が空白デリミタ。…正気か？
  list($usec, $sec) = explode(" ", microtime());
  
  // ここはデフォルト
  // エポック秒：まぁ全部はいらんのでちょっと小さめに
  $tokens[] = $sec - 1079340000;
  // マイクロ秒
  $tokens[] = (int)($usec * 1000 * 1000);

  // 乱数
  if ($flg_ran) {
    $tokens[] = mt_rand();
  }

  // プロセスID
  if ($flg_pid) {
    $tokens[] = getmypid();
  }

  // XXX thread ID …PHPだと取れない ；；

  // IPアドレス
  if ($flg_ip) {
    $tokens[] = str_replace(".", "", network_util::get_local_ip());
  }

  // で、作成
  $ret = array();
  foreach($tokens as $token) {
    $ret[] = code_util::to62($token);
  }
  return implode($delimiter, $ret);
}


/**
 * token作成
 *
 * 「秒-ミリ秒-プロセスID-乱数」で構成されたtokenを作る。
 *
 * @access public
 * @param string $delimiter 各値をつなげるデリミタ文字。省略時は'-'を用いる
 * @return string 一意のトークン
 */
static function get($delimiter = "-")
{
  // とりあえず乱数＋プロセスID
  // XXX ま、問題なかろうて
  return tokenizer::_get($delimiter, true, true, false);
}


} // end of class

