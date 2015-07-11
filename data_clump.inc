<?php

/**
 * データの塊(clump)クラス
 *
 * 実際には、データを固まりで扱い、かつ
 * ・cgiインタフェースとのやり取り
 * ・テンポラリテーブルとのやり取り
 * を統括する。
 *
 * ・DB、KVSなどとのやり取り
 * は、窓口にはなるが、実処理は data_clump_adaptor系クラス群が担当している
 *
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 */

/*
【CRUDのモードに関する考察】
Create
	もっとも深いレイヤーのみ書き込み、残りのレイヤーに対しては削除を行う

Read
	浅いレイヤーから順次探していく
	また、もっとも浅いレイヤー意外で見つかったら、見つかったレイヤーより浅いところのすべてに書き込んでおく

Update
	もっとも深いレイヤーのみ書き込み、残りのレイヤーに対しては削除を行う

Delete
	すべてのレイヤーに対して共通して消しこむ

 */


require_once('basis_clump.inc');
require_once('tokenizer.inc');
require_once('calendar_util.inc');
require_once('security.inc');
//require_once('session_data_db.inc');
require_once('conv_util.inc');

class data_clump extends basis_clump
{
//public:
// validation用
const V_MUST     =    1; // 必須
const V_STRING   =    2; // 文字列
const V_NUMERIC  =    4; // 数値(パラメタは値を示す
const V_DIGIT    =    8; // 数字(パラメタは桁数を示す
const V_ALPHA    =   16; // 英字
const V_ALPNUM   =   32; // 英数字
const V_POST     =   64; // 郵便番号(nnn-nnnn、nnnnnnn、nnn nnnnの３パターンフォロー
const V_EMAIL1   =  128; // メアド
const V_DATE     =  256; // 日付
const V_M_STRING =  512; // 全角交じりの文字列(サイズのチェックを 全角:2文字 半角:1文字として扱う
const V_KATAKANA_UTF8 = 1024; // UTF-8文字コードのカタカナ(文字長の規定はしてないんで、基本、V_M_STRINGと一緒がいいと思う)

// DB用変数の型
// XXX 論理和とかあり得ないから連番で
const DATATYPE_INT    = 1;
const DATATYPE_STRING = 2;
const DATATYPE_DATE   = 3;
const DATATYPE_FLOAT  = 4;

//
public function __construct()
{
  parent::__construct();
  //$this->init();
}

// 初期化
public function init()
{
  //
  parent::init();

  //
  $this->tblname_ = "";         // 正規DBテーブル名
  $this->dbh_ = array();        // データハンドル(keyが深度(レイヤー)で、０から)
  $this->adaptor_ = array();	// アダプタクラス格納
  //
  $this->tmpname_ = "";         // テンポラリテーブル名
  $this->tmp_key_ = "";         // テンポラリテーブル用キー

  //
  $this->nokeyname_ = array();  // Keyではない名前一式
  $this->keyname_ = array();    // Keyになってる名前一式

  //
  $this->data_types_ = array();

  //
  $this->cgi_name_ = array();   // CGI上の名前
  $this->tmp_name_ = array();   // テンポラリテーブル上の名前
  $this->db_name_ = array();    // 正規DB上の名前

  //
  $this->dic_selected_ = array();
  $this->dic_checed_ = array();

  //
  $this->v_type_ = array();
  $this->v_param_ = array();

  //
  $this->clear_mono_set_db_target();

  //
  $this->identifier_ = '';

  //
  $this->insert_date_name_ = '';
  $this->update_date_name_ = '';

  // 接頭接尾のクリア
  $this->set_cgi_name_prefix('');
  $this->set_cgi_name_suffix('');
}

/**
 * DBテーブル名の設定
 *
 * @access public
 * @param string $s テーブル名
 */
public function set_table_name($s) { $this->tblname_ = $s; }
/**
 * DBテーブル名の取得
 *
 * @access public
 * @return string テーブル名
 */
public function get_table_name() { return $this->tblname_; }


/**
 * データストア接続用識別子の設定
 *
 * XXX 普段は未使用。DBのシャーディングなんかで「database名違うのにテーブル名同じだよ orz」的な状況で使う
 *
 * @access public
 * @param string $s 識別子
 */
public function set_identifier($s) { $this->identifier_ = $s; }
/**
 * データストア接続用識別子の取得
 *
 * XXX 基本的には設定しないんでテーブル名。
 *
 * @access public
 * @return string 識別子
 */
public function get_identifier()
{
  if ('' !== $this->identifier_) {
    return $this->identifier_;
  }
  // else
 return $this->get_table_name();
}


/**
 * テンポラリテーブル名の設定
 *
 * データを一時的に格納するためのもの
 * XXX 非推奨
 *
 * @deprecated
 * @access public
 * @param string $s テーブル名
 */
public function set_session_table_name($s) { $this->tmpname_ = $s; }
/**
 * テンポラリテーブル名の取得
 * XXX 非推奨
 *
 * @deprecated
 * @access public
 * @return string テーブル名
 */
public function get_session_table_name() { return $this->tmpname_; }

/**
 * cgi名に付ける前置詞
 * 
 * 「複数個」の同じような情報があり、1対nのケースの時なんかに使う
 *
 * @access public
 * @param string $v 前置詞
 */
public function set_cgi_name_prefix($v) { $this->cgi_name_prefix_ = $v; }
public function get_cgi_name_prefix() { return $this->cgi_name_prefix_; }

/**
 * cgi名に付ける接尾辞
 * 
 * 「複数個」の同じような情報があり、1対nのケースの時なんかに使う
 *
 * @access public
 * @param string $v 接尾辞
 */
public function set_cgi_name_suffix($v) { $this->cgi_name_suffix_ = $v; }
public function get_cgi_name_suffix() { return $this->cgi_name_suffix_; }



/**
 * データハンドルの設定 本体
 *
 * @access public
 * @param db_handle $handle データハンドル系のクラス
 * @param int $layer 深度
 */
public function set_n_level_layer_data_operation_handle($handle, $layer)
{
  // ハンドルの設定
  // XXX …いらない？
  $this->dbh_[$layer] = $handle;

  // アダプタの生成
  // XXX なにかで使えそうなら、お外にはじき出してfactory化する?
  $name = '';
//var_dump($handle);
//var_dump($layer);

  if (true === $handle->is_rdb()) {
    require_once('data_clump_rdb_adaptor.inc');
    $name = 'data_clump_rdb_adaptor';
  } else
  if (true === $handle->is_memcache()) {
    require_once('data_clump_memcache_adaptor.inc');
    $name = 'data_clump_memcache_adaptor';
  } else
  if (true === $handle->is_apc()) {
    require_once('data_clump_apc_adaptor.inc');
    $name = 'data_clump_apc_adaptor';
  }

  // XXX エラーチェックしてないけどいいよね？
  $this->adaptor_[$layer] = new $name;
  $this->adaptor_[$layer]->set_data_handle($handle);

  //
  return true;
}

/**
 * データハンドルの設定
 *
 * DBのみならこっちで?
 *
 * @access public
 * @param db_handle $o db_handleクラス(からの派生クラス)のインスタンス

 */
public function set_db($o) { return $this->set_n_level_layer_data_operation_handle($o, 0); }

/**
 * 第１レイヤーデータハンドルの設定
 *
 * @access public
 * @param db_handle $o db_handleクラス(からの派生クラス)のインスタンス
 */
public function set_1st_level_layer_data_operation_handle($o)
{
  return $this->set_n_level_layer_data_operation_handle($o, 0);
}

/**
 * 第２レイヤーデータハンドルの設定
 *
 * @access public
 * @param db_handle $o db_handleクラス(からの派生クラス)のインスタンス
 */
public function set_2nd_level_layer_data_operation_handle($o)
{
  return $this->set_n_level_layer_data_operation_handle($o, 1);
}


/**
 * データハンドルの取得
 *
 * XXX 非推奨。限りなくprotectedなメソッドだと思って?
 *
 * @access public
 * @return db_handle db_handleクラス(からの派生クラス)のインスタンス
 */
public function get_db() { return $this->dbh_[0]; }



// publicか微妙なのでちとコメントは書かない：非推奨
public function set_tmp_key($s) { $this->tmp_key_ = $s; }
public function get_tmp_key() { return $this->tmp_key_; }
// keyをトークナイザで作成する：非推奨
public function make_tmp_key() { $this->set_tmp_key(tokenizer::get()); }


/**
 * Selectedアイテムの登録
 *
 * set_all_to_convで、特殊置換をする設定
 *
 * @access public
 */
public function push_selected_item($s) { $this->dic_selected_[] = $s; }
public function set_selected_item(array $a) { $this->dic_selected_ = $a; } // 非推奨
public function get_selected_item() { return $this->dic_selected_; }

/**
 * Checkedアイテムの登録
 *
 * set_all_to_convで、特殊置換をする設定
 *
 * @access public
 */
public function push_checked_item($s) { $this->dic_checed_[] = $s; }
public function set_checked_item(array $a) { $this->dic_checed_ = $a; } // 非推奨
public function get_checked_item() { return $this->dic_checed_; }


/**
 * 「レコード作成日の自動挿入」の作成
 *
 * カラム名を設定しておくと、insertのタイミングで自動で日付を設定してくれる
 *
 * @access public
 */
public function set_insert_date_name($s) { $this->insert_date_name_ = $s; }
protected function get_insert_date_name() { return $this->insert_date_name_; }

/**
 * 「レコード修正日の自動挿入」の作成
 *
 * カラム名を設定しておくと、updateのタイミングで自動で日付を設定してくれる
 *
 * @access public
 */
public function set_update_date_name($s) { $this->update_date_name_ = $s; }
protected function get_update_date_name() { return $this->update_date_name_; }




// XXX 以下、publicだけどprotectedっぽいのでコメントだけ書かないどく

// dbでの要素名のアクセサ
public function set_db_name($n, $v) { $this->db_name_[$n] = $v; }
public function get_db_name($n) { return $this->db_name_[$n]; }
// テンポラリテーブルでの要素名のアクセサ
public function set_tmp_name($n, $v) { $this->tmp_name_[$n] = $v; }
public function get_tmp_name($n) { return $this->tmp_name_[$n]; }
// cgi form名のアクセサ
public function set_cgi_name($n, $v) { $this->cgi_name_[$n] = $v; }
public function get_cgi_name_plain($n) { return $this->cgi_name_[$n]; }
public function get_cgi_name($n) { return $this->get_cgi_name_prefix() . $this->cgi_name_[$n] . $this->get_cgi_name_suffix(); }

// keyname関連
public function push_keyname($n) { $this->keyname_[] = $n; }
public function get_all_key_names()   { return $this->keyname_; }

public function push_nokeyname($n) { $this->nokeyname_[] = $n; }
public function get_all_no_key_names() { return $this->nokeyname_; }


// publicだけどあんまり触らせる気ないんだよねぇ
// 指定項目のupdate(mono update)用の色々
public function get_mono_set_db_target()
{
  return $this->mono_set_db_target_;
}

public function push_mono_set_db_target($name)
{
  $this->mono_set_db_target_[] = $name;
}

protected function set_data_type($inname, $data_type)
{
  $this->data_types_[$inname] = $data_type;
}

public function get_data_type($inname)
{
  return $this->data_types_[$inname];
}


public function clear_mono_set_db_target()
{
  $this->mono_set_db_target_ = array();
}
// calculation用のいろいろ
  // calculation_col_name
  public function get_calculation_col_name() { return $this->calculation_col_name_; }
  protected function set_calculation_col_name($v) { $this->calculation_col_name_ = $v; }
  // calculation_sign
  public function get_calculation_sign() { return $this->calculation_sign_; }
  protected function set_calculation_sign($v) { $this->calculation_sign_ = $v; }
  // calculation_num
  public function get_calculation_num() { return $this->calculation_num_; }
  protected function set_calculation_num($v) { $this->calculation_num_ = $v; }


/**
 * データセッタ
 *
 * データを変更。flgの指示によってはDB変更も連動させる
 * XXX 主キーに対する挙動が未設定
 *
 * @access public
 * @param string $name 名前
 * @param string $value 設定する値
 * @param boolean $flg 変更用フラグ。trueでDBの更新も行う。デフォルトはfalse
 * @return boolean 問題があればfalse
 */
public function set_value($name, $value, $flg = false)
{
//print "$name , $value ; \n";
  // 存在チェック
  if( !($this->is_name($name)) ) {
    return false;
  }

  // データの設定
  $this->set_data($name, $value);

  // flgが true の場合はDBも更新
  if ($flg) {
    //
    return $this->_mono_set_db($name);
  }

  // flg が false なら、DBを更新せずに終了
  return true;
}

/**
 * データゲッタ
 *
 * データを取得。存在しない場合空文字。
 * データとしての空文字なのか存在しないのかが重要な場合はis_name使って
 *
 * @access public
 * @param string $name 名前
 * @return string 値。データがなければ空文字
 */
public function get_value($name)
{
  // XXX 存在チェックはget_dataメソッドに依存
  return $this->get_data($name);
}


/**
 * 要素の設定
 *
 * data一式設定。
 * key_flgは、key|pk|k|ok|true|on のいずれかでkeyであるとみなす
 * XXX 一応publicだけど、生で設定するのちょいとやだよねぇ？
 *
 * @access public
 * @param string $inname 内部呼称用の名前
 * @param string $cginame CGI Formでの名前
 * @param string $tmpname テンポラリテーブル上での名前
 * @param string $dbname DB上での名前
 * @param string $keyflg PKの場合、ここをpkにする
 * @param int $data_type DB上の型の定義。でふぉはstring(事実上のノーチェック)
 * @return boolean 何か問題があればfalse
 */
public function push_element
($inname, $cginame = "", $tmpname = "", $dbname = "", $keyflg = "", $data_type = data_clump::DATATYPE_STRING)
{

  // 最低でもinnameはあること。なきゃNG
  if( !isset($inname)) {
    return false;
  }
  if( "" === $inname) {
    return false;
  }

  // 二重にcallしていないかチェック
  // XXX 上書きしないで無視する
  if (true === $this->is_name($inname)) {
    return true;
  }

  // まずnameを登録
  parent::push_element($inname);

  // CGI名称の設定
  if ("" === $cginame) {
    $cginame = $inname;
  }
  $this->set_cgi_name($inname, $cginame);

  // テンポラリテーブル名称の設定
  if ("" === $tmpname) {
    $tmpname = $inname;
  }
  $this->set_tmp_name($inname, $tmpname);

  // 正規DB名称の設定
  if ("" === $dbname) {
    $dbname = $inname;
  }
  $this->set_db_name($inname, $dbname);

  // keyの判定
  if(preg_match("/^(key|pk|k|ok|true|on)$/i", $keyflg)) {
    $this->push_keyname($inname);
  } else {
    $this->push_nokeyname($inname);
  }

  // データタイプの登録
  $this->set_data_type($inname, $data_type);

  //
  return true;
}

/**
 * validate
 *
 * pkを含む総てのデータに対してvalidationする
 * 実態は input用validate
 * XXX テンプレート指示子の名前は「cgi名になる」点に留意
 *
 * @access public
 * @param conv $conv テンプレートエンジンのインスタンス
 * @return boolean validationが総てOKならtrue、一つでも駄目ならfalse
 */
public function is_valid($conv = null)
{
  //
  return $this->is_valid_insert($conv);
}


/**
 * input用validate
 *
 * pkを含む総てのデータに対してvalidationする
 * MWの世界観じゃやらんのだけど。auto incrimentの時はこれ使えないから気をつけてね。update使って。
 * XXX テンプレート指示子の名前は「cgi名になる」点に留意
 *
 * @access public
 * @param conv $conv テンプレートエンジンのインスタンス
 * @return boolean validationが総てOKならtrue、一つでも駄目ならfalse
 */
public function is_valid_insert($conv = null)
{
  //
  $targets = $this->get_all_names();
  return $this->_is_valid($targets, $conv);
}


/**
 * update用validate
 *
 * pk以外の総てのデータに対してvalidationする
 * XXX テンプレート指示子の名前は「cgi名になる」点に留意
 *
 * @access public
 * @param conv $conv テンプレートエンジンのインスタンス
 * @return boolean validationが総てOKならtrue、一つでも駄目ならfalse
 */
public function is_valid_update($conv = null)
{
  //
  $targets = $this->get_all_no_key_names();
  return $this->_is_valid($targets, $conv);
}


/**
 * validate要素の決定
 *
 * 現状サポートしているのは以下の通り
const V_MUST     =    1; // 必須
const V_STRING   =    2; // 文字列
const V_NUMERIC  =    4; // 数値(パラメタは値を示す
const V_DIGIT    =    8; // 数字(パラメタは桁数を示す
const V_ALPHA    =   16; // 英字
const V_ALPNUM   =   32; // 英数字
const V_POST     =   64; // 郵便番号(nnn-nnnn、nnnnnnn、nnn nnnnの３パターンフォロー
const V_EMAIL1   =  128; // メアド
const V_DATE     =  256; // 日付
const V_M_STRING =  512; // 全角交じりの文字列(サイズのチェックを 全角:2文字 半角:1文字として扱う
const V_KATAKANA_UTF8 = 1024; // UTF-8文字コードのカタカナ(文字長の規定はしてないんで、基本、V_M_STRINGと一緒がいいと思う)

 *
 *
 * @access public
 * @param string $inname 内部呼称用の名前
 * @param int $type validationのtype。定数をor演算子でつなげて
 * @param string $param 範囲値が欲しい場合、min-maxの形で
 * @return boolean 何か問題があればfalse
 */
public function push_validate($inname, $type, $param = '')
{
  $this->v_type_[$inname] = $type;
  $this->v_param_[$inname] = $param;
  //
  return true;
}


/**
 * validate本体
 *
 * validate処理の本体を行うところ
 * より正確には「基本的なvalidationを行う」ところ。
 * XXX テンプレート指示子の名前は「cgi名になる」点に留意
 *

テンプレート条件
hoge部分には、適切な値をぶち込んでください


テンプレート書式
name="hoge" value="%%%hoge%%%" // CONV書式
name="hoge" value="{$hoge}" // Smarty書式

$$$if_error_hoge$$$
とにかくエラーざます
$$$/if_error_hoge$$$

{if true === $if_error_hoge}
とにかくエラーざます
{/if}

// 以下適宜略

$$$if_error_must_hoge$$$
hogeがないyo 必須だよをい

$$$if_error_min_hoge$$$
ちっちゃすぎるから！！

$$$if_error_max_hoge$$$
でけぇよをい
なんぼなんでも入れ過ぎだろをい

$$$if_error_numeric_hoge$$$
$$$if_error_digit_hoge$$$
数値っつってるだろヲイ

$$$if_error_alpha_hoge$$$
英字だよ英字

$$$if_error_alphanum_hoge$$$
英数だけしかだめだよ

$$$if_error_zip_hoge$$$
郵便番号っつってわかんねぇか？

$$$if_error_email_hoge$$$
メアドだよメアド

$$$if_error_katakana_hoge$$$
カタカナじゃないとヤダ

 * @access public
 * @param array $target 対象になるデータ
 * @param conv $conv テンプレートエンジンのインスタンス
 * @return boolean validationが総てOKならtrue、一つでも駄目ならfalse
 */
public function _is_valid(array $targets, $conv = null)
{
  //
  $flg = true;
  // 毎回is_nullが面倒なので捨てオブジェクト。無駄かなぁ？
  if (true === is_null($conv)) {
    $conv = new f_conv;
  }

  // XXX とりあえずぶち込むかなぁ
  // XXX 冗長なので削除したほうがいいかも
  $this->set_all_to_conv($conv);

  // ぶん回してチェック
  foreach($targets as $name) {
    // なければ次へ
    if (false === isset($this->v_type_[$name])) {
      continue;
    }
    //
    $type = $this->v_type_[$name];
    $param = explode('-', $this->v_param_[$name]);
    $param[] = ''; // 雑にエラー回避
    $value = $this->get_value($name);
    //
    $min = $param[0] . '';
    $max = $param[1] . '';

    // 空文字 かつ 必須じゃないなら、とっとと次にまわす?
    if ( !(self::V_MUST & $type) & (0 == strlen($value)) ) {
      continue;
    }

    // エラーメッセージはcgi_nameにしておく
    $cgi_name = $this->get_cgi_name($name);

    // 先に範囲チェック
    // XXX 空文字なら無判定。判定が必要ならMUSTとか入ってるっしょ？ きっと。
    if ('' !== $value) {
      // 範囲をとる値を取得
      // XXX V_NUMERICだけ別処理。通常はlengthだけど、numericの時だけは値そのもの
      if (self::V_NUMERIC & $type) {
        $len = $value;
      } else
      if (self::V_M_STRING & $type) {
        $code = mw_area_setting::get_inside_encoding_type();
        $len = mb_strwidth($value, $code);
      } else {
        $len = strlen($value);
      }


      // minチェック
      if ('' !== $min) {
        if ($min > $len) {
          $flg = false;
          $conv->multiDic('if_error', new simple_put());
          $conv->multiDic('if_error_' . $cgi_name, new simple_put());
          $conv->multiDic('if_error_min_' . $cgi_name, new simple_put());
        }
      }

      // maxチェック
      if ('' !== $max) {
        if ($max < $len) {
          $flg = false;
          $conv->multiDic('if_error', new simple_put());
          $conv->multiDic('if_error_' . $cgi_name, new simple_put());
          $conv->multiDic('if_error_max_' . $cgi_name, new simple_put());
        }
      }
    }

    // XXX とりあえず逐次。なんかきれいに書けねぇかなぁ…
    // 必須
    if (self::V_MUST & $type) {
      if (0 == strlen($value)) {
        $flg = false;
        $conv->multiDic('if_error', new simple_put());
        $conv->multiDic('if_error_' . $cgi_name, new simple_put());
        $conv->multiDic('if_error_must_' . $cgi_name, new simple_put());
      }
    }

    // 文字列：事実上「未チェック」なので書かない

    // 数値 and 数字
    if ((self::V_NUMERIC & $type) || (self::V_DIGIT & $type) ) {
      if (false == is::is_digit($value)) {
        $flg = false;
        $conv->multiDic('if_error', new simple_put());
        $conv->multiDic('if_error_' . $cgi_name, new simple_put());
        $conv->multiDic('if_error_numeric_' . $cgi_name, new simple_put());
        $conv->multiDic('if_error_digit_' . $cgi_name, new simple_put());
      }
    }

    // 英字
    if (self::V_ALPHA & $type) {
      // XXX
      if (false == ctype_alpha($value)) {
        $flg = false;
        $conv->multiDic('if_error', new simple_put());
        $conv->multiDic('if_error_' . $cgi_name, new simple_put());
        $conv->multiDic('if_error_alpha_' . $cgi_name, new simple_put());
      }
    }

    // 英数字
    if (self::V_ALPNUM & $type) {
      if (false == is::is_alphanum($value)) {
        $flg = false;
        $conv->multiDic('if_error', new simple_put());
        $conv->multiDic('if_error_' . $cgi_name, new simple_put());
        $conv->multiDic('if_error_alphanum_' . $name, new simple_put());
      }
    }

    // 郵便番号(nnn-nnnn、nnnnnnn、nnn nnnnの３パターンフォロー
    if (self::V_POST & $type) {
      if (false == is::is_zip($value)) {
        $flg = false;
        $conv->multiDic('if_error', new simple_put());
        $conv->multiDic('if_error_' . $cgi_name, new simple_put());
        $conv->multiDic('if_error_zip_' . $cgi_name, new simple_put());
      }
    }

    // email：type1
    if (self::V_EMAIL1 & $type) {
      if (false == is::is_email($value)) {
        $flg = false;
        $conv->multiDic('if_error', new simple_put());
        $conv->multiDic('if_error_' . $cgi_name, new simple_put());
        $conv->multiDic('if_error_email_' . $cgi_name, new simple_put());
      }
    }

    // 日付
    if (self::V_DATE & $type) {
      $cal_obj = new calendar();
      $cal_obj->set_string($value);
      if (false == $cal_obj->is()) {
        $flg = false;
        $conv->multiDic('if_error', new simple_put());
        $conv->multiDic('if_error_' . $cgi_name, new simple_put());
        $conv->multiDic('if_error_date_' . $cgi_name, new simple_put());
      }
    }

    // カタカナ(UTF-8)
    if (self::V_KATAKANA_UTF8 & $type) {
      if (false == is::is_katakana_utf8($value)) {
        $flg = false;
        $conv->multiDic('if_error', new simple_put());
        $conv->multiDic('if_error_' . $cgi_name, new simple_put());
        $conv->multiDic('if_error_katakana_' . $cgi_name, new simple_put());
      }
    }

  } // end of foreach

  //
  return $flg;
}


/**
 * CGIからの情報の吸い取り
 *
 * 詳細はset_from_cgi_insertと一緒?
 *
 * @access public
 * @param cgi_request $req cgi_requestクラスのインスタンス
 * @param boolean $empty_overwrite_flg 空文字の上書きフラグ trueにすると空なら空文字を上書きする
 * @return boolean 何か問題があればfalse
 */
public function set_from_cgi($req, $empty_overwrite_flg = false)
{
  // 全データを取得
  return $this->set_from_cgi_insert($req, $empty_overwrite_flg);
}

/**
 * CGIからの情報の吸い取り
 *
 * DBへの自動格納は行わない!!
 *
 * @access public
 * @param cgi_request $req cgi_requestクラスのインスタンス
 * @param boolean $empty_overwrite_flg 空文字の上書きフラグ trueにすると空なら空文字を上書きする
 * @return boolean 何か問題があればfalse
 */
public function set_from_cgi_insert($req, $empty_overwrite_flg = false)
{
  // 全データを取得
  return $this->set_from_cgi_detail($req, $this->get_all_names(), $empty_overwrite_flg);
}

/**
 * CGIからの情報の吸い取り:update用
 *
 * DBへの自動格納は行わない!!
 *
 * @access public
 * @param cgi_request $req cgi_requestクラスのインスタンス
 * @param boolean $empty_overwrite_flg 空文字の上書きフラグ trueにすると空なら空文字を上書きする
 * @return boolean 何か問題があればfalse
 */
public function set_from_cgi_update($req, $empty_overwrite_flg = false)
{
  // key以外の全データを取得
  return $this->set_from_cgi_detail($req, $this->get_all_no_key_names(), $empty_overwrite_flg);
}



/**
 * CGIからの情報の吸い取り
 *
 * DBへの自動格納は行わない!!
 *
 * @access public
 * @param cgi_request $req cgi_requestクラスのインスタンス
 * @param vector $target 対象とするcgi name attribute値
 * @param boolean $empty_overwrite_flg 空文字の上書きフラグ trueにすると空なら空文字を上書きする
 * @return boolean 何か問題があればfalse
 */
public function set_from_cgi_detail($req, array $target, $empty_overwrite_flg = false){

  // 全データを取得
  foreach($target as $name) {
    // 情報の(とりあえずの)格納
    $wk = $req->find($this->get_cgi_name($name));
    // 上書きフラグが false で
    if (false === $empty_overwrite_flg) {
      if ("" === $wk) {
        continue;
      }
    }
    //
    $this->set_value($name, $wk, false);
//print "$name <= $wk <br />\n";
  }

//print "set_from_cgi <BR>\n";
//print_r($this->data_);
//print "<BR>\n";

  return true;
}

// XXX 非推奨。DBに限ってるあたりがだめぽ?
protected function _make_session_instance()
{
  // オブジェクト生成＆基本データの挿入
  $se = new session_data_db;
  $se->init();
  $se->set_db($this->get_db());
  $se->set_id($this->get_tmp_key());
  if ("" !== $this->get_session_table_name()) {
    $se->set_tablename($this->get_session_table_name());
  }

  //
  return $se;
}

/**
 * テンポラリテーブルへのデータの格納
 *
 * @deprecated
 * @access public
 * @param session_data_obj $se セッションdataクラス。nullなら新規に作る：nullは非推奨で?
 * @return boolean 何か問題があればfalse
 */
public function set_to_tmp($se)
{
  if (true === is_null($se)) {
    // キーチェック
    if("" === $this->get_tmp_key()) {
      // キーが設定されてなければ作る
      // XXX エラーのほうがよいか？？？
      $this->make_tmp_key();
    }

    // オブジェクト生成＆基本データの挿入
    $se = $this->_make_session_instance();
  }

  // 現在情報を読み込む
  $se->read();

  // オブジェクトに全データを突っ込む
  foreach($this->get_all_names() as $name){
    // 格納
    $se->add_once($this->get_tmp_name($name), $this->get_value($name));
  }

  // 書き込み
  return $se->write();
}


/**
 * テンポラリテーブルから情報を引っ張る
 *
 * tmp_keyの設定がない場合はNGになるので注意
 *
 * @deprecated
 * @access public
 * @param session_data_obj $se セッションdataクラス。nullなら新規に作る：nullは非推奨で?
 * @return boolean 何か問題があればfalse
 */
public function set_from_tmp($se)
{
  
  if (true === is_null($se)) {
    // キーチェック
    if("" === $this->get_tmp_key()) {
      return false;
    }

    // オブジェクト生成＆基本データの挿入
    $se = $this->_make_session_instance();
  }

  // 全データの取得
  if(!($se->read())) {
    return false;
  }

  // オブジェクトに全データを突っ込む
  foreach($this->get_all_names() as $name) {
    // 格納
    $wk = $se->find($this->get_tmp_name($name));
    $this->set_value($name, $wk, false);
  }

  //
  return true;
}

/**
 * 正規DBへの登録へのラッパー
 *
 * 実態はset_to_db
 *
 * @access public
 * @return boolean 何か問題があればfalse
 */
public function set() { $this->set_to_db(); }

/**
 * 正規DBへの登録
 *
 * @access public
 * @return boolean 何か問題があればfalse
 */
public function set_to_db()
{
  return $this->_set_to_datahandle('set');
}


/**
 * データの更新系
 *
 * 塊全体をinsert/updateする
 * DBハンドル未設定の場合「あえて」PHPエラー出すようにしてる
 *
 * @access protected
 * @return boolean 何か問題があればfalse
 */
protected function _set_to_datahandle($method)
{
  //
  $ret = true;

  // １レイヤーしかないなら速やかに書き込む
  $count = count($this->adaptor_);
  if (1 === $count) {
    $ret = $this->adaptor_[0]->$method($this);
  } else {
    // もっとも深いレイヤーのみ書き込み、残りのレイヤーに対しては削除を行う
    $ret = $this->adaptor_[$count - 1]->$method($this);
    // XXX retがfalseの時は消さないほうがいいかなぁ？
    if (true === $ret) {
      for($i = 0; $i < $count - 2; $i ++) {
        $this->adaptor_[$i]->delete($this);
      }
    }
  }

  // calculationの時用の内部情報の操作
  // XXX 微妙に汚い気がするのだが、どうすっかねぇ…
  if ('calculation' === $method) {
    $i = $this->get_value($this->get_calculation_col_name());
    if ('-' === $this->get_calculation_sign()) {
      $i = $i - $this->get_calculation_num();
    } else {
      $i = $i - $this->get_calculation_num();
    }
    //
    $this->set_value($this->get_calculation_col_name(), $i);
  }

  //
  return $ret;
}


/**
 * データのinsert
 *
 * 塊全体をinsertする
 * DBハンドル未設定の場合「あえて」PHPエラー出すようにしてる
 *
 * @access public
 * @return boolean 何か問題があればfalse
 */
public function insert()
{
  // レコード作成日付の自動挿入
  if ('' !== $this->get_insert_date_name()) {
    $this->set_value_nowdate( $this->get_insert_date_name() );
  }

  return $this->_set_to_datahandle('create');
}


/**
 * データのupdate
 *
 * 塊全体をupdateする
 * DBハンドル未設定の場合「あえて」PHPエラー出すようにしてる
 *
 * @access public
 * @return boolean 何か問題があればfalse
 */
public function update()
{
  // レコード編集日付の自動挿入
  if ('' !== $this->get_update_date_name()) {
    $this->set_value_nowdate( $this->get_update_date_name() );
  }

  return $this->_set_to_datahandle('update_all');
}


/**
 * データのdelete
 *
 * 塊全体をdeleteする
 * DBハンドル未設定の場合「あえて」PHPエラー出すようにしてる
 * テーブル名_deleteというテーブルがある場合、全情報をそっちにinsert
 *
 * @access public
 * @return boolean 何か問題があればfalse
 */
public function del()
{
  // すべてのレイヤーに共通して書き込む
  foreach($this->adaptor_ as $ap) {
    $ap->delete($this);
  }
}

/**
 * データ１つのupdate
 *
 * あんまり散発的にやるとDBに負荷かかるから注意してね
 *
 * @access public
 * @param string $sql_p sqlのパーツ。具体的にはsetに渡すSQL文そのもの
 * @return boolean 何か問題があればfalse
 */
public function _mono_set_db($target_name)
{
  // 対象項目を設定して
  $this->push_mono_set_db_target($target_name);

  // 「レコード編集日付の自動挿入」があれば足し込んで
  if ('' !== $this->get_update_date_name()) {
    $this->push_mono_set_db_target($this->get_update_date_name());
    $this->set_value_nowdate( $this->get_update_date_name() );
  }

  // callして
  $ret = $this->_set_to_datahandle('update_mono');

  // 対象項目をはずして
  $this->clear_mono_set_db_target();

  // fin
  return $ret;
}


/**
 * 情報全体の取得メイン処理
 *
 * XXX ここはタイプに寄らないねぇ
 *
 * @access protected
 * @return boolean 何か問題があればfalse
 */
protected function _get($method)
{
//print "data_clump into _get({$this->get_table_name()}:{$method})\n";

  // まずは情報を取得
  $apawk = array();
  $flg = false;
  foreach($this->adaptor_ as $ap) {
    // 取得してみる
    $ret = $ap->$method($this);

    // 取得できたら抜ける
    if (true === $ret) {
      $flg = true;
      break;
    }
    //else
    $apawk[] = $ap;
  }
//var_dump($flg);

  // どこにもなければ突っ返す
  if (false === $flg) {
    return false;
  }
  // else
  // あるらしいっぽいのでそんな感じげな処理?

  // 浅瀬にデータをぶち込んでおく
  foreach($apawk as $ap) {
    $ap->set($this);
  }

  //
  return true;
}

/**
 * 情報全体の取得
 *
 * 当然ながらkey設定しないとNGになります。
 *
 * @access public
 * @return boolean 何か問題があればfalse
 */
public function get()
{
  return $this->_get('read_pk');
}

/**
 * pkによらない情報全体の取得
 *
 * pkではなく「現在設定されている値」からwhere句をつむぎます
 * …
 *
 * @access public
 * @return boolean 何か問題があればfalse
 */
public function get_nopk()
{
  return $this->_get('read_nopk');
}


/**
 * 情報をハッシュ配列にして取得
 *
 * XXX 「data」なので、デバッグ以外ではあんまり使わない想定で。get_all_view_values() をメインで使って下さい。
 *
 * @access public
 * @param 追加要素 $extra_param 追加的要素用の文字列
 * @return map ハッシュ配列
 */
public function get_all_data_to_hash($extra_param = array())
{
  $ret = array();
  // まずはすべての項目名をげと
  $all_names = $this->get_all_names();

  // で、設定
  foreach($all_names as $name) {
    $ret[$name] = $this->get_value($name);
  }

  // 追加があればそちらも
  if (false === empty($extra_param)) {
    foreach($extra_param as $name) {
      $ret[$name] = $this->get_value($name);
    }
  }

  //
  return $ret;
}

/**
 * 「表示用のデータ」を一式
 *
 * 表示につかう「情報の配列」を作る用のメソッド
 * XXX ベースはget_all_data_to_hash()と一緒だけど意味が違うので、現状同じコードをcopyで
 *
 * @access public
 * @param 追加要素 $extra_param 追加的要素用の文字列
 * @return map ハッシュ配列
 */
public function get_all_view_values($extra_param = array()) {
  $ret = array();
  // まずはすべての項目名をげと
  $all_names = $this->get_all_names();

  // で、設定
  foreach($all_names as $name) {
    $ret[$name] = $this->get_value($name);
  }

  // 追加があればそちらも
  if (false === empty($extra_param)) {
    foreach($extra_param as $name) {
      $ret[$name] = $this->get_value($name);
    }
  }

  //
  return $ret;
}


/**
 * monoDicでデータ一式を設定する
 *
 * XXX 平たくお便利系メソッド
 * XXX get_all_view_values() のデータを基準にしてみる：この方が継承時に楽
 * XXX テンプレート指示子の名前は「cgi名になる」点に留意
 *
 * @access public
 * @param convインスタンス $conv 置換エンジンインスタンス
 */
public function set_all_to_conv($conv)
{
  // クラス名取得
  $cn = get_class($conv);
//var_dump($cn);

  foreach($this->get_all_view_values() as $name => $value) {
    // $nameを「cgi側の名前」にする
    // XXX cgi_nameが設定されていない(extraな値)も含まれるので、自力でprefixとsuffixを付ける
    $name = $this->get_cgi_name_prefix() . $name . $this->get_cgi_name_suffix(); 

    // 単置換用
    if ('f_conv' === $cn) {
      $conv->monoDic($name, security::sanitize_html($value));
    } else
    if ('secure_conv' === $cn) {
      $conv->monoDic($name, $value);
    } else {
      // XXX 一旦、エスケープとか考えずにぶち込んでみる
      $conv->monoDic($name, $value);
    }
  }
  // 特殊置換用処理
  $awk = $this->get_selected_item();
  if (false === empty($awk)) {
    foreach($awk as $s) {
      conv_util::monoDicSelected($conv, $s, $this->get_value($s));
    }
  }
  $awk = $this->get_checked_item();
  if (false === empty($awk)) {
    foreach($awk as $s) {
      conv_util::monoDicChecked($conv, $s, $this->get_value($s));
    }
  }

  //
  return true;
}


/**
 * 指定されたカラムに現在日付を設定
 *
 * この処理において自動反映フラグがデフォtrueなのは変なので
 * デフォはfalseにしてある
 *
 * @access public
 * @param string $name 現在日付を設定したいカラム名
 * @param boolean $flg 変更用フラグ。trueでDBの更新も行う。デフォルトはfalse
 */
public function set_value_nowdate($name, $flg = false)
{
  return $this->set_value($name, calendar_util::get_now_string(), $flg);
}


/**
 * 指定されたカラムにトークン設定
 *
 * この処理において自動反映フラグがデフォtrueなのは変なので
 * デフォはfalseにしてある
 *
 * @access public
 * @param string $name トークンを設定したいカラム名
 * @param boolean $flg 変更用フラグ。trueでDBの更新も行う。デフォルトはfalse
 */
public function set_value_token($name, $flg = false)
{
  return $this->set_value($name, tokenizer::get(), $flg);
}


/**
 * 指定されたカラムにトークン設定：Webサーバ複数台用
 *
 * トークンにIPを追加したいとき用(Webサーバが複数台ある時など)
 *
 * @access public
 * @param string $name トークンを設定したいカラム名
 * @param boolean $flg 変更用フラグ。trueでDBの更新も行う。デフォルトはfalse
 */
public function set_value_token_with_ip($name, $flg = false)
{
  return $this->set_value($name, tokenizer::_get('-', true, true, true), $flg);
}


/**
 * set_all_to_convされたconvインスタンスを取得する
 *
 * secure_convインスタンスを返す。
 * 後で、smarty系のインスタンスも返せるように作り直すか？
 * 非推奨
 *
 * @deprecated
 * @access public
 * @param string $name トークンを設定したいカラム名
 * @return convインスタンス 置換エンジンインスタンス
 */
public function get_conv($class_name = 'secure_conv')
{
  //
  $o = new $class_name;

  //
  $this->set_all_to_conv($o);

  //
  return $o;
}

/**
 * radioボタンを設定する
 *
 * @access public
 * @param array $names radioボタンとして設定したい値
 * @param convインスタンス $conv 置換エンジンインスタンス
 */
public function set_radio($names, $conv)
{
  foreach($names as $name) {
    conv_util::monoDicChecked($conv, $name, $this->get_value($name));
  }
}

/*
 * ハッシュ配列からインポート
 *
 * @access public
 * @param array $hash_data インポート情報
 * @return boolean 何かがあったらfalse
 */
public function set_all_data_from_hash($datas)
{
  foreach($datas as $key => $val) {
    $this->set_value($key, $val, false);
  }
  //
  return true;
}

/*
 * db_dataオブジェクトからインポート
 *
 * XXX 差分とかその他細かいエラーチェックが雑なので注意
 *
 * @access public
 * @param array $res db_dataインスタンス
 * @return boolean 変な差分があったらfalse。DBカラムが多いとか、clumpの設定が多いとか、その辺
 */
public function set_from_dbdata($res)
{
  // すべての項目名をげと
  $all_names = $this->get_all_names();
  // エラーチェック用込みで
  $row = $res->get_row();

  // ぶん回して設定
  foreach($all_names as $key) {
    // DB name(レコード名)を取得
    $db_name = $this->get_db_name($key);

    // データの設定
    $this->set_value($key, $row[$db_name], false);

    // エラーチェック用
    unset($all_names[$key]);
    unset($row[$db_name]);
  }
// XXX 空のはず。空じゃなければ、なんか漏れてる
//var_dump($all_names);
//var_dump($row);

  //
  if ( (false === empty($all_names))||(false === empty($row)) ) {
    return false;
  }
  // else
  return true;
}

/**
 * 数値の加減算
 *
 * XXX 設定したらすぐにDB等に書きに行くので留意
 *
 * @access public
 * @param string $col カラム名
 * @param string $sign +または-
 * @param int $num 加減算させる値
 * @return boolean 成功したらtrue、なにか失敗があったらfalse
 */
public function update_calculation($col, $sign, $num)
{
  // 対象項目を設定して
  $this->set_calculation_col_name($col); // XXX ノーチェック。いいよねぇ？
  $this->set_calculation_sign($sign); // XXX ノーチェックだけどいいかねぇ？ 外から入ってくる値でもないし
  $this->set_calculation_num($num + 0); // XXX すごい雑にエスケープっぽいことを

  // callして
  $ret = $this->_set_to_datahandle('calculation');

  // fin
  return $ret;
}

protected function _get_property_list() {
  $p = parent::_get_property_list();
  $p = array_merge($p, array_keys(get_object_vars($this)));
  return $p;
}

/**
 * auto incrementの時に、pkにIDを入れる
 *
 * @access public
 */
public function set_insert_id() {
  // pkカラム名を取得
  // XXX 複数カラムは未考慮
  $keys = $this->get_all_key_names();

  // IDを把握
  // XXX memcachedとかは未考慮
  $id = $this->get_db()->get_inserted_id();

  // 設定
  $this->set_value($keys[0], $id);
}


//private:
private $dbh_;          // DBハンドル
private $adaptor_;	// アダプタクラス

private $tmpname_;      // テンポラリテーブル名
private $tmp_key_;      // テンポラリテーブル用キー

private $tblname_;      // 正規DBテーブル名(Keyはkeyname_で確認)

private $cgi_name_;     // CGI上の名前:hashは論理名で
private $tmp_name_;     // テンポラリテーブル上の名前:hashは論理名で
private $db_name_;      // 正規DB上の名前:hashは論理名で

private $nokeyname_;    // Keyではない論理名一式
private $keyname_;      // Keyになってる論理名一式

private $data_types_;	// DB上の型。int/string/float/dateの識別程度

private $insert_date_name_;	// レコードを作成したときに自動で日付を入れたい時のカラム名
private $update_date_name_;	// レコードを編集したときに自動で日付を入れたい時のカラム名

private $cgi_name_prefix_;	// 複数個段組の時などの、form側の名前のprefix
private $cgi_name_suffix_;	// 複数個段組の時などの、form側の名前のsuffix


//
private $v_type_;       // validate用。タイプのint値を格納
private $v_param_;      // validate用。範囲値を格納

// データストア(DB含む)との接続用識別子
private $identifier_;

// set_all_to_conv用
private $dic_selected_;
private $dic_checed_;

// mono update用：更新対象のname配列
private $mono_set_db_target_;

// calculationメソッド用
private $calculation_col_name_;
private $calculation_sign_;
private $calculation_num_;

} // end of class

