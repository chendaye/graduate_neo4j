package top.chendaye666.equitruss;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.neo4j.dbms.api.DatabaseManagementService;
import org.neo4j.graphdb.*;
import top.chendaye666.httpjni.JniUtil;

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
import java.util.HashMap;
import java.util.Iterator;
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
    @Path("/search/{node_id}/{k_value}/{attr_count}/{selection}")
    public Response search(@PathParam( "node_id" ) String node_id, @PathParam( "k_value" ) int k_value, @PathParam("attr_count") int attr_count, @PathParam("selection") int selection ) throws IOException {
        final GraphDatabaseService db = dbms.database("neo4j");
        ArrayList<String> resp = new ArrayList<>();
        ArrayList<Long> relationships = new ArrayList<>();
        ArrayList<Long> nodes = new ArrayList<>();
        String nodePath = "./node_"+readWriteTxtUtils.generateStr(5)+".txt";
        readWriteTxtUtils.fileExist(nodePath);
        final File nodeFile = new File(nodePath);
        final FileOutputStream nodeOutputStream = new FileOutputStream(nodeFile);
        String relationshipPath = "./relationship_"+readWriteTxtUtils.generateStr(5)+".txt";
        final File relationshipFile = new File(relationshipPath);
        final FileOutputStream relationshipOutputStream = new FileOutputStream(relationshipFile);
        readWriteTxtUtils.fileExist(relationshipPath);
        // 从一个起点出发
        String query = "match res=(p:Author{authorId:'"+node_id+"'})-[r1:Article]-(p1:Author)-[r2:Article]-(p2:Author)" +
                " match (p1)-[r3:Article]-()-[r4:Article]-() "+
                " match (p2)-[r5:Article]-()-[r6:Article]-() "
                +" return r1,r2,r3,r4,r5,r6";
        try {
            try (Transaction tx = db.beginTx() ) {
                Result result = tx.execute(query);
                while (result.hasNext()){
                    Map<String,Object> row = result.next();
                    for ( Map.Entry<String,Object> column : row.entrySet() ){
                        String key = column.getKey();
                        Relationship relationship = (Relationship) column.getValue();
                        long id = relationship.getId();
                        Node start = (Node)relationship.getStartNode();
                        Node end = (Node)relationship.getEndNode();
                        System.out.println(key+":"+relationship.getStartNodeId()+"->"+relationship.getEndNodeId());
                        if (!relationships.contains(id)){
                            relationships.add(id);
                            String r = start.getId()+"\t"+end.getId()+"\n";
                            relationshipOutputStream.write(r.getBytes());
                            if (!nodes.contains(start.getId())){
                                String startStr = readWriteTxtUtils.parseNode(start);
                                nodeOutputStream.write(startStr.getBytes());
                                nodes.add(start.getId());
                            }
                            if(!nodes.contains(end.getId())){
                                String endStr = readWriteTxtUtils.parseNode(end);
                                nodeOutputStream.write(endStr.getBytes());
                                nodes.add(end.getId());
                            }
                        }
//                    resp.add(s);
                    }
                }
                tx.commit();
            }
            //TODO: 调用 Jni： relationship.txt node.txt vertex  result_path query_k attr_cnt algo_type
        }catch (IOException e){
            e.printStackTrace();
        }finally {
            nodeOutputStream.flush();
            nodeOutputStream.close();
            relationshipOutputStream.flush();
            relationshipOutputStream.close();
        }

        return Response.ok().entity(objectMapper.writeValueAsString(resp)).build();
    }



}
