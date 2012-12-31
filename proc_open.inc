<?php

/**
 * 双方向パイプクラス
 *
 * コマンドラインのstdinに何か渡してstdoutに帰ってくるようなやつのラッパー
 *
 *
 * @package magicweapon
 * @link http://www.grid-works-guild.net/MagicWeapon/ MagicWeapon
 * @access  public
 * @author  Michiaki Wuzawa <wuzawa@m-fr.net>
 * @create  $LastChangedDate$
 * @version $Revision$
 **/

class proc_open
{
//public:

public function __construct()
{
  //
  $this->init();
}

//
public function init()
{
  // 要素を一式clear
  $this->set_cmd_line('');
  $this->set_stdin_string('');
  $this->set_stdout_string('');
}

// cmd_line
public function get_cmd_line() { return $this->cmd_line_; }
public function set_cmd_line($v) { $this->cmd_line_ = $v; }

// stdin_string
public function get_stdin_string() { return $this->stdin_string_; }
public function set_stdin_string($v) { $this->stdin_string_ = $v; }

// stdout_string
public function get_stdout_string() { return $this->stdout_string_; }
protected function set_stdout_string($v) { $this->stdout_string_ = $v; }

/**
 * 処理を行う
 *
 * @access public
 * @return boolean 何がしかエラーがあった場合、false
 */
public function execute()
{
  // 戻し用の配列を作成
  $ret = array(
    0 => array('pipe', 'r'),
    1 => array('pipe', 'w'),
    2 => array('pipe', 'w'),
  );

  // プロセスをこじ開ける
  $rc = proc_open($this->get_cmd_line(), $ret, $p, NULL, NULL);
//var_dump( $this->get_cmd_line() );

  // しくったら返却
  if (false === is_resource($rc)) {
    //
    return false;
  }

//
//var_dump($p);

  // stdinに必要なデータをぶち込む
  fwrite($p[0], $this->get_stdin_string());
  fclose($p[0]);
//var_dump( $this->get_stdin_string() );

  // stdoutの値を受け取る
  $this->set_stdout_string(stream_get_contents($p[1]));
  fclose($p[1]);
//var_dump($this->get_stdout_string());

  // プロセスを閉じる
  proc_close($rc);

  //
  return true;
}


//private:
private $cmd_line_;
private $stdin_string_;
private $stdout_string_;
} // end of class


