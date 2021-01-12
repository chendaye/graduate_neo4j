# gcc和g++的区别总结：
  - gcc: GNU C Compiler
  - g++: GNU C++ Compiler
  
  > 差异

- g++会自动链接C++标准库，比如algorith,string,vector等。
- gcc会根据文件后缀(.c,.cpp)自动识别是C文件还是C++文件，g++均认为是C++文件。
- gcc编译C文件少很多宏定义，gcc编译C++会多一些宏定义。

# 编译命令

```c++
#   在用gcc编译c++文件时,为了能够使用STL,需要加参数 –lstdc++ ,但这并不代表 gcc –lstdc++ 和 g++等价
gcc -lstdc++  -I snap-core/ -I glib-core/ -I snap-adv/ -I glib-adv/ ./main.cpp

g++  -I snap-core/ -I glib-core/ main.cpp -o   main.o

g++ main.cpp -o main -I snap-core/ -I glib-core/ ./common.o  ./EquiTrussAttribute.o ./fptree.o 

```

# Makefile


