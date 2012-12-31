<?php

require_once('file_util.inc');
require_once('pair.inc');


////////////////////////////
// 構造体系クラス
////////////////////////////

// ストックデータ用クラス
class lexical_analysis_stock_data
{
//public:
public function get_data() { return $this->data_; }
public function push_data($n, $v)
{
//print "push data ($n, $v) \n";
  $wk = new pair;
  $wk->set($n, $v);
  $this->data_[] = $wk;
}

//
public function push_isdata_from_buf($n)
{
  if ("" != $this->get_buf()) {
    $this->push_data($n, $this->get_buf());
    $this->set_buf("");
  }
}
//
public function push_data_from_buf($n)
{
  $this->push_data($n, $this->get_buf());
  $this->set_buf("");
}

//
public function add_buf($c) { $this->buf_ .= $c; }
public function set_buf($s) { $this->buf_ = $s; }
public function get_buf() { return $this->buf_; }

//private:
private $data_ = array();	// vector<pair *> トークンデータ一式
private $buf_ = "";		// バッファデータ
} // end of class


// 解析元データ保持用クラス
class lexical_analysis_data
{
//public:
public function __construct()
{
  $this->init();
}

//
public function init()
{
  $this->set_data("");
}

//
public function set_length($i) { $this->length_ = $i; }
public function get_length() { return $this->length_; }

//
public function get_data() { return $this->data_; }
public function set_data($s)
{
  $this->data_ = $s;
  $this->set_length(strlen($s));
  $this->set_count(-1);	// fetchみたく「インクリして使う」ので-1スタート
}

// サイズ判定
// 現在のiteratorカウンタとlengthとのサイズチェック
public function is_length()
{
  if ($this->get_length() <= $this->get_count()) {
    // でかすぎるよ?
    return false;
  }
  //
  return true;
}

//
public function set_count($i) { $this->count_ = $i; }
public function get_count() { return $this->count_; }

// インクリメント
// XXX 自動でis_lengthやっとくこと
public function inc_count() {
  $this->set_count($this->get_count() + 1);
  return $this->is_length();
}

// 要素データの取得
public function get_element() { return $this->data_[$this->get_count()]; }
// XXX ２文字の特殊用
public function get_element_next(){ return $this->data_[$this->get_count()+1]; }


//private:
private $data_;		// データ
private $count_;	// 現在のiteratorカウンタ
} // end of class




////////////////////////////
// 処理クラス
////////////////////////////

// １モード中の１文字あたりの処理担当
class lexical_analysis_engine_unit
{
//public:
//
public function set_next_mode($s) { $this->next_mode_ = $s; }
public function set_processing_list($a) { $this->processing_list_ = $a; }

//
public function get_my_char() { return $this->my_char_; }

//
public function set_my_char($s)
{
  // XXX 雑に判定
  if ("'" == $s[0]) {
    // 一文字だけ切り抜く
    $this->my_char_ = $s[1];
  } else {
    // 全部を対象にする
    $this->my_char_ = $s;
  }
//print "my char is ... " . $this->my_char_ . "\n";
}

//
public function set_processing_string($s)
{
  // &で切り切り
  $this->set_processing_list( explode("&", $s) );
//print_r ($this->processing_list_);
}

//
public function run($data_obj, $stock_obj)
{
  ///////////////////////////
  // 処理をする
  // XXX 複数の処理が存在しうる
  ///////////////////////////
  foreach($this->processing_list_ as $processing) {
//print "処理 : $processing\n";
    // error処理
    if ("error" == $processing) {
      //throw new Exception("lexical analysis error!!");
      throw new Exception( "error!! (" . $this->next_mode_ . ")" );
    }

    // no：無処理
    if ("no" == $processing) {
      continue;
    }

    // 足しこみ系
    if ("+" == $processing[0]) {
      // +char：my_charをbufへ足しこむ
      if ("+char" == $processing) {
        $stock_obj->add_buf($data_obj->get_element());
      } else {
        // +'x'：指定された文字をbufへ足しこむ
        // XXX 一文字限定。かなり荒い処理
        $stock_obj->add_buf($processing[2]);
      }
    }

    // push:'name',data：'name'にデータ(dataならbufの内容)をpush
    //   もしdata部分が無指定なら「空文字」扱いでpush
    if ( preg_match("/^push/", $processing)) {
      // まずは分解
      $push_data = explode(",", $processing);
      $push_data[] = "";

      // 名前の抽出
      // XXX これもかなり荒い
      $abuf = explode("'", $push_data[0]);
      $push_name = $abuf[1];
      
      // ストックオブジェへのぶち込み
      if ("data" == $push_data[1]) {
        // バッファぶち込みつつクリアもしておく
        $stock_obj->push_data_from_buf($push_name);
      } else if ("isdata" == $push_data[1]) {
        // バッファぶち込みつつクリアもしておく
        $stock_obj->push_isdata_from_buf($push_name);
      } else {
        // XXX dataじゃなければ無条件で空文字にしておく
        $stock_obj->push_data($push_name, "");
      }
    }

  }

  // 次のモードを復帰して終わり
  return $this->next_mode_;
}

//private:
private $my_char_;	// 自分自身の文字：１文字：+char用
private $next_mode_;	// 次のモード用文字列
private $processing_list_;	// 処理文字列:配列
	// &で区切って複数の処理が存在しうる
	// no：無処理
	// +char：my_charをbufへ足しこむ
	// +'x'：指定された文字をbufへ足しこむ
	// push:'name',data：'name'にデータ(dataならbufの内容)をpush
	//   もしdata部分が無指定なら「空文字」扱いでpush
}



// １モードあたりの処理担当
class lexical_analysis_engine
{

// 文字列から必要な要素切り出して取得
public function set_mode_name_string($s)
{
  // とりあえずタブで切る
  $abuf = explode("\t", $s);

  // XXX 一番目の要素を問答無用で
  $this->mode_name_ = $abuf[0];
//print "mode name = " . $this->mode_name_ . "\n";
}
public function get_mode_name() { return $this->mode_name_; }

// 各文字処理用インスタンスの生成
public function set_processing_data($s)
{
//print "processing data is $s \n";
  // まず分解
  $abuf = explode("\t", $s);
  // XXX errorのためにアバウトな処理を追加
  $abuf[] = "";

  // １番目の要素の先頭に# があればそれはコメントなので終了
  if ("#" == $abuf[1][0]) {
//print "this string is comment\n";
    return ;
  }

  // インスタンス作成
  $obj = new lexical_analysis_engine_unit;

  // 設定
  $obj->set_my_char($abuf[1]);
//print "my char is " . $obj->get_my_char() . "\n";
  $obj->set_processing_string($abuf[2]);
//print "processing string is " . $abuf[2] . "\n";
  $obj->set_next_mode($abuf[3]);
//print "next mode is " . $abuf[3] . "\n";

  // 処理用のインスタンスとして登録
  $this->processing_obj_[ $obj->get_my_char() ] = $obj;
  //
  if ("hs" == $obj->get_my_char()) {
    $this->hs_flg_ = true;
//print "hs flg is true\n";
  }
  //
  if ("crlf" == $obj->get_my_char()) {
    $this->crlf_flg_ = true;
//print "crlf flg is true\n";
  }
  //
  return ;
}


public function analysis($data_obj, $stock_obj)
{
  // 対象となる一文字を取り出す
  $itr = $data_obj->get_element();

  // 基本文字のチェック
  $do_obj = NULL;
  if (isset($this->processing_obj_[$itr])) {
    // 存在する処理なので処理オブジェを確定
    $do_obj = $this->processing_obj_[$itr];
  } else {
    // 特殊処理：hs ホワイトスペース
    // XXX 改行は「含まない」点に注意
    if ( $this->hs_flg_ ) {
      // スペース // タブ
      if ((' ' == $itr) || ("\t" == $itr)) {
        $do_obj = $this->processing_obj_['hs'];
      }
    }
    // 特殊処理：crlf 改行
    if ( $this->crlf_flg_ ) {
      // LFのみのケース
      if ('\n' == $itr) {
        $do_obj = $this->processing_obj_['crlf'];
      } else if ('\r' == $itr) {
        // CRLFを考慮
        if ('\n' == $data_obj->get_element_next()) {
          $data_obj->inc_count();
        }
        $do_obj = $this->processing_obj_['crlf'];
      }
    }
  }

  // 全て該当しなければanyのオブジェクトを割り当てる
  if (is_null($do_obj)) {
    $do_obj = $this->processing_obj_['any'];
  }

  // で、処理
  // XXX 復帰値でmode文字列を返す
  return $do_obj->run($data_obj, $stock_obj);
}

//
private $mode_name_;		// mode名：ようは自分自身の名前
private $processing_obj_;	// 処理用オブジェクト
				// map<name, lexical_analysis_engine_unit *>
private $hs_flg_ = false;	// もしhsセクションがあればtrue
private $crlf_flg_ = false;	// もしcrlfセクションがあればtrue
} // end of class


/**
 * 字句解析プログラム
 *
 * オートマトンとかそのあたりに類するやつ
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/
class lexical_analysis
{
//public:

/**
 * 設定ファイルの設定
 *
 * @access public
 * @param string $s configファイル名
 */
public function set_config_file($s) { $this->config_fn_ = $s; }
/**
 * 設定ファイルの取得
 *
 * @access private
 * @return string configファイル名
 */
private function get_config_file() { return $this->config_fn_; }


/**
 * 解析する文字列の設定
 *
 * @access public
 * @param string $s 解析する文字列
 */
public function set_data($s) { $this->data_ = $s; }
/**
 * 解析する文字列の取得
 *
 * @access private
 * @return string 解析する文字列
 */
private function get_data() { return $this->data_; }

/**
 * 解析する文字列の設定(ファイル名指定)
 *
 * @access public
 * @param string $filename 解析する文字列の入ったファイル名
 */
public function set_data_file($filename)
{
  // ファイルデータを取得しぃつつ突っ込む
  $this->set_data(file_get_contents($filename));
}


/**
 * 文字列の解析
 *
 * @access public
 * @param string $data 解析対象の文字列(指定しない場合、get_dataで取得する)
 * @return array 解析結果の配列。形式はvector<pair *>
 */
function parse($data = "")
{
  /////////////////////////
  // configの分解
  /////////////////////////
  // データの解析
  $fh = fopen($this->get_config_file(), "r");
  $mode = "";
  if ($fh) {
    while (!feof($fh)) {
      $wk = fgets($fh);
      // いらん改行を削除
      $wk = rtrim($wk);
//print "config line is \n\t$wk \n";
//print "$wk \n";

      // 空改行なら次へ
      if (preg_match("/^$/", $wk)) {
//print "empty ... next!!\n";
        continue;
      }

      // 先頭に # がある場合はコメントなので次
      if ("#" == $wk[0]) {
//print "comment ... next!!\n";
        continue;
      }

      // さて。処理でもすっか
//print "$wk \n";
      // 新しいモードか否か
      if ("\t" != $wk[0]) {
        // 新しいモード作成
        $obj = new lexical_analysis_engine;
        $obj->set_mode_name_string($wk);

        // コメントとかはずした正しいmode名を取得
        $mode = $obj->get_mode_name();

        // 二重登録チェック
        if (isset($this->analysis_obj_list_[$mode])) {
//print "二重登録??? ( $mode ) \n";
        } else {
          // モード登録
          $this->analysis_obj_list_[$mode] = $obj;
        }
      } else {
        // 現在のモードに対してプロセス登録
        $this->analysis_obj_list_[$mode]->set_processing_data($wk);
      }
    }
    fclose($fh);
  }

  /////////////////////////
  // 解析
  /////////////////////////
  // データの取得
  if ("" == $data) {
    $data = $this->get_data();
  }

  // データ保持用のクラスにデータを格納
  // XXX 稀に２文字解析とかありえるので
  $data_obj = new lexical_analysis_data;
  $data_obj->set_data($data);

  // ぶん回しつつ解析
  $stock_obj = new lexical_analysis_stock_data;
  $mode = "base";

  try {
    while( $data_obj->inc_count() ) {
//print $data_obj->get_element() . "\n";

      // 分析
      // XXX 参照渡しであることが前提?
      // XXX ついでに次のモード名を取得：ここでいいのか？
//print "mode is $mode \n";
      $mode = $this->analysis_obj_list_[$mode]->analysis($data_obj, $stock_obj);
    }
  } catch (Exception $e) {
    // エラーなら問答無用で終了?
// XXX
print "get exception!!!\n";
print $e->getMessage() . "\n";
print $e->getCode() . "\n";
print $e->getFile() . "\n";
print $e->getLine() . "\n";
    //exit;
    return NULL;
  }
  
  // vector<pair *>
  return $stock_obj->get_data();
}


//private:
private $config_fn_;	// configファイル名
private $data_;		// 解析対象のデータ
private $length_;	// $data_の長さ：strlenで取得

//
private $analysis_obj_list_;	// 分析インスタンスの配列：ハッシュで
				// map<name, lexical_analysis_engine *>
} // end of class


