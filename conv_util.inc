<?php
/**
 * CONVを楽に使うためのユーティリティ群
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

require_once('secure_conv.inc');

/**
 * checked & selected を楽に設定する
 *
 * @access public
 * @param conv $conv convインスタンス
 * @param string $name checkedにしたい、HTMLのnameアトリビュート値
 * @param stringまたはarray $value valueの値が一致したらcheckedにする
 */
class conv_util
{

// checked用：radio , checkbox用
static public function monoDicChecked($conv, $name, $value)
{
  //
  if (is_array($value)) {
    $values = $value;
  } else {
    $values = array($value);
  }
  $conv->monoDicToku($name, "checked=\"checked\"", "", $values);
}

/**
 * selected を楽に設定する
 *
 * @access public
 * @param conv $conv convインスタンス
 * @param string $name checkedにしたい、HTMLのnameアトリビュート値
 * @param stringまたはarray $value valueの値が一致したらselectedにする
 */
static public function monoDicSelected($conv, $name, $value)
{
  //
  if (is_array($value)) {
    $values = $value;
  } else {
    $values = array($value);
  }
//  $values = array($value);
  $conv->monoDicToku($name, "selected=\"selected\"", "", $values);
}

/**
 * 「dataが空の時」に対応したsimple_loop
 *
 * @access public
 * @param conv $conv convインスタンス
 * @param array $data データ。形としてはvector<map<key,val> >の形式
 * @param string $name loopするmultiDic用のname
 * @param string $empty_name dataが空の時のmultiDicの名前。デフォルトは'if_' . $name . '_empty'
 */
static public function set_loop($conv, $data, $name, $empty_name = "")
{
  // まずempty nameの解決
  if ("" === $empty_name) {
    $empty_name = 'if_' . $name . '_empty';
  }
  // dataがあればloop、なければempty表示
  if (false === empty($data)) {
    $sl = new simple_loop;
    $sl->set_data($data);
    $conv->multiDic($name, $sl);
  } else {
    $sp = new simple_put;
    $conv->multiDic($empty_name, $sp);
  }
  
  //
  return ;
}


} // end of class


/**
 * CONV内で単純なループを処理するためのクラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/
class simple_loop
{
//public:
public function __construct($data = array())
{
  //
  if (false == empty($data)) {
    $this->set_data($data);
  }
}

/**
 * データの設定
 *
 * @access public
 * @param array $data loopで解決するデータ。形としてはvector<map<key,val> >の形式
 */
public function set_data($d) {$this->data_ = $d; }

/**
 * セキュリティ(HTMLエスケープ)をoffにしたい時
 *
 * @access public
 */
public function secure_off() { $this->secure_flg_ = false; }

/**
 * 複置換時にconv系クラスからcallされるメソッド
 *
 * @access public
 */
function  m_conv($body, $param) {
//print $body;exit;
//print "simple_loop start \n";
  $ret = "";
  // ぶん回す
  // XXX 空っぽだと怒られるのでとりあえず
  if (false === empty($this->data_)) {
    foreach ($this->data_ as $wk) {
      // 取り出し
      if (true === $this->secure_flg_) {
        $c = new secure_conv;
      } else {
        $c = new f_conv;
      }
      foreach($wk as $key => $value) {
        if (is_bool($value)) {
          if (true === $value) {
            $sp = new simple_put;
            $c->multiDic($key, $sp);
          }
        } else 
        if (is_array($value)) {
          $sl = new simple_loop;
          $sl->set_data($value);
          $c->multiDic($key, $sl);
        } else {
//print "$key, $value \n";
          $c->monoDic($key, $value);
        }
      }
      // 置換
      $ret .= $c->conv($body);
    }
  }
  //
  return $ret;
}

//private:
private $data_;
private $secure_flg_ = true;
} // end of class


/**
 * 単純な出力制御
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/
class simple_put
{
//public:
/**
 * 複置換時にconv系クラスからcallされるメソッド
 *
 * @access public
 */
function  m_conv($body, $param) {
  return $body;
}

} // end of class

/**
 * 非推奨クラス
 *
 * @deprecated
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/
// cgi_requestからの一式を出力
class simple_request_put
{
//public:

//
public function set_request($o) { $this->req_ = $o; }
public function get_request() { return $this->req_; }

//
public function set_conv($o) { $this->conv_ = $o; }
public function get_conv() { return $this->conv_; }

// メイン処理
public function set()
{
  //
  $c = $this->get_conv();
  $req = $this->get_request();

  // まず名前を一式げとする
  $allnames = $req->get_all_names();

  // XXX 空っぽだと怒られるのでとりあえず?
  if (false === empty($allnames)) {
    foreach($allnames as $name) {
      // monoDic
      $c->monoDic($name, $req->find($name));

      // check_box、radioはname_cで
      //conv_util::monoDicChecked($c, $name . '_c', $req->find_array($name));
      conv_util::monoDicChecked($c, 'checked_' . $name, $req->find_array($name));

      // selectはname_sで
      //conv_util::monoDicSelected($c, $name . '_s', $req->find_array($name));
      conv_util::monoDicSelected($c, 'selected_' . $name, $req->find_array($name));
    }
  }

  //
  return true;
}

//private:
private $req_;
private $conv_;
}

