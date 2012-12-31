<?php

require_once('emoji_convert.inc');

$s = '[emoji:100]test[emoji:1]test2[emoji:2]test3[emoji:e4]hogera';


$obj = new emoji_convert;
$ss = $obj->convert($s);

print $ss;

