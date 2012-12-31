<?php

require_once('xml_hasharray_converter.inc');

// array to xml
$obj = new xml_hasharray_converter;

$obj->set_value('name', 'value');
$obj->set_value('name2', 'value2');
$obj->set_value('name3', 'にほんご');
$obj->set_document_element_name('SEEBOX_hogehoge');
$s = $obj->make_xml();
print "xml is \n$s \n";

//==================
// xml to array

$obj2 = new xml_hasharray_converter;
$obj2->set_xml($s);
print "name is " . $obj2->get_value('name') . "\n";
print "name2 is " . $obj2->get_value('name2') . "\n";
print "name3 is " . $obj2->get_value('name3') . "\n";
