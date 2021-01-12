package top.chendaye666.httpjni;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.neo4j.dbms.api.DatabaseManagementService;
import org.neo4j.graphdb.GraphDatabaseService;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.Response;
import java.io.IOException;
import java.util.*;

//
@Path( "/louvainimpl" )
public class LouvainImpl {
    private final ObjectMapper objectMapper;
    private DatabaseManagementService dbms;
    private DataProcess dataProcess = new DataProcess();
    private long[] physicians; // 图中所有节点
    private int communities; // 图中社区的个数（初始时=node个数）
    private long graphWeightSum = 0; // 图的权重和
    private boolean communityUpdate = false;
    private HashMap<Long, HashSet<Long>> nodeNeighbors = new HashMap<>(); // 每一个node的邻居节点
    private HashMap<Long, HashMap<Long, Double>> nodeNeighborsWeights = new HashMap<>(); // 邻边的权重
    private HashMap<Integer, HashSet<Long>> nodesInCommunity = new HashMap<>(); // which nodes are in key community
    private HashMap<Integer, HashSet<Long>> nodesInNodeCommunity = new HashMap<>(); // which nodes are in key node community
    private HashMap<Long, Integer> communityForNode = new HashMap<>(); // which community is a node in
    private HashMap<Long, Integer> nodeCommunityForNode = new HashMap<>(); // which node community is a node in
    private HashMap<Integer, Integer> nodeCommunitiesToCommunities = new HashMap<>(); // which communities map to node communities
    private ArrayList<Long> communityWeights = new ArrayList<>();
    private ArrayList<Long> nodeCommunityWeights  = new ArrayList<>();


    public LouvainImpl(@Context DatabaseManagementService dbms){
        this.dbms = dbms;
        this.objectMapper = new ObjectMapper();
    }

    @GET
    @Path("/impllimit/{limit}")
    public Response implLimit(@PathParam("limit") int limit) throws IOException {
        long start = System.currentTimeMillis();
        System.out.println("Louvain Algorithm run in "+ start);
        final GraphDatabaseService db = dbms.database("neo4j");
        Map<String, String> results = new HashMap<String,String>(){{
            put("algorithm","ans");
        }};
        String query = "MATCH (n:Physician) RETURN n LIMIT "+limit;
        physicians = dataProcess.getPhysicians(db, query);
        communities = physicians.length; // 初始社区个数=节点个数
        HashMap<Long, HashSet<Long>> nodeNeighbors = dataProcess.getNodeNeighbors();
        HashMap<Long, HashMap<Long, Double>> nodeNeighborsWeights = dataProcess.getNodeNeighborsWeights();

        for (Map.Entry<Long, HashMap<Long, Double>> entry : nodeNeighborsWeights.entrySet()){
            System.out.println(entry.getKey()+"的邻边："+entry.getValue().toString());
            HashMap<Long, Double> value = entry.getValue();
            for (Map.Entry<Long, Double> item : value.entrySet()){
                System.out.println(item.getValue() + 1);
            }
        }
        long end = System.currentTimeMillis();
        System.out.println("Louvain Algorithm finished in "+end+"总运行时长="+(end - start));
        return Response.ok().entity(objectMapper.writeValueAsString(communities)).build();
    }

    @GET
    @Path("/impl")
    public Response impl() throws IOException {
        long start = System.currentTimeMillis();
        System.out.println("Louvain Algorithm run in "+ start);
        final GraphDatabaseService db = dbms.database("neo4j");
        Map<String, String> results = new HashMap<String,String>(){{
            put("algorithm","ans");
        }};
        physicians = dataProcess.getPhysicians(db); // 所有节点
        nodeNeighbors = dataProcess.getNodeNeighbors(); // 节点邻居
        nodeNeighborsWeights = dataProcess.getNodeNeighborsWeights();  // 节点邻边权重
        communities = physicians.length; // 初始社区个数=节点个数

        long end = System.currentTimeMillis();
        System.out.println("Louvain Algorithm finished in "+end+"总运行时长="+(end - start));
        return Response.ok().entity(objectMapper.writeValueAsString(communities)).build();
    }

}
