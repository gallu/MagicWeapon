<?php

// SimpleTest用基底ファイル

require_once '/opt/www/simpletest/unit_tester.php';
require_once '/opt/www/simpletest/reporter.php';

// 対象になるクラスをinclude
require_once('access_analysis_util.inc');

class simple_test_TT extends UnitTestCase
{
//
public function __construct()
{
  $this->UnitTestCase();
}

// 
public function test()
{
  // 正常系
  $awk = file('retrieval_word_list', FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
  $ans = mb_convert_encoding('日本語', 'eucjp-win', 'eucjp-win,JIS,UTF-8,ASCII');
//var_dump($ans);
  foreach($awk as $s) {
    $ret = access_analysis_util::get_retrieval_word($s);
//var_dump($ret);
    $ans2 = mb_convert_encoding($ret['word'], 'eucjp-win', 'UTF-8,eucjp-win,SJIS,JIS,ASCII');
    $this->assertIdentical($ans, $ans2);
  }

  // 存在しない系
  $awk = file('retrieval_word_nolist', FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
  foreach($awk as $s) {
    $ret = access_analysis_util::get_retrieval_word($s);
    $this->assertIdentical($ret, array());
  }


}
}

// 実働部分。変更不要
$test = new simple_test_TT;
$test->run(new TextReporter());

