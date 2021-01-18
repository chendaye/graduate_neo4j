package top.chendaye666.httpjni;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.neo4j.dbms.api.DatabaseManagementService;
import org.neo4j.graphdb.*;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

/**
 * 算法实现准备
 */
@Path( "/louvain" )
public class LouvainHttp {

    private final ObjectMapper objectMapper;
    private DatabaseManagementService dbms = null;
    public LouvainHttp(){
        this.objectMapper = new ObjectMapper(); // jackson
    }

    public LouvainHttp(@Context DatabaseManagementService dbms )
    {
        this.dbms = dbms;
        this.objectMapper = new ObjectMapper(); // jackson
    }

    public static void main(String[] args) {
        JniUtil jniUtil = new JniUtil();
        // /Users/lengo/Library/Java/Extensions, /Library/Java/Extensions, /Network/Library/Java/Extensions, /System/Library/Java/Extensions, /usr/lib/java, .]
        System.out.println(System.getProperty("java.library.path"));
        System.out.println(jniUtil.getNum(1));
    }


    @GET
    @Produces( MediaType.APPLICATION_JSON )
    @Path( "/hello/{nodeId}" )
    public Response hello(@PathParam( "nodeId" ) int nodeId ) throws JsonProcessingException {
        // 调用Jni代码
        JniUtil jniUtil = new JniUtil();
        nodeId = jniUtil.getNum(nodeId);
        String msg = "Hello World, nodeId=" + nodeId;
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
    @Path("/dump")
    public Response dump() throws IOException {
        final GraphDatabaseService db = dbms.database("neo4j");
        ArrayList<String> resp = new ArrayList<>();
        String query = "MATCH p=()-[r:SHARE_MEMBER]->() RETURN p LIMIT 6";
        try (Transaction tx = db.beginTx() ) {
            Result result = tx.execute(query);

            while (result.hasNext()){
                Map<String,Object> row = result.next();
                for ( Map.Entry<String,Object> column : row.entrySet() ){
                    String s = column.getValue().toString();
                    resp.add(s);
                    System.out.println(s);
                }
            }

            tx.commit();
        }

        return Response.ok().entity(objectMapper.writeValueAsString(resp)).build();
    }



    @GET
    @Path("/dump2")
    public Response dump2(GraphDatabaseService db) throws IOException {
        ArrayList<String> resp = new ArrayList<>();
        String query = "MATCH p=()-[r:SHARE_MEMBER]->() RETURN p LIMIT 6";
        try (Transaction tx = db.beginTx() ) {
            Result result = tx.execute(query);

            while (result.hasNext()){
                Map<String,Object> row = result.next();
                for ( Map.Entry<String,Object> column : row.entrySet() ){
                    String s = column.getValue().toString();
                    resp.add(s);
                    System.out.println(s);
                }
            }

            tx.commit();
        }

        return Response.ok().entity(objectMapper.writeValueAsString(resp)).build();
    }



}
