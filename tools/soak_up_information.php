<?php

/**
 * DBのテーブル定義からclumpファイルを一式作るtool
 *
 *
使い方
cd (base.incに書いてある base_dirのディレクトリ)
php (MWが置いてあるディレクトリ/tools/soak_up_information.php

ポイント
カレントディレクトリを「base.incに書いてある base_dirのディレクトリ」にしておくこと。
なんでかってぇと、「include('./batch/cron_base.inc');」ってのがあるので、その場所じゃないとファイル名が解決できないから。
この辺、後日色々とパラメタ追加して融通をきかせる予定ではあるんだけど、現状とりあえず。

効果
・基準ディレクトリ/lib/clump/base/に、現在のDB定義にそったclump_baseクラスを作る
・基準ディレクトリ/lib/clump/に、ファイルがなければ、新規にclumpクラスを作る

 *
 * ToDo
 * ・シャーディングしてる状況とか「adminとfrontでdatabaseを分けている」とかに未対応
 * ・
 *
 */

// 各種情報を一気に取り込む
include('./batch/cron_base.inc');
//
require_once('conv.inc');
require_once('conv_util.inc');

// type判定用
$type_guess = array (
  'TINYINT' => 'int',
  'SMALLINT' => 'int',
  'MEDIUMINT' => 'int',
  'INT' => 'int',
  'INTEGER' => 'int',
  'BIGINT' => 'int',
  'DECIMAL' => 'int',
  'NUMERIC' => 'int',

  'FLOAT' => 'float',
  'DOUBLE' => 'float',
  'DOUBLE PRECISION' => 'float',
  'REAL' => 'float',

  'DATE' => 'date',
  'DATETIME' => 'date',
  'TIMESTAMP' => 'date',
  'TIME' => 'date',
  'YEAR' => 'date',
);

  // 基準ディレクトリを取得
  // XXX 後で可変にしとかないとねぇ
//var_dump($argv);
  $base_dir = $bp;
//var_dump($base_dir);

  // おけつは / にしておきたいので
  if ('/' !== $base_dir[strlen($base_dir) - 1]) {
    $base_dir .= '/';
  }
//var_dump($base_dir);

  // その他定数
  // XXX 後でいぢりやすいようにここに固めておく
  // lib
  $lib_dir = $base_dir . 'lib/';
  // clump格納先
  $clump_dir = $base_dir . 'lib/clump/';
  // clump_base格納先
  $clump_base_dir = $base_dir . 'lib/clump/base/';

  // テンプレート(書式はconv)
  $clump_template_fn = dirname(__FILE__) . '/clump.tpl';
  $clump_base_template_fn = dirname(__FILE__) . '/clump_base.tpl';

//var_dump($dbh);

  // ディレクトリを確認＆なければ切る
  if (false === is_dir($clump_dir)) {
    $r = mkdir($clump_dir, 0755, true);
    if (false === $r) {
      print "{$clump_dir} is not maked\n";
      exit(-1);
    }
  }
  if (false === is_dir($clump_base_dir)) {
    $r = mkdir($clump_base_dir, 0755, true);
    if (false === $r) {
      print "{$clump_base_dir} is not maked\n";
      exit(-1);
    }
  }

  // 指定データベースのテーブル一覧を取得
  $sql = 'SHOW TABLE STATUS FROM ' . $dbh->get_database_name() . ';'; // XXX
  $res = $dbh->query($sql);
  $res->set_fetch_type_hash(); // hash名で欲しいので

  // テーブルごとにloop
  while($res->fetch()) {
//var_dump($res->get_row());
//var_dump($res->get_data('Name'));
//var_dump($res->get_data('Comment'));

    // １テーブルの情報を取得
    $sql = 'SHOW FULL COLUMNS FROM ' . $res->get_data('Name') . ';'; // XXX
    $res2 = $dbh->query($sql);
    $res2->set_fetch_type_hash(); // hash名で欲しいので

    // 必要なカラム情報群を把握
    $cols = array();
    $pk = array();
    while($res2->fetch()) {
      $awk = array();
      $awk['Field'] = $res2->get_data('Field');
      $awk['Type'] = $res2->get_data('Type');
      $awk['Comment'] = $res2->get_data('Comment');
      $awk['Key'] = $res2->get_data('Key');
      // PKチェック：いずれ、uniqueとかもやりたいねぇ
      if ( 0 === strcasecmp('PRI', $awk['Key']) ) {
        $pk[$awk['Field']] = 'pk';
        $awk['Key'] = 'pk';
      }
      // Typeの判定
      // XXX ちょいとべただねぇ…
      @list($type, $dummy) = explode('(', $awk['Type'], 2);
      $type = strtoupper($type);
      // 推測
      if (true === isset($type_guess[$type])) {
        $type_string = $type_guess[$type];
      } else {
        $type_string = 'string';;
      }
      // XXX いったんベタで文字列保持
      switch($type_string) {
        case 'int':
          $s = 'DATATYPE_INT';
          break;

        case 'string':
          $s = 'DATATYPE_STRING';
          break;

        case 'date':
          $s = 'DATATYPE_DATE';
          break;

        case 'float':
          $s = 'DATATYPE_FLOAT';
          break;

        default: // XXX 念のため
          $s = 'DATATYPE_STRING';
          break;
      }
      //
      $awk['TypeString'] = 'data_clump::' . $s;

      //
      $cols[] = $awk;

    }
//var_dump($cols);
//var_dump($pk);

    //
    $table_comment   = $res->get_data('Comment');
    $table_name      = $res->get_data('Name');
    $clump_name      = $res->get_data('Name') . '_clump';
    $clump_base_name = $res->get_data('Name') . '_clump_base';

    // lib/clump/base/にpush_elementだけのbaseを組み上げる
    $conv = new f_conv();
    $conv->monoDic('table_comment', $table_comment);
    $conv->monoDic('clump_base_name', $clump_base_name);
    $conv->monoDic('table_name', $table_name);
    //
    $conv->multiDic('recodes', new simple_loop($cols));
    //
    $s = $conv->conv( file_get_contents($clump_base_template_fn) );
    file_put_contents($clump_base_dir . $clump_base_name . '.inc', $s);

    // lib/clumpに、ファイルがなければ、clumpファイルを作る
    $fn = $clump_dir . $clump_name . '.inc';
    if (false === is_file($fn)) {
      //
      $conv->monoDic('clump_name', $clump_name);
      //
      $s = $conv->conv( file_get_contents($clump_template_fn) );
      file_put_contents($fn, $s);
    }
  }

