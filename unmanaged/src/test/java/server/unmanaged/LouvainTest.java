package server.unmanaged;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.junit.Rule;
import org.junit.Test;
import org.neo4j.harness.junit.rule.Neo4jRule;
import org.neo4j.test.server.HTTP;
import top.chendaye666.physiciansharedpatient.Louvain;

import java.util.ArrayList;
import java.util.HashMap;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertTrue;

public class LouvainTest {
    // jackson
    private static final ObjectMapper objectMapper = new ObjectMapper();
    @Rule
    public Neo4jRule neo4jRule = new Neo4jRule().withFixture(MODEL_STATEMENT).withUnmanagedExtension("/algorithm", Louvain.class);

    // 构建测试图
    public static final String MODEL_STATEMENT =
            new StringBuilder()
                    .append("CREATE (p1:Physician {providerId: 'p1'})")
                    .append("CREATE (p2:Physician {providerId: 'p2'})")
                    .append("MERGE (p1)-[:SHARE_MEMBER {weight:0.2}]->(p2)")
                    .append("CREATE (p3:Physician {providerId: 'p3'})")
                    .append("CREATE (p4:Physician {providerId: 'p4'})")
                    .append("MERGE (p3)-[:SHARE_MEMBER {weight:0.2}]->(p4)")
                    .append("CREATE (p5:Physician {providerId: 'p5'})")
                    .append("CREATE (p6:Physician {providerId: 'p6'})")
                    .append("CREATE (p7:Physician {providerId: 'p7'})")
                    .append("CREATE (p8:Physician {providerId: 'p8'})")
                    .append("MERGE (p5)-[:SHARE_MEMBER {weight:0.05}]->(p6)")
                    .append("MERGE (p5)-[:SHARE_MEMBER {weight:0.1}]->(p7)")
                    .append("MERGE (p5)-[:SHARE_MEMBER {weight:0.1}]->(p8)")
                    .append("MERGE (p6)-[:SHARE_MEMBER {weight:0.1}]->(p7)")
                    .append("MERGE (p6)-[:SHARE_MEMBER {weight:0.1}]->(p8)")
                    .append("MERGE (p7)-[:SHARE_MEMBER {weight:0.15}]->(p8)")
                    .append("CREATE (p9:Physician {providerId: 'p9'})")
                    .toString();

    @Test
    public void respondTest() {
        HTTP.Response response = HTTP.GET(neo4jRule.httpURI().resolve("/algorithm/louvain/hello/123").toString());

        System.out.println(response.content().toString());
//        assertTrue(actual.equals(expected));
//
//        response = HTTP.GET(neo4j.httpURI().resolve("/v1/service/dump").toString());
//        ArrayList<HashMap<String, Object>> results  = response.content();
//        assertArrayEquals(nodes.toArray(), results.toArray());
    }

    @Test
    public void helloTest() throws JsonProcessingException {
        HTTP.Response response = HTTP.GET(neo4jRule.httpURI().resolve("/algorithm/louvain/helloworld").toString());
        HashMap actual = response.content();
        System.out.println(actual.get("hello"));
    }


}
