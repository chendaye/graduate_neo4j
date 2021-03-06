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
package top.chendaye666.domain;

// tag::mapping.annotations[]
import org.neo4j.springframework.data.core.schema.GeneratedValue;
import org.neo4j.springframework.data.core.schema.Id;
import org.neo4j.springframework.data.core.schema.Node;

// end::mapping.annotations[]

/**
 * @author top.chendaye666
 */
// tag::mapping.annotations[]
@Node("Person")
public class PersonEntity {
	// 使用Neo4j 内部ID
	@Id @GeneratedValue
	private Long id;

//	@Id
	private final String name;

	private final Integer born;

	public PersonEntity(Integer born, String name) {
		this.born = born;
		this.name = name;
	}

	public Long getId() {
		return id;
	}
	public Integer getBorn() {
		return born;
	}

	public String getName() {
		return name;
	}

}
// end::mapping.annotations[]
