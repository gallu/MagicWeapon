<?php

//
// logクラス(テスト)
//

require_once("./log.php");

// 基本設定
$log = new log();
$log->set_filename("./test.log"); // ログを書き出すファイル名(パス)
$log->set_pname("winny");         // プログラム名

// デフォルトonのものたち
//$log->spreadtime_off();         // offならエポックで出るはず。。
//$log->off();                    // offならファイルに書き込まれないはず。。

// ログを残したいなと思ったら...
$log->add("ultra fatal error!");      // 残す！
$log->pause();                        // 区切りを入れて..
$log->add("--");                      // 残す！
$log->add("ろぐをしっかり残そう");    // 残す！

//var_dump($log);

$log->flush(); // ファイルに書き出し

// そのまま次のログに使える。
$log->add("2回目");               // 残す！
$log->add(": fatal error.");      // 残す！

$log->flush();


?>
