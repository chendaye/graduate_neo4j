# Neo4j 数据处理

## Cypher 文档

[Cypher 官方文档](https://neo4j.com/docs/cypher-manual/4.1/administration/indexes-for-search-performance/#administration-indexes-search-performance)

[Cypher 全文索引](https://neo4j.com/docs/cypher-manual/4.1/administration/indexes-for-full-text-search/#administration-indexes-fulltext-search-query)


[cypher 数据导入](https://neo4j.com/developer/guide-import-csv/)

[cypher 语法](https://neo4j.com/docs/cypher-manual/4.1/syntax/#query-syntax)

## neo4j-admin
```html

# 导入数据

# neo4j-admin
./neo4j-admin import --database neo4j --nodes=../import/providers.csv --relationships=SHARE_MEMBER=../import/shared_members.csv

```


## Load CSV

```html

# 删除数据
MATCH (n) DETACH DELETE n;

# shell 处理csv
sed -i '1i physicianId' ./providers.csv
sed -i '1i startId,endId,transactions,patients,max_day' ./shared_members.csv

# 建立Author节点
LOAD CSV WITH HEADERS  FROM 'file:///node_1.csv' AS row CREATE (:Author {authorId:row.author_id,name:row.node,articles:row.articles,words:row.words,attribute:row.attribute});

# 建立Paper节点
LOAD CSV WITH HEADERS  FROM 'file:///node_0.csv' AS row CREATE (:Paper {paperId:row.paper_id,title:row.title,journal:row.journal,year:row.year,ee:row.ee,mdate:row.mdate,key:row.key,publtype:row.publtype,reviewid:row.reviewid,rating:row.rating});



# 建索引(很重要) B-Tree，节点索引
CREATE INDEX index_authorId FOR (n:Author) ON (n.authorId);
CREATE INDEX index_paperId FOR (n:Paper) ON (n.paperId);


# 建立边 Article
LOAD CSV WITH HEADERS FROM 'file:///relationship_0.csv' AS row MATCH (e:Author {authorId: row.start}),(c:Author {authorId: row.end}) MERGE  (e)-[:Article { weight:toInteger(row.weight)}]-(c);

# 建立边 AuthorPaper
LOAD CSV WITH HEADERS FROM 'file:///relationship_10.csv' AS row MATCH (e:Author {authorId: row.author_id}),(c:Paper {paperId: row.paper_id}) MERGE  (e)-[:AuthorPaper]-(c);

# 去除自环
match (n:Author)-[r:Article]-(m:Author) where n.authorId=m.authorId delete r;

```

## 建全文索引

[参考](https://neo4j.com/docs/cypher-manual/4.1/administration/indexes-for-full-text-search/#administration-indexes-fulltext-search-query)

> A full-text index allows you to write queries that match within the contents of indexed string properties. 
>For instance, the btree indexes described in previous sections can only do exact matching or prefix matches on strings.

> 全文索引可以匹配 查询字符串里面的内容； btree索引只能精确匹配和前缀匹配

```bash
# 在 Author节点的 name上建立全文索引
CALL db.index.fulltext.createNodeIndex("AuthorName", ["Author"], ["name"])
CALL db.index.fulltext.createNodeIndex("PaperTitle", ["Paper"], ["title"])

# 利用全文索引（模糊）搜索
CALL db.index.fulltext.queryNodes("AuthorName", "Jean-Marc Cadiou") YIELD node, score
RETURN node.id,node.authorId,node.name, score

# 利用全文索引（精确）搜索；在搜索字符串上加上单引号（ Apache Lucene 语法）
CALL db.index.fulltext.queryNodes("AuthorName",'"Jean-Marc Cadiou"') YIELD node, score
RETURN node.id,node.authorId,node.name, score

CALL db.index.fulltext.queryNodes("AuthorName",'"Jean-Marc Cadiou"') YIELD node, score
RETURN node

# 删除全文索引
CALL db.index.fulltext.drop("AuthorName")

# 查看索引
CALL db.indexes
```

## B-Tree索引

[B-Tree索引](https://neo4j.com/docs/cypher-manual/4.1/administration/indexes-for-search-performance/)
```bash
CREATE INDEX index_name IF NOT EXISTS FOR (n:Author) ON (n.authorId)
CREATE INDEX index_name IF NOT EXISTS FOR (n:Author) ON (n.name)

CREATE INDEX index_name IF NOT EXISTS FOR (n:Paper) ON (n.paperId)
CREATE INDEX index_name IF NOT EXISTS FOR (n:Paper) ON (n.title)
```

## shell数据预处理


```bash
# 去除重复的节点
awk -F ',' '{print $1}' physician-shared-patient-patterns-2015-days30.txt > nodes.txt

awk -F ',' '{print $2}' physician-shared-patient-patterns-2015-days30.txt >> nodes.txt
 
perl -ne 'print unless $seen{$_}++' nodes.txt > providers.csv

# 添加shcame: npi:ID(Provider)
sed -i -e '1i\
            npi:ID(Provider)
            ' providers.csv

# 去掉多余空格
sed -e "s/ //g" physician-shared-patient-patterns-2015-days30.txt > shared_members.csv


# 添加第一行
sed -i '1i Id' ./providers.csv

# 删除第二行
sed -i '2d' providers.csv

# or
vim
:2d

```



match res=(u:Author {name:'Mark F. Hornick'})-[:AuthorPaper]->(p:Paper) 
where id(p) = 3768185 or id(p)=3697993 or id(p)=4425960 or id(p)=3009778 
or id(p)=2881361 or id(p)=2881368 or id(p)=4031609 or id(p)=3009329 or id(p)=4569647 
or id(p)=5484490 or id(p)=3023305 or id(p)=4031828 return res



match (p:Paper) where id(p) = 3768185  return p
union
match (p:Paper) where id(p)=3697993  return p
union
match (p:Paper) where id(p)=4425960  return p
union
match (p:Paper) where (p)=3009778  return p
union
match (p:Paper) where id(p)=2881361 return p
union
match (p:Paper) where id(p)=2881368 return p
union
match (p:Paper) where id(p)=4031609  return p
union
match (p:Paper) where id(p)=3009329  return p
union
match (p:Paper) where id(p)=4569647  return p
union
match (p:Paper) where id(p)=5484490  return p
union
match (p:Paper) where id(p)=3023305  return p
union
match (p:Paper) where id(p)=4031828 return p