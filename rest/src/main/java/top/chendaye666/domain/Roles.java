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

import org.neo4j.springframework.data.core.schema.RelationshipProperties;

import java.util.List;

/**
 * ACTED_IN 参演  person -> movie
 *
 * 所演角色
 *
 * @author top.chendaye666
 */
// tag::mapping.relationship.properties[]
@RelationshipProperties
public class Roles {

	private final List<String> roles;

	public Roles(List<String> roles) {
		this.roles = roles;
	}

	public List<String> getRoles() {
		return roles;
	}
}
// end::mapping.relationship.properties[]
