<?php

require_once('make_calender.inc');

$c = new f_conv;
$ob = new make_calendar;
$ob->set_year(2006);
$ob->set_month(6);
$c->multiDic("loop", $ob);

$template = '$$$loop$$$
    <tr>
        <td>%%%d0%%% , %%%opt0%%%
        <td>%%%d1%%% , %%%opt1%%%
        <td>%%%d2%%% , %%%opt2%%%
        <td>%%%d3%%% , %%%opt3%%%
        <td>%%%d4%%% , %%%opt4%%%
        <td>%%%d5%%% , %%%opt5%%%
        <td>%%%d6%%% , %%%opt6%%%
    </tr>
$$$/loop$$$';

print $template . "\n";

print $c->conv($template);


?>
