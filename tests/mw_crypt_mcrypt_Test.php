<?php

/*
 * mcrypt暗号
 */

//
use PHPUnit\Framework\TestCase;

// 対象になるクラスをinclude
require_once('mw_crypt_mcrypt.inc');

//
class crypt_mcrypt_Test extends TestCase {
  /*
   * @test
   */
  public function testCrypt() {
    //
    $data = array(
      'asoejr9873',
      'あいうえおかきくけこ',
      '漢字交じり',
      '漢字iii交じり',
    );
    $cipher = array(
      'rijndael_128',
      'rijndael_192',
      'rijndael_256',
      'blowfish',
    );
    $mode = array (
      'cbc',
      'ecb',
    );

    // 等しい系テスト
    foreach($data as $d) {
      foreach($cipher as $c) {
        foreach($mode as $m) {
          //
          $obj = new mw_crypt_mcrypt();
          $s = "set_cipher_{$c}";
          $obj->$s();
          $s = "set_mode_{$m}";
          $obj->$s();
          $obj->set_key_base('test');
          $crypttext = $obj->encrypt($d);
          //
          $plaintext = $obj->decrypt_base64_decodde($crypttext);

          //
          $this->assertSame($d, $plaintext);
        }
      }
    }

    //
  }
}
