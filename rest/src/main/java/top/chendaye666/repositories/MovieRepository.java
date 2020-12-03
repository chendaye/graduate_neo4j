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
package top.chendaye666.repositories;

// tag::getting.started[]

import org.neo4j.driver.internal.shaded.reactor.util.annotation.Nullable;
import org.neo4j.springframework.data.repository.ReactiveNeo4jRepository;
import reactor.core.publisher.Mono;
import top.chendaye666.domain.MovieEntity;

// end::getting.started[]

/**
 * @author chendaye
 *
 * 过滤方法
 * https://docs.spring.io/spring-data/neo4j/docs/current/reference/html/#repositories.definition
 */
// tag::getting.started[]
public interface MovieRepository extends ReactiveNeo4jRepository<MovieEntity, Long> {
	Mono<MovieEntity> findOneByTitle(String title);

	@Nullable
	Mono<MovieEntity> findByTitle(String title);
}
// end::getting.started[]
