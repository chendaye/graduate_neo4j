# 文档

## Neo4j 文档

[官方文档](https://neo4j.com/docs/)

[developer doc](https://neo4j.com/developer/get-started/)

## spring-data-neo4j-nx

[项目原始参考地址](https://github.com/neo4j/sdn-rx/tree/master/examples/rest)

[spring-data-neo4j 参考文档](https://docs.spring.io/spring-data/neo4j/docs/current/reference/html/#)

[Spring Data Neo4j⚡️RX doc](https://neo4j.github.io/sdn-rx/current/#introduction)



[Spring Data REST Reference Guide](https://docs.spring.io/spring-data/rest/docs/current/reference/html/#reference)



# 建模

## 建模节点实体

> 一个节点实体就是一个 Java 类； 代表一个特定的域实体； 对应图中的一个节点。比如：Move People

## 建模关系实体

> 关系实体是指节点和实体之间的关系

## 建模节点实体之间的关系


# 官方图示例

## Node

> Movie 电影

- id
- released :发行时间
- tagline :标语
- title :名称

> Person 人

- id
- born :出生日期
- name :姓名

## Relationship

> ACTED_IN 参演  person -> movie

- id
- roles :边代表的电影角色

> DIRECTED 导演  person -> movie

- id

> FOLLOWS 关注 person -> person

- id

> PRODUCED 作者 person -> movie

- id

> REVIEWED  影评 person -> movie

- id
- rating :评级
- summary :概要

> WROTE 编剧 person -> movie

- id

# Cypher

**Cypher 就是 图的遍历**

## start

> 查找图形中的起始节点

- 从单个节点开始查询 
```cyper
start user=node(1) # 从节点 id=1 开始查询
    return user;
```

- 从多个节点开始查询
```cypher
start user=node(1,3)
match user-[:HAS_SEEN]->movie
return distinct movie;  # 返回电影（不重复）
```

- 使用全文索引 Lucene 匹配起始节点
- 全文索引 Lucene 单独学
```cypher
start john=node:users(name = "John")
return join;

# 使用本地 Lucene 查询索引查找节点
# 这种方式可以使用 强大的 Lucene 查询结构，比如：通配符、多属性匹配
start john=node:users(name:"John")
return join;
```

- 基于模式的索引查找节点
- 指定使用模式索引的标签被定义为节点标识符的一个部分，用冒号分隔
- 必须在圆括号内翻译官与标识符相关的标签
- 查询索引被指定为 where 语句的一部分
- 基于标签的索引只能用于完全属性查找
- 对于全文索引必须使用手动方法创建索引
```cypher
match (john:USER)
where john.name='John'
return john;
```

- 多个起始节点
```cypher
# 返回 john Jack 都看过的电影
# 两个模式中都由 movie 节点，有相同的名字，因此代表同一个节点
start john=node:users(name:"John"),
      jack=node:users(name:"jack)
match john-[:HAS_SEEN]->movie,jack-[:HAS_SEEN]->movie
return movie;
```


## match

> 匹配图形模式

- ：后面指定关系类型
- -[:HAS_SEEN]-> HAS_SEEN 从左导到右方向
- user 看过 movie
```cypher
start user=node(1)
match (user)-[:HAS_SEEN]->(movie)
return movie;
```

- 如果不清楚关系的方向 可以 不要方向箭头
```cypher
start user=node(1)
match (user)-[:IS_FRIEND_OF]-(user)
return movie;
```

- 节点和关系可以命名，可以在之后引用
```cypher
start user=node(1)
match (user)-[r:IS_FRIEND_OF]-(user)
return movie;
```

- 节点也可以匿名
- 从 user 节点返回所有 HAS_SEEN 关系 而不关心 movie 节点
- 节点可以不要圆括号； 匿名节点必须要
```cypher
start user=node(1)
match (user)-[:HAS_SEEN]->()
return movie;
```

- 复杂匹配
```cypher
# john 的朋友看过的电影
start john=node(1)
match john-[:IS_FRIEND_OF]->()-[:HAS_SEEN]->(movie)
return movie;

# john看过 & john 的朋友看过的电影
# 用逗号分隔 就是 并(and)
start john=node(1)
match 
john-[:IS_FRIEND_OF]->()-[:HAS_SEEN]->(movie),
john-[:HAS_SEEN]->(movie)
return movie;

# john没看过 & john 的朋友看过的电影
# 第二个模式 是where的一部分； 过滤第一个模式返回的结果
start john=node(1)
match john-[:IS_FRIEND_OF]->()-[:HAS_SEEN]->(movie)
where NOT john-[:HAS_SEEN]->(movie)
return movie;
```

## where

> 过滤数据

- john 1992年之后出生的朋友
```cypher
start john=node:users("name:John")
match john-[:IS_FRIEND_OF]->(friend)
where friend.yearofBirth > 1992
return friend;
```

- 正则表达式 =~
- 正则表达式放在 /.../ 之间；遵循java 正则表达式语法
```cypher
start john=node:users("name:John")
match john-[:IS_FRIEND_OF]->(friend)
where friend.email =~ /*.gmail.com/
return friend;
```

- 因为存储的是半结构化的数据。不是每个节点一定有某个属性
```cypher
start john=node:users("name:John")
match john-[:IS_FRIEND_OF]->(friend)
where has(friend.email)  # 有邮箱的朋友
return friend;
```

## return

> 返回感兴趣的结果， 包括：节点、关系、节点(关系)属性

- 只返属性
```cypher
start john=node:users("name:John")
match john-[:IS_FRIEND_OF]->(friend)
where has(friend.email)  # 有邮箱的朋友
return friend.name;
```

- 返回关系
```cypher
start john=node:users("name:John")
match john-[r:HAS_SEEN]->(movie)
return r;

# 可以只返回关系的属性
start john=node:users("name:John")
match john-[r:HAS_SEEN]->(movie)
return r.start; 
```

- 返回路径 path
- 返回引用的路径，和相关电影名字
```cypher
start john=node(1)
match 
moviePath = john-[:IS_FRIEND_OF]->()-[:HAS_SEEN]->(movie)
where NOT john-[:HAS_SEEN]->(movie)
return movie.name,moviePath;
```


- 分页
- order：分页之前对结果排序
- skip: 划分结果集，跳转指定页
- linit: 限制返回数量
```cypher
start john=node:users("name:John")
match john-[r:HAS_SEEN]->(movie)
return movie
order by movie.name # 结果排序
skip 20 # 跳过前面2页； 10*2=20
limit 10 # 每页返回 10 个
```
## create

- 创建节点
```cypher
create newuser
{
    name:'Greace',
    yearOfBirth: 2000,
    email: 'top.chendaye666@gmail.com'
}
return newuser;
```

- 创建关系
- 使Grace 成为john 朋友
```cypher
start john = node:users(name = "John"), grace = node(10)
create john-[r:IS_FRIEND_OF]->grace
return r;
```

## 事务

> 执行cypher时，每一个查询都包含于一个事务中； 所有事务都由 Neo4j的殷勤负责；查询成功自动提交，失败，自动回滚

> 可以在一个事务中执行多个查询，并且在事务的最后提交或者回滚每一个查询

- 一次多个查询
- 同时新建一个节点和一个关系
```cypher
# 创建了John的朋友Grace Spencer两次，将得到两个Grace Spencer节点和两个从John节点的IS_FRIEND_OF关系。
# 这可能不是想要的结果。要只创建不存在的图形实体，应该使用创建唯一的（create unique）Cypher命令
start john = node:user(name = "John")
create john
        -[r:IS_FRIEND_OF]->
        (grace {
        name: 'Grace',
        yearOfBirth: 2000,
        email: 'top.chendaye666@gmail.com'
})
return r,grace; 

# 对前面例子的唯一改变是使用create unique命令。多次运行这段程序将不会产生实体数的乘积——将总是有一个Grace Spencer节点
# 和一个在John和Grace节点之间的IS_FRIEND_OF关系
start john = node:user(name = "John")
create unique john
        -[r:IS_FRIEND_OF]->
        (grace {
        name: 'Grace',
        yearOfBirth: 2000,
        email: 'top.chendaye666@gmail.com'
})
return r,grace;
```

## 删除

```cypher
# 只能删除没有任何关系的节点
start grace = node(10)
delete grace;

# 要删除一个节点，也要删除它的关系
start grace = node(10)
match grace-[r]-()
delete grace,r;

# 删除属性
start n=node(1)
delete n.group;
```

## 更新

- 更新单个节点
- 先选中节点，再用set更新
- 如果属性不存在将创建属性
- 不允许有空属性值
```cypher
start john=node:users(name = "john")
set john.yearOfBirth = 1973

# 多个属性
start user=node(1,2)
set user.group = 'ADMINT'
```

## 聚合

- count
```cypher
start user=node(*)
match user-[:IS_FRIEND_OF]-()
return user,count(*)
order by count(*) desc;
```

- avg
```cypher
start john=node:users(name = "John")
match john-[:IS_FRIEND_OF]-(friend)
where HAS(friend.teayOfBirth)
return ave(2020-friend.yearOfBirth);
```


## 函数

- TYPE函数: 返回每一个关系的类型与数量
```cypher
start n=node:users(name='John')
match n-[rel]-()
return TYPE(rel),count(*);
```

- 迭代查询函数
- ·HAS（graphEntity.propertyName）——如果一个节点或关系具有给定名字的属性存在，则返回true。
- ·NODES（path）——把一个路径转换成一个可迭代的节点集。
- ·ALL（x in collection where predicate（x））——如果collection中的每一个单个元素匹配了给定的predicate，则返回true。
- Neo4j支持许多具有相似目的的函数。例如，与NODES（path）函数类似、RELATIO　NSHIPS（path）函数返回在一个给定路径上的所有节点集。
- ·NONE（x in collection where predicate（x））——如果提供的集合中没有元素匹配谓词表述，返回true；否则，返回false。
- ·ANY（x in collection where predicate（x））——如果至少有一个元素匹配谓词表述，返回true；如果没有匹配的，返回false。
- ·SINGLE（x in collection where predicate（x））——如果正好有一个元素匹配谓词表述，返回true；如果没有或多于一个匹配的，这个函数返回false。
```cyper
start john=node:users(name = "john"),
      kate=node:users(name = "kate")
match p = john-[:IS_FRIEND_OF*1..3]-(kate)  # 递归3级，找朋友, p是路径
where ALL(   # 路径上的人 都要有facebookI
    user in NODES(p)
    where HAS(User.facebookId)
)
return p;
```


## with 管道

> 将一个插叙输出连接到另一个查询中

- 类似于sql 中 的 HAVING 过滤聚合函数的作用
```cypher
start n=node(1)
match n-[rel]-()
with TYPE(rel) as type, count(*) as count # 不返回聚合计数；使用管道输送计数
where count > 1
return type,count;
```

## cypher 兼容性

> cypher 变化非常快，有新旧查询语法兼容问题

- 指定cypher版本
```cypher
CYPHER 1.8 start n=node(1)
match n-[rel]-()
with TYPE(rel) as type, count(*) as count # 不返回聚合计数；使用管道输送计数
where count > 1
return type,count;
```