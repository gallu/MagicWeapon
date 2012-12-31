<?php

//
require_once('html_parser.inc');

$obj = new html_parser();
$obj->set_html_string( file_get_contents('http://www.m-fr.net'));

$awk = $obj->get_html_array();
//var_dump($awk);
//exit;

foreach($awk as $wk) {
  //
  if (true === $wk->is_contents()) {
    $s = $wk->get_contents();
    //$wk->set_contents($s);
print "contents!!({$s})\n";
  }
  else
  if (true === $wk->is_comment()) {
    $s = $wk->get_comment();
print "comment!!({$s})\n";
  }
  else
  if (true === $wk->is_html_tag()) {
print "tag!!\n";
    $sss = $wk->get_element_name();
print "tag name is {$sss}\n";
    if (true === $wk->is_html_start_tag()) {
print "start tag!!\n";
    $aaa = $wk->get_attribute_array(); // multimapで
var_dump($aaa);
    //$wk->set_attribute_array($aaa2); // multimapで
    }
    if (true === $wk->is_html_end_tag()) {
print "end tag!!\n";
    }
  }
}
