<?php

require_once('tokenizer.inc');

print tokenizer::get() . "\n";
print tokenizer::get("") . "\n";
print tokenizer::get("_") . "\n";
print tokenizer::get("&") . "\n";
print tokenizer::get("+") . "\n";

var_dump( tokenizer::_get('-', true, true, true) );


?>
