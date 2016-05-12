<?php

require_once('mw_crypt_mcrypt.inc');
require_once('debug_util.inc');

$obj = new mw_crypt_mcrypt();
$obj->set_cipher_rijndael_256();
$obj->set_key_base('test');
//
$ciphertext = $obj->encrypt('あいうえお');
var_dump($ciphertext);

//
$plaintext = $obj->decrypt_base64_decodde($ciphertext);
var_dump($plaintext);
