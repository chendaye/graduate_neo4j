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
 */
public class Search {
    @Context
    public GraphDatabaseService db;
    public Search(){}


    /**
     * todo: MATCH (u:Author) CALL top.chendaye666.equitruss.getNeighbor(u) YIELD community RETURN community
     * This procedure takes a Node and gets the relationships going in and out of it
     *
     * @param node  The node to get the relationships for
     * @return  A RelationshipTypes instance with the relations (incoming and outgoing) for a given node.
     */
    @Procedure(value = "top.chendaye666.equitruss.getNeighbor")
    @Description("Get community by equitruss.")
    public Stream<Equitruss> getNeighbor(@Name("node") Node node) {
        ArrayList<Node> nodes = new ArrayList<>();
        try(Transaction tx = db.beginTx()) {
            String query = "match (p:Author) where id(p) < 2 return p";
            Result execute = tx.execute(query);
            while (execute.hasNext()){
                Map<String,Object> row = execute.next();
                for ( Map.Entry<String,Object> column : row.entrySet() ){
                    Node author = (Node) column.getValue();
                    nodes.add(author);
                }
            }
        }
        return Stream.of(new Equitruss(nodes));
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
        public List<Node> community;

        public Equitruss(List<Node> community) {
            this.community = community;
        }
    }
}
