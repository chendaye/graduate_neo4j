
package top.chendaye666.equitruss;

import org.junit.jupiter.api.*;
import org.neo4j.driver.*;
import org.neo4j.graphdb.Node;
import org.neo4j.harness.Neo4j;
import org.neo4j.harness.Neo4jBuilders;
import top.chendaye666.example.GetRelationshipTypes;

import java.util.Map;

import static org.assertj.core.api.Assertions.assertThat;
@TestInstance(TestInstance.Lifecycle.PER_CLASS)
public class SearchTests {

    private static final Config driverConfig = Config.builder().withoutEncryption().build();
    private static Driver driver;
    private Neo4j embeddedDatabaseServer;
    // 构建测试图
    public static final String MODEL_STATEMENT =
            new StringBuilder() // 词的索引:词:词频
                    .append("CREATE (p1:Author {authorId: '1', words: '1:DB:2@2:CV:1'})")
                    .append("CREATE (p2:Author {authorId: '2', words: '4:MD:7@2:CV:1'})")
                    .append("CREATE (p3:Author {authorId: '3', words: '5:AI:1@2:CV:1@4:DM:4'})")
                    .append("CREATE (p4:Author {authorId: '4', words: '2:CV:1@3:ML:2@4:DM:1'})")
                    .append("CREATE (p5:Author {authorId: '5', words: '2:CV:1@1:DB:2@4:DM:1'})")
                    .append("CREATE (p6:Author {authorId: '6', words: '2:CV:1@4:DM:1'})")
                    .append("CREATE (p7:Author {authorId: '7', words: '2:CV:8@4:DM:6'})")
                    .append("CREATE (p8:Author {authorId: '8', words: '2:CV:1@4:DM:1@3:ML:4'})")
                    .append("CREATE (p9:Author {authorId: '9', words: '2:CV:5@4:DM:2'})")
                    .append("CREATE (p10:Author {authorId: '10', words: '2:CV:3@4:DM:6'})")
                    .append("CREATE (p11:Author {authorId: '11', words: '2:CV:1@4:DM:1@5:AI:1'})")
                    .append("CREATE (p12:Author {authorId: '12', words: '2:CV:1@4:DM:1'})")
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
     * We should be getting the correct values when there is only one type in each direction
     */
    @Test
    public void getNeighbor() {
        // In a try-block, to make sure we close the session after the test
        try(Session session = driver.session()) {
            //Create our data in the database.
            session.run(MODEL_STATEMENT);
            //Execute our procedure against it.
            Result result = session.run("MATCH (u:Author {id:4}) CALL top.chendaye666.equitruss.getNeighbor(u) YIELD community RETURN community");
            result.forEachRemaining(res -> {
                System.out.println(res.toString());
            });
//            while (result.hasNext()){
//                Map<String,Object> row = (Map<String, Object>) result.next();
//                for ( Map.Entry<String,Object> column : row.entrySet() ){
//                    Node author = (Node) column.getValue();
//                    System.out.println(author.toString());
//                }
//            }
//            System.out.println(result.toString());
//            assertThat(2 < 1);
        }
    }
}


