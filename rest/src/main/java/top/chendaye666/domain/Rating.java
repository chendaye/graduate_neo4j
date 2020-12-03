package top.chendaye666.domain;

import org.neo4j.springframework.data.core.schema.RelationshipProperties;

import java.util.List;

/**
 * REVIEWED  影评 person -> movie
 *
 * 评级
 */
@RelationshipProperties
public class Rating {
    public List<String> getRating() {
        return rating;
    }

    private final List<String> rating;

    public Rating(List<String> rating) {
        this.rating = rating;
    }

   
}
