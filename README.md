# tinychain
A mini blockchain implementation in C++14
构建迷你区块链——《深入浅出区块链》

## toolchain
* gcc/clang support C++14
* boost 1.56+ (datetime)
* cmake
* git
* sqlite3 (apt-get install libsqlite3-dev / brew install sqlite3)

## build
On workpath of tinychain:
```
$ mkdir -p build && cd build
$ cmake ..
$ make
```

## run
On workpath of tinychain:
```
$ cd build/bin
$ mkdir -p webroot && cp -f ../../etc/index.html webroot
$ ./tinychain
```

## 开发路径（TODO-LIST）
1. 基础代码
1. PoW 挖矿代码
1. P2P网络
1. 加密与账户模型
1. 命令行与展示
1. 共识可替换为PoS/DPoS etc
