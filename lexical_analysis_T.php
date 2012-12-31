<?php

require_once('lexical_analysis.inc');

$obj = new lexical_analysis;
$obj->set_config_file("./xml_make.conf");
$obj->set_data_file("./xml_data.xml");

$datas = $obj->parse();

foreach ($datas as $data) {
  print "-------------\n";
  print $data->get_first() . "\t" . $data->get_second() . "\n";
}
print "-------------\n";


?>
