# Parallel Distributed Processing
- 授業"並列分散処理"で作成したプログラムが格納されています.

## lifegame_conc.c
- Conwayのライフゲーム(8192×8192のグリッド)
- 初期値は擬似乱数で生成
- 画面表示はグリッドの一部のみ
- 並列化(pthread)

## nqbt_omp.c
- Nクイーン問題
- 並列化(OpenMP)

### コンパイル方法
```
$ gcc -fopenmp hoge.c
```
## mpicomputepi.c
- モンテカルロ法による円周率の計算
- 並列化(OpenMPI)

### コンパイル方法
```
$ mpicc -Wall -o hoge hoge.c
```
