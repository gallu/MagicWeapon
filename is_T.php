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
