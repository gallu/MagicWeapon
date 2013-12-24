<?php
/**
 * php.ini設定 セッション用
 *
 * そこそこ独断
 *
 * @package magicweapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

ini_set('session.entropy_file', '/dev/urandom');
ini_set('session.entropy_length', '16');
ini_set('session.hash_function', '1');
ini_set('session.use_only_cookies', '1');
ini_set('session.cookie_lifetime', '0');
ini_set('session.cache_expire', 60);
ini_set('session.use_trans_sid', '0');
ini_set('session.name', 'session_id');

//
ini_set('session.auto_start', 'off');

