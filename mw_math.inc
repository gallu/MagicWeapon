<?php

/**
 * MWの数学クラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

class mw_math {

/**
 * 平方根を求める
 *
 * 引数がGMP(任意長整数)でもOKなあたりがポイント
 *
 * @access public
 * @param intまたはGMPオブジェクト $x 平方根を求めたい値
 * @return float(っつかdouble) 平方根
 */
static public function sqrt($x)
{
  list($a, $b) = gmp_sqrtrem($x);
  $a = gmp_strval($a);
  $b = gmp_strval($b);
  $c = sqrt(pow($a, 2) + $b) - $a;
  $pp = $a + $c;

  //
  return $pp;
}


/**
 * 平均値を求める
 *
 * かなり仮想的なメソッド。基本的にはvectorまたは vector<vector>を受け取る
 * 単純な配列が依頼パラメタなら、それに対する返答を。
 * 「配列の配列」が依頼パラメタなら、return もまた「配列の配列」になる。
 * typeで現状取り得るのは以下の通り
 *   'arithmetical' : 相加平均(デフォルト)
 *
 * @access public
 * @param array(vector)またはarray(vector<vector>) $base 値
 * @param string $type 平均値の種別
 * @return array(hash)またはarray(vector<hash>) 戻り値。value:平均値, total:合計値: count:データ個数
 */
static function average(array $base, $type = 'arithmetical')
{

}



} // end of class

