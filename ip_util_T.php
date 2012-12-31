<?php

// 対象になるクラスをinclude
require_once('ip_util.inc');

$ip = '63.255.255.255';
$net = '69.208.0.0/4';

$ret = ip_util::ip_in_network($ip, $net);
var_dump($ret);

