# tinychain
A mini blockchain implementation in C++14 构建迷你区块链——《深入浅出区块链》

## 简要说明 INTRODUNCTION
tinychain是一个业余的项目，旨在为大家提供微型的简单易懂的供学习的代码。
以下是涉及的一些基础技术：
* 非对称加密算法 - cryptopp:RSA
* 本地持久化存储 - sqlite3
* P2P网络发现算法 - gossip
* P2P网络传输协议 - protobuf
* 命令交互协议 - JSON-RPC
* HTTP Server/Websocket Server - SimpleWeb

## 锻造工具 Toolchain
* gcc/clang support C++14
* boost 1.56+ (datetime)
* cmake
* git
* sqlite3 (apt-get install libsqlite3-dev / brew install sqlite3)
* cryptopp (apt-get install libcryptopp-dev / brew install cryptopp)

## 构建 Build
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

## 运行 Run
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

## 开发路径 TODO-LIST
1. 基础代码 - (DONE, to be improved)
1. PoW 挖矿代码 - (DONE, to be improved)
1. 加密与账户模型 (DOING)
1. P2P网络- (TODO)
1. 命令行与Web动态演示 - (DOING)
1. 共识可替换为PoS/DPoS/PBFT etc - (TODO)

## 仓库介绍 Source code view
* cli-tinychain文件夹：命令行
* contrib文件夹：json与mongoose工具
* etc文件夹：tinychain演示网页等
* include文件夹：tinychain中类的设计以及第三方库的头文件
* src文件夹：tinychain和第三方库的具体实现

## 库的依赖
如果你无法通过apt/brew安装cryptopp,可以选择源码安装
```
$ wget https://www.cryptopp.com/cryptopp565.zip
$ mkdir -p cryptopp
$ unzip cryptopp565.zip -d cryptopp
$ make -f GNUmakefile
$ make install
```

## cli-tinychain 命令说明
#### getnewkey
创新一对新的key pair
无参数

#### listkeys
查看已经创建的key pairs
无参数

#### startmining
开始挖矿，可多次启动，每次启动都会创建一个新的线程
可选参数-address
```
$ ./cli-tinychain startmining
$ ./cli-tinychain startmining $address
```

#### stopmining
停止挖矿，将停止所有挖矿线程，停止过程将耗费几秒钟
无参数

#### getheight
查看当前高度
无参数

#### getlastblock
查看最新区块
无参数

#### send
发送交易
必填参数-address amount
```
$ ./cli-tinychain send $adress $amount
```
