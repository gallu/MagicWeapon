<?php

require_once('page_controll.inc');

  $obj = new page_controll;

$a = array("0", "1", "2", "3", "4", "5", "6", "7", "8");
  $obj->set_datas($a);
  $obj->set_page_num(7);
  $obj->set_par_item_num(1);

  $obj->make_list();

  print "max page is " . $obj->get_max_page() . "\n";
  $ret = $obj->make_range(4);
  var_dump($ret);

  $list = $obj->get_list();
  print_r($list);

  print "now page is " . $obj->get_page_num() . "\n";

  if ($obj->is_next()) {
    print "can next\n";
  }
  if ($obj->is_back()) {
    print "can back\n";
  }

