# tinychain
A mini blockchain implementation in C++14
构建迷你区块链——《深入浅出区块链》

## toolchain
* gcc/clang support C++14
* boost 1.56+ (datetime)
* cmake
* git

## build
On workpath of tinychain:
```
$ mkdir build
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
