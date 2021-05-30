#!/bin/bash

#tools フォルダ下で実行
cd `dirname $0`

# コンパイル
g++ -std=c++17 -O2 -Wall -Wextra -o JK.out main.cpp
#[st, en] のseed のファイルを処理する
st=0
en=300
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
# 並列処理
seq -f '%04g' $st $en | xargs -t -n1 -P8 -I{} bash -c "f1 {}"
seq -f '%04g' $st $en | xargs -t -n1 -P8 -I{} bash -c "f2 {}"
# score.txt に書き込まれたスコアの計算
python3 evaluate.py
