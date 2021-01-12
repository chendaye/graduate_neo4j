> 稍微大一点的项目里面，一般都会包含多个文件。尤其是包含多个头文件，各自头文件的实现，和包含main函数的文件。这样的好处就是更容易开发和维护。


[参考](https://www.jianshu.com/p/e5c6a255076b)



```bash
# 在当前目录直接编译的话,会找不到头文件
g++ main.cpp -o main

# 通过 -I 选项 链接上去
g++ main.cpp -o main -I ../myinclude/

# 头文件虽然找到了，但是没有提示错误，没有定义sum函数。sum函数是在myhead.cpp文件上面定义的。
# 也就是需要把myhead.cpp文件编译一下
g++ -c myhead.cpp -o myhead.o


# 最后 
 g++ main.cpp -o main -I ../myinclude/    ../myinclude/myhead.o
```