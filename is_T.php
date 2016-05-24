<?php

// 対象になるクラスをinclude
require_once('is.inc');

function assertIdentical($b1, $b2) {
  if ($b1 === $b2) {
    echo "ok\n";
  } else {
    echo "ng...........................\n";
  }
}

  // 等しい系テスト
  assertIdentical(is::is_katakana_utf8('a'), false);
  assertIdentical(is::is_katakana_utf8('あ'), false);
  assertIdentical(is::is_katakana_utf8('アあ'), false);
  //
  assertIdentical(is::is_katakana_utf8('ア'), true);
  assertIdentical(is::is_katakana_utf8('アー'), true);
  assertIdentical(is::is_katakana_utf8('アーダヶ'), true);


  // ひらがな
  assertIdentical(is::is_kana_utf8('a'), false);
  assertIdentical(is::is_kana_utf8('ア'), false);
  assertIdentical(is::is_kana_utf8('アあ'), false);
  //
  assertIdentical(is::is_kana_utf8('あ'), true);
  assertIdentical(is::is_kana_utf8('あいう'), true);
  assertIdentical(is::is_kana_utf8('あーん'), true);

  // ドメイン
  assertIdentical(is::is_domain('test.com'), true);
  assertIdentical(is::is_domain('www.hogehoge.jp'), true);
  assertIdentical(is::is_domain('www.hoge-hoge.jp'), true);
  //
  assertIdentical(is::is_domain('www.hogehoge.jp.'), false); // …微妙だけどねぇ
  assertIdentical(is::is_domain('www..hogehoge.jp'), false);
  assertIdentical(is::is_domain('www.hoge_hoge.jp'), false);
  assertIdentical(is::is_domain('www.hoge&%$#"hoge.jp'), false);


  // メアド
  assertIdentical(is::is_email('hoge@geho.com'), true);
  assertIdentical(is::is_email('hoge.@aa.jp'), true);
  assertIdentical(is::is_email('hoge.+test@aa.jp'), true);
  assertIdentical(is::is_email('hoge.-test@aa.jp'), true);
  assertIdentical(is::is_email('"hoge@aaa+test"@aa.jp'), true);
  assertIdentical(is::is_email('hoge@ge-ho.com'), true);
  assertIdentical(is::is_email('"><script>alert(\'or/**/1=1#\')</script>"@example.jp'), true);
  assertIdentical(is::is_email('"><script>alert(\'or 1=1#\')</script>"@example.jp'), true);
  //
  assertIdentical(is::is_email('hoge@1.2.3.4'), false);
  assertIdentical(is::is_email('hoge@ho_ge.com'), false);
  assertIdentical(is::is_email('hoge@ho_ge'), false);
  assertIdentical(is::is_email('@hoge.com'), false);
  assertIdentical(is::is_email('hoge@'), false);

  // is_array
  $awk = array(1,2,3);
  assertIdentical(is::is_array($awk), true);
  assertIdentical(is::is_array(array()), true);
  $awk = new arrayObject();
  assertIdentical(is::is_array($awk), true);
  //
  assertIdentical(is::is_array(null), false);
  assertIdentical(is::is_array(10), false);
  assertIdentical(is::is_array('abc'), false);
  
