<?php

require("csv_row.inc");

$row = new csv_row;
//print_r($row);

$row->push('data1');
//print_r($row);
$row->push('da"ta2');
$row->push('data""3"');
//print_r($row);
$awk = array('test', 'test2', 'test3');
$row->push($awk);

print $row->get_string();
print "\n";
