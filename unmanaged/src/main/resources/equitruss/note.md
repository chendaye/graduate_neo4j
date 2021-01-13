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

# gcc 参数详解

## 步骤

> gcc 与 g++ 分别是 gnu 的 c & c++ 编译器 gcc/g++ 在执行编译工作的时候，总共需要4步
- 预处理,生成 .i 的文件[预处理器cpp]
- 将预处理后的文件转换成汇编语言, 生成文件 .s [编译器egcs]
- 有汇编变为目标代码(机器代码)生成 .o 的文件[汇编器as]
- 连接目标代码, 生成可执行程序 [链接器ld]

## 参数

> -x language filename  设定文件所使用的语言, 使后缀名无效, 对以后的多个有效

```c++
gcc -x c hello.pig 
```

> -x none filename 关掉上一个选项，也就是让gcc根据文件名后缀，自动识别文件类型 

```c++
gcc -x c hello.pig -x none hello2.c 
```

> -c 只激活预处理,编译,和汇编,也就是他只把程序做成obj文件, 生成 .o 的 obj 文件

```c++
gcc -c hello.c 
```

> -o 制定目标名称, 默认的时候, gcc 编译出来的文件是 a.out, 很难听, 如果你和我有同感，改掉它

```c++
gcc -o hello.exe hello.c
gcc -o hello.asm -S hello.c
```

> -S 只激活预处理和编译，就是指把文件编译成为汇编代码。 生成 .s 的汇编代码

```c++
gcc -S hello.c 
```

> -E 只激活预处理,这个不生成文件, 你需要把它重定向到一个输出文件里面

```c++
gcc -E hello.c > pianoapan.txt 
gcc -E hello.c | more 
```

> -C 在预处理的时候, 不删除注释信息, 一般和-E使用, 有时候分析程序，用这个很方便的

> -M 生成文件关联的信息。包含目标文件所依赖的所有源代码你可以用 gcc -M hello.c 

> -O0 、-O1 、-O2 、-O3 编译器的优化选项的 4 个级别，-O0 表示没有优化, -O1 为默认值，-O3 优化级别最高。

> -g 只是编译器，在编译的时候，产生调试信息。


> -MM 和上面的那个一样，但是它将忽略由 #include<file> 造成的依赖关系。 

> -MD 和-M相同，但是输出将导入到.d的文件里面 


> -MMD 和 -MM 相同，但是输出将导入到 .d 的文件里面。

> -Wa,option 此选项传递 option 给汇编程序; 如果 option 中间有逗号, 就将 option 分成多个选项, 然 后传递给会汇编程序

> -Wl.option 此选项传递 option 给连接程序; 如果 option 中间有逗号, 就将 option 分成多个选项, 然 后传递给会连接程序

> -llibrary 制定编译的时候使用的库

> -Ldir 制定编译的时候，搜索库的路径。比如你自己的库，可以用它制定目录，不然编译器将只在标准库的目录找。这个dir就是目录的名称

> -gstabs 此选项以 stabs 格式声称调试信息, 但是不包括 gdb 调试信息

> -gstabs+ 此选项以 stabs 格式声称调试信息, 并且包含仅供 gdb 使用的额外调试信息

> -ggdb 此选项将尽可能的生成 gdb 的可以使用的调试信息

> -static 此选项将禁止使用动态库，所以，编译出来的东西，一般都很大，也不需要什么动态连接库，就可以运行

> -share 此选项将尽量使用动态库，所以生成文件比较小，但是需要系统由动态库。

> -include file 包含某个代码,简单来说,就是便以某个文件,需要另一个文件的时候,就可以用它设定,功能就相当于在代码中使用 #include<filename>。
```c++
gcc hello.c -include /root/pianopan.h 
```

> -traditional 试图让编译器支持传统的C语言特性。 GCC 是 GNU 的 C 和 C++ 编译器。实际上，GCC 能够编译三种语言：C、C++ 和 Object C（C 语言的一种面向对象扩展）。利用 gcc 命令可同时编译并连接 C 和 C++ 源程序。

        
> -Idir 在你是用 #include "file" 的时候, gcc/g++ 会先在当前目录查找你所制定的头文件, 如果没有找到, 他回到默认的头文件目录找,
> 如果使用 -I 制定了目录,他会先在你所制定的目录查找, 然后再按常规的顺序去找。
> 对于 #include<file>, gcc/g++ 会到 -I 制定的目录查找, 查找不到, 然后将到系统的默认的头文件目录查找 。

> -I- 就是取消前一个参数的功能, 所以一般在 -Idir 之后使用。

> -idirafter dir 在 -I 的目录里面查找失败, 将这个目录里面查找

> -iprefix prefix 、-iwithprefix dir 一般一起使用, 当 -I 的目录查找失败, 会到 prefix+dir 下查找

> -nostdinc 使编译器不再系统默认的头文件目录里面找头文件, 一般和 -I 联合使用,明确限定头文件的位置

> -nostdin C++ 规定不在 g++ 指定的标准路经中搜索, 但仍在其他路径中搜索, 此选项在创 libg++ 库使用 


> -imacros file 将 file 文件的宏, 扩展到 gcc/g++ 的输入文件, 宏定义本身并不出现在输入文件中


> -Dmacro 相当于 C 语言中的 #define macro;定义宏macro,宏的内容定义为字符串`1’.

> -Dmacro=defn 相当于 C 语言中的 #define macro=defn

> -Umacro 相当于 C 语言中的 #undef macro

> -undef 取消对任何非标准宏的定义

> -pipe 使用管道代替编译中临时文件, 在使用非 gnu 汇编工具的时候, 可能有些问题

```c++
gcc -pipe -o hello.exe hello.c 
```

> -ansi 关闭 gnu c中与 ansi c 不兼容的特性, 激活 ansi c 的专有特性（包括禁止一些 asm inline typeof 关键字, 以及 UNIX,vax 等预处理宏）

> -fno-asm 此选项实现 ansi 选项的功能的一部分，它禁止将 asm, inline 和 typeof 用作关键字

> -fno-strict-prototype 只对 g++ 起作用, 使用这个选项, g++ 将对不带参数的函数,都认为是没有显式的对参数的个数和类型说明,而不是没有参数。
>而 gcc 无论是否使用这个参数, 都将对没有带参数的函数, 认为城没有显式说明的类型

> -fthis-is-varialble 就是向传统 c++ 看齐, 可以使用 this 当一般变量使用

> -fcond-mismatch  允许条件表达式的第二和第三参数类型不匹配, 表达式的值将为 void 类型

> -funsigned-char 、-fno-signed-char、-fsigned-char 、-fno-unsigned-char
> 这四个参数是对 char 类型进行设置, 决定将 char 类型设置成 unsigned char(前两个参数)或者 signed char(后两个参数)



## 常用

- -ansi	只支持 ANSI 标准的 C 语法。这一选项将禁止 GNU C 的某些特色， 例如 asm 或 typeof 关键词。
- -c	只编译并生成目标文件。
- -DMACRO	以字符串"1"定义 MACRO 宏。
- -DMACRO=DEFN	以字符串"DEFN"定义 MACRO 宏。
- -E	只运行 C 预编译器。
- -g	生成调试信息。GNU 调试器可利用该信息。
- -IDIRECTORY	指定额外的头文件搜索路径DIRECTORY。
- -LDIRECTORY	指定额外的函数库搜索路径DIRECTORY。
- -lLIBRARY	连接时搜索指定的函数库LIBRARY。
- -m486	针对 486 进行代码优化。
- -o FILE	生成指定的输出文件。用在生成可执行文件时。
- -O0	不进行优化处理。
- -O 或 -O1	优化生成代码。
- -O2	进一步优化。
- -O3	比 -O2 更进一步优化，包括 inline 函数。
- -shared	生成共享目标文件。通常用在建立共享库时。
- -static	禁止使用共享连接。
- -UMACRO	取消对 MACRO 宏的定义。
- -w	不生成任何警告信息。
- -Wall	生成所有警告信息。

 
# Makefile

[Makefile 详解](https://seisman.github.io/how-to-write-makefile/overview.html)

# Test

```bash
# equiTruss
./main.o /data/code/graduate_neo4j/unmanaged/src/main/resources/equitruss/dblp_datasets/Graph/dblp20%_vertices.txt \
 /data/code/graduate_neo4j/unmanaged/src/main/resources/equitruss/dblp_datasets/Attribute/dblp_attributes_int.txt \
 /data/code/graduate_neo4j/unmanaged/src/main/resources/equitruss/dblp_datasets/Queries/vertices_rand300_6truss_dblp.txt \
 ./result/result.txt \
 ./result/detail.txt \
 ./result/equitruss.txt \
 ./result/equitruss_index_ \

# minim
./main.o ./dblp_datasets/minim/graph.txt \
./dblp_datasets/minim/attr.txt \
./dblp_datasets/minim/query.txt \
 ./dblp_datasets/minim/result
```


