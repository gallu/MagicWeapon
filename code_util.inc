<?php

/**
 * コード系ユーティリティ
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/


class code_util {

	/**
	 * 都道府県取得
	 *
	 * @access public
	 * @param int $i 都道府県コード
	 * @return string コードにあった値 問題があれば空文字
	 */
	static public function prefecture($i)
	{
		if (1 > $i) { return ""; }
		if (47 < $i) { return ""; }

		//
		$area_val = array(
    '1' => '北海道',
    '2' => '青森県',
    '3' => '岩手県',
    '4' => '宮城県',
    '5' => '秋田県',
    '6' => '山形県',
    '7' => '福島県',
    '8' => '茨城県',
    '9' => '栃木県',
    '10' => '群馬県',
    '11' => '埼玉県',
    '12' => '千葉県',
    '13' => '東京都',
    '14' => '神奈川県',
    '15' => '新潟県',
    '16' => '富山県',
    '17' => '石川県',
    '18' => '福井県',
    '19' => '山梨県',
    '20' => '長野県',
    '21' => '岐阜県',
    '22' => '静岡県',
    '23' => '愛知県',
    '24' => '三重県',
    '25' => '滋賀県',
    '26' => '京都府',
    '27' => '大阪府',
    '28' => '兵庫県',
    '29' => '奈良県',
    '30' => '和歌山県',
    '31' => '鳥取県',
    '32' => '島根県',
    '33' => '岡山県',
    '34' => '広島県',
    '35' => '山口県',
    '36' => '徳島県',
    '37' => '香川県',
    '38' => '愛媛県',
    '39' => '高知県',
    '40' => '福岡県',
    '41' => '佐賀県',
    '42' => '長崎県',
    '43' => '熊本県',
    '44' => '大分県',
    '45' => '宮崎県',
    '46' => '鹿児島県',
    '47' => '沖縄県' );
		//
		return $area_val[$i];
	}

	/**
	 * 62進数変換
	 *
	 * @access public
	 * @param int $dec 変換前の数値:10進数前提
	 * @return string 数値を62進数に変換
	 */
	static public function to62($dec)
	{
		//
		static $ar = array("0","1","2","3","4","5","6","7","8","9",
              "a", "b","c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
              "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
              "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
              "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
              );

              //
              $ret = "";

              do {
              	$bottom = $dec % 62;
              	$ret = $ar[$bottom] . $ret;
              	$dec = (int)($dec/62);
              } while($dec);

              return $ret;
	}

	/**
	 * 62進数を10進数に変換
	 *
	 * @access public
	 * @param int $dec 変換前の数値:10進数前提
	 * @return string 数値を62進数に変換
	 */
	static public function from62to10($s)
	{
		// XXX 重複やだなぁ…どしよ？
		static $ar = array("0","1","2","3","4","5","6","7","8","9",
              "a", "b","c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
              "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
              "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
              "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
              );


              // 一旦反転…毎回だと重いかなぁ？ 反転データもっといたほうがよい？
              $arr = array_flip($ar);

              //
              $ret = 0;

              // 先頭から一文字づつ精査
              for($i = 0; $i < strlen($s); $i ++) {
                $ret *= 62; // まず桁をずらして
                $ret += $arr[$s[$i]]; // 現在の桁の数値を足しこむ
              }

              //
              return $ret;
	}

	/**
	 * 金額用フォーマット
	 *
	 * ３桁ごとに金額に，(正確にはdelimiterの値)をぶち込む
	 * XXX 負の値とかまだテストしてない〜
	 *
	 * @access public
	 * @param int $i 金額
	 * @param string $delimiter デリミタ
	 * @return string デリミタで区切られた文字列
	 */
	static public function money_format($i, $delimiter = ',')
	{
		$awk = array();
		while(0 !== $i) {
			array_unshift($awk,  sprintf("%03d", $i % 1000));
			$i = (int)floor($i / 1000);
		}

		// XXX 先頭要素だけ０paddingを取り除く
		$awk[0] = (string)$awk[0] + 0;

		if (false === empty($awk)) {
			$ret = implode($delimiter, $awk);
		} else {
			$ret = '0';
		}

		//
		return $ret;
	}

	//private:


} // end of class

