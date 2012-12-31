<?php

require_once('multibyte.inc');

$o = new multibyte;
$s = "";

$o->set_sjis();
$o->convert($s);

$o->set_euc();
$o->convert($s);

$o->set_jis();
$o->convert($s);

$o->set_utf8();
$o->convert($s);

$o->set_guess_string("Sjis");
print $o->get_charset_string() . "\n";

$o->set_guess_string("eUC");
print $o->get_charset_string() . "\n";

$o->set_guess_string("shift-jis");
print $o->get_charset_string() . "\n";

$o->set_guess_string("shift_jis");
print $o->get_charset_string() . "\n";

$o->set_guess_string("jis");
print $o->get_charset_string() . "\n";

$o->set_guess_string("utf-8");
print $o->get_charset_string() . "\n";

$o->set_guess_string("utf8");
$o->convert($s);


?>
