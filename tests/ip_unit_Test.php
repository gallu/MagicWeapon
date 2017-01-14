<?php

/*
 * ip_unit
 */

//
use PHPUnit\Framework\TestCase;

// 対象になるクラスをinclude
require_once('ip_util.inc');

//
class ip_unitTest extends TestCase {
  /*
   * @test
   */
  public function testTrue() {
    // データを用意
    // true 各種
    $tdata = array(
      '69.208.0.0/4 , 64.0.0.0',
      '69.208.0.0/4 , 70.0.0.0',
      '69.208.0.0/4 , 79.255.255.255',
      '69.208.0.0/8 , 69.0.0.0',
      '69.208.0.0/8 , 69.100.0.0',
      '69.208.0.0/8 , 69.255.255.255',
      '69.208.0.0/24 , 69.208.0.0',
      '69.208.0.0/24 , 69.208.0.100',
      '69.208.0.0/24 , 69.208.0.255',
      '69.208.0.0/28 , 69.208.0.0',
      '69.208.0.0/28 , 69.208.0.10',
      '69.208.0.0/28 , 69.208.0.15',
      '69.208.0.0/30 , 69.208.0.0',
      '69.208.0.0/30 , 69.208.0.2',
      '69.208.0.0/30 , 69.208.0.3',
      '69.208.0.0/32 , 69.208.0.0',
      '69.208.0.0 , 69.208.0.0',
    );
//var_dump($tdata);
    foreach($tdata as $wk) {
      $awk = explode(',', $wk);
      $net = trim($awk[0]);
      $ip  = trim($awk[1]);
      $ret = ip_util::ip_in_network($ip, $net, "{$wk} false.");
      $this->assertSame($ret, true);
    }
  }

  /*
   * @test
   */
  public function testFalse() {
    // データを用意
    // false 各種
    $fdata = array(
      '69.208.0.0/4 , 63.255.255.255',
      '69.208.0.0/4 , 80.0.0.0',
      '69.208.0.0/32 , 69.208.0.1',
      '69.208.0.0 , 69.208.0.1',
    );
//var_dump($tdata);
    foreach($fdata as $wk) {
      $awk = explode(',', $wk);
      $net = trim($awk[0]);
      $ip  = trim($awk[1]);
      $ret = ip_util::ip_in_network($ip, $net);
      $this->assertSame($ret, false);
    }
  }
}
