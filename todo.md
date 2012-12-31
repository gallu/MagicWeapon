# ToDo list

* data_clumpのトランザクション対応各種
* data_clump DBのdeeps、２段でよくね？
* 絵文字周り修正っつか作り直し
* cgi_request 全体的に見直し
* cgi_response 全体的に見直し：ヘッダの削除系メソッドとかis系メソッドとかキャッシュ部分の見直しとか
* session data系見直し＆作り直し
* sql_utilをmw_makesqlにしたことによる、empty_mail_analysis.inc empty_mail_cushion.inc page_controll_limit.inc の改修
* postgresqlでいろいろ：upsertとか
* f_pdoにあるdbh_pdoクラスのconnect。接続のDB文字コードをutf8固定にしてる。後で切り出さないとねぇ…
* PDO系でのトランザクションどしよ？ 現状SQL文で開始させてるけど、beginTransactionメソッドとか使った方がよいかねぇ？ 的なお悩みが少し…
* soak_up_information.phpに対応する「存在しない(削除した)テーブルのclumpを削除、またはrename、またはお知らせする」系のバッチ作成
* data_clumpで、プリペアドステートメントをストックしてその辺のマシンコストをお安くする系の設計

