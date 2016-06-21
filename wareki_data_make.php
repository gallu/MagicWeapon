<?php

//
$lines = file('./wareki_data');
$_24 = '';
foreach($lines as $line) {
  // データ読み込みと分解
  $awk = explode("\t", rtrim($line));
  if ('' !== $awk[0]) {
    $_24 = $awk[0];
  }
  $awk[0] = $_24;

  // 開始通算日と終了通算日を把握
  $start = date('z', strtotime("2015-{$awk[2]}"));
  $end = date('z', strtotime("2015-{$awk[3]}"));
  // 相対日の出力(目確認用)
  $base = strtotime("2015-1-1");
  $start_t = date('m/d', strtotime("+{$start} days", $base));
  $end_t = date('m/d', strtotime("+{$end} days", $base));

  // case文の出力
  for($i = $start; $i <= $end; $i ++) {
    //
    echo "    case {$i}:\n";
  }
  echo "      \$ret['開始日通算'] = {$start};
      \$ret['開始日確認'] = '{$start_t}';
      \$ret['終了日通算'] = {$end};
      \$ret['終了日確認'] = '{$end_t}';
      \$ret['二十四節気'] = '{$awk[0]}';
      \$ret['候']         = '{$awk[1]}';
      \$ret['七十二候']       = '{$awk[4]}';
      \$ret['七十二候(開き)'] = '{$awk[5]}';
      \$ret['七十二候(読み)'] = '{$awk[6]}';
      \$ret['七十二候(意味)'] = '{$awk[7]}';
      break;\n\n";
    //
//var_dump($awk);
//break;
}

