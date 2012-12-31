<?php

/**
 * USER-AGENTを見て許可したり不許可にしたりするクラス
 *
 * 一応単体でも使えるけど基本コントローラのゲートキーパで使うよねぇ
 * 以下書式例
# コメントあり
@pc
@mobile
@docomo
@softbank
@au
機種名

 *
 *
 *
 *
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

require_once('mobile_info.inc');

class user_agent_check {

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
 * mobile_infoインスタンスの設定
 *
 * @access public
 * @param mobile_info_obj $m mobile_infoのインスタンス
 */
public function set_mobile_obj($m) { $this->mobj_ = $m; }
public function get_mobile_obj() { return $this->mobj_; }

/**
 * 許可リストの設定
 *
 * @access public
 * @param string $s 許可リスト(改行区切り
 */
public function set_arrow_string($s)
{
  return $this->_set_permission_string($s, true);
}

/**
 * 拒否リストの設定
 *
 * @access public
 * @param string $s 拒否リスト(改行区切り
 */
public function set_denny_string($s)
{
  return $this->_set_permission_string($s, false);
}

/**
 * メソッドの説明(一行)：基本的にメソッド名
 *
 * @access protected
 * @param string $s リスト文字列
 * @param boolean $flg trueなら許可リスト、falseなら拒否リスト
 */
protected function _set_permission_string($s, $flg)
{
  // まずこっち設定
  $this->arrow_denny_ = $flg;

//var_dump($s);
  // とりあえず入ってきた文字列を配列にする
  // ちぃと処理雑
  // まず\rを\nに変換して
  $s = str_replace(array("\r\n", "\r"), array("\n", "\n"), $s);
  // 配列に置き換える
  $awk = explode("\n", $s);
//var_dump($awk);

  $this->category_ = array();
  $this->type_ = array();
  // 振り分け処理
  foreach($awk as $line) {
    $line = trim($line);
    if ('' === $line) {
      continue;
    }
    if ('#' === $line[0]) {
      continue;
    }
    //
    if ('@' === $line[0]) {
      $this->category_[] = $line;
    } else {
      $this->type_[] = $line;
    }
  }
//var_dump($this->category_);
//var_dump($this->type_);

  //
  return ;
}

/**
 * 判定
 *
 * @access public
 * @return boolean trueならアクセス許可、falseならアクセス不可
 */
public function is_permission()
{
$ret = "";

  // まず大枠
  if (false === empty($this->category_)) {
    foreach($this->category_ as $category) {
      // 一端べったべたに
      if ('@docomo' === $category) {
        $wk = $this->mobj_->is_docomo();
      } else
      if ('@ez' === $category) {
        $wk = $this->mobj_->is_ez();
      } else
      if ('@au' === $category) {
        $wk = $this->mobj_->is_ez();
      } else
      if ('@softbank' === $category) {
        $wk = $this->mobj_->is_softbank();
      } else
      if ('@mobile' === $category) {
        $wk = $this->mobj_->is_mobile();
      } else
      if ('@pc' === $category) {
        $wk = !($this->mobj_->is_mobile());
      } else {
        // 書式が変〜
        $wk = false;
      }

      // 判定
      if (true === $wk) {
        $ret = $this->arrow_denny_;
        break;
      }
    }
  }
  // なにかひっかかってたら
  if ("" !== $ret) {
    return $ret;
  }

  // 機種名個別
  if (false === empty($this->type_)) {
    foreach($this->type_ as $type) {
      if ($type === $this->mobj_->get_type()) {
        $ret = $this->arrow_denny_;
        break;
      }
    }
  }
  // なにかひっかかってたら
  if ("" !== $ret) {
    return $ret;
  }

  // 全部引っかからなければ
  return !($this->arrow_denny_);
}


//private:
private $mobj_;
private $category_;	// @がついてる、大枠
private $type_;		// 機種名そのもの
private $arrow_denny_;	// booleanで

} // end of class

