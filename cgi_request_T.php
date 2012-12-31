<?php

/**
 * CGI リクエストクラス
 *
 **/

$_GET = array("test" => array("t1", "t2", "t3"), "ttest" => "ngt1", "tes" => "ngt2");
$_POST = array("test" => array("pt1", "pt2", "pt3"), "ttest" => "pngt1", "tes" => "pngt2");
$_COOKIE = array("test" => array("ct1", "ct2", "ct3"), "ttest" => "cngt1", "tes" => "cngt2");

require_once('cgi_request.inc');

$req = new cgi_request;

//$req->set_hi_priority_post();
//$req->set_hi_priority_get();
//$req->set_only_get();
//$req->set_only_post();

$key = "test";
$ret = $req->find_array($key);
print_r($ret);
print $req->find($key) . "\n";

$ret = $req->find_cookie_array($key);
print_r($ret);
print $req->find_cookie($key) . "\n";

?>
