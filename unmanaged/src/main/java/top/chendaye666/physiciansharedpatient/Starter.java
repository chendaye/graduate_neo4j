package top.chendaye666.physiciansharedpatient;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.neo4j.annotations.api.PublicApi;
import org.neo4j.graphdb.*;
import org.neo4j.kernel.impl.factory.GraphDatabaseFacade;


import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Context;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

/**
 * 算法实现
 */
@Path( "/starter" )
public class Starter {
    // jackson
    private static final ObjectMapper objectMapper = new ObjectMapper();

    @GET
    @Produces( MediaType.APPLICATION_JSON )
    @Path( "/hello/{nodeId}" )
    public Response hello(@PathParam( "nodeId" ) long nodeId ) throws JsonProcessingException {
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
    @Path("/useNeo4j")
    public Response useNeo4j(@Context GraphDatabaseService db) throws IOException{
        HashMap<String, String> results = new HashMap<>() {
            {
                put("use", "neo4j");
            }
        };


        try(Transaction tx = db.beginTx()) {
            for (Node n : tx.getAllNodes()){
                n.getPropertyKeys();
                for (Relationship relationship : n.getRelationships()) {
                    relationship.getPropertyKeys();
                    relationship.getStartNode();
                }
            }
            for (Relationship relationship : tx.getAllRelationships()) {
                relationship.getPropertyKeys();
                relationship.getNodes();
            }
        }
        return Response.ok().entity(objectMapper.writeValueAsString(results)).build();
    }
}
