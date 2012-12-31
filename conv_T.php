<?php
require_once("conv.inc");

class foo2 {
  function m_conv($body, $param) {
    return ($body . "\n" . $body);
  }
}

class foo {
  function m_conv($body, $param) {
    $c = new f_conv;
    $c->monoDic("name", "複置換name");
    $wk = new foo2;
    $c->multiDic("list2", $wk);
    return $c->conv($body);
  }
}


$c = new f_conv;
$c->monoDic("name", "name_values");

$data = array("1", "10");
$c->monoDicToku("area", "OK", "NG...", $data);


$wk = new foo;
$c->multiDic("list", $wk);

$c->T_put();
print "\n";
print "----  CONV Start ---------------------------------------\n";
print $c->conv(file_get_contents("./conv_T.tmp"));


