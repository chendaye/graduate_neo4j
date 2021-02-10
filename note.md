# 内科医生共享病例社群分析

## 数据导入

[cypher 数据导入](https://neo4j.com/developer/guide-import-csv/)

[cypher 语法](https://neo4j.com/docs/cypher-manual/4.2/clauses/create/#create-create-a-relationship-between-two-nodes)

### neo4j-admin
```html

# 导入数据

# neo4j-admin
./neo4j-admin import --database neo4j --nodes=../import/providers.csv --relationships=SHARE_MEMBER=../import/shared_members.csv

```


### Load CSV

> 方式一

```html
# cypher-shell
LOAD CSV FROM 'file:///providers.csv' AS line CREATE (:Provider {id: toInteger(line[0])});

```

> 方式二


```
#companies.csv
Id,Name,Location,Email,BusinessType
1,Neo4j,San Mateo,contact@neo4j.com,P
2,AAA,,info@aaa.com,
3,BBB,Chicago,,G

# people.csv 
employeeId,Name,Company
1,Bob Smith,1
2,Joe Jones,3
3,Susan Scott,2
4,Karen White,1


# clear data
MATCH (n)
DETACH DELETE n;


# load Employee nodes
LOAD CSV WITH HEADERS FROM 'file:///people.csv' AS row
MERGE (e:Employee {employeeId: row.employeeId, name: row.Name})
RETURN count(e);

# load Company nodes
LOAD CSV WITH HEADERS FROM 'file:///companies.csv' AS row
WITH row WHERE row.Id IS NOT NULL
WITH row,
(CASE row.BusinessType
 WHEN 'P' THEN 'Public'
 WHEN 'R' THEN 'Private'
 WHEN 'G' THEN 'Government'
 ELSE 'Other' END) AS type
MERGE (c:Company {companyId: row.Id, hqLocation: coalesce(row.Location, "Unknown")})
SET c.emailAddress = CASE trim(row.Email) WHEN "" THEN null ELSE row.Email END
SET c.businessType = type
RETURN count(c);


# create relationships
LOAD CSV WITH HEADERS FROM 'file:///people.csv' AS row
MATCH (e:Employee {employeeId: row.employeeId})
MATCH (c:Company {companyId: row.Company})
MERGE (e)-[:WORKS_FOR]->(c)
RETURN *;
```

```html
sed -i '1i physicianId' ./providers.csv

sed -i '1i startId,endId,transactions,patients,max_day' ./shared_members.csv

# 建立节点
LOAD CSV WITH HEADERS  FROM 'file:///node_1.csv' AS row CREATE (:Author {authorId:row.author_id,name:row.node,articles:row.articles,words:row.words,attribute:row.attribute});
LOAD CSV WITH HEADERS  FROM 'file:///node_2.csv' AS row CREATE (:Author {authorId:row.author_id,name:row.node,articles:row.articles,words:row.words,attribute:row.attribute});
LOAD CSV WITH HEADERS  FROM 'file:///node_3.csv' AS row CREATE (:Author {authorId:row.author_id,name:row.node,articles:row.articles,words:row.words,attribute:row.attribute});
LOAD CSV WITH HEADERS  FROM 'file:///node_4.csv' AS row CREATE (:Author {authorId:row.author_id,name:row.node,articles:row.articles,words:row.words,attribute:row.attribute});
LOAD CSV WITH HEADERS  FROM 'file:///node_5.csv' AS row CREATE (:Author {authorId:row.author_id,name:row.node,articles:row.articles,words:row.words,attribute:row.attribute});
LOAD CSV WITH HEADERS  FROM 'file:///node_6.csv' AS row CREATE (:Author {authorId:row.author_id,name:row.node,articles:row.articles,words:row.words,attribute:row.attribute});
LOAD CSV WITH HEADERS  FROM 'file:///node_7.csv' AS row CREATE (:Author {authorId:row.author_id,name:row.node,articles:row.articles,words:row.words,attribute:row.attribute});


LOAD CSV WITH HEADERS  FROM 'file:///node_0.csv' AS row CREATE (:Paper {paperId:row.paper_id,title:row.title,journal:row.journal,year:row.year,ee:row.ee,mdate:row.mdate,key:row.key,publtype:row.publtype,reviewid:row.reviewid,rating:row.rating});
LOAD CSV WITH HEADERS  FROM 'file:///node_2.csv' AS row CREATE (:Paper {paperId:row.paper_id,title:row.title,journal:row.journal,year:row.year,ee:row.ee,mdate:row.mdate,key:row.key,publtype:row.publtype,reviewid:row.reviewid,rating:row.rating});
LOAD CSV WITH HEADERS  FROM 'file:///node_3.csv' AS row CREATE (:Paper {paperId:row.paper_id,title:row.title,journal:row.journal,year:row.year,ee:row.ee,mdate:row.mdate,key:row.key,publtype:row.publtype,reviewid:row.reviewid,rating:row.rating});
LOAD CSV WITH HEADERS  FROM 'file:///node_4.csv' AS row CREATE (:Paper {paperId:row.paper_id,title:row.title,journal:row.journal,year:row.year,ee:row.ee,mdate:row.mdate,key:row.key,publtype:row.publtype,reviewid:row.reviewid,rating:row.rating});
LOAD CSV WITH HEADERS  FROM 'file:///node_5.csv' AS row CREATE (:Paper {paperId:row.paper_id,title:row.title,journal:row.journal,year:row.year,ee:row.ee,mdate:row.mdate,key:row.key,publtype:row.publtype,reviewid:row.reviewid,rating:row.rating});
LOAD CSV WITH HEADERS  FROM 'file:///node_6.csv' AS row CREATE (:Paper {paperId:row.paper_id,title:row.title,journal:row.journal,year:row.year,ee:row.ee,mdate:row.mdate,key:row.key,publtype:row.publtype,reviewid:row.reviewid,rating:row.rating});
LOAD CSV WITH HEADERS  FROM 'file:///node_7.csv' AS row CREATE (:Paper {paperId:row.paper_id,title:row.title,journal:row.journal,year:row.year,ee:row.ee,mdate:row.mdate,key:row.key,publtype:row.publtype,reviewid:row.reviewid,rating:row.rating});
LOAD CSV WITH HEADERS  FROM 'file:///node_8.csv' AS row CREATE (:Paper {paperId:row.paper_id,title:row.title,journal:row.journal,year:row.year,ee:row.ee,mdate:row.mdate,key:row.key,publtype:row.publtype,reviewid:row.reviewid,rating:row.rating});
LOAD CSV WITH HEADERS  FROM 'file:///node_9.csv' AS row CREATE (:Paper {paperId:row.paper_id,title:row.title,journal:row.journal,year:row.year,ee:row.ee,mdate:row.mdate,key:row.key,publtype:row.publtype,reviewid:row.reviewid,rating:row.rating});
LOAD CSV WITH HEADERS  FROM 'file:///node_10.csv' AS row CREATE (:Paper {paperId:row.paper_id,title:row.title,journal:row.journal,year:row.year,ee:row.ee,mdate:row.mdate,key:row.key,publtype:row.publtype,reviewid:row.reviewid,rating:row.rating});
LOAD CSV WITH HEADERS  FROM 'file:///node_11.csv' AS row CREATE (:Paper {paperId:row.paper_id,title:row.title,journal:row.journal,year:row.year,ee:row.ee,mdate:row.mdate,key:row.key,publtype:row.publtype,reviewid:row.reviewid,rating:row.rating});
LOAD CSV WITH HEADERS  FROM 'file:///node_12.csv' AS row CREATE (:Paper {paperId:row.paper_id,title:row.title,journal:row.journal,year:row.year,ee:row.ee,mdate:row.mdate,key:row.key,publtype:row.publtype,reviewid:row.reviewid,rating:row.rating});
LOAD CSV WITH HEADERS  FROM 'file:///node_13.csv' AS row CREATE (:Paper {paperId:row.paper_id,title:row.title,journal:row.journal,year:row.year,ee:row.ee,mdate:row.mdate,key:row.key,publtype:row.publtype,reviewid:row.reviewid,rating:row.rating});
LOAD CSV WITH HEADERS  FROM 'file:///node_14.csv' AS row CREATE (:Paper {paperId:row.paper_id,title:row.title,journal:row.journal,year:row.year,ee:row.ee,mdate:row.mdate,key:row.key,publtype:row.publtype,reviewid:row.reviewid,rating:row.rating});



# 建索引(很重要)
CREATE INDEX index_authorId FOR (n:Author) ON (n.authorId);
CREATE INDEX index_paperId FOR (n:Paper) ON (n.paperId);



# 建立边
LOAD CSV WITH HEADERS FROM 'file:///relationship_0.csv' AS row 
MATCH (e:Author {authorId: row.start}) MATCH (c:Author {authorId: row.end}) 
MERGE  (e)-[:Article { weight:toInteger(row.weight)}]-(c);

LOAD CSV WITH HEADERS FROM 'file:///relationship_0.csv' AS row MATCH (e:Author {authorId: row.start}),(c:Author {authorId: row.end}) MERGE  (e)-[:Article { weight:toInteger(row.weight)}]-(c);
LOAD CSV WITH HEADERS FROM 'file:///relationship_1.csv' AS row MATCH (e:Author {authorId: row.start}),(c:Author {authorId: row.end}) MERGE  (e)-[:Article { weight:toInteger(row.weight)}]-(c);
LOAD CSV WITH HEADERS FROM 'file:///relationship_2.csv' AS row MATCH (e:Author {authorId: row.start}),(c:Author {authorId: row.end}) MERGE  (e)-[:Article { weight:toInteger(row.weight)}]-(c);
LOAD CSV WITH HEADERS FROM 'file:///relationship_3.csv' AS row MATCH (e:Author {authorId: row.start}),(c:Author {authorId: row.end}) MERGE  (e)-[:Article { weight:toInteger(row.weight)}]-(c);
LOAD CSV WITH HEADERS FROM 'file:///relationship_4.csv' AS row MATCH (e:Author {authorId: row.start}),(c:Author {authorId: row.end}) MERGE  (e)-[:Article { weight:toInteger(row.weight)}]-(c);
LOAD CSV WITH HEADERS FROM 'file:///relationship_5.csv' AS row MATCH (e:Author {authorId: row.start}),(c:Author {authorId: row.end}) MERGE  (e)-[:Article { weight:toInteger(row.weight)}]-(c);
LOAD CSV WITH HEADERS FROM 'file:///relationship_6.csv' AS row MATCH (e:Author {authorId: row.start}),(c:Author {authorId: row.end}) MERGE  (e)-[:Article { weight:toInteger(row.weight)}]-(c);
LOAD CSV WITH HEADERS FROM 'file:///relationship_7.csv' AS row MATCH (e:Author {authorId: row.start}),(c:Author {authorId: row.end}) MERGE  (e)-[:Article { weight:toInteger(row.weight)}]-(c);
LOAD CSV WITH HEADERS FROM 'file:///relationship_8.csv' AS row MATCH (e:Author {authorId: row.start}),(c:Author {authorId: row.end}) MERGE  (e)-[:Article { weight:toInteger(row.weight)}]-(c);
LOAD CSV WITH HEADERS FROM 'file:///relationship_9.csv' AS row MATCH (e:Author {authorId: row.start}),(c:Author {authorId: row.end}) MERGE  (e)-[:Article { weight:toInteger(row.weight)}]-(c);


LOAD CSV WITH HEADERS FROM 'file:///relationship_10.csv' AS row MATCH (e:Author {authorId: row.author_id}),(c:Paper {paperId: row.paper_id}) MERGE  (e)-[:AuthorPaper]-(c);
LOAD CSV WITH HEADERS FROM 'file:///relationship_11.csv' AS row MATCH (e:Author {authorId: row.author_id}),(c:Paper {paperId: row.paper_id}) MERGE  (e)-[:AuthorPaper]-(c);
LOAD CSV WITH HEADERS FROM 'file:///relationship_12.csv' AS row MATCH (e:Author {authorId: row.author_id}),(c:Paper {paperId: row.paper_id}) MERGE  (e)-[:AuthorPaper]-(c);
LOAD CSV WITH HEADERS FROM 'file:///relationship_13.csv' AS row MATCH (e:Author {authorId: row.author_id}),(c:Paper {paperId: row.paper_id}) MERGE  (e)-[:AuthorPaper]-(c);
LOAD CSV WITH HEADERS FROM 'file:///relationship_14.csv' AS row MATCH (e:Author {authorId: row.author_id}),(c:Paper {paperId: row.paper_id}) MERGE  (e)-[:AuthorPaper]-(c);
LOAD CSV WITH HEADERS FROM 'file:///relationship_15.csv' AS row MATCH (e:Author {authorId: row.author_id}),(c:Paper {paperId: row.paper_id}) MERGE  (e)-[:AuthorPaper]-(c);
LOAD CSV WITH HEADERS FROM 'file:///relationship_16.csv' AS row MATCH (e:Author {authorId: row.author_id}),(c:Paper {paperId: row.paper_id}) MERGE  (e)-[:AuthorPaper]-(c);
LOAD CSV WITH HEADERS FROM 'file:///relationship_17.csv' AS row MATCH (e:Author {authorId: row.author_id}),(c:Paper {paperId: row.paper_id}) MERGE  (e)-[:AuthorPaper]-(c);
LOAD CSV WITH HEADERS FROM 'file:///relationship_18.csv' AS row MATCH (e:Author {authorId: row.author_id}),(c:Paper {paperId: row.paper_id}) MERGE  (e)-[:AuthorPaper]-(c);
LOAD CSV WITH HEADERS FROM 'file:///relationship_19.csv' AS row MATCH (e:Author {authorId: row.author_id}),(c:Paper {paperId: row.paper_id}) MERGE  (e)-[:AuthorPaper]-(c);




# 去除自环
match (n:Author)-[r:Article]-(m:Author) where n.authorId=m.authorId delete r;







MATCH (e:Author {authorId: "3038149328"}),(c:Author {authorId: "2307802211"}) MERGE  (e)-[:ARTICLE { weight:7}]->(c);
MATCH (n {authorId: "2307802211"})-[r:ARTICLE]-() DELETE r;




# 建立一个关系
MATCH (e:Physician {physicianId: "1003001017"}) MATCH (c:Physician {physicianId: "1003001108"}) MERGE  (e)-[:SHARE_MEMBER { transactions:12, patients:8, max_day:2}]->(c);

MATCH (e:Author {authorId: "2668963400"}) MATCH (c:Author {authorId: "2406253400"}) 
CREATE  (e)-[:Article { weight:12}]->(c);

# 添加第一行
sed -i '1i Id' ./providers.csv

# 删除第二行
sed -i '2d' providers.csv

# or
vim
:2d
```



## 数据预处理

[原始数据]( https://downloads.cms.gov/foia/physician-shared-patient-patterns-2015-days30.zip )

```html
# 去除重复的节点
awk -F ',' '{print $1}' physician-shared-patient-patterns-2015-days30.txt > nodes.txt

awk -F ',' '{print $2}' physician-shared-patient-patterns-2015-days30.txt >> nodes.txt
 
perl -ne 'print unless $seen{$_}++' nodes.txt > providers.csv

# 添加shcame: npi:ID(Provider)
sed -i -e '1i\
            npi:ID(Provider)
            ' providers.csv

# 添加关系文件 shcame： :START_ID(Provider),:END_ID(Provider),transactions:int,patients:int,max_day:int
sed -i -e '1i\
    :START_ID(Provider),:END_ID(Provider),transactions:int,patients:int,max_day:int
    ' physician-shared-patient-patterns-2015-days30.txt

# 去掉多余空格
sed -e "s/ //g" physician-shared-patient-patterns-2015-days30.txt > shared_members.csv





# 原始数据
 :START_ID(Provider),:END_ID(Provider),transactions:int,patients:int,max_day:int
1000026017,1598773715,56        ,23        ,29
1000310429,1144645573,68        ,12        ,36
1003000126,1003951625,147       ,53        ,28
1003000126,1003975400,38        ,18        ,0
1003000126,1013051119,30        ,15        ,0


# provider
npi:ID(Provider)
1000026017
1000310429
1003000126
1003000134
1003000142

# shared_members
:START_ID(Provider),:END_ID(Provider),transactions:int,patients:int,max_day:int
1000026017,1598773715,56,23,29
1000310429,1144645573,68,12,36
1003000126,1003951625,147,53,28
1003000126,1003975400,38,18,0
1003000126,1013051119,30,15,0

# 数据量
mported:
  961309 nodes
  34856901 relationships
  105532012 properties

```

## 查询搜素数据

```sql
match res=(p:Author{authorId:'2045129800'})-[r1:Article]-(p1:Author)-[r2:Article]-(p2:Author)
match (p1)-[r3:Article]-() 
match (p2)-[r5:Article]-()
return r1,r2,r3,r5,p,p1,p2;


match res=(p:Author{authorId:'2045129800'})-[r1:Article]-(p1:Author)-[r2:Article]-(p2:Author) return r1,r2,p,p1,p2;
```


