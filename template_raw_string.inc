<?php

/**
 * テンプレート用の「この文字はエスケープしないで」目印用クラス
 *
 * 「全体的にエスケープベースだけどこれだけはエスケープしたくない」時に。
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

class template_raw_string {

public function __construct($s)
{
  $this->s_ = $s;
}

/**
 * 文字列の取得
 *
 * @access publicかprivateかprotectedか
 * @return string 文字列
 */
public function get_string()
{
  return $this->s_;
}


//private:
private $s_;
} // end of class

