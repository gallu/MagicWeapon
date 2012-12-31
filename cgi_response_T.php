<?php

require_once('cgi_response.inc');

  # オブジェクト作成
  $ob = new cgi_response;

  # サイズ
  $ob->set_length(1023);
  //$ob->put();
  //$ob->is_need_convert();


  # 通常の出力用
  $ob->set_content_html();
  $ob->put();
  $ob->is_need_convert();

/*

  # いろいろなラッパー
  $ob->set_content_plain();
  $ob->is_need_convert();
  $ob->put();
  # 画像系
  $ob->set_content_jpeg();
  $ob->is_need_convert();
  $ob->put();
  $ob->set_content_gif();
  $ob->is_need_convert();
  $ob->put();
  $ob->set_content_png();
  $ob->is_need_convert();
  $ob->put();

  $ob->set_location("http://foo.com/"); # Locationヘッダ用
  $ob->is_need_convert();
  $ob->put();

  $ob->set_status204(); # 画面を変化させない
  $ob->put();
  $ob->set_nocache(); # 画面データをキャッシュしない
  $ob->is_need_convert();
  $ob->put();

*/

?>
