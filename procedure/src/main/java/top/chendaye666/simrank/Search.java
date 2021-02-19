package top.chendaye666.simrank;

import org.neo4j.graphdb.*;
import org.neo4j.procedure.Context;
import org.neo4j.procedure.Description;
import org.neo4j.procedure.Name;
import org.neo4j.procedure.Procedure;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.stream.Stream;
import top.chendaye666.simrank.util.DataUtils;
import top.chendaye666.simrank.streams.Center;
import top.chendaye666.simrank.util.ReadWriteTxtUtils;

/**
 * https://neo4j.com/docs/java-reference/current/extending-neo4j/procedures-and-functions/procedures/
 * https://neo4j.com/docs/cypher-manual/4.1/clauses/call/
 * https://neo4j.com/docs/graph-data-science/current/algorithms/louvain/#algorithms-louvain-examples-stream
 */
public class Search {
    @Context
    public GraphDatabaseService db;
    public Search(){}


    /**
     * 计算中心度
     * @param node
     * @param config
     * @return
     */
    @Procedure(value = "top.chendaye666.simrank.search")
    @Description("center simrank.")
    public Stream<Center> search(@Name("node") Node node, @Name("config") String config) {
        ArrayList<Center> res = new ArrayList<>();
        // 计算中心度
        String configPath = "/tmp/simrank/config/"+config;
        if (!ReadWriteTxtUtils.fileExist(configPath)) return res.stream();
        String ans = generateGraph(node, configPath);
        System.out.println("JniSimrank="+ans);
        if (ans == null) return res.stream();
        //todo：new vid, old vid, val
//        String ans = "1,4,0.373333@2,11,0.373333@4,9,0.373333@6,10,0.373333@0,2,0.320000@5,1,0.266667@-1,-1,-1.0@-1,-1,-1.0@1,4,0.373333@2,11,0.373333@4,9,0.373333@6,10,0.373333@0,2,0.320000@5,1,0.266667@-1,-1,-1.0@-1,-1,-1.0@1,4,0.373333@16,2,0.373333@4,9,0.373333@6,10,0.373333@0,2,0.320000@5,1,0.266667@-1,-1,-1.0@-1,-1,-1.0@1,4,0.373333@2,11,0.373333@4,9,0.373333@6,10,0.373333@0,2,0.320000@5,1,0.266667@-1,-1,-1.0@-1,-1,-1.0@1,4,0.373333@2,11,0.373333@4,9,0.373333@6,10,0.373333@0,2,0.320000@5,1,0.266667@-1,-1,-1.0@-1,-1,-1.0@1,4,0.373333@2,11,0.373333@4,9,0.373333@6,10,0.373333@0,2,0.320000@5,1,0.266667@-1,-1,-1.0@-1,-1,-1.0@1,4,0.373333@16,2,0.373333";
        ArrayList<String[]> rank = DataUtils.parseRank(ans);
        for (String[] s : rank){
            if (!s[0].equals("-1")){
                res.add(new Center(s[0], s[1], s[2]));
            }
        }
        return res.stream();
    }

    /**
     * simrank 生成子图，并执行算法
     * @param node
     * @param config
     * @return
     */
    public String generateGraph(Node node, String config){
        if (node == null) return null;
        int node_id = (int) node.getId(); // 查询的节点id
        // 获取数据
        String query = "match res=(p:Author)-[r1:Article]-(p1) where id(p)="+node_id+" return p1,p";
        // 修改配置
        System.out.println("node_id="+node_id+";config="+config);
        int[] ints = DataUtils.communityGenerate(db, query);
        if (ints == null) return null;
        HashMap<String, String> kv = new HashMap<>();
        kv.put("-en", ints[0]+"");
        kv.put("-vn", ints[1]+"");
        try {
            ReadWriteTxtUtils.replaceContent(config, kv);
        }catch (IOException e){
            e.printStackTrace();
        }
        top.chendaye666.simrank.JniUtil jni = new JniUtil();
        String ans = jni.query(config, node_id);
        return ans;
    }

}
