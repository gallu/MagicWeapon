<?php

require_once('/opt/www/mw.conf');
require_once("crypt_factory.inc");
$config = './crypt.conf';

$plain_text = "暗号化したい文字";


/*
 * 暗号化処理
 */
$en_o = crypt_factory::get_instance($config);

// 暗号化したい文字列セット
$en_o->set_plain_text($plain_text);

// 暗号化する。
$en_o->encrypt();

// 暗号文字列を受け取る
$encrypted_unit = $en_o->get_encrypted_unit();


/*
 * 複合化処理
 */
$de_o = crypt_factory::get_instance($config);
$de_o->set_encrypted_unit($encrypted_unit);
$de_o->decrypt();
$decrypted_text = $de_o->get_plain_text();


?>
平文：<?= $plain_text ?><br />
暗号化文字列：<?= $encrypted_unit ?><br />
複合化文字列：<?= $decrypted_text ?><br />

<?php
if ($plain_text === $decrypted_text) {
  print "test OK\n";
} else {
  print "test NG...\n";
}
