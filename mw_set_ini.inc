<?php
/**
 * php.ini設定
 *
 * そこそこ独断
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

// 内部文字コード指定
require_once('mw_set_ini_encoding.inc');
mw_set_ini_encoding('UTF-8');

// 文字コードの自動判定用
// XXX sjisの可能性が高い場合は「sjis-win,eucjp-win,JIS,UTF-8,ASCII」のほうがよさげ
mb_detect_order("UTF-8,sjis-win,eucjp-win,JIS,ASCII");

// XXX 5.6.0から非推奨
if (version_compare(PHP_VERSION, '5.6.0') < 0) {
  // request文字の自動変換とかしない
  ini_set('mbstring.http_input', 'pass');

  // response文字の自動変換とかしない
  ini_set('mbstring.http_output', 'pass');
} else {
  // request文字の自動変換とかしない
  ini_set('input_encoding', '');

  // response文字の自動変換とかしない
  ini_set('output_encoding', '');
}

// 厳密なエンコーディング検出を行いません
ini_set('mbstring.strict_detection', '0');

// 文字コード変換で「おかしな値」が入ってきたときに出す、代替文字の指定
ini_set('mbstring.substitute_character', '?');

// mb_output_handler()使わないから設定潰す〜
ini_set('mbstring.http_output_conv_mimetypes', NULL); // XXX NULLでよいかしらん？

// 「スクリプトをinternal_encodingとは違うエンコードで書きたい場合の指定」はいらない
ini_set('mbstring.script_encoding', NULL);


// エラーメッセージのHTMLタグをオフにする
ini_set('html_errors', false);

// データベースおよびテクストファイルを含む外部ソースから データを返す全ての関数のクオートをつけない
ini_set('magic_quotes_runtime', 0);

// タイムアウト設定
ini_set('max_execution_time', 60);

// エラーメッセージを、サーバーのエラーログまたはerror_logに記録する
ini_set("log_errors", true);

// 5.4以降は常にonなので、そうしておく
ini_set('short_open_tag', true);

// 一応念のため〜
ini_set('register_globals', 'off');

// CookieがJavaScriptからはアクセスできなくなる…かもしれない程度の保証
ini_set('session.cookie_httponly', true);

// ンなに重くてもこまるでしょう、的な
ini_set('memory_limit', '64M');

// PHPという素性は一応隠すよ〜
ini_set('expose_php', 'off');

// GET、POSTのパース最大時間(秒)
ini_set('max_input_time', 30);
// パラメタの最大数
ini_set('max_input_vars', 500);

