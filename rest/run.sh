java -jar /home/hadoop/code/whu_neo4j/target/movie-api.jar \
--org.neo4j.driver.uri=bolt://whu1.top.chendaye666.top:7687 \
--org.neo4j.driver.authentication.username=neo4j \
--org.neo4j.driver.authentication.password=lengo > /tmp/neo4j.log 2>&1 &
