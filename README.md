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

Specifically, build on Windows (example):
```
> mkdir build
> cd build
> cmake .. -G "Visual Studio 15 2017 Win64" -DBOOST_ROOT=/local/boost_1_67_0
```
Open tinychain.sln under build subfolder with Visual Studio to compile.

## run
On workpath of tinychain:
```
$ cd build/bin
$ mkdir -p webroot && cp -f ../../etc/index.html webroot
$ ./tinychain
```

Specifically, run on Windows (example):
```
> cd build\bin\Release
> mkdir webroot
> copy /Y ..\..\..\etc\index.html webroot
> tinychain.exe
```

## 开发路径（TODO-LIST）
1. 基础代码
1. PoW 挖矿代码
1. P2P网络
1. 加密与账户模型
1. 命令行与展示
1. 共识可替换为PoS/DPoS etc

## 仓库介绍
cli-tinychain文件夹：命令行
contrib文件夹：json与mongoose工具
etc文件夹：tinychain演示网页等
include文件夹：tinychain中类的设计以及第三方库的头文件
src文件夹：tinychain和第三方库的具体实现
