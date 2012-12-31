<?php

require_once('dice.inc');

$obj = new dice;
print $obj->get() . "\n";
$obj->set('1d4+2d6+1d8+3');
print $obj->get() . "\n";
print $obj->get() . "\n";
