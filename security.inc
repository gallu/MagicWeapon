<?php
/**
 * securityクラス
 *
 * 基本は適切なエスケープ
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/
class security
{

/**
 * HTML用エスケープ処理
 *
 * JavaScript系用じゃないから注意してね！！
 *
 * @access public
 * @param string $base_string エスケープ対象の文字列
 * @param string $charset 文字コード
 * @return string エスケープされた文字コード
 */
static public function sanitize_html($base_string, $charset = '')
{
  if ('' === $charset) {
    return htmlspecialchars($base_string, ENT_QUOTES);
  }
  // else
  return htmlspecialchars($base_string, ENT_QUOTES, $charset);
}


/**
 * 正規表現用エスケープ処理
 *
 * @deprecated
 * @access public
 * @param string $base_string エスケープ対象の文字列
 * @return string エスケープされた文字コード
 */
static public function sanitize_regex($base_string)
{
  $ret = "";
  // XXX 数字のみの時にint扱いされないように。…カンベンしてくれ；；
  $base_string = $base_string . "";

  $length = strlen($base_string);
  for($i = 0; $i < $length; $i ++) {
    $itr = $base_string[$i];
    switch($itr) {
      // 以下の文字は '\'でエスケープしておく
      case '#':
      case '/':
      case '?':
      case '*':
      case '+':
      case '^':
      case '$':
      case '.':
      case '[':
      case ']':
      case '|':
      case '(':
      case ')':
      case '{':
      case '}':
      case '-':
      case '\\':
        $ret .= '\\';
        break;

      default:
        break;
    }
    $ret .= $itr;
  }
  //
  return $ret;
}

/**
 * DB用エスケープ処理
 *
 * XXX 基本、PDO系とかでプレスホルダーがよいと思う
 *
 * @deprecated
 * @access public
 * @param string $base_string エスケープ対象の文字列
 * @return string エスケープされた文字コード
 */
static public function sanitize_db($base_string)
{
  // 実装
$ret = "";
  // XXX 数字のみの時にint扱いされないように。…カンベンしてくれ；；
  $base_string = $base_string . "";
  $length = strlen($base_string);
  for($i = 0; $i < $length; $i ++) {
    $itr = $base_string[$i];
    switch($itr) {
      // 以下の文字は '\'でエスケープしておく
      case '\'':
      case ';':
      case '"':
      case '\\':
        $ret .= '\\';
        break;


      default:
        break;
    }
    $ret .= $itr;
  }

  //
  return $ret;
}

/**
 * DB用エスケープ処理、両端にシングルクォート付き
 *
 * XXX 基本、PDO系とかでプレスホルダーがよいと思う
 *
 * @deprecated
 * @access public
 * @param string $base_string エスケープ対象の文字列
 * @return string 両端にシングルクォート付きのエスケープされた文字コード
 */
static public function sanitize_db_add_quote($base_string, $quote_string = "'")
{
  // まずエスケープ
  $ret = security::sanitize_db($base_string);

  // クォートをつける
  return $quote_string . $ret . $quote_string;
}

/**
 * header Injection用エスケープ処理
 *
 * XXX とりあえず単純にCRLFを削除
 *
 * @access public
 * @param string $base_string エスケープ対象の文字列
 * @return string 両端にシングルクォート付きのエスケープされた文字コード
 */
static public function sanitize_header_fields_data($base_string)
{
  $ret = preg_replace('/[\r\n]/', '', $base_string);
  return $ret;
}

} // end of class

