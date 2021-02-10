package top.chendaye666.equitruss.streams;

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
public class Equitruss {
    public Long id;
    public String authorId;
    public String name;
    public Long count;
    public String community;
    public String words;
    public String raw;

    public Equitruss(Long id, String authorId, String name, Long count, String community, String words, String raw) {
        this.id = id;
        this.authorId = authorId;
        this.name = name;
        this.count = count;
        this.community = community;
        this.words = words;
        this.raw = raw;
    }
}
