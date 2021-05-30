#!/bin/bash

#tools フォルダ下で実行
cd `dirname $0`
# コンパイル
g++ -std=c++17 -O2 -Wall -Wextra -o JK.out main.cpp
#[st, en] のseed のファイルを処理する．procsはプロセス数
st=0
en=100
procs=8
# インタラクティブ処理の関数
f1(){
  cargo run --release --bin tester in/$1.txt ./JK.out > out/$1.txt
}
# ビジュアライザに通して，スコアを標準出力として取得する関数
f2(){
   cargo run --release --bin vis in/$1.txt out/$1.txt >> score.txt
}
# xargs で関数使うための処理
export -f f1
export -f f2

#以前の処理を削除
rm -f score.txt
rm -rf out
mkdir out

# option

usage(){
  cat <<EOM
使い方：
  -s : 開始 seed
  -e : 終了 seed
  -P : プロセス数
ただし，開始 seed から終了 seed までの入力ファイルは tools/in 下に置いておいてください．
EOM

  exit 2
}

while getopts "s:e:P:" optKey; do
  case "$optKey" in
    s)
      st=${OPTARG}
      ;;
    e)
      en=${OPTARG}
      ;;
    P)
      procs=${OPTARG}
      ;;
    '-h' | '--help' | *)
      usage
      ;;
  esac
done

# 並列処理
seq -f '%04g' $st $en | xargs -t -n1 -P$procs -I{} bash -c "f1 {}"
seq -f '%04g' $st $en | xargs -t -n1 -P$procs -I{} bash -c "f2 {}"
# score.txt に書き込まれたスコアの計算
python3 evaluate.py
