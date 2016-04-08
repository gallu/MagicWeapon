<?php

require_once('probability.inc');

$obj = new probability();
//
$obj->push('item_1', 1);
$obj->push('item_2', 1);
$obj->push('item_3', 1);
//var_dump($obj);
var_dump( $obj->choice() );
