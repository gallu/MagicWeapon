<?php

require_once '/opt/www/simpletest/unit_tester.php';
require_once '/opt/www/simpletest/reporter.php';

require_once('basis_clump.inc');

class basis_clump_T extends UnitTestCase
{
    public function __construct()
    {
        $this->UnitTestCase();
    }

    public function test()
    {
        $obj = new basis_clump();
        // まず未設定の時のテスト
        $this->assertEqual($obj->is_name('test'), false);
        $this->assertEqual($obj->get_data('test'), "");

        // 設定してみる
        $this->assertEqual($obj->push_element('test'), true);
        $this->assertEqual($obj->is_name('test2'), false);
        $this->assertEqual($obj->is_name('test'), true);
        $this->assertEqual($obj->get_data('test'), "");
        $obj->set_data('test', "data");
        $this->assertEqual($obj->get_data('test'), "data");

        // 二重設定
        $this->assertEqual($obj->push_element('test'), false);
    }
}

$test = new basis_clump_T;
$test->run(new TextReporter());

