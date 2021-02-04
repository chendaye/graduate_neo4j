#  Armadillo 下载
[Armadillo](http://arma.sourceforge.net/download.html)

# 解压

```bash
# 分两步
xz -d ...
tar -xvf ...
```

# 安装 cmake

> 允许开发者编写一种平台无关的 CMakeList.txt 文件来定制整个编译流程，
> 然后再根据目标用户的平台进一步生成所需的本地化 Makefile 和工程文件

```bash
# ubuntu
apt install cmake

# mac
#...

# 生成本地化的 Makefile 文件
cmake CMakeList.txt
```

# 编译安装 Armadillo

```bash
make

make all
```