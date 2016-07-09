<?php

/**
 * マジックバイト(マジックナンバー)によるファイルフォーマット推測クラスで使う戻り値用クラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

class magic_byte_guess_return {
//
public function __construct($params) {
  // 「空配列」の場合、unknownとする
  if (true === empty($params)) {
    $this->extension_    = '';
    $this->content_type_ = '';
    $this->type_         = 'unknown';
    $this->description_  = 'unknown';
    //
    return ;
  }
  // else
  $this->extension_    = $params['extension'];
  $this->content_type_ = $params['content-type'];
  $this->type_         = $params['type'];
  $this->description_  = $params['description'];
}

/**
 * ファイルタイプが判定できているかどうか？
 *
 * 正しくは「unknownかどうか」なので、これがtrueなら「未判定」状態
 *
 * @access public
 * @return boolean 「未判定」ならtrue、「判定できている」ならfalse
 */
public function is_unknown() {
  return $this->_is_type('unknown');
}

/**
 * ファイルタイプが画像かどうか
 *
 * @access public
 * @return boolean 「画像」ならtrue、「画像以外」ならfalse
 */
public function is_image() {
  return $this->_is_type('image');
}

/**
 * ファイルタイプが圧縮ファイルかどうか
 *
 * @access public
 * @return boolean 「圧縮ファイル」ならtrue、「圧縮ファイル以外」ならfalse
 */
public function is_compression() {
  return $this->_is_type('compression');
}

// type判定用内部処理
protected function _is_type($s) {
  if ($s === $this->type_) {
    return true;
  }
  // else
  return false;
}

/**
 * 標準的な拡張子の取得
 *
 * @access public
 * @return string 拡張子
 */
public function get_extension() { return $this->extension_; }

/**
 * ファイルタイプ詳細の取得
 *
 * @access public
 * @return string ファイルタイプの詳細
 */
public function get_description() { return $this->description_; }

/**
 * Content-typeの取得
 *
 * @access public
 * @return string Content-type
 */
public function get_content_type() { return $this->content_type_; }


//private:
private $extension_;
private $content_type_;
private $type_;
private $description_;

} // end of class

