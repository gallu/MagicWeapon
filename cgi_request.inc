<?php

/**
 * CGI リクエストクラス
 *
 * CGIリクエストをEUCに正しく変換して突っ返す
 * 他言語だとインスタンス必須だが、PHPではいらないかも
 * XXX バッファリングによる性能向上は後日マターって感じで
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  yyyy/mm/dd
 * @version $Id$
 **/

//require_once('debug_util.inc');

//
class cgi_request {

//
public function __construct()
{
  $this->init();
}

//
public function init()
{
  // デフォルトをここにしておく
  $this->set_hi_priority_post();
  // 文字コードが未指定なら、デフォルトUTF-8で設定しておく
  if (false === defined('MW_INPUT_ENCODING')) {
    define('MW_INPUT_ENCODING', 'UTF-8');
  }
  //
  if ('' === mb_internal_encoding()) {
    mb_internal_encoding('UTF-8');
  }
}

/**
 * 非推奨なので気にしないように
 */
public function set_priority($s) { $this->priority_ = $s; }
public function get_priority() { return $this->priority_; }

/**
 * GetもPostも見に行くが、Post情報を優先させる
 * @access    public
 */
public function set_hi_priority_post() { $this->priority_ = "pg"; }

/**
 * GetもPostも見に行くが、Get情報を優先させる
 * @access    public
 */
public function set_hi_priority_get() { $this->priority_ = "gp"; }

/**
 * Getのみを見に行く
 * @access    public
 */
public function set_only_get() { $this->priority_ = "g"; }

/**
 * Postのみを見に行く
 * @access    public
 */
public function set_only_post() { $this->priority_ = "p"; }

/**
 *
 * 全部の名前取得：PostとGet
 *
 */
public function get_all_names()
{
  $p_key = array_keys($_POST);
  $g_key = array_keys($_GET);
  // 配列を足しこむ
  $ret =  array_unique(array_merge($p_key, $g_key));
  //
  return $ret;
}


/**
 * 下請けの下請け。[配列 | 単変数]を配列に設定する
 * XXX これってUTIL行きなのか？
 * @access    protected
 * @param     &array	$base	値を入れ込むべき要素
 * @param     array	$add	足したい配列
 */
protected function _add_array(&$base, $add)
{
  if (is_array($add)) {
    foreach ($add as $wk) {
      array_push($base, $wk);
    }
  } else {
    array_push($base, $add);
  }
}

/**
 * 下請け関数。Get情報を取得
 * @access    public
 * @param     string	$key	検索用のkey。name=valueのname部分に相当
 * @return    arrayまたはstring	結果(の配列)。値が無い場合はnullを返す
 */
public function _get_get($key) {
  if (isset($_GET[$key])) {
    return $_GET[$key];
  }
  return null;
}

/**
 * 下請け関数。Post情報を取得
 * @access    public
 * @param     string    $key    検索用のkey。name=valueのname部分に相当
 * @return    arrayまたはstring 結果(の配列)。値が無い場合はnullを返す
 */
public function _get_post($key) {
  if (isset($_POST[$key])) {
    return $_POST[$key];
  }
  return null;
}

/*
 * 指定されたキーの存在チェック
 */
public function is_exist($key)
{
  // キー指定がなければ突っ返す
  if ("" == $key) {
    return false;
  }

  // 判定開始
  $flg = false;
  $length = strlen($this->priority_);
  for($i = 0; $i < $length; $i ++) {
    $itr = $this->priority_[$i];
    // pならpost情報を取得
    if ("p" == $itr) {
      $wk = $this->_get_post($key);
    }
    // gならget情報を取得
    if ("g" == $itr) {
      $wk = $this->_get_get($key);
    }
    // データがあったらtrueに
    if (!(is_null($wk))) {
      $flg = true;
    }
  }

  //
  return $flg;
}

/**
 * GetもしくはPostから情報を検索、結果を配列にして返す
 * XXX データは全て文字コードをEUCに直してから返す
 * @access    public
 * @param     string	$key	検索用のkey。name=valueのname部分に相当
 * @return    array(string)	結果の配列。ない場合は空配列を返す
 */
public function find_array($key = "")
{
$ret = array();
  // キー指定がなければ突っ返す
  if ("" == $key) {
    return $ret;
  }

  // 判定
  $length = strlen($this->priority_);
  for($i = 0; $i < $length; $i ++) {
    $itr = $this->priority_[$i];
    // pならpost情報を取得
    if ("p" == $itr) {
      $wk = $this->_get_post($key);
    }
    // gならget情報を取得
    if ("g" == $itr) {
      $wk = $this->_get_get($key);
    }
    // データがあったっぽいなら
    if (!(is_null($wk))) {
//debug_util::dump_string_put($wk);
      // 足しこむ
      $this->_add_array($ret, $wk);
    }
  }

  // 文字コード置換
  $eret = array();
  foreach ($ret as $buf) {
//debug_util::dump_string_put($buf);
    // XXX magic_quote対策追加
    if (1 === get_magic_quotes_gpc()) {
//print "magic_quote<br>\n";
      $buf = stripslashes($buf);
    }
//print "befor buf $buf<br>";
//debug_util::dump_string_put($buf);

    // 絵文字処理用(一旦ダミーメソッド)
    $buf = $this->interruption_emoji($buf);
//print "befor buf $buf<br>";
//debug_util::dump_string_put($buf);
    // 文字コード変換
    $buf = mb_convert_encoding($buf, mb_internal_encoding(), MW_INPUT_ENCODING);

//debug_util::dump_string_put($buf);
//print "\n";
//var_dump($buf);
    array_push($eret, $buf);
  }

  //
  return $eret;
}

// 単数用ラッパー
/**
 * GetもしくはPostから情報を検索、結果を単値で返す
 * @access    public
 * @param     string	$key	検索用のkey。name=valueのname部分に相当
 * @return    string	結果。ない場合はから文字を返す
 */
public function find($key = "")
{
  $ret = $this->find_array($key);
  if (0 == count($ret)) {
    return "";
  }
  //
  return $ret[0];
}

/**
 * Cookieから情報を検索、結果を配列にして返す
 *
 * XXX とりあえず文字コード置換はかけない
 *
 * @access    public
 * @param     string	$key	検索用のkey。name=valueのname部分に相当
 * @return    array(string)	結果の配列。ない場合は空配列を返す
 */
public function find_cookie_array($key = "")
{
  $ret = array();

  // キー指定がなければ突っ返す
  if ("" == $key) {
    return $ret;
  }

  // データが無ければ突っ返す
  if (!(isset($_COOKIE[$key]))) {
    return $ret;
  }
  // データ取得
  $wk = $_COOKIE[$key];

  // 配列ではない(単値)の場合
  if (!(is_array($wk))) {
    // 配列に
    $ret = array($wk);
  } else {
    // そのまんま
    $ret = $wk;
  }

  // magic_quoteがoffなら
  if (0 === get_magic_quotes_gpc()) {
    return $ret;
  }

  // onなら
  $ret2 = array();
  foreach ($ret as $buf) {
    // XXX magic_quote対策追加
    $ret2[] = stripslashes($buf);
  }

  //
  return $ret2;
}


// 単数用ラッパー
/**
 * Cookieから情報を検索、結果を返す
 * @access    public
 * @param     string    $key    検索用のkey。name=valueのname部分に相当
 * @return    string    結果。ない場合はから文字を返す
 */
public function find_cookie($key = "")
{
  $ret = $this->find_cookie_array($key);
  if (0 == count($ret)) {
    return "";
  }
  //
  return $ret[0];
}

/*
XXX 作る？
get_path_info()
*/

/**
 * １画面に複数遷移先があるときの判断を一発で
 * XXX type="image"が未考慮
 *
 * 使う側のイメージとしては
 * // デフォルトは配列のtop
 * $btn_name_array = ('rss_list_button', 'rss_result_button');
 * $btn_name = $req->choice($btn_name_array);
 * 
 * // デフォルトを明示的に指定
 * $btn_name_array = ('rss_list_button', 'rss_result_button');
 * $btn_name = $req->choice($btn_name_array, 'no_choice');
 *
 * @access    public
 * @param     array     $names  チェック対象のname一覧
 * @param     string    $default 対象がすべてNGだった時のデフォルト
 * @return    array(string)     結果の配列。ない場合は空配列を返す
 */
public function choice($names, $default = "")
{
  // デフォルト設定。ない場合は「配列の先頭」を選択
  $ret = $default;
  if ("" == $ret) {
    $ret = $names[0];
  }

  // チェック
  foreach ($names as $name) {
    // 見つかったら
    if ("" != $this->find($name)) {
      // 設定して抜け出す
      $ret = $name;
      break;
    }
  }
  //
  return $ret;
}

/**
 * uploadファイルの取得
 *
 * @access public
 * @param string $name FORMで指定したname
 * @param boolean $encode_flg trueなら文字codeをencodeする。defaultはfalse
 * @return string ファイルの中身。エラーの場合空文字列
 */
public function get_file($name, $encode_flg = false)
{
  // 一応チェック
  if (false === is_uploaded_file($_FILES[$name]['tmp_name'])) {
    return "";
  }
  // XXX move_uploaded_file はしない。tmpの場所指定とか面倒だし
  // XXX あとで別途「get_file_to_file」とかつくるかも

  // 中身をとりだす
  $s = file_get_contents($_FILES[$name]['tmp_name']);  

  // もしencode必要ならする
  if (true === $encode_flg) {
    $s = mb_convert_encoding($s, mb_internal_encoding(), MW_INPUT_ENCODING);
  }

  //
  return $s;
}

/**
 * 絵文字用のポリモフィズム用ダミーメソッド
 *
 * @access public
 * @param string $s データ
 * @return string 出力
 */
public function interruption_emoji($s)
{
  //
  return $s;
}


//private:
private $priority_;

} // end of class

