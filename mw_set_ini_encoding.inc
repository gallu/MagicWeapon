<?php
/**
 * 内部エンコーディング周りphp.ini設定
 *
 * そこそこ独断
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

function mw_set_ini_encoding($encode = 'UTF-8') {
  // charsetの送信
  ini_set('default_charset', $encode);

  // 内部文字コード指定
  // XXX 些かベタいが…やむを得ないか…
  // XXX 追加とかあったら教えてください
  $e = 'ja_JP.UTF-8'; // デフォルトUTF-8で
  switch( strtolower($encode) ) {
    case 'utf-8':
    case 'utf-8-mobile#docomo':
    case 'utf-8-mobile#kddi-a':
    case 'utf-8-mobile#kddi-b':
    case 'utf-8-mobile#softbank':
      $e = 'ja_JP.UTF-8';
      break;

    case 'euc-jp':
    case 'eucjp-win':
    case 'cp51932':
      $e = 'ja_JP.EUC-JP';
      break;

    case 'sjis':
    case 'sjis-win':
    case 'cp932':
    case 'sjis-mac':
    case 'sjis-mobile#docomo':
    case 'sjis-mobile#kddi':
    case 'sjis-mobile#softbank':
      $e = 'ja_JP.Shift_JIS';
      break;

  }
  setlocale(LC_ALL, $e);

  // XXX 一旦日本語限定
  ini_set('mbstring.language', 'Japanese');

  //
  if (version_compare(PHP_VERSION, '5.6.0') < 0) {
    ini_set('mbstring.internal_encoding', $encode); /// 5.6.0から非推奨
  } else {
    ini_set('internal_encoding', $encode); // 5.6.0以降はこっち
  }
  mb_internal_encoding($encode);
}

