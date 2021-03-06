<?php

/**
 * IDとパスワードによる認証機構
 *
<pre>
テーブルフォーマット例
-- 管理画面用管理者テーブル。ログイン情報系込み。割と固定なカラムも多し。
DROP TABLE IF EXISTS admin_users;
CREATE TABLE admin_users (
  `id` varbinary(128) NOT NULL COMMENT '管理者を識別するためのID',
  `pass` varbinary(128) NOT NULL COMMENT 'ログインパスワード。password_hashが現在のお勧め',
  `lock_time` bigint unsigned NOT NULL COMMENT 'ロックタイム(エポック秒)。0以外なら「入っている時間まではログイン出来ない」',
  `err_count` int unsigned NOT NULL COMMENT 'ロックタイム(エポック秒)。0以外なら「入っている時間まではログイン出来ない」',
  `account_stop` int unsigned NOT NULL default 0 COMMENT 'これが０以外(基本は１)なら「アカウント停止状態」と見なす',
  -- 以下、authentication_id_passクラス系の処理では使わない(けどまぁよくつかう)カラム群
  `name` varchar(128) NOT NULL COMMENT '表示名',
  `role` int unsigned NOT NULL COMMENT '権限系。使わなければ削除で。',
  `created` datetime NOT NULL COMMENT '作成日時',
  `updated` datetime NOT NULL COMMENT '修正日時',
  `memo` text NOT NULL COMMENT 'ユーザに対するmemo',
  PRIMARY KEY (`id`)
)CHARACTER SET 'utf8mb4', ENGINE=InnoDB, COMMENT='1レコードが1ユーザを意味するテーブル';
</pre>
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 **/

class authentication_id_pass {

public function __construct()
{
  //
  $this->init();
}

/**
 * 初期化
 *
 * @access public
 */
public function init()
{
  $this->set_id('');
  $this->set_raw_password('');
  $this->set_authentication_clump(null);
  $this->set_is_lock_start(false);
  //
  $this->set_lock_time(3600); // デフォルトは1時間にしておく
  $this->set_err_count(5); // デフォルトは5回にしておく
}

/**
 * ログインID取得
 *
 * @access public
 * @return string ログインID
 */
public function get_id() { return $this->id_; }
/**
 * ログインID設定
 *
 * @access public
 * @param string $v ログインID
 */
public function set_id($v) { $this->id_ = $v; }

/**
 * raw_password(生パスワード)取得
 *
 * @access protected
 * @return string 生パスワード
 */
protected function get_raw_password() { return $this->raw_password_; }
/**
 * raw_password(生パスワード)設定
 *
 * @access public
 * @param string $v 生パスワード
 */
public function set_raw_password($v) { $this->raw_password_ = $v; }

/**
 * authentication_clump取得
 *
 * @access public
 * @return object authentication_clumpのインスタンス
 */
public function get_authentication_clump() { return $this->authentication_clump_; }
/**
 * authentication_clump設定
 *
 * @access public
 * @param object $v authentication_clumpのインスタンス
 */
public function set_authentication_clump($v) { $this->authentication_clump_ = $v; }

/**
 * ロック時間取得
 *
 * ロック回数を超えた「連続のパスワード入力ミス」で、ここで設定する「ロック時間」だけログインがロックされる
 *
 * @access public
 * @return int 時間(エポックタイム)
 */
public function get_lock_time() { return $this->lock_time_; }
/**
 * ロック時間設定
 *
 * ロック回数を超えた「連続のパスワード入力ミス」で、ここで設定する「ロック時間」だけログインがロックされる
 *
 * @access public
 * @param int 時間(エポックタイム)
 */
public function set_lock_time($v) { $this->lock_time_ = $v; }

/**
 * ロック回数取得
 *
 * この回数を超えた「連続のパスワード入力ミス」で、「ロック時間」だけログインがロックされる
 *
 * @access public
 * @return int ロック回数
 */
public function get_err_count() { return $this->err_count_; }
/**
 * ロック回数設定
 *
 * この回数を超えた「連続のパスワード入力ミス」で、「ロック時間」だけログインがロックされる
 *
 * @access public
 * @param string $v 生パスワード
 */
public function set_err_count($v) { $this->err_count_ = $v; }

/**
 * ロック開始確認
 *
 * @access public
 * @return boolean trueなら「新たにロックされた」
 */
public function is_lock_start() { return $this->is_lock_start_; }
/**
 * ログインID設定
 *
 * @access protected
 * @param boolean $v ロック状態
 */
protected function set_is_lock_start($v) { $this->is_lock_start_ = $v; }


/**
 * ログイン処理
 *
 * loginにまつわる処理を一式
 *
 * @access public
 * @return boolean ログイン成功ならtrue、失敗ならfalse
 */
public function login()
{
  // IDの把握
  $id = $this->get_id();

  // そのIDある？
  $obj = $this->get_authentication_clump();
  $obj->set_value('id', $id); // XXX magicnumber
  $r = $obj->get();
//var_dump($r);
  if (false === $r) {
    // ないらしいので認証失敗
//echo "id not find.\n";
    return false;
  }

  // アカウント停止状態かどうか確認
  if (true === $obj->is_name('account_stop')) {
    if (0 !== $obj->get_value('account_stop')) {
      // アカウント停止状態なので認証失敗
//var_dump( $obj->get_value('account_stop') );
//echo "account_stop.\n";
      return false;
    }
  }

  // ログインロック時間の確認
  if (0 !== $obj->get_value('lock_time')) {
    // 時間経過が一定未満ならNG
    if (time() <= $obj->get_value('lock_time')) {
//echo "lock_time.\n";
      return false;
    }
    // else
    // 時間経過が一定に達していたら、一端ロック時間を外す
    $obj->set_value('lock_time', 0, true); // とっととupdate
  }


  // パスワードの比較
  if ( false === $this->password_verify($obj) ) { 
//echo "password_verify.\n";
    // エラー回数をインクリメント
    $num = $obj->get_value('err_count');
    $num ++;
    $obj->set_value('err_count', $num);
    // 一定回数に達したら、ログインロック
    if ($this->get_err_count() < $num) {
      // ロック解除時間を設定
      $obj->set_value('lock_time', time() + $this->get_lock_time());
      // 「ロックした」のでtrueに
      $this->set_is_lock_start(true);
    }
    $obj->update(); // err_countと、状況によってはlock_time、をupdate
    //
    return false;
  }

  // 情報一通りOKっぽい

  // ロックカウントが１以上なら、クリアしておく
  if (0 < $obj->get_value('err_count')) {
    $obj->set_value('err_count', 0, true);
  }

  // ログインはOKだったよ? ノ
  return true;
}

/**
 * パスワードの比較
 *
 * @access public
 * @return boolean パスワード一致ならtrue、不一致ならfalse
 */
protected function password_verify($obj) {
  // IDの把握
  $id = $this->get_id();

  // パスワードのストレッチング
  $raw_pass = $this->get_raw_password();
  $pobj = di::create('authentication_password_stretching');
  $password_string = $pobj->stretching($id, $raw_pass);
//var_dump($password_string);
//var_dump($obj->get_value('pass'));

  // 比較
  return $password_string === $obj->get_value('pass'); // XXX magicnumber
}


//private:
private $id_;
private $raw_password_;
private $password_stretching_class_name_;
private $authentication_clump_;
private $err_count_;
private $lock_time_;
private $is_lock_start_;

} // end of class

