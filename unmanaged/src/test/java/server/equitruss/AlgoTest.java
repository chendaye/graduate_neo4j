package server.equitruss;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.junit.Rule;
import org.junit.Test;
import org.neo4j.graphdb.*;
import org.neo4j.harness.junit.rule.Neo4jRule;
import org.neo4j.test.server.HTTP;
import top.chendaye666.equitruss.TestJniHttp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;


// dbms.unmanaged_extension_classes=top.chendaye666.unmanaged=/unmanaged/test,top.chendaye666.physiciansharedpatient=/algorithm
public class AlgoTest {
    // jackson
    private static final ObjectMapper objectMapper = new ObjectMapper();
    @Rule
    public Neo4jRule neo4jRule = new Neo4jRule()
            .withUnmanagedExtension("/equitruss", TestJniHttp.class)
            .withFixture(MODEL_STATEMENT);


    // 构建测试图
    public static final String MODEL_STATEMENT =
            new StringBuilder() // 词的索引:词:词频
                    .append("CREATE (p1:Author {authorId: '1', keywords: '1:DB:2@2:CV:1'})")
                    .append("CREATE (p2:Author {authorId: '2', keywords: '4:MD:7@2:CV:1'})")
                    .append("CREATE (p3:Author {authorId: '3', keywords: '5:AI:1@2:CV:1@4:DM:4'})")
                    .append("CREATE (p4:Author {authorId: '4', keywords: '2:CV:1@3:ML:2@4:DM:1'})")
                    .append("CREATE (p5:Author {authorId: '5', keywords: '2:CV:1@1:DB:2@4:DM:1'})")
                    .append("CREATE (p6:Author {authorId: '6', keywords: '2:CV:1@4:DM:1'})")
                    .append("CREATE (p7:Author {authorId: '7', keywords: '2:CV:8@4:DM:6'})")
                    .append("CREATE (p8:Author {authorId: '8', keywords: '2:CV:1@4:DM:1@3:ML:4'})")
                    .append("CREATE (p9:Author {authorId: '9', keywords: '2:CV:5@4:DM:2'})")
                    .append("CREATE (p10:Author {authorId: '10', keywords: '2:CV:3@4:DM:6'})")
                    .append("CREATE (p11:Author {authorId: '11', keywords: '2:CV:1@4:DM:1@5:AI:1'})")
                    .append("CREATE (p12:Author {authorId: '12', keywords: '2:CV:1@4:DM:1'})")
                    .append("MERGE (p1)-[:Article]->(p2)")
                    .append("MERGE (p1)-[:Article]->(p3)")
                    .append("MERGE (p1)-[:Article]->(p4)")
                    .append("MERGE (p2)-[:Article]->(p3)")
                    .append("MERGE (p2)-[:Article]->(p4)")
                    .append("MERGE (p2)-[:Article]->(p5)")
                    .append("MERGE (p3)-[:Article]->(p4)")
                    .append("MERGE (p3)-[:Article]->(p5)")
                    .append("MERGE (p3)-[:Article]->(p10)")
                    .append("MERGE (p3)-[:Article]->(p11)")
                    .append("MERGE (p3)-[:Article]->(p12)")
                    .append("MERGE (p10)-[:Article]->(p11)")
                    .append("MERGE (p10)-[:Article]->(p12)")
                    .append("MERGE (p11)-[:Article]->(p12)")
                    .append("MERGE (p5)-[:Article]->(p6)")
                    .append("MERGE (p6)-[:Article]->(p7)")
                    .append("MERGE (p6)-[:Article]->(p8)")
                    .append("MERGE (p6)-[:Article]->(p9)")
                    .append("MERGE (p7)-[:Article]->(p8)")
                    .append("MERGE (p7)-[:Article]->(p9)")
                    .append("MERGE (p8)-[:Article]->(p9)")
                    .toString();

    /**
     * 测试基础请求
     * @throws JsonProcessingException
     */
    @Test
    public void helloTest() throws JsonProcessingException {
        HTTP.Response response = HTTP.GET(neo4jRule.httpURI().resolve("/equitruss/test/helloworld").toString());
        HashMap actual = response.content();
        System.out.println(actual.get("hello"));
    }

    /**
     * 测试参数传递
     */
    @Test
    public void respondTest() {
        HTTP.Response response = HTTP.GET(neo4jRule.httpURI().resolve("/equitruss/test/hello/4/4/2").toString());
        System.out.println(response.content().toString());
    }

    /**
     * 测试文件生成
     * @throws JsonProcessingException
     */
    @Test
    public void dumpTest() throws JsonProcessingException {
        // node_id, k_value, attr_count, selection
        HTTP.Response response = HTTP.GET(neo4jRule.httpURI().resolve("/equitruss/test/dump/1/4/4/2").toString());
        System.out.println("dumpTest start");
        ArrayList<Object> actual = response.content();
        System.out.println(actual.toString());
        System.out.println("dumpTest end");

    }

}
