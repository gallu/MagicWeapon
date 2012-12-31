<?php

/**
 * HTML パーサ
 *
 * ざっくりとしたHTMLのパーサ。HTMLタグ、要素、コメントを切り分ける。
 *
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

require_once('multimap.inc');


class html_parser {

public function __construct()
{
  //
  //parent::__construct();
  $this->init();
}

//
public function init()
{
}

/**
 * HTML stringの設定
 *
 * 実際には、HTMLのstringを受け取って速やかにパース、配列化して情報を設定
 *
 * @access public
 * @param string $html_string HTMLまたはXHTML(XMLでもいいけどさ)
 * @return boolean 正常終了ならtrue。何か問題あればfalse
 */
public function set_html_string($html_string)
{
//var_dump($html_string);
  //
  $awk = array();

  // XXX 本当ならオートマトンかねぇ…
  $mode = 'm1';
  $len = strlen($html_string);
  $buf = '';
  for($i = 0; $i < $len; $i ++) {
    //
    if ('m1' === $mode) {
      //
      if (('<' === $html_string[$i])&&('!' === $html_string[$i+1])&&('-' === $html_string[$i+2])&&('-' === $html_string[$i+3])) {
        // データをバッファから配列にpushして
        $buf = trim($buf);
        if ('' !== $buf) {
          //$awk[] = $buf;
          $obj = new html_parser_parts_contents();
          $obj->set_contents($buf);
          $awk[] = $obj;
          $buf = '';
        }
        // モード変えて
        $mode = 'm3';
        // posを進めて( --とかいらないし )
        $i += 3;
        // 次〜
        continue;
      } else
      if ('<' === $html_string[$i]) {
        // データをバッファから配列にpushして
        $buf = trim($buf);
        if ('' !== $buf) {
          //$awk[] = $buf;
          $obj = new html_parser_parts_contents();
          $obj->set_contents($buf);
          $awk[] = $obj;
          $buf = '';
        }
        // モード変えて
        $mode = 'm2';
        // 次〜
        continue;
      }
    } else
    if ('m2' === $mode) {
      //
      if ('>' === $html_string[$i]) {
        // データをバッファから配列にpushして
        $buf = trim($buf);
        if ('' !== $buf) {
          //$awk[] = $buf;
          if ('/' === $buf[0]) {
            $obj = new html_parser_parts_end_tag();
            $obj->set_element_name( substr($buf, 1) );
          } else {
            $obj = new html_parser_parts_start_tag();
            $obj->set_element_and_attributes($buf);
          }
          $obj->set_raw_string( '<' . $buf . '>' );
//var_dump( '<' . $buf . '>' );
          $awk[] = $obj;
//var_dump($obj);
          $buf = '';
        }
        // モード変えて
        $mode = 'm1';
        // 次〜
        continue;
      }
    } else
    if ('m3' === $mode) {
      //
      if (('-' === $html_string[$i])&&('-' === $html_string[$i+1])&&('>' === $html_string[$i+2])) {
        // データをバッファから配列にpushして
        $buf = trim($buf);
        if ('' !== $buf) {
          //$awk[] = $buf;
          $obj = new html_parser_parts_comment();
          $obj->set_comment($buf);
          $awk[] = $obj;
          $buf = '';
        }
        // モード変えて
        $mode = 'm1';
        // posを進めて( --とかいらないし )
        $i += 2;
        // 次〜
        continue;
      }
    }
    // else
    $buf .= $html_string[$i];
  }
//var_dump($awk);
  //
  $this->set_html_array($awk);

  //
  return true;
}

/**
 * 現在保持情報(配列)からの、HTML stringの作成
 *
 * @access public
 * @return string データがない、異常がある、などの場合は空文字を返す
 */
public function get_html_string()
{
  // XXX 未実装
}

/**
 * HTMLをパースした結果としての配列をげと
 *
 * @access public
 * @param 型 名前 説明
 * @return 戻り値の型 簡単な説明
 */
public function get_html_array() { return $this->html_parts_array_; }
protected function set_html_array($a) { $this->html_parts_array_ = $a; }


//private:
private $html_parts_array_;

} // end of class


// ----------------------------------------------------------------
// 以下、ここでしか使わないクラスなので、このファイルにまとめてる〜
// ------------------------------------------------------------------

// 結果用クラスの基底
class html_parser_parts_base {

public function __construct()
{
  //
  //parent::__construct();
  $this->init();
}

//
public function init()
{
  //
  $this->contents_ = false;
  $this->comment_ = false;
  $this->html_tag_ = false;
  $this->html_start_tag_ = false;
  $this->html_end_tag_ = false;
}

public function is_contents() { return $this->contents_; }
public function is_comment() { return $this->comment_; }
public function is_html_tag() { return $this->html_tag_; }
public function is_html_start_tag() { return $this->html_start_tag_; }
public function is_html_end_tag() { return $this->html_end_tag_; }

// private:
// XXX 内部向けオンリーなので、ウルトラ雑に、protected
protected $contents_;
protected $comment_;
protected $html_tag_;
protected $html_start_tag_;
protected $html_end_tag_;
} // end of class



// こんてんつだぉ〜
class html_parser_parts_contents extends html_parser_parts_base {
//
public function init()
{
  //
  parent::init();
  //
  $this->contents_ = true;
  //
  $this->set_contents('');
}

//public:
  // contents
  public function get_contents() { return $this->contents_string_; }
  public function set_contents($v) { $this->contents_string_ = $v; }


//private:
private $contents_string_;
} // end of class


// こめんと〜
class html_parser_parts_comment extends html_parser_parts_base {
//
public function init()
{
  //
  parent::init();
  //
  $this->comment_ = true;
  //
  $this->set_comment('');
}

//public:
  // comment
  public function get_comment() { return $this->comment_string_; }
  public function set_comment($v) { $this->comment_string_ = $v; }


//private:
private $comment_string_;

} // end of class



// タグ系〜
class html_parser_parts_tag extends html_parser_parts_base {
//
public function init()
{
  //
  parent::init();
  //
  $this->html_tag_ = true;
  //
  $this->set_element_name('');
  $this->set_raw_string('');
}

//public:
  // element_name
  public function get_element_name() { return $this->element_name_; }
  public function set_element_name($v) { $this->element_name_ = strtolower($v); }

  // raw_string_
  public function get_raw_string() { return $this->raw_string_; }
  public function set_raw_string($v) { $this->raw_string_ = ($v); }


//private:
private $element_name_;
private $raw_string_;
} // end of class


// 開始タグ
class html_parser_parts_start_tag extends html_parser_parts_tag {
//
public function init()
{
  //
  parent::init();
  //
  $this->html_start_tag_ = true;
  //
  $this->set_attribute_array(new multimap());
}

//public:
  // attribute_array
  public function get_attribute_array() { return $this->attribute_array_; }
  public function set_attribute_array($v) { $this->attribute_array_ = $v; }

//
public function set_element_and_attributes($s) {
  // 分離
  // XXX 一端雑に。後で丁寧にやりなおす？
  // XXX METAタグの keywords とかでこけてるねぇ…後で〜 ノ
  $awk = explode(' ', $s);
//var_dump($s);
//var_dump($awk);

  // タグ名を入れる
  //$this->set_element_name($awk[0]);
  $this->set_element_name(array_shift($awk));
//var_dump($this->get_element_name());
//var_dump($awk);

  // アトリビュート系をぶち込む
  if (false === empty($awk)) {
    // とりあえずmultimapインスタンスげと
    $mobj = $this->get_attribute_array();
    // ぶん回しつつset
    foreach($awk as $wk) {
      //
      if (false === strpos($wk, '=')) {
        continue;
      }

      // まずは分解
//var_dump($wk);
//exit;
      list($name, $value) = explode('=', $wk, 2);
//var_dump($name);
//var_dump($value);
//print "\n";
      // 雑にリテラル除去
      if (('"' === $value[0])||("'" === $value[0])) {
        $value = substr($value, 1, strlen($value) - 2);
      }
//var_dump($value);
//print "\n";

      // 設定
      $mobj->insert($name, $value);
    }
  }
}


//private:
private $attribute_array_;
} // end of class


// 終了タグ
class html_parser_parts_end_tag extends html_parser_parts_tag {
//
public function init()
{
  //
  parent::init();
  //
  $this->html_end_tag_ = true;
}

} // end of class
