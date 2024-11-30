# minesweeper
だよん

WASDで操作
Spaceで掘り
Fで旗

爆弾以外全て掘ったら勝ち。途中で爆弾掘ったら負け。

`-d`コマンドで`[easy, advanced, hard, impossible]`渡すと難易度変更。
終

## run
```
bazel run mine
```

## example
```
remain: 20

 w w w w w w w w w
 1 2 w w w w w w w
 - 1 1 1 1 w w w w
 - - - - 1 1 2 w w
 - - - - - - 1 2 2
 - 1 1 1 - - - - -
 1 2 w 1[-]- - - -
 w w w 2 - - - 1 1
 w w w 1 - - - 1 w
```
