#!/bin/bash
rm -rf out
mkdir out
g++ -std=c++17 -O2 -o b main.cpp
a=0
while [ $a -lt 100 ]
do
	if [ $a -lt 10 ]
	then
		cargo run --release --bin tester in/000$a.txt ./b > out/000$a.txt
	else
		cargo run --release --bin tester in/00$a.txt ./b > out/00$a.txt
	fi
	a=`expr $a + 1`
done
a=0
while [ $a -lt 100 ]
do
	if [ $a -lt 10 ]
	then
		cargo run --release --bin vis in/000$a.txt out/000$a.txt > out/score_000$a.txt
	else
		cargo run --release --bin vis in/00$a.txt out/00$a.txt > out/score_00$a.txt
	fi
	a=`expr $a + 1`
done
python3 evaluate.py
