package server.unmanaged;

import org.junit.Rule;
import org.junit.Test;

import java.io.IOException;
import java.util.List;
import java.util.Map;

//import org.neo4j.doc.server.HTTP;
import org.neo4j.harness.junit.rule.Neo4jRule;
import org.neo4j.test.server.HTTP;
import top.chendaye666.unmanaged.ColleaguesCypherExecutionResource;
import top.chendaye666.unmanaged.ColleaguesResource;

import static junit.framework.TestCase.assertEquals;
import static org.hamcrest.CoreMatchers.equalTo;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.core.IsCollectionContaining.hasItem;

public class UnmanagedExtensionsDocIT
{
    @Rule
    public Neo4jRule neo4j = new Neo4jRule()
            .withFixture( "UNWIND ['Keanu Reeves','Hugo Weaving','Carrie-Anne Moss','Laurence Fishburne'] AS actor " +
                    "MERGE (m:Movie  {name: 'The Matrix'}) " +
                    "MERGE (p:Person {name: actor}) " +
                    "MERGE (p)-[:ACTED_IN]->(m) " )
            .withUnmanagedExtension( "/path/to/my/extension1", ColleaguesCypherExecutionResource.class )
            .withUnmanagedExtension( "/path/to/my/extension2", ColleaguesResource.class );

    @Test
    public void shouldRetrieveColleaguesViaExecutionEngine() throws IOException
    {
        // When
        HTTP.Response response = HTTP.GET( neo4j.httpURI().resolve(
                "/path/to/my/extension1/colleagues-cypher-execution/Keanu%20Reeves" ).toString() );

        // Then
        assertEquals( 200, response.status() );

        Map<String, Object> content = response.content();
        List<String> colleagues = (List<String>) content.get( "colleagues" );

        assertThat( colleagues.size(), equalTo( 3 ) );
        assertThat( colleagues, hasItem( "Laurence Fishburne" ) );
        assertThat( colleagues, hasItem( "Hugo Weaving" ) );
        assertThat( colleagues, hasItem( "Carrie-Anne Moss" ) );
    }

    @Test
    public void shouldRetrieveColleaguesViaTransactionAPI() throws IOException
    {
        // When
        HTTP.Response response = HTTP.GET( neo4j.httpURI().resolve(
                "/path/to/my/extension2/colleagues/Keanu%20Reeves" ).toString() );

        // Then
        assertEquals( 200, response.status() );

        Map<String, Object> content = response.content();
        List<String> colleagues = (List<String>) content.get( "colleagues" );

        assertThat( colleagues.size(), equalTo( 3 ) );
        assertThat( colleagues, hasItem( "Laurence Fishburne" ) );
        assertThat( colleagues, hasItem( "Hugo Weaving" ) );
        assertThat( colleagues, hasItem( "Carrie-Anne Moss" ) );
    }
}
