<?php

/*
 * Configファイル
 */

//
use PHPUnit\Framework\TestCase;

// 対象になるクラスをinclude
require_once('mw_config.inc');

//
class configTest extends TestCase {
  /*
   * テスト前処理
   */
  public static function setUpBeforeClass(){
    static::$obj = new mw_config;
    // ファイルを設定
    static::$obj->set_file(__DIR__ . '/mw_config.conf');
  }


  /*
   * @test
   */
  public function test0comment() {
    $this->assertNull( static::$obj->find('command_ng1') );
    $this->assertNull( static::$obj->find('command_ng2') ); // 存在していない名前
  }

  /*
   * @test
   */
  public function test1nomal() {
    // command = includefile.inc:classname
    $robj = static::$obj->find('command');
    $this->assertNotNull($robj);
    $this->assertSame($robj->get_include_filename(), 'include.inc');
    $this->assertSame($robj->get_classname(), 'class');
  }

  /*
   * @test
   */
  public function test1abbreviation() {
    // 省略表記
    // command = :
    $robj = static::$obj->find('command_abbreviation');
    $this->assertNotNull($robj);
    $this->assertSame($robj->get_include_filename(), 'command_abbreviation.inc');
    $this->assertSame($robj->get_classname(), 'command_abbreviation');
    $this->assertSame($robj->get_template_filename(), 'command_abbreviation.tpl');
  }

  /*
   * @test
   */
  public function test1abbreviation2() {
    // 省略表記２
    // command = dir/:
    $robj = static::$obj->find('command_abbreviation2');
    $this->assertNotNull($robj);
    $this->assertSame($robj->get_include_filename(), 'dir/command_abbreviation2.inc');
    $this->assertSame($robj->get_classname(), 'command_abbreviation2');
    $this->assertSame($robj->get_template_filename(), 'command_abbreviation2.tpl');
  }

  /*
   * @test
   */
  public function test_add() {
    // 追加: テンプレートまで全記述
    // command_add1 = include.inc:class template.tpl
    $robj = static::$obj->find('command_add1');
    $this->assertNotNull($robj);
    $this->assertSame($robj->get_include_filename(), 'include.inc');
    $this->assertSame($robj->get_classname(), 'class');
    $this->assertSame($robj->get_template_filename(), 'template.tpl');

    // エラーテンプレートまで全記述
    // command_add2 = include2.inc:class2 template2.tpl,error_template.tpl
    $robj = static::$obj->find('command_add2');
    $this->assertNotNull($robj);
    $this->assertSame($robj->get_include_filename(), 'include2.inc');
    $this->assertSame($robj->get_classname(), 'class2');
    $this->assertSame($robj->get_template_filename(), 'template2.tpl');
    $this->assertSame($robj->get_error_template_filename(), 'error_template.tpl');

    // テンプレート名を省略、エラーテンプレート名を記述
    // command_add3 = include3.inc:class3 ,error_template3.tpl
    $robj = static::$obj->find('command_add3');
    $this->assertNotNull($robj);
    $this->assertSame($robj->get_include_filename(), 'include3.inc');
    $this->assertSame($robj->get_classname(), 'class3');
    $this->assertSame($robj->get_template_filename(), 'command_add3.tpl');
    $this->assertSame($robj->get_error_template_filename(), 'error_template3.tpl');

    // sslまで記述
    // command_add4 = include4.inc:class4 template4.tpl,error_template4.tpl ssl
    $robj = static::$obj->find('command_add4');
    $this->assertNotNull($robj);
    $this->assertSame($robj->get_include_filename(), 'include4.inc');
    $this->assertSame($robj->get_classname(), 'class4');
    $this->assertSame($robj->get_template_filename(), 'template4.tpl');
    $this->assertSame($robj->get_error_template_filename(), 'error_template4.tpl');
    $this->assertSame($robj->get_ssl_type(), 'ssl');

    // 省略＋ssl
    // command_composite = dirc/: composite.tpl ssl
    $robj = static::$obj->find('command_composite');
    $this->assertNotNull($robj);
    $this->assertSame($robj->get_include_filename(), 'dirc/command_composite.inc');
    $this->assertSame($robj->get_classname(), 'command_composite');
    $this->assertSame($robj->get_template_filename(), 'composite.tpl');
    $this->assertSame($robj->get_ssl_type(), 'ssl');
  }

  /*
   * @test
   */
  public function test_add2() {
    // 追加＆省略表記
    //command = dir/: input.tpl ssl
    $robj = static::$obj->find('command_composite');
    $this->assertNotNull($robj);
    $this->assertSame($robj->get_include_filename(), 'dirc/command_composite.inc');
    $this->assertSame($robj->get_classname(), 'command_composite');
    $this->assertSame($robj->get_template_filename(), 'composite.tpl');
    $this->assertSame($robj->get_error_template_filename(), '');
    $this->assertSame($robj->get_ssl_type(), 'ssl');
  }

  /*
   * @test
   */
  public function test_multi_space() {
    // 空白が複数個
    $robj = static::$obj->find('command_ex1');
    $this->assertNotNull($robj);
    $this->assertSame($robj->get_include_filename(), 'include4.inc');
    $this->assertSame($robj->get_classname(), 'class4');
    $this->assertSame($robj->get_template_filename(), 'template4.tpl');
    $this->assertSame($robj->get_error_template_filename(), 'error_template4.tpl');
    $this->assertSame($robj->get_ssl_type(), 'ssl');
  }

//
static private $obj;
}
