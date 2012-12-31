<?php

require_once('access_analysis_util.inc');

$uri = 'http://www.google.co.jp/se/a/r/ch?hl=ja&q=%E6%97%A5%E6%9C%AC%E8%AA%9E&btnG=Google+%E6%A4%9C%E7%B4%A2&lr=lang_ja';
$uri = 'http://wwwwww.google.co.jp/se/a/r/ch?hl=ja&q=%E6%97%A5%E6%9C%AC%E8%AA%9E&btnG=Google+%E6%A4%9C%E7%B4%A2&lr=lang_ja';
$ret = access_analysis_util::get_retrieval_word($uri);
var_dump($ret);

//$awk = file('retrieval_word_list', FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);

