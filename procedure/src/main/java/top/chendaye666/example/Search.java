package top.chendaye666.example;

import org.neo4j.graphdb.*;
import org.neo4j.procedure.Context;
import org.neo4j.procedure.Description;
import org.neo4j.procedure.Name;
import org.neo4j.procedure.Procedure;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.stream.Stream;

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
     * todo: MATCH (u:Author) CALL top.chendaye666.example.getNeighbor(u) YIELD community RETURN community
     * This procedure takes a Node and gets the relationships going in and out of it
     *
     * 返回一行
     *
     * @param node  The node to get the relationships for
     * @return  A RelationshipTypes instance with the relations (incoming and outgoing) for a given node.
     */
    @Procedure(value = "top.chendaye666.example.getNeighbor")
    @Description("Get community by equitruss.")
    public Stream<Neighbor> getNeighbor(@Name("node") Node node) {
        ArrayList<String> neighbors = new ArrayList<>();
        ArrayList<String> words = new ArrayList<>();
        try(Transaction tx = db.beginTx()) {
            String query = "match (p:Author) where id(p) < 20 return p";
            Result result = tx.execute(query);
            while (result.hasNext()){
                Map<String,Object> row = result.next();
                for ( Map.Entry<String,Object> column : row.entrySet() ){
                    Node author = (Node) column.getValue();
                    neighbors.add((String) author.getProperty("authorId"));
                    words.add((String) author.getProperty("words"));
                }
            }
        }
        return Stream.of(new Neighbor(neighbors, words));
    }

    /**
     * 返回一个table，每一行对应一个节点数据
     * @param node
     * @return
     */
    @Procedure(value = "top.chendaye666.example.neighborField")
    @Description("Get community by equitruss.")
    public Stream<NeighborField> neighborField(@Name("node") Node node) {
        ArrayList<NeighborField> ans = new ArrayList<>();
        try(Transaction tx = db.beginTx()) {
            String query = "match (p:Author) where id(p) < 20 return p";
            Result result = tx.execute(query);
            while (result.hasNext()){
                Map<String,Object> row = result.next();
                for ( Map.Entry<String,Object> column : row.entrySet() ){
                    Node author = (Node) column.getValue();
                    ans.add(new NeighborField((String) author.getProperty("authorId"), (String) author.getProperty("words")));
                }
            }
        }
        return ans.stream();
    }





    public static class NeighborField{
        public String authorId;
        public String word;
        public NeighborField(String authorId, String word){
            this.authorId = authorId;
            this.word = word;
        }

        @Override
        public String toString() {
            return "NeighborField{" +
                    "authorId='" + authorId + '\'' +
                    ", word='" + word + '\'' +
                    '}';
        }
    }

    public static class Neighbor{
        public List<String> neighbor;
        public List<String> word;
        public Neighbor(List<String> neighbor, List<String> word){
            this.neighbor = neighbor;
            this.word = word;
        }
    }
    
}
