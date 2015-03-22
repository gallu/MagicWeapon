<?php
/**
 * コントローラクラス
 *
 * MVCにおけるCを担当するクラス
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

require_once('cgi_request.inc');
require_once('cgi_response.inc');
require_once('load_manager.inc');
require_once('multibyte.inc');
require_once('security.inc');
require_once('multimap.inc');
require_once('guess_from_data_encoding.inc');
require_once('mw_config.inc');
require_once('mobile_info.inc');
require_once('user_agent_check.inc');
require_once('config_extension.inc');
require_once('mw_area_setting.inc');
require_once('di.inc');

class controller {

//public:
// configインスタンスのアクセサ
protected function set_config_obj($o) { $this->cfobj_ = $o; }
public function get_config_obj() { return $this->cfobj_; }

//
protected function set_cgi_request_obj($o) { $this->reqobj_ = $o; }
public function get_cgi_request_obj() { return $this->reqobj_; }
//
protected function set_cgi_response_obj($o) { $this->resobj_ = $o; }
public function get_cgi_response_obj() { return $this->resobj_; }
//
protected function set_bag_obj($o) { $this->bag_ = $o; }
public function get_bag_obj() { return $this->bag_; }

//
protected function set_mobile_obj($o) { $this->mobobj_ = $o; }
public function get_mobile_obj() { return $this->mobobj_; }
//
protected function set_inside_encoding_obj($o)
  { $this->inside_encoding_obj_ = $o; }
public function get_inside_encoding_obj()
  { return $this->inside_encoding_obj_; }

//
protected function set_output_encoding_obj($o)
  { $this->output_encoding_obj_ = $o; }
public function get_output_encoding_obj()
  { return $this->output_encoding_obj_; }

//
public function set_base_path($s) { $this->bp_ = $s; }
public function get_base_path() { return $this->bp_; }

public function __construct()
{
  // バッファリング開始
  //ob_clean(); // XXX エラーを引き起こす可能性があるので一端コメントアウト
  ob_start();

  //
  $this->init();
}

/**
 * 初期化処理
 *
 * @access public
 */
public function init()
{
  $this->set_config_obj(new config_extension);
  $this->set_mobile_obj(new mobile_info);
  $this->set_base_path("");
}

/**
 * インスタンス生成
 *
 * request & response をはじめとしてさまざまなインスタンスを生成
 * bag、multibyte系など
 *
 * @access public
 * @return boolean 何かあったらfalse
 */
public function make_instancees()
{
  // 鞄インスタンス生成
  // XXX 準備いらないし直で
  $this->set_bag_obj( new multimap );

  // 内部エンコード用クラス作成
  // inは任意、outはconfig指定
  // デフォルトはEUC
  $ieobj = new multibyte;
  // outの設定
  $ieobj->set_euc();
  if ($wk = $this->get_config_obj()->find('inside_encoding_type')) {
    $ieobj->set_guess_to_string($wk);
    // 内部文字コードの広域設定
    mw_area_setting::set_inside_encoding_type($wk);
  }
  // inの推測
  $gobj = new guess_from_data_encoding;
  if ("" !== $this->get_config_obj()->find('encoding_hinting_name')) {
    $gobj->set_encoding_string($this->get_config_obj()->find('encoding_hinting_name'));
  }
  // 推測
  $gobj->guess();
  // 推測値設定
  $ieobj->set_guess_from_string($gobj->get_encoding_string());
//var_dump($ieobj);
//print "inside " . $ieobj->get_charset_string() . "\n";
  //
  $this->set_inside_encoding_obj($ieobj);

  // 出力エンコード用クラス作成
  // inは「内部エンコードのout 、outはconfig指定
  // デフォルトはEUC
  $opobj = new multibyte;
  // outの設定
  $opobj->set_sjis();
  if ($wk = $this->get_config_obj()->find('output_encoding_type')) {
    $opobj->set_guess_to_string($wk);
  }
  // inの設定
  if ($wk = $this->get_config_obj()->find('inside_encoding_type')) {
    $opobj->set_guess_from_string($wk);
  }
//print "output " . $opobj->get_charset_string() . "\n";
  $this->set_output_encoding_obj($opobj);

  // CGI REQUESTインスタンスの生成
  $req = $this->get_instance('cgi_request', 'cgi_request.inc:cgi_request');

  // 一応「Post Get」順とする
  // XXX 後でコンフィグではじく？
  $req->set_hi_priority_post();
  $req->set_inside_encoding_obj($this->get_inside_encoding_obj());
  //
  $this->set_cgi_request_obj($req);

  // CGI RESPONSEインスタンスの生成
  $res = new cgi_response;
  //
  $this->set_cgi_response_obj($res);

  // テンプレートエンジンインスタンス
  // XXX 場合によっては邪魔な可能性もあるけど、まぁ、大抵使うでしょ
  $teobj = $this->get_instance('template_engine', 'secure_conv.inc:secure_conv');

  // テンプレートエンジンごとの設定
  // XXX 外に切り出すかねぇ…Smarty以外の追加もあるだろうしねぇ…
  // Smarty系列の設定
  if (true === is_subclass_of($teobj, 'Smarty')) {
    $teobj->compile_dir  = $this->get_config_obj()->find('smarty_compile_dir');
    $teobj->template_dir = $this->get_config_obj()->find('template_dir');
    $teobj->config_dir   = $this->get_config_obj()->find('smarty_config_dir');
  }
//var_dump($teobj);
  $this->get_bag_obj()->insert('conv', $teobj);

  //
  return true;
}

/**
 * configファイル設定
 *
 * configファイル名を設定：その後必要な処理を走らせる
 *
 * @access public
 * @param string $fn configファイル名
 * @return boolean 何かあったらfalse
 */
public function set_config($fn)
{
  // まずはlocalなコンフィグを取り込む
  $bp = $this->get_base_path();
  if ("" != $bp) {
    $this->get_config_obj()->set_base_path($bp);
  }
  $this->get_config_obj()->set_file($fn);
//var_dump($this->get_config_obj());

  ////////////////////////////////
  // コンフィグからの設定各種
  ////////////////////////////////

  // common_include_dirの解決っていうかset_ini
  foreach ($this->get_config_obj()->find_array('common_include_dir') as $path) {
    set_include_path(get_include_path() . PATH_SEPARATOR . $path);
//print $path . "\n";
//print get_include_path() . "<BR>\n";
  }

  // DIコンテナ設定を取り込んでおく
  $s = $this->get_config_obj()->find('di_config');
  if ('' !== $s) {
    DI::set_config($s);
  }

  // data_clump関連の設定
  require_once('data_clump_manager.inc');
  //
  data_clump_manager::set_config($this->get_config_obj()->find('data_clump_config'));

  //
  return true;
}

/**
 * 入り口チェック
 *
 * あらゆる処理の前に走るメソッド
 * ・user-agent-arrow または user-agent-denny が設定されていれば機種チェック
 *
 *
 *
 * @access public
 * @return boolean 問題があればfalse
 */
public function entrance_check()
{
$ret = true;

  // 機種判定
  // XXX 両方設定されているケースはとりあえず未考慮
  $afn = $this->get_config_obj()->find('user-agent-arrow');
  $dfn = $this->get_config_obj()->find('user-agent-denny');
//var_dump($afn);
//var_dump($dfn);
  if ( ("" !== $afn) || ("" !== $dfn) ) {
    $obj = new user_agent_check;
    $obj->set_mobile_obj($this->get_mobile_obj());
    $fn = "";
    if ("" !== $afn) {
      $fn = $afn;
      $obj->set_arrow_string(file_get_contents($fn));
    } else {
      $fn = $dfn;
      $obj->set_denny_string(file_get_contents($fn));
    }
//var_dump($fn);

    // 判定
    $ret = $obj->is_permission();

    // XXX アクセス不可な時の処理
    if (false === $ret) {
      $uri = $this->get_config_obj()->find('user-agent-disapprove_uri');
      if ('' !== $uri) {
        // XXX ちぃとベタ
        header('Location:' . $uri);
        exit;
      }
      // else
      // XXX …これもかなりベタ orz
      echo 'your user-agent is denny.';
      exit;
    }
  }
//var_dump($ret);

//var_dump( $this->get_config_obj()->find('only_mobile') );
  if ('on' === $this->get_config_obj()->find('only_mobile')) {
    $mm = $this->get_mobile_obj();
//var_dump($mm);
    // 
    if (false === $mm->is_mobile()) {
      // XXX ちぃとベタ
      header('Location:' . $this->get_config_obj()->find('no_mobile_uri'));
      exit;
    }
    // uidチェック
    $id = $mm->get_sid();
//var_dump($id);
    if ('' === $id) {
      // XXX ちぃとベタ
      header('Location:' . $this->get_config_obj()->find('no_uid_uri'));
      exit;
    }
  }

  //
  return $ret;
}

/**
 * 出口チェック
 *
 * あらゆる処理の後に走るメソッド
 * 継承先で必要なら実装してください
 *
 * @access public
 * @return boolean 問題があればfalse
 */
public function exit_check() { return true; }

/**
 * 入力データパース&デコード
 *
 * PHP的にはやることないなぁ
 *
 * @access public
 * @return boolean 問題があればfalse
 */
public function parse_decode() 
{
  //
  return true;
}

/**
 * Model割り当て部分メイン
 *
 * @access public
 * @param string $cstring コマンド名
 * @param int $recursive_counter 再帰用カウンタ
 * @return base_model_inc base_model系インスタンス
 */
public function run_main($cstring, $recursive_counter)
{
//print "run_main c = $cstring\n";
//var_dump($this->mapdata_);
//var_dump($recursive_counter);
//exit;

  // ダイレクトに「一文字めアンダースコア」なcstring呼び出したときNG
  if ( (0 === $recursive_counter)&&('_' === $cstring[0]) ) {
    throw new Exception("\"" . $cstring . "\" is invalid command(2)!!");
  }

  // 探してみる。無ければエラーで突っ返す
  if (is_null($model_data = $this->mapdata_->find($cstring))) {
    throw new Exception("\"" . $cstring . "\" is invalid command!!");
  }
//var_dump($model_data);
//print "include is " . $model_data->get_include_filename() . "<BR>\n";
//print "class is " . $model_data->get_classname() . "<BR>\n";

  // include処理
  // XXX load_fileのルーチンだとmodelのエラーが出てこない…
  require_once($model_data->get_include_filename());

  // オブジェクト作成
  $classname =  $model_data->get_classname();
  $model_ins = new $classname;

  // 必要な情報の設定
  // XXX 基本的にreferer渡しなので情報は共有できるはず
  $model_ins->set_config($this->get_config_obj());
  $model_ins->set_cgi_request($this->get_cgi_request_obj());
  $model_ins->set_cgi_response($this->get_cgi_response_obj());
  $model_ins->set_inside_encoding_obj($this->get_inside_encoding_obj());
  $model_ins->set_my_name($cstring);
  $model_ins->set_bag_obj($this->get_bag_obj());
  $model_ins->set_mobile($this->get_mobile_obj());
  //
  $model_ins->set_bag('recursive_counter', $recursive_counter);

//print "\nbag \n";
//var_dump($this->get_bag_obj());

  // mapファイルに設定があってかつそれが上書きされてなきゃ設定する〜
//print "Trap 1\n";
//var_dump($model_data->get_template_filename());
//var_dump($model_ins->is_tmp_filename());
  if (("" !== $model_data->get_template_filename())&&(false === $model_ins->is_tmp_filename())) {
//print "Trap 2\n";
    $model_ins->set_tmp_filename($model_data->get_template_filename());
  }
  if (("" !== $model_data->get_error_template_filename())&&(false === $model_ins->is_error_tmp_filename())) {
//print "Trap 3\n";
    $model_ins->set_error_tmp_filename($model_data->get_error_template_filename());
  }

  // インスタンスにキック
  $r = $model_ins->initialize();
  // 明示的にinitializeでfalseが帰ってきたときだけ、一切の処理をしない
  if (false !== $r) {
    if ($model_ins->is_recursive_on()) {
      // 再帰的読み込みっぽいので、引き続きcall
      $m_ins = $this->run_main($model_ins->get_recursive_command(), $recursive_counter + 1);
    } else {
      $model_ins->execute(); // ここでBodyが作られてる…はず!!
      if ($model_ins->is_recursive_on()) {
        // 再帰的読み込みっぽいので、引き続きcall
        $m_ins = $this->run_main($model_ins->get_recursive_command(), $recursive_counter + 1);
        // 戻ってきた値を横取りして抱え込む
        // XXX base64とか増えたら処理ふやさんとなぁ
        $model_ins->set_body($m_ins->get_body());
      }
    }
    // 終了処理
    $model_ins->finalize();
  }
//var_dump($model_ins);

  // で、復帰
  // XXX インスタンスそのものを復帰させる。後でbase64とかでてくるから
//print "\nbag \n";
//var_dump($this->get_bag_obj());
  return $model_ins;
}


/**
 * メイン処理
 *
 * 必要な処理を必要な順番でcallする
 *
 * @access public
 */
public function run()
{
  // 例外処理捕捉開始
  try {
    // インスタンス生成
    if (!($this->make_instancees())) {
      throw new Exception("make re instance error!!");
    }

    // 入り口チェック
    if (!($this->entrance_check())) {
      throw new Exception("entrance ceck error!!");
    }

    // 入力データパース&デコード
    if (!($this->parse_decode())) {
      throw new Exception("parse or decode ceck error!!");
    }

    // マッピング
    // マッピングファイルの取得
    $map_file = $this->get_config_obj()->find('mapping_file_path');
//print $map_file . "\n";

    // マッピングファイルの解体
    //$this->mapdata_ = new config;
    //$this->mapdata_->set_file($map_file);
    $this->mapdata_ = new mw_config;
    $this->mapdata_->set_file($map_file);

    // Controller メイン
    // スイッチ文字列の取得
    // XXX cも可変にしたほうがいいんだろうなぁ…
    $cstring = $this->get_cgi_request_obj()->find("c");

    if ("" == $cstring) {
      // XXX どうする？ 一応indexに張るけど
      $cstring = "index";
    }

    // 指定されたmodelを叩く
    // XXX return値、stringでいいのか？？？
    // XXX よくないからインスタンスげとしてみた
//print "cstring is $cstring \n";
    $model_obj = $this->run_main($cstring, 0);

    // View処理
    if (true === $model_obj->is_direct_output_on()) {
      //
      $this->get_cgi_response_obj()->set_nocache();
      $this->get_cgi_response_obj()->put();
      //
      echo $model_obj->get_body();
    } else {
      // XXX ちと過去との互換性用に、すこしだけごにょった処理〜
      
      if ('' != $this->get_config_obj()->find('view_class')) {
        // こっちが「古い遺産」用の、後で消したいコード
        $vobj = $this->get_instance('view_class', 'view.inc:view');
      } else {
        // こっちが本道
        $vobj = $this->get_instance('view', 'view.inc:view');
      }
      //
      $vobj->set_body($model_obj->get_body());
      $vobj->set_res_obj($this->get_cgi_response_obj());
      $vobj->set_multi_obj($this->get_output_encoding_obj());
      $vobj->set_model_obj($model_obj);
      $vobj->output();
    }

    // XXX ここでよいか？
    ob_end_flush();

  } catch (Exception $e) {
// XXX
  print "get exception!!!\n";
  print security::sanitize_html($e->getMessage()) . "\n";
//print $e->getCode() . "\n";
//print $e->getFile() . "\n";
//print $e->getLine() . "\n";
//print_r($e->getTrace());
//print $e->getTraceAsString() . "\n";
  }

  // うちとこの終了処理
  // XXX tryの中にいれる？
  $this->exit_check();
}

// DIで吸収してみた〜
protected function get_instance($name, $default)
{
  // View処理
  $v = $this->get_config_obj()->find($name);
  if ("" !== $v) {
    //
    $awk = explode(":", $v);
    $inc = trim($awk[0]);
    $class = trim($awk[1]);

    //
    require_once($inc);
    $obj = new $class;
  } else {
    $obj = di::create($name, $default);
  }

  //
  return $obj;
}

//private:
private $bp_;		// base_path(基底になるパス)
private $cfobj_;	// サイト固有のconfig。configクラスインスタンス
private $mobobj_;	// mobile_infoクラスのインスタンス
private $reqobj_;	// CGI REQUEST インスタンス
private $resobj_;	// CGI RESPONSE インスタンス
private $mapdata_;	// マッピングファイル用：configインスタンス
private $inside_encoding_obj_;	// 内部エンコード用 multibyteインスタンス
private $output_encoding_obj_;	// 出力用 multibyteインスタンス
private $bag_obj_;	// model持ち回り用鞄インスタンス


} // end of class

