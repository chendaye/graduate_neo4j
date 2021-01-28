package top.chendaye666.equitruss;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.apache.commons.lang3.StringUtils;
import org.neo4j.dbms.api.DatabaseManagementService;
import org.neo4j.graphdb.*;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Map;

/**
 * 算法实现准备
 */
@Path( "/equitruss" )
public class EquitrussJniHttp {

    private final ObjectMapper objectMapper;
    private DatabaseManagementService dbms = null;
    public EquitrussJniHttp(){
        this.objectMapper = new ObjectMapper(); // jackson
    }

    public EquitrussJniHttp(@Context DatabaseManagementService dbms )
    {
        this.dbms = dbms;
        this.objectMapper = new ObjectMapper(); // jackson
    }

    @GET
    @Produces( MediaType.APPLICATION_JSON )
    @Path( "/hello/{nodeId}" )
    public Response hello(@PathParam( "nodeId" ) int nodeId ) throws JsonProcessingException {
        String msg = "Hello World, nodeId=" + nodeId;
        // Do stuff with the database
        return Response.ok().entity(objectMapper.writeValueAsString(msg)).build();
    }


    /**
     * http://localhost:7474/search/equitruss/search/1/15/10/1
     * match (p:Author{authorId:'2045129800'}) set p:LONG set p.community=1 return  p.name,p.community, labels(p) AS labels;
     * @param node_id
     * @param k_value
     * @param attr_count
     * @param selection
     * @return
     * @throws IOException
     */
    @GET
    @Path("/search/{node_id}/{k_value}/{attr_count}/{selection}")
    public Response search(@PathParam( "node_id" ) int node_id, @PathParam( "k_value" ) int k_value, @PathParam("attr_count") int attr_count, @PathParam("selection") int selection ) throws IOException {
        final GraphDatabaseService db = dbms.database("neo4j");
        // 判断节点是否存在
        try(Transaction tx = db.beginTx()) {
            Result execute = tx.execute("match (n:Author) where id(n)=" + node_id + " return n");
//            tx.commit();
            if (!execute.hasNext()) return Response.ok().entity(objectMapper.writeValueAsString("node not exists!")).build();
        }
        // 获取数据
        String query = "match res=(p:Author)-[r1:Article]-(p1:Author) where id(p)="+node_id+" return p1";
        String[] path = DataUtils.communityGenerate(db, query, node_id);
        //调用 Jni： relationship.txt node.txt vertex  resultPath query_k attr_cnt algo_type
        JniUtil jni = new JniUtil();
        String ans = jni.query(path[0], path[1], node_id, path[2], k_value, attr_count, selection);
//        String ans = "881893:4:0.042062:2:1,2,3,4:15,190868,522503,637353,664837,743606,881893#1,2,3,4@419175,533944,881893,1457212#1,2,3,4";
        String[] split = ans.split(":");
//        System.out.println(split.length);
        if (split.length != 6) return Response.ok().entity(objectMapper.writeValueAsString("没有符合条件的community！")).build();
        ArrayList<int[]> ints = DataUtils.parseCommunity(ans);
        if (ints.size() == 3){
            StringBuilder current_id_str = new StringBuilder();
            current_id_str.append('[');
            for (int current_id : ints.get(2)){
                current_id_str.append(String.valueOf(current_id)+',');
            }
            current_id_str.deleteCharAt(current_id_str.length() - 1);
            current_id_str.append(']');
            String key = node_id+"_"+k_value+"_"+attr_count+"_"+selection;
            writeCommunity(db, node_id, current_id_str.toString(), ans.split(":")[5].split("#")[1], key);
        }
        // 搜索结果
        String[] communities = split[5].split("@");
        ArrayList<String[]> answer = new ArrayList<>();
        for (String s : communities){
            answer.add(s.split("#"));
        }
        return Response.ok().entity(objectMapper.writeValueAsString(answer)).build();
    }

    // 设置社区编号
    public void writeCommunity(GraphDatabaseService db, int node_id, String current_id_str, String attr_str, String key){
        String query = "match (p:Author) where id(p) in "+current_id_str+"  set p.community_"+key+"="+node_id+",p.common_attribute_"+key+"='"+attr_str+"'";
        System.out.println(query);
        try (Transaction tx = db.beginTx()){
            tx.execute(query);
            tx.commit();
        }
    }

    @GET
    @Path("/txt/{node_id}")
    public Response txt(@PathParam( "node_id" ) int node_id) throws IOException {
        final GraphDatabaseService db = dbms.database("neo4j");
        String query = "match res=(p:Author{authorId:'"+node_id+"'})-[r1:Article]-(p1:Author) return p,p1";
        try (Transaction tx = db.beginTx()){
            Result result = tx.execute(query);

            System.out.println(result.hasNext());

            tx.commit();
        }
//        String[] path = DataUtils.mutilStepGenerate(db, query);
        return Response.ok().entity(objectMapper.writeValueAsString("ok")).build();
    }

    @GET
    @Path("/txtv2/{node_id}")
    public Response txtv2(@PathParam( "node_id" ) int node_id) throws IOException {
        final GraphDatabaseService db = dbms.database("neo4j");
        // 从一个起点出发 2045129800
        String query = "match res=(p:Author{authorId:'"+node_id+"'})-[r1:Article]-(p1:Author) return p,p1";
        String[] path = DataUtils.communityGenerate(db, query, node_id);
        return Response.ok().entity(objectMapper.writeValueAsString(path)).build();
    }
}
