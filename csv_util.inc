<?php

/**
 * CSV扱う用のお便利メソッド群
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

class csv_util {

/**
 * 「配列の配列」からCSV文字列を作成
 *
 * 細かいエラーチェックとかしてないんで注意しつつ。配列はarrayとarrayObjectのどちらにも対応
 * XXX // 一旦、全カラムにダブルクォートつける。省略したい用件が出てきたら、第二引数増設して対応
 * XXX // 改行コードも\r\n指定可能なほうがいいかねぇ？
 * @access public
 * @param array(array()) $base_data 配列の配列(CSVになるべき元データ)
 * @return string CSV文字列
 */
static public function make_csv_string($base_data) {
  $ret = '';
  foreach($base_data as $line) {
    // 作業領域
    $awk = array();
    foreach($line as $w) {
      // ダブルクォートを強制変換
      $w = str_replace('"', '""', $w);
      // 囲んで
      $w = '"' . $w . '"';
      // 追加
      $awk[] = $w;
    }
    // 追加
    $ret .= implode(',', $awk) . "\n";
  }
  //
  return $ret;
}

/**
 *  「一本の文字列」をCSVとして適切にパース
 *
 * @access public
 * @param string $csv_string csv文字列
 * @return array(array()) CSV(配列の配列)
 */
static public function parse_string($csv_string) {
  // とりあえず「疑似ファイル」ん中に入れる
  $fp = fopen('php://temp', 'r+');
  fwrite($fp, $csv_string);
  fseek($fp, 0, SEEK_SET);

  // 改めて、分解
  $ret = array();
  while($row = fgetcsv($fp)) {
    $ret[] = $row;
  }

  //
  return $ret;
}

} // end of class

