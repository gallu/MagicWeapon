<?php

/**
 * 携帯機種情報の取得
 *
 * 珍しくコンストラクタに処理がある点に注意
 * あと。このクラスこそシングルトンでもいいような…
 * 以下、参考にしたURI
http://www.hajimeteno.ne.jp/i-mode/user_agent.html
http://developers.softbankmobile.co.jp/dp/tool_dl/web/useragent.php
http://www.nttdocomo.co.jp/service/imode/make/content/spec/useragent/index.html
http://www.au.kddi.com/ezfactory/tec/spec/4_4.html

XXX UIDと機種固有番号は別物っぽ。両方実装しないとね orz

 *
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

/*
■ドコモ（DoCoMo・i-mode）編

サイトタグにutn属性をつけると、固体識別番号を送信してもらえます。
ただし、固体識別番号の送信前に、固体識別番号送信の諾否をユーザーに確認する画面が表示され、
ユーザーが確認画面で「YES」ボタンを押した場合のみ固体識別番号が送信されます。

utn属性の付加の仕方
例：
<A href="URL" utn>
<FORM method="メソッド" action="URL" utn>

送信されてくる固体識別番号

MOVA
「DoCoMo/1.0/X503i/c10/ser*********** 」
ユーザーエージェントの後ろに付加される形
serの後ろが固体識別番号

FOMA
「DoCoMo/2.0 YYYY(c10;serXXXXXXXXXXXXXXX; iccxxxxxxxxxxxxxxxxxxxx)」

serXXXXXXXXXXXXXXX
ser：FOMA端末個体識別子
iccxxxxxxxxxxxxxxxxxxxx
icc：FOMAカード個体識別子

■AU（ezweb）編

AUは、基本的にサブスクライバID（EZ番号）と呼ばれる固体識別子を毎回送信しています。
（ただし、現在サブスクライバIDの「送信する・しない」をユーザーが選択できるようになっています）

サブスクライバIDは、環境変数　'HTTP_X_UP_SUBNO'　に収められています。
perl　：　$ENV｛'HTTP_X_UP_SUBNO'｝
PHP　：　$_SERVER['HTTP_X_UP_SUBNO']

形式：xxxxxxxxxxxxxx_t*.ezweb.ne.jp

■SOFTBANK（ボーダフォン・j-sky）編

ソフトバンクは、パケット通信対応機のみ固体識別番号を取得できます。

vodafoneの場合、固体識別番号は、常にユーザーエージェントに含まれています。

ユーザーエージェント例
SoftBank/1.0/910T/TJ001/SN Serial Browser/NetFront/3.3 Profile/MIDP-2.0 Configuration/CLDC-1.1
Vodafone/1.0/V802SH/SHJ001[/Serial] Browser/UP.Browser/7.0.2.1 Profile/MIDP-2.0 Configuration/CLDC-1.1 Ext-J-Profile/JSCL-1.2.2 Ext-V-Profile/VSCL-2.0.0
J-PHONE/5.0/V801SH[/Serial] SH/0005aaProfile/MIDP-1.0 Configuration/CLDC-1.0 Ext-Profile/JSCL-1.1.0

/Serial　の部分が固体識別番号となります。

携帯電話製造番号はユーザがユーザID通知の送出を許可した場合のみ追加されます。
 */

class mobile_info {

public function __construct()
{
  //
  //parent::__construct();
  $this->init();
}

/**
 * 初期化
 *
 * 初期化とか言いつつ実処理一通りここでやってる
 * 現状分解不可能な、拾い上げたuser-agentは以下のとおり

Mozilla/4.0 (compatible; MSIE 6.0; KDDI-HI37) Opera 8.00 [ja]
Mozilla/4.0 (compatible; MSIE 6.0; KDDI-SA39) Opera 8.60 [ja]
Mozilla/4.0 (compatible; MSIE 6.0; KDDI-KC38) Opera 8.50 [ja]
P505i DoCoMo/1.0/P505i/c20/TB/W20H10

 *
 * @access public
 */
public function init()
{
  // 初期化
  // XXX 初期状態っていうかどれにも引っかからなかったとき用の設定
  $this->set_sid('');
  $this->set_carrier(0);
  $this->set_type("");

  // user-agent取得
  $ua = getenv('HTTP_USER_AGENT');
//var_dump($ua);
  if (false === $ua) {
// XXX false、空文字設定だとfalseになるっぽ？
//print "getenv false ?\n";
    return false;
  }


  // スマフォ用
  if (false !== strpos($ua, 'iPhone')) {
    // iPhone
    $this->set_carrier(2048);
    //
    return ;
  }
  // else
  if (false !== strpos($ua, 'Android')) {
    // Android
    $this->set_carrier(4096);
    // XXX 若干怪しいんだけど…
    $awk =  explode(' ', $ua);
    $this->set_type($awk[6]);
    //
    return ;
  }
  // else


  // 以下、いわゆるガラケー用
  // -----------------------------------------------

  // まず分割
  $awk = explode('/', $ua);

  // 推測と処理
  // 携帯前提で出現可能性順
  if (0 === strncmp('KDDI', $awk[0], 4)) {
    $this->set_carrier(8);
    $this->set_type(substr($awk[0], 5, (strpos($awk[0], ' ') - 5)));
    $this->set_sid(getenv('HTTP_X_UP_SUBNO'));
  } else
  if (0 === strncmp('DoCoMo', $awk[0], 6)) {
    if ('1.0' === $awk[1]) {
      // DoCoMo/1.0/機種名/キャッシュサイズ/個体識別情報
      $this->set_carrier(1);
      $this->set_type($awk[1]);
      //$this->set_sid();
    } else {
      // DoCoMo/2.0 機種名 (キャッシュサイズ;個体識別情報)
      $this->set_carrier(3);
      // 機種名の取得
      $type = substr($awk[1], 4);
      $pos = strpos($type, '(');
      if (false !== $pos) {
        $type = substr($type, 0, $pos);
      }
      $this->set_type($type);
      //$this->set_sid();
    }
    // iモードID取得処理
    $this->set_sid(getenv('HTTP_X_DCMGUID'));
  } else
  if (0 === strncmp('SoftBank', $awk[0], 8)) {
    $this->set_carrier(128 + 1024);
    $this->set_type($awk[2]);
    //$this->set_sid();
    // 想定書式
    // SoftBank/[ブラウザバージョン]/[機種名]/[端末バージョン]/[端末シリアル番号]■[ユーザエージェント種別]/[ユーザエージェント名]/[ユーザエージェントバージョン]■[Java情報(Profile■/Configuration/)]

    // XXX 判定式本当はうそ
    // XXX ない場合のuser-agentは以下のとおり
    // SoftBank/1.0/811SH/SHJ001 Browser/NetFront/3.3 Profile/MIDP-2.0 Configuration/CLDC-1.1
    // XXX とりあえず無問題なのでこのままにするけど必要に応じて修正して orz
//var_dump($awk[4]);
/*
    if ("" !== $awk[4]) {
      if (0 === strncmp('SN', $awk[4], 2)) {
        //
        $aaawk = explode(' ', $awk[4]);
        $this->set_sid($aaawk[0]);
      }
    }
*/
    // XXX こっちらしいっぽ
    $this->set_sid(getenv('HTTP_X_JPHONE_UID'));
  } else
  if (0 === strncmp('UP.Browser', $awk[0], 10)) {
    $this->set_carrier(8 + 16);
    $pos = strpos($awk[2], '-') + 1;
    $pos2 = strpos($awk[2], ' ');
    $type = substr($awk[2], $pos, ($pos2 - $pos));
    $this->set_type($type);
    $this->set_sid(getenv('HTTP_X_UP_SUBNO'));
  } else
  if (0 === strncmp('Vodafone', $awk[0], 8)) {
    $this->set_carrier(128 + 512);
    $this->set_type($awk[2]);

    // XXX 以下のロジック嘘云々は上のSoftBankの時のロジック参照して〜
    // XXX 以下資料
    // Vodafone/1.0/V705SH/SHJ001 Browser/VF-NetFront/3.3 Profile/MIDP-2.0 Configuration/CLDC-1.1
//var_dump($awk[4]);
/*
    if ("" !== $awk[4]) {
      if (0 === strncmp('SN', $awk[4], 2)) {
        //
        $aaawk = explode(' ', $awk[4]);
        $this->set_sid($aaawk[0]);
      }
    }
*/
    // XXX こっちらしいっぽ
    $this->set_sid(getenv('HTTP_X_JPHONE_UID'));
  } else
  if (0 === strncmp('J-PHONE', $awk[0], 8)) {
    $this->set_carrier(128 + 256);
    $this->set_type($awk[2]);
    // XXX 以下のロジック嘘云々は上のSoftBankの時のロジック参照して〜
    // XXX 以下資料
    // Vodafone/1.0/V905SH/SHJ001 Browser/VF-NetFront/3.3 Profile/MIDP-2.0 Configuration/CLDC-1.1
//var_dump($awk[4]);
/*
    if ("" !== $awk[3]) {
      if (0 === strncmp('SN', $awk[3], 2)) {
        //
        $aaawk = explode(' ', $awk[3]);
        $this->set_sid($aaawk[0]);
      }
    }
*/
    // XXX こっちらしいっぽ
    $this->set_sid(getenv('HTTP_X_JPHONE_UID'));
  } else {
//print "$ua \n";
    $this->set_carrier(0);
    $this->set_type("");
    //$this->set_sid();
  }

  //
  return true;
}

//
protected function get_carrier() { return $this->carrier_; }
protected function set_carrier($i) { $this->carrier_ = $i; }

// ビット演算用内部関数
protected function _is_carrier($i, $j) { return ($i & $j)?true:false; }

/**
 * DoCoMoキャリア判定
 *
 * @access public
 * @return boolean DoCoMo系のキャリアならtrue
 */
public function is_docomo()
{
  return $this->_is_carrier($this->get_carrier(), 1);
}

/**
 * FOMA判定
 *
 * @access public
 * @return boolean FOMAならtrue
 */
public function is_foma()
{
  return $this->_is_carrier($this->get_carrier(), 2);
}

/**
 * EZキャリア判定
 *
 * @access public
 * @return boolean EZ系のキャリアならtrue
 */
public function is_ez()
{
  return $this->_is_carrier($this->get_carrier(), 8);
}
public function is_au() { return $this->is_ez(); }

/**
 * HDML系機種判定
 *
 * @access public
 * @return boolean HDML系の機種ならtrue
 */
public function is_hdml()
{
  return $this->_is_carrier($this->get_carrier(), 16);
}


/**
 * SoftBankキャリア判定
 *
 * SoftBank系全部を指し示す点に注意!!
 *
 * @access public
 * @return boolean SoftBank系のキャリアならtrue
 */
public function is_softbank()
{
  return $this->_is_carrier($this->get_carrier(), 128);
}

/**
 * J-PHONE端末判定
 *
 * @access public
 * @return boolean J-PHONE端末ならtrue
 */
public function is_terminal_jphone()
{
  return $this->_is_carrier($this->get_carrier(), 256);
}
/**
 * Vodafone端末判定
 *
 * @access public
 * @return boolean Vodafone端末ならtrue
 */
public function is_terminal_vodafone()
{
  return $this->_is_carrier($this->get_carrier(), 512);
}
/**
 * SoftBank端末判定
 *
 * @access public
 * @return boolean SoftBank端末ならtrue
 */
public function is_terminal_softbank()
{
  return $this->_is_carrier($this->get_carrier(), 1024);
}


/**
 * iPhone端末判定
 *
 * @access public
 * @return boolean iPhone端末ならtrue
 */
public function is_iphone()
{
  return $this->_is_carrier($this->get_carrier(), 2048);
}
/**
 * Android端末判定
 *
 * @access public
 * @return boolean Android端末ならtrue
 */
public function is_android()
{
  return $this->_is_carrier($this->get_carrier(), 4096);
}



/**
 * 携帯かどうか判定
 *
 * 当面、is_docomo()、is_ez、is_softbankをorで
 *
 * @access public
 * @return boolean 携帯系ならtrue
 */
public function is_mobile()
{
  $ret = $this->is_softbank() || $this->is_ez() || $this->is_docomo();
  return $ret;
}
/**
 * スマートフォンかどうか判定
 *
 * @access public
 * @return boolean スマフォ系ならtrue
 */
public function is_smartphone()
{
  $ret = $this->is_iphone() || $this->is_android();
  return $ret;
}

//
protected function set_type($s) { $this->type_ = $s; }

/**
 * 機種名取得
 *
 * XXX スマートフォンは一旦未対応
 *
 * @access public
 * @return string 機種名。不明の場合空文字
 */
public function get_type() { return $this->type_; }


// XXX 処理雑
protected function set_sid($s) { $this->sid_ = $s . ''; }
/**
 * サブスクライバID取得
 *
 * 非公式サイトで便利
 *
 * @access public
 * @return string サブスクライバID。不明の場合空文字
 */
public function get_sid() { return $this->sid_; }

//private:
private $type_;		// 機種名
private $sid_;		// サブスクライバID
private $carrier_;	// キャリア名：実際には数値ビットで見ること
			//   1:DoCoMo
			//   2:FOMA
			//   4:予約
			//   8:au
			//  16:HDML端末
			//  32:予約
			//  64:予約
			// 128:softbank系
			// 256:J-PHONE端末
			// 512:Vodafone端末
			//1024:SoftBank端末
			//2048:iPhone端末
			//4096:Android端末

} // end of class

