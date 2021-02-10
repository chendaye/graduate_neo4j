package top.chendaye666.simrank.streams;


import org.neo4j.graphdb.Node;
import org.neo4j.graphdb.Path;
import org.neo4j.graphdb.Relationship;

import java.util.List;
import java.util.Map;

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
public class Center {
    public String nid;
    public String oid;
    public String value;

    public Center(String nid, String oid, String value){
        this.nid = nid;
        this.oid = oid;
        this.value = value;
    }
}
