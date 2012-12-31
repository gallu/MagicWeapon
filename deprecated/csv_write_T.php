<?php

require("/var/www/MW/csv_write.inc");

$row1 = array('data1_1', 'data1_2', 'da"ta1_3', 'da""ta1_4"');
$row2 = array('data2_1', 'data2_2', 'da"ta2_3', 'da""ta2_4"');
$row3 = array('data3_1', 'data3_2', 'da"ta3_3', 'da""ta3_4"');

// インスタンス作成
$writer = new csv_write;
//print_r($writer);

// 行データを配列からpush
$writer->push_row($row1);
//print_r($writer);

$writer->push_row($row2);
$writer->push_row($row3);
//print_r($writer);

//$writer->set_separator("\t");
//$writer->set_crlf("===\n");

print $writer->get_string() . "\n";

