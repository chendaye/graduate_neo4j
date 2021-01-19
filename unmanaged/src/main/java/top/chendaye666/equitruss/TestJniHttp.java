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
@Path( "/test" )
public class TestJniHttp {

    private final ObjectMapper objectMapper;
    private DatabaseManagementService dbms = null;
    public TestJniHttp(){
        this.objectMapper = new ObjectMapper(); // jackson
    }

    public TestJniHttp(@Context DatabaseManagementService dbms )
    {
        this.dbms = dbms;
        this.objectMapper = new ObjectMapper(); // jackson
    }

    @GET
    @Produces( MediaType.APPLICATION_JSON )
    @Path( "/hello/{k_value}/{attr_count}/{selection}" )
    public Response hello(@PathParam( "k_value" ) int k_value, @PathParam("attr_count") int attr_count, @PathParam("selection") int selection ) throws JsonProcessingException {
        String msg = "k_value=" + k_value+";"+"attr_count="+attr_count+";"+"selection="+selection;
        // Do stuff with the database
        return Response.ok().entity(objectMapper.writeValueAsString(msg)).build();
    }

    @GET
    @Path("/helloworld")
    public Response helloWorld() throws IOException {
        Map<String, String> results = new HashMap<String,String>(){{
            put("hello","world");
        }};
        return Response.ok().entity(objectMapper.writeValueAsString(results)).build();
    }





    @GET
    @Path("/dumpnode/{node_id}/{k_value}/{attr_count}/{selection}")
    public Response dumpnode(@PathParam( "node_id" ) String node_id, @PathParam( "k_value" ) int k_value, @PathParam("attr_count") int attr_count, @PathParam("selection") int selection ) throws IOException {
        final GraphDatabaseService db = dbms.database("neo4j");
        ArrayList<String> resp = new ArrayList<>();
        // MATCH p=(n:Author)-[r:ARTICLE]-() where n.authorId="3369894700" RETURN n,p LIMIT 25
        // match res=(p:Author{authorId:'3369894700'})-[:ARTICLE*1..2]-() return res  从一个点出发，深度为 3
        String query = "MATCH (n:Author) return n limit 60";
        try (Transaction tx = db.beginTx() ) {
            Result result = tx.execute(query);

            while (result.hasNext()){
                Map<String,Object> row = result.next();
                for ( Map.Entry<String,Object> column : row.entrySet() )
                {
                    String key = column.getKey();
                    Node node = (Node)column.getValue();
                    String str = key + ": " + node.getLabels() +":"+ node.getDegree()+"; ";
                    resp.add(str);
                    // 节点的邻边
                    Iterator<Relationship> iterator = node.getRelationships().iterator();
                    while (iterator.hasNext()){
                        Node endNode = (Node) iterator.next().getEndNode();
                        //System.out.println("relation" + iterator.next().getEndNode());
                    }
                }
            }

            tx.commit();
        }
        return Response.ok().entity(objectMapper.writeValueAsString(resp)).build();
    }


    @GET
    @Path("/dumprelationship/{node_id}/{k_value}/{attr_count}/{selection}")
    public Response dumprelationship(@PathParam( "node_id" ) String node_id, @PathParam( "k_value" ) int k_value, @PathParam("attr_count") int attr_count, @PathParam("selection") int selection ) throws IOException {
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
        String query = "match res=(p:Author{authorId:'"+node_id+"'})-[r1:Article]-()-[r2:Article]-() return r1,r2";
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
//                    System.out.println(key+":"+relationship.getStartNodeId()+"->"+relationship.getEndNodeId());
                        if (!relationships.contains(id)){
                            relationships.add(id);
                            String r = start.getProperty("authorId")+"\t"+end.getProperty("authorId");
                            relationshipOutputStream.write(r.getBytes());
                            if (!nodes.contains(start.getId())){

                            }
                            if(!nodes.contains(end.getId())){

                            }
                        }
//                    resp.add(s);
                    }
                }

                tx.commit();
            }
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
