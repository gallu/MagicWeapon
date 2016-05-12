<?php

require_once('mw_crypt_make_secret_key.inc');
require_once('debug_util.inc');

//
$base_string = 'test';
$length = 16;
$key = mw_crypt_make_secret_key::make_key($base_string, $length);

echo debug_util::dump_string($key, false), "\n";
