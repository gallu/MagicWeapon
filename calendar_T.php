<?php

  require_once('calendar.inc');

  $cal = new calendar;
  
  // セッターテスト
  $cal->set_year(1);
  $cal->set_month(2);
  $cal->set_day(3);
  $cal->set_hour(4);
  $cal->set_minute(5);
  $cal->set_second(6);
  
  //printAll($cal);

/*  
  $cal->set_birth_day_from(10);
  printAll($cal);
  $cal->set_birth_day_to(10);
  printAll($cal);
*/
  // 現在時刻設定
  $cal->set_now_date();
  printAll($cal);
  $cal->calculation_year(2);
  printAll($cal);
  $cal->calculation_year(-3);
  printAll($cal);

  print "第".$cal->get_weeks_number_of_month(). "週目". "\n";
  // 文字列系テスト
  for($i = 0; $i < 12; $i++){
    $cal->set_month($i);
    print $i. "\t". $cal->get_month_string(). "\t". $cal->get_month_string_short(). "\n";
  }
  
  for($i = 0; $i < 14; $i++){
    $cal->set_day($i);
    print $i. "\t". $cal->get_week_string(). "\t". $cal->get_week_string_short(). "\n";
  }
  
  $cal->set_string("2006 2 15 12:13:14");
  print $cal->get_string(). "\n";
  print $cal->get_string("-"). "\n";
  
  print "GMT--". "\n";
  $cal->set_zoneGMT();
  $cal->set_stringApache("12/Feb/2006:05:48:55 +0900");
  print $cal->get_stringZ(). "\n";
  print $cal->get_stringZ("#"). "\n";

  print "JST--". "\n";
  $cal->set_zoneJ();
  print $cal->get_stringZ(). "\n";
  print $cal->get_string_for_cookie(). "\n";
  
  print "第".$cal->get_weeks_number_of_month(). "週目". "\n";
  
  $cal->set_matubi();
  printAll($cal);
  
  print "第".$cal->get_weeks_number_of_month(). "週目". "\n";

  
  //$cal->set_day(34);
  
  // is()テスト
  if($cal->is()){
    print "ok". "\n";
  }else{
    print "ng". "\n";
  }

  print $cal->get_julian(). "\n";

  $cal->calculation_day(-10);
  print $cal->get_string(). "\n";

  print $cal->get_julian(). "\n";

  $cal->calculation_day(365);
  print $cal->get_string(). "\n";

  $cal->calculation_hour(6);
  print $cal->get_string(). "\n";
  $cal->calculation_hour(-48);
  print $cal->get_string(). "\n";

  print $cal->get_julian(). "\n";
  print $cal->get_week_string_short(). "\n";

  $cal2 = $cal->deep_copy();
  
  //$cal->calculation_day(1);
  //$cal->calculation_hour(1);

  if($cal->equal($cal2)){
    print "Equal". "\n";
  }else{
    print "not equal". "\n";
  } 

  if($cal->equal_date($cal2)){
    print "Equal". "\n";
  }else{
    print "not equal". "\n";
  } 

  if($cal->equal_time($cal2)){
    print "Equal". "\n";
  }else{
    print "not equal". "\n";
  } 

  
  $age = $cal->make_age();
  print "age is $age \n";

//
$cal = new calendar;
$cal2 = new calendar;

$cal->set_now_date();
$cal->set_hour(0);
$cal->set_min(0);
$cal->set_sec(0);

for($i = 0; $i < 10000; $i ++) {
  $s = $cal->get_string();
  $cal2->set_julian($cal->get_julian());
  $s2 = $cal2->get_string();
  
  if ($s !== $s2) {
    print "error $s / $s2 <br>\n";
  }
  //print "$s <br>\n";
  $cal->calculation_day(1);
}

$cal->set_string('19701125020300');
printAll($cal);


////////////

  function printAll($cal)
  {
    print "year\t". $cal->get_year(). "\n";
    print "mon\t". $cal->get_month(). "\n";
    print "day\t". $cal->get_day(). "\n";
    print "hour\t". $cal->get_hour(). "\n";
    print "min\t". $cal->get_minute(). "\n";
    print "min2\t". $cal->get_min(). "\n";
    print "sec\t". $cal->get_second(). "\n";
    print "sec2\t". $cal->get_sec(). "\n";
  }
  
  
  
?>
