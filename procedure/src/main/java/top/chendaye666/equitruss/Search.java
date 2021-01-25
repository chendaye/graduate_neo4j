package top.chendaye666.equitruss;

import org.neo4j.dbms.api.DatabaseManagementService;
import org.neo4j.graphdb.*;
import org.neo4j.logging.Log;
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
     * todo: MATCH (u:Author) CALL top.chendaye666.equitruss.getNeighbor(u) YIELD community RETURN community
     * This procedure takes a Node and gets the relationships going in and out of it
     *
     * 返回一行
     *
     * @param node  The node to get the relationships for
     * @return  A RelationshipTypes instance with the relations (incoming and outgoing) for a given node.
     */
    @Procedure(value = "top.chendaye666.equitruss.getNeighbor")
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
    @Procedure(value = "top.chendaye666.equitruss.neighborField")
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


    @Procedure(value = "top.chendaye666.equitruss.search")
    @Description("Get community by equitruss.")
    public Stream<Equitruss> search(@Name("node") Node node, int k_value, int attr_count, int selection) {
        if (node == null) return null;
        int node_id = (int) node.getId(); // 查询的节点id
        // 获取数据
        String query = "match res=(p:Author)-[r1:Article]-(p1:Author) where id(p)="+node_id+" return p,p1";
        String[] path = DataUtils.communityGenerate(db, query, node_id);
        //调用 Jni： relationship.txt node.txt vertex  resultPath query_k attr_cnt algo_type
        JniUtil jni = new JniUtil();
        String ans = jni.query(path[0], path[1], node_id, path[2], k_value, attr_count, selection);
        //        ans = "4:3:0.000023:0:1,2:1,2,3,4#1,2";
        ArrayList<int[]> ints = DataUtils.parseCommunity(ans);
        if (ints.size() == 3){
            StringBuilder current_id_str = new StringBuilder();
            current_id_str.append('[');
            for (int current_id : ints.get(2)){
                current_id_str.append(String.valueOf(current_id)+',');
            }
            current_id_str.deleteCharAt(current_id_str.length() - 1);
            current_id_str.append(']');
            writeCommunity(db, node_id, k_value, attr_count, selection, current_id_str.toString(), ans.split(":")[5].split("#")[1]);
        }
        ArrayList<Equitruss> res = new ArrayList<>();

        return res.stream();
    }

    // 设置社区编号
    public void writeCommunity(GraphDatabaseService db, int node_id, int k_value, int attr_count, int selection, String current_id_str, String attr_str){
        String query = "match (p:Author) where id(p) in "+current_id_str+"  set p.community_"+node_id+"_"+k_value+"_"+attr_count+"_"+selection+"="+node_id+", p.common_attribute_"+node_id+"='"+attr_str+"'";
        System.out.println(query);
        try (Transaction tx = db.beginTx()){
            tx.execute(query);
            tx.commit();
        }
    }

    /**
     * This is the output record for our search procedure. All procedures
     * that return results return them as a Stream of Records, where the
     * records are defined like this one - customized to fit what the procedure
     * is returning.
     * <p>
     * These classes can only have public non-final fields, and the fields must
     * be one of the following types:
     *
     * <ul>
     *     <li>{@link String}</li>
     *     <li>{@link Long} or {@code long}</li>
     *     <li>{@link Double} or {@code double}</li>
     *     <li>{@link Number}</li>
     *     <li>{@link Boolean} or {@code boolean}</li>
     *     <li>{@link Node}</li>
     *     <li>{@link Relationship}</li>
     *     <li>{@link Path}</li>
     *     <li>{@link Map} with key {@link String} and value {@link Object}</li>
     *     <li>{@link List} of elements of any valid field type, including {@link List}</li>
     *     <li>{@link Object}, meaning any of the valid field types</li>
     * </ul>
     */
    public static class Equitruss {
        public Long id;
        public String authorId;
        public String name;
        public List<Long> community;
        public List<String> words;

        public Equitruss(Long id, String authorId, String name, List<Long> community, List<String> words) {
            this.id = id;
            this.authorId = authorId;
            this.name = name;
            this.community = community;
            this.words = words;
        }

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