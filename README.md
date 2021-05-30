# AHC003

[AHC003](https://atcoder.jp/contests/ahc003) のリポジトリ

アルゴリズム自体は[ブログ](https://jupiro.hatenablog.com/entry/AHC003)を見てください．

## 並列実行のやり方

```sh
$ ./tools/run.sh
```
で seed が 0 から 100 までが並列で実行されます．

`-s` と `-e` で開始 seed と 終了 seed を指定できます．

seed が 1200 から 1300 までを並列実行する時は，

```sh
$ ./tools/run.sh -s 1200 -e 1300
```

のようにします．ただし，入力ファイルはあらかじめ ```tools/in``` 下に置いておいてください．(clone した状態なら 0 から 2999 までの seed の入力ファイルが置いてあります．)

プロセス数は `-P` で指定できます． (デフォルトは 8)

```sh
$ ./tools/run.sh -s 1200 -e 1300 -P 5
```

最初の一回は Rust の実行の関係で時間かかるかもしれません．