/*
 * Copyright (c) 2019-2020 "Neo4j,"
 * Neo4j Sweden AB [https://neo4j.com]
 *
 * This file is part of Neo4j.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package spring_boot;

// tag::faq.template-reactive[]

import org.junit.jupiter.api.Test;
import org.neo4j.springframework.boot.test.autoconfigure.data.ReactiveDataNeo4jTest;
import org.neo4j.springframework.data.core.ReactiveNeo4jTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.DynamicPropertyRegistry;
import org.springframework.test.context.DynamicPropertySource;
import org.testcontainers.containers.Neo4jContainer;
import org.testcontainers.junit.jupiter.Container;
import org.testcontainers.junit.jupiter.Testcontainers;
import reactor.test.StepVerifier;
import top.chendaye666.domain.MovieEntity;
import top.chendaye666.domain.PersonEntity;
import top.chendaye666.domain.Roles;

import static java.util.Collections.singletonList;

// end::faq.template-reactive[]

/**
 * @author Michael J. Simons
 */
// tag::faq.template-reactive[]
@Testcontainers
@ReactiveDataNeo4jTest
class ReactiveTemplateExampleTest {

	@Container
	private static Neo4jContainer<?> neo4jContainer = new Neo4jContainer<>("neo4j:4.1");

	@DynamicPropertySource
	static void neo4jProperties(DynamicPropertyRegistry registry) {
		registry.add("org.neo4j.driver.uri", neo4jContainer::getBoltUrl);
		registry.add("org.neo4j.driver.authentication.username", () -> "neo4j");
		registry.add("org.neo4j.driver.authentication.password", neo4jContainer::getAdminPassword);
	}

	@Test
	void shouldSaveAndReadEntities(@Autowired ReactiveNeo4jTemplate neo4jTemplate) {

		MovieEntity movie = new MovieEntity(
			"The Love Bug",
			"A movie that follows the adventures of Herbie, Herbie's driver, "
				+ "Jim Douglas (Dean Jones), and Jim's love interest, "
				+ "Carole Bennett (Michele Lee)", 1997);

		movie.getActorsAndRoles().put(new PersonEntity(1931, "Dean Jones"), new Roles(singletonList("Didi")));
		movie.getActorsAndRoles().put(new PersonEntity(1942, "Michele Lee"), new Roles(singletonList("Michi")));

		StepVerifier.create(neo4jTemplate.save(movie))
			.expectNextCount(1L)
			.verifyComplete();

		StepVerifier.create(neo4jTemplate
			.findById("Dean Jones", PersonEntity.class)
			.map(PersonEntity::getBorn)
		)
			.expectNext(1931)
			.verifyComplete();

		StepVerifier.create(neo4jTemplate.count(PersonEntity.class))
			.expectNext(2L)
			.verifyComplete();
	}
}
// end::faq.template-reactive[]
