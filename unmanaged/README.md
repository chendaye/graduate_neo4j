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
