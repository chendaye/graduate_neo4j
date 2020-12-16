# 内科医生共享病例社群分析

## 数据预处理

[原始数据]( https://downloads.cms.gov/foia/physician-shared-patient-patterns-2015-days30.zip )

```bash
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

# 导入数据
neo4j-enterprise-2.3.2/bin/neo4j-import 
--into neo4j-enterprise-2.3.2/data/graph.db 
--nodes:Provider providers.csv 
--relationships:SHARE_MEMBER shared_members.csv

```

```bash
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
```