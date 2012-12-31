<?php
/**
 * CONV(置換)クラス
 *
 * いわゆる「テンプレートエンジン」。
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

class f_conv {
// public:

public function __construct()
{
  $this->init();
}

//
public function init()
{
  $this->toku_ = array();
  $this->toku_yes_ = array();
  $this->toku_no_ = array();
  $this->mono_ = array();
  $this->fuku_ = array();
}

/**
 * テスト用内部データ取得
 *
 * 文字列で内部データを取得する。
 * stdoutに出すならT_putのほうが楽
 *
 * @access public
 * @return string 内部データ
 */
public function T_string() {
$ret = "";

  $ret .= "mono dictionary\n";
  foreach($this->mono_ as $name => $value) {
    $ret .= "  " . $name . ": " . $value . "\n";
  }
  $ret .= "\n";

  $ret .= "multi dictionary\n";
  foreach($this->fuku_ as $name => $value) {
    $ret .= "  " . $name . ": " . get_class($value) . "\n";
  }
  $ret .= "\n";

  $ret .= "tokusyu dictionary\n";
  foreach($this->toku_ as $name => $value) {
    $ret .= "  " . $name . "\n";
    $ret .= "	yes : " . $this->toku_yes_[$name] . "\n";
    $ret .= "	 no : " . $this->toku_no_[$name] . "\n";
    $ret .= "	data: ";

    foreach($this->toku_[$name] as $n => $v) {
      $ret .= $v . " , ";
    }
    $ret .= "\n";
  }
  //
  $ret .= "T_put Fin\n";

  //
  return $ret;
}

/**
 * テスト用内部データ出力
 *
 *
 * @access public
 */
public function T_put() {
  echo $this->T_string();
}

/**
 * 特殊置換系登録
 *
 * 主にchekbox、select、radioなど用の設定
 *
 * @access public
 * @param string $name 指示子名
 * @param string $y データ配列内に値が有るときの設定内容
 * @param string $n データ配列内に値が無いときの設定内容
 * @param array $d データ配列
 * @return boolean 何か問題があればfalse
 */
public function monoDicToku($name, $y, $n, $d) {
  if ("" === $name) {
    return false;
  }

  // データが配列ではないときに無理やり配列にしてみる
  if (is_array($d)) {
    $da = $d;
  } else {
  // XXX データが空なら配列にしない
      if ("" !== $d) {
      $da = array($d);
    } else {
      $da = $d;
    }
  }

  // セット
  $this->toku_[$name] = $da;
  $this->toku_yes_[$name] = $y;
  $this->toku_no_[$name] = $n;

  //
  return true;
}

/**
 * 単置換登録
 *
 * @access public
 * @param string $name 指示子名
 * @param string $value 設定する値
 * @return boolean 何か問題があればfalse
 */
public function monoDic($name, $value) {
  if ("" === $name) {
    return false;
  }
  //
  $this->mono_[$name] = $value;

  //
  return true;
}

/**
 * 複置換登録
 *
 * @access public
 * @param string $name 指示子名
 * @param クラスインスタンス $obj 実行されるべきクラス
 * @return boolean 何か問題があればfalse
 */
// 複置換系
public function multiDic($name, $obj) {
  if ("" === $name) {
    return false;
  }
  //
  $this->fuku_[$name] = $obj;

  //
  return true;
}

/**
 * 置換実施
 *
 * @access public
 * @param string $ret 置換もとの文字列
 * @return string 置換後の文字列。エラーの場合空文字列
 */
function conv($ret) {
  //
  if ("" === $ret) {
    return "";
  }

  // 複置換処理
//print "base is ($ret)\n";
  while( 1 == (preg_match("/(.*?)[\$]{3}([0-9a-zA-Z_-]*)[:]{0,1}(.*?)[\$]{3}(.*)/s", $ret, $wk)) ) {
    $before = $wk[1];
    $f_name = $wk[2];
    $f_param = $wk[3];
    $after = $wk[4];
//print "after is ($atfer)\n";
    if ( 1 == (preg_match( "/(.*?)[\$]{3}[\/]" . $f_name . "[\$]{3}(.*)/s", $after, $wk))) {
      $base_body = $wk[1];
//print "function name is $f_name($f_param)\n";
//print "base body is ($base_body)\n";
//print "after data is ($wk[2])\n";
      // 処理
      // 関数が存在すればまわす
      $buf = "";
      if (isset($this->fuku_[$f_name])) {
        $f = $this->fuku_[$f_name];
//print "call function $f_name($f_param)($f)<BR>\n";

//print_r($f);
        // オブジェクトのメソッドをcall
        $buf = $f->m_conv($base_body, $f_param);
      } else {
//print "not find : function $f_name($f_param)\n";
      }

      // 足しこむ
      $ret = $before . $buf . $wk[2];
//print "add is ($ret)\n";
    } else {
      // 指定部分を無視して残り
//print "no match\n";
      $ret = $before . $after;
    }
//print "multi: ret is ($ret)\n";
  }

  // 特殊単置換の処理
  while( 1 == (preg_match("/(.*)\*\*\*([0-9a-zA-Z_-]*):{0,1}(.*?)\*\*\*(.*)/s", $ret, $wk)) ) {
    $before = $wk[1];
    $name = $wk[2];
    $p = "" . $wk[3];
    $after = $wk[4];

    // ぶん回し
    $flg = "";
    if (isset($this->toku_[$name])) {
      if (is_array($this->toku_[$name])) {
        foreach ($this->toku_[$name] as $wk) {
//print "toku wk is $wk\n";
          if (0 == strcmp($wk, $p)) {
            $flg = "OK";
            break;
          }
        }

        // 判定
        if ("OK" == $flg) {
          $ret = $before . $this->toku_yes_[$name] . $after;
        } else {
          $ret = $before . $this->toku_no_[$name] . $after;
        }
      }

    } else {
      // データがない場合は無視
      $ret = $before . $after;
    }
//print "toku: ret is ($ret)\n";
  }

  // 単置換
  while( 1 == (preg_match("/(.*)%%%([0-9a-zA-Z_-]*)%%%(.*)/s", $ret, $wk)) ) {
//print "change $wk[2] <BR> \n";
    // 変数系
    if (isset($this->mono_[$wk[2]])) {
      $ret = $wk[1] . $this->mono_[$wk[2]] . $wk[3];
    } else {
      // XXX エラーどうする？
      $ret = $wk[1] . $wk[3];
    }
//print "mono: ret is ($ret)\n";
  }

  // XXX 単置換の追加
  $ret = preg_replace("/###/", "%%%", $ret);
  // XXX 特殊置換用の追加
  $ret = preg_replace("/@@@/", "***", $ret);

  //
  return $ret;
}


// private:
//
private $toku_;	// 特殊置換用データ
private $toku_yes_;	// 特殊置換用データ:値が一致したときの設定データ
private $toku_no_;	// 特殊置換用データ:値が一致しないときの設定データ

//
private $mono_;	// 単置換用データ

//
private $fuku_;	// 複置換用データ

} // end of class

