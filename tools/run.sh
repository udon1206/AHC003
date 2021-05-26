#!/bin/bash
rm -rf out
mkdir out
g++ -std=c++17 -O2 -o b.out main.cpp

f1(){
  cargo run --release --bin tester in/$1.txt ./b.out > out/$1.txt
}
f2(){
  cargo run --release --bin vis in/$1.txt out/$1.txt > out/score_$1.txt
}
export -f f1
export -f f2
seq -f '%04g' 1000 1299 | xargs -t -n1 -P8 -I{} bash -c "f1 {}"
seq -f '%04g' 1000 1299 | xargs -t -n1 -P8 -I{} bash -c "f2 {}"

python3 evaluate.py
