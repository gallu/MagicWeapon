<?php

require_once('difficulty_imagining_identification.inc');
//
$obj = new difficulty_imagining_identification();
var_dump( $obj->get() );
var_dump( $obj->get_base64() );
