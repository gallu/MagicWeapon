<?php

/**
 * MW用 Smartyラッパー
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/


require_once('Smarty.class.php');

class mw_smarty extends Smarty {

/*
 * HTMLタグを直接assignで扱いたい時用
 *
 * @access public
 */
public function assign_unsecure_raw($tpl_var, $value = null)
{
  //
  parent::assign($tpl_var, $value);
}

/*
 * assignのセキュアラッパー
 *
 * @access public
 */
public function set($tpl_var, $value = null)
{
  //
  parent::assign($this->_esc($tpl_var), $this->_esc($value));
}

/*
 * 内部処理用関数
 *
 * @access protected
 */
protected function _esc($str){
  if(is_array($str)){
    $ret = array();
    foreach($str as $key => $val) {
      $ret[$key] = $this->_esc($val);
    }
    return $ret;
  }else{
    if (is_null($str)) {
      return null;
    }
    // else
    // XXX
    if (true === is_string($str) ) {
      return htmlspecialchars($str,ENT_QUOTES);
    }
    //else
    return $str;
  }
}


/**
 * 単置換のラッパー
 *
 * @access public
 * @param string $n 名前
 * @param string $v 値
 * @return boolean
 */
public function monoDic($n, $v)
{
  $this->assign($n, $v);
}

/**
 * 複置換のラッパー
 *
 * XXX 相当にアバウト orz
 *
 * @access publicかprivateかprotectedか
 * @param 型 名前 説明
 * @return 戻り値の型 簡単な説明
 */
public function multiDic($n, $v)
{
  $cn = get_class($v);
//var_dump($cn);

   if ('simple_put' === $cn) {
     $this->assign($n, 1);
   }
}


//private:


} // end of class

