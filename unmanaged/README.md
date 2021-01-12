# 前置

```bash 
# clone 技巧
git clone --depth 1  https://github.com/neo4j/neo4j-documentation.git

# 账号密码
git config --global credential.helper store
```



# 配置扩展目录

```bash
#Comma separated list of JAXRS packages containing JAXRS Resource, one package name for each mountpoint.
# http://whu1.chendaye666.top:7474/unmanaged/test/helloworld/123 
# 扩展类的包名=路由地址
dbms.unmanaged_extension_classes=top.chendaye666.server.server.unmanaged=/unmanaged/test
```


# Louvain 算法的实现

## 原理

最大化整个图结构的模块度(Modularity)

## 概念

> 模块度Modularity: 

```
这个定义是描述社区内紧密程度的值Q
```

> 模块度增量delta Q: 
```
把一个孤立的点放入一个社区C后，计算Modularity的变化，其中计算过程的要点是，
首先计算1个点的Modularity，和社区C的Modularity，再计算合并后新社区的Modularity，
新社区的Modularity减去前两个Modularity就是delta Q

delta Q = 孤立点加入社区后的模块度 - (孤立点的模块度 + 原先社区的模块度)



```

## 计算过程

> 每个点作为一个community，然后考虑每个community的邻居节点，合并到community，然后看delta Q；找到最大的正delta Q，
> 合并点到community；多进行几轮，至不再变动，那么结束；
> 其中存在的问题是，不同的节点访问顺序将导致不同的结果，试验中发现这个顺序对结果影响不大，但是会在一定程度上影响计算时间。


> 将新的community作为点，重复上述过程。那么如何确定新的点之前的权重呢？
> 答案是将两个community之间相邻的点之间的权重和作为两个community退化成一个点后的新的权重

