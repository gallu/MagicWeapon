<?php

// 対象になるクラスをinclude
require_once('csv_util.inc');

//
$awk = [
  ['"',2,'3,4'],
  [1,2,3],
  [1,2,3],
];
$awk[] = new ArrayObject(['a', 'b', 'c']);
//
$s = csv_util::make_csv_string($awk);
echo $s;

//
$csv = csv_util::parse_string("1,2,3\n4,\"5,5.5\n5.6\",6\n7,8,9\n");
var_dump($csv);
