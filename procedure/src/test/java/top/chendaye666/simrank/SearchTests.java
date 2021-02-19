
package top.chendaye666.simrank;

import org.junit.jupiter.api.*;
import org.neo4j.driver.*;
import org.neo4j.harness.Neo4j;
import org.neo4j.harness.Neo4jBuilders;

import java.util.Iterator;

@TestInstance(TestInstance.Lifecycle.PER_CLASS)
public class SearchTests {

    private static final Config driverConfig = Config.builder().withoutEncryption().build();
    private static Driver driver;
    private Neo4j embeddedDatabaseServer;
    // 构建测试图
    public static final String MODEL_STATEMENT =
            new StringBuilder() // 词的索引:词:词频
                    .append("CREATE (p1:Author {name: 'aa', authorId: '1', words: '1:DB:2@2:CV:1', attribute:'1,2,3', articles:'12#title#junal#1993@13#title#junal#1996'})")
                    .append("CREATE (p2:Author {name: 'aa', authorId: '2', words: '4:MD:7@2:CV:1', attribute:'1,2,3', articles:'122#title#junal#2000'})")
                    .append("CREATE (p3:Author {name: 'aa', authorId: '3', words: '5:AI:1@2:CV:1@4:DM:4', attribute:'1,2,3', articles:'132#title#junal#1999'})")
                    .append("CREATE (p4:Author {name: 'aa', authorId: '4', words: '2:CV:1@3:ML:2@4:DM:1', attribute:'1,2,3', articles:'124#title#junal#1997'})")
                    .append("CREATE (p5:Author {name: 'aa', authorId: '5', words: '2:CV:1@1:DB:2@4:DM:1', attribute:'1,2,3', articles:'1223#title#junal#2005'})")
                    .append("CREATE (p6:Author {name: 'aa', authorId: '6', words: '2:CV:1@4:DM:1', attribute:'1,2,3', articles:'1232#title#junal#2006'})")
                    .append("CREATE (p7:Author {name: 'aa', authorId: '7', words: '2:CV:8@4:DM:6', attribute:'1,2,3', articles:'142#title#junal#2010'})")
                    .append("CREATE (p8:Author {name: 'aa', authorId: '8', words: '2:CV:1@4:DM:1@3:ML:4', attribute:'1,2,3', articles:'1542#title#junal#2015'})")
                    .append("CREATE (p9:Author {name: 'aa', authorId: '9', words: '2:CV:5@4:DM:2', attribute:'1,2,3', articles:'162#title#junal#2020@1378#title#junal#1986'})")
                    .append("CREATE (p10:Author {name: 'aa', authorId: '10', words: '2:CV:3@4:DM:6', attribute:'1,2,3', articles:'1452#title#junal#1996'})")
                    .append("CREATE (p11:Author {name: 'aa', authorId: '11', words: '2:CV:1@4:DM:1@5:AI:1', attribute:'1,2,3', articles:'1882#title#junal#1997'})")
                    .append("CREATE (p12:Author {name: 'aa', authorId: '12', words: '2:CV:1@4:DM:1', attribute:'1,2,3', articles:'1288#title#junal#1970'})")
                    .append("CREATE (p13:Author {name: 'aa', authorId: '13', words: '2:CV:1@4:DM:1', attribute:'1,2,3', articles:'1762#title#junal#2008@130#title#junal#2016'})")
                    .append("MERGE (p1)-[:Article {weight: toInteger(1)}]->(p2)")
                    .append("MERGE (p1)-[:Article {weight: toInteger(2)}]->(p3)")
                    .append("MERGE (p1)-[:Article {weight: toInteger(3)}]->(p4)")
                    .append("MERGE (p2)-[:Article {weight: toInteger(4)}]->(p3)")
                    .append("MERGE (p2)-[:Article {weight: toInteger(5)}]->(p4)")
                    .append("MERGE (p2)-[:Article {weight: toInteger(6)}]->(p5)")
                    .append("MERGE (p3)-[:Article {weight: toInteger(7)}]->(p4)")
                    .append("MERGE (p3)-[:Article {weight: toInteger(8)}]->(p5)")
                    .append("MERGE (p3)-[:Article {weight: toInteger(9)}]->(p10)")
                    .append("MERGE (p3)-[:Article {weight: toInteger(10)}]->(p11)")
                    .append("MERGE (p3)-[:Article {weight: toInteger(11)}]->(p12)")
                    .append("MERGE (p10)-[:Article {weight: toInteger(12)}]->(p11)")
                    .append("MERGE (p10)-[:Article {weight: toInteger(13)}]->(p12)")
                    .append("MERGE (p11)-[:Article {weight: toInteger(14)}]->(p12)")
                    .append("MERGE (p5)-[:Article {weight: toInteger(15)}]->(p6)")
                    .append("MERGE (p6)-[:Article {weight: toInteger(16)}]->(p7)")
                    .append("MERGE (p6)-[:Article {weight: toInteger(17)}]->(p8)")
                    .append("MERGE (p6)-[:Article {weight: toInteger(18)}]->(p9)")
                    .append("MERGE (p7)-[:Article {weight: toInteger(19)}]->(p8)")
                    .append("MERGE (p7)-[:Article {weight: toInteger(20)}]->(p9)")
                    .append("MERGE (p8)-[:Article {weight: toInteger(21)}]->(p9)")
                    .toString();

    @BeforeAll
    void initializeNeo4j() {
        this.embeddedDatabaseServer = Neo4jBuilders.newInProcessBuilder()
                .withDisabledServer()
                .withProcedure(Search.class)
                .build();

        this.driver = GraphDatabase.driver(embeddedDatabaseServer.boltURI(), driverConfig);
    }

    @AfterAll
    void closeDriver(){
        this.driver.close();
        this.embeddedDatabaseServer.close();
    }

    @AfterEach
    void cleanDb(){
        try(Session session = driver.session()) {
            session.run("MATCH ()-[r:Article]-() DELETE r");
            session.run("MATCH (n) DETACH DELETE n");
        }
    }



    /**
     * id= 4,119
     */
    @Test
    public void search() {
        try(Session session = driver.session()) {
            session.run(MODEL_STATEMENT);
            //Execute our procedure against it.
            // MATCH (u:Author) where id(u) = 15  CALL top.chendaye666.simrank.search(u, 'AL-naive') YIELD nid,oid,value RETURN nid,oid,value
            final Result result = session.run("MATCH (u:Author {authorId:'4'})  CALL top.chendaye666.simrank.search(u, 'AL-naive') YIELD nid,oid,value RETURN nid,oid,value");
            while (result.hasNext()){
                Record next = result.next();
                Iterator<Value> iterator = next.values().iterator();
                while (iterator.hasNext()){
                    final Value val = iterator.next();
                    System.out.println("test="+val.toString());
                }
            }
        }
    }
}


