<?php

//
// 確立テーブルクラス
//
// XXX C++のmultimap相当のものがないのでhash配列で代用
// XXX 重複キーが使えなくなる点に気をつけること
//
//

class estab_table {
//public:
  function init() { $this->data_ = array(); }
  function set($s, $i) { $this->data_[$s] = $i; }

function decision() {

  // まずは総計を出す
  $total = 0;
  foreach ($this->data_ as $key => $val) {
    $total += $val;
  }
//print "total is " . $total . "\n";

  // 乱数発生
  $ran = mt_rand(0, $total - 1);
//print "ran is " . $ran . "\n";

  // 判定
  $total = 0;
  $ret = "";
  foreach ($this->data_ as $key => $val) {
    $total += $val;
    if ($total > $ran) {
      $ret = $key;
      break;
    }
  }

  return $ret;
}



//private:
  var $data_ = array();
}


