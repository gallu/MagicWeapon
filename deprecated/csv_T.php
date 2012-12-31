<?php

require_once("csv.inc");

// テストデータ

$test_data = "aaa,bbb,ccc";
$test_data = "\"aaa\",\"bb\"b,\"cc\"\"cc,cc\",\"dd\nee\rff\n\"\n111,222,333\n\n\nabc\r\n";


$csv_obj = new csv_parse();
$csv_obj->set_data($test_data);
$csv_obj->parse();

$data = $csv_obj->get_parsed_data();


var_dump($data);





?>
