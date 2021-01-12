package server.unmanaged;
import java.util.*;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.junit.Rule;
import org.junit.Test;
import org.neo4j.graphdb.*;
import org.neo4j.harness.junit.rule.Neo4jRule;
import org.neo4j.test.server.HTTP;
import top.chendaye666.httpjni.LouvainHttp;


// dbms.unmanaged_extension_classes=top.chendaye666.unmanaged=/unmanaged/test,top.chendaye666.physiciansharedpatient=/algorithm
public class LouvainTest {
    // jackson
    private static final ObjectMapper objectMapper = new ObjectMapper();
    @Rule
    public Neo4jRule neo4jRule = new Neo4jRule()
            .withUnmanagedExtension("/algorithm", LouvainHttp.class)
            .withFixture(MODEL_STATEMENT);


    // 构建测试图
    public static final String MODEL_STATEMENT =
            new StringBuilder()
                    .append("CREATE (p1:Physician {providerId: 'p1'})")
                    .append("CREATE (p2:Physician {providerId: 'p2'})")
                    .append("MERGE (p1)-[r:SHARE_MEMBER {patients: toInteger(1)}]->(p2)")//.append("set r.weight = 10")
                    .append("MERGE (p2)-[:SHARE_MEMBER {patients: toInteger(1)}]->(p1)")
                    .append("CREATE (p3:Physician {providerId: 'p3'})")
                    .append("CREATE (p4:Physician {providerId: 'p4'})")
                    .append("MERGE (p3)-[:SHARE_MEMBER {patients: toInteger(1)}]->(p4)")
                    .append("CREATE (p5:Physician {providerId: 'p5'})")
                    .append("CREATE (p6:Physician {providerId: 'p6'})")
                    .append("CREATE (p7:Physician {providerId: 'p7'})")
                    .append("CREATE (p8:Physician {providerId: 'p8'})")
                    .append("MERGE (p5)-[:SHARE_MEMBER {patients: toInteger(1)}]->(p6)")
                    .append("MERGE (p5)-[:SHARE_MEMBER {patients: toInteger(1)}]->(p7)")
                    .append("MERGE (p5)-[:SHARE_MEMBER {patients: toInteger(1)}]->(p8)")
                    .append("MERGE (p6)-[:SHARE_MEMBER {patients: toInteger(1)}]->(p7)")
                    .append("MERGE (p6)-[:SHARE_MEMBER {patients: toInteger(1)}]->(p8)")
                    .append("MERGE (p7)-[:SHARE_MEMBER {patients: toInteger(1)}]->(p8)")
                    .append("CREATE (p9:Physician {providerId: 'p9'})")
                    .append("MERGE (p1)-[:SHARE_MEMBER {patients: toInteger(1)}]->(p9)")
                    .append("MERGE (p2)-[:SHARE_MEMBER {patients: toInteger(1)}]->(p9)")
                    .toString();




    @Test
    public void respondTest() {
        HTTP.Response response = HTTP.GET(neo4jRule.httpURI().resolve("/algorithm/louvain/hello/123").toString());

        System.out.println(response.content().toString());
    }

    @Test
    public void helloTest() throws JsonProcessingException {
        HTTP.Response response = HTTP.GET(neo4jRule.httpURI().resolve("/algorithm/louvain/helloworld").toString());
        HashMap actual = response.content();
        System.out.println(actual.get("hello"));
    }


    @Test
    public void testMyExtensionWithFunctionFixture()
    {
        final GraphDatabaseService graphDatabaseService = neo4jRule.defaultDatabaseService();
        try ( Transaction transaction = graphDatabaseService.beginTx() )
        {
            // Given
            Result result = transaction.execute( "MATCH (n:Physician) return n" );
//            String s = result.resultAsString();
//            System.out.println(s);
            String str = "";

            while (result.hasNext()){
                Map<String,Object> row = result.next();
                for ( Map.Entry<String,Object> column : row.entrySet() )
                {
                    String key = column.getKey();
                    Node node = (Node)column.getValue();
                    str += key + ": " + node.getLabels() + node.getDegree()+"; ";

                    Iterator<Relationship> iterator = node.getRelationships().iterator();
                    while (iterator.hasNext()){
                        System.out.println("relation" + iterator.next().getEndNode());
                    }
                }
                str += "\n";
            }
            System.out.println(str);
            // Then
//            assertEquals( 1, count( result ) );
            transaction.commit();
        }
    }

    @Test
    public void dumpTest() throws JsonProcessingException {
        HTTP.Response response = HTTP.GET(neo4jRule.httpURI().resolve("/algorithm/louvain/dump").toString());
        System.out.println("dumpTest start");
        ArrayList<Object> actual = response.content();
        System.out.println(actual.toString());
        System.out.println("dumpTest end");

    }

    @Test
    public void implTest() throws JsonProcessingException {
        HTTP.Response response = HTTP.GET(neo4jRule.httpURI().resolve("/algorithm/louvainimpl/impl").toString());
//        ArrayList<Object> actual = response.content();
        System.out.println(response.content().toString());
    }

    @Test
    public void implLimitTest() throws JsonProcessingException {
        HTTP.Response response = HTTP.GET(neo4jRule.httpURI().resolve("/algorithm/louvainimpl/impllimit/9").toString());
//        ArrayList<Object> actual = response.content();
        System.out.println(response.content().toString());
    }
}
