<?php

require_once('magic_byte_guess.inc');

//
$awk = array(
  'magic_byte_guess.gif',
  'magic_byte_guess.png',
  'magic_byte_guess.tar',
  'magic_byte_guess.jpg',
  'magic_byte_guess.tar.gz',
  'magic_byte_guess.inc',
  'unknown.inc',
);
foreach($awk as $fn) {
  $r = magic_byte_guess::for_file($fn);
  echo "{$fn} is ...\n";
  var_dump($r);
  //
  $r = magic_byte_guess::for_string(@file_get_contents($fn));
  echo "{$fn} is ...\n";
  var_dump($r);
}
