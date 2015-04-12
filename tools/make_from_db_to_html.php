<?php

/**
 * DBのテーブル定義からHTML作る時用のベース情報を作成する
 *
 * 具体的には カラム名[tab]コメント の配列を、引数で与えられたテーブル名についてoutputする
 */

// 各種情報を一気に取り込む
include('./batch/cron_base.inc');

// 引数からテーブル名を把握
$name = $argv[1]; // XXX エラーチェックなし

// １テーブルの情報を取得
$sql = 'SHOW FULL COLUMNS FROM `' . $name . '` ;'; // XXX
$res = $dbh->query($sql);
$res->set_fetch_type_hash(); // hash名で欲しいので

//
while($res->fetch()) {
  echo $res->get_data('Field');
  echo "\t";
  echo $res->get_data('Comment');
  echo "\n";
}

