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

import org.neo4j.springframework.data.core.schema.*;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static org.neo4j.springframework.data.core.schema.Relationship.Direction.INCOMING;


/**
 * @author top.chendaye666
 */

@Node("Movie") // <.>
public class MovieEntity {

	// 使用Neo4j 内部ID
	@Id @GeneratedValue
	private Long id;

	private final String title;

	private Integer  released;

	// @Property 给属性取别名
	@Property("tagline")
	private final String description;

	//	This is the constructor to be used by your application code
	public MovieEntity(String title, String description, Integer released) {
		this.description = description;
		this.title = title;
		this.released = released;
	}


	// This defines a relationship to a class of type PersonEntity and the relationship type ACTED_IN
	// tag::mapping.relationship.properties[]  关系 ACTED_IN 有属性 roles
	@Relationship(type = "ACTED_IN", direction = INCOMING)
	private Map<PersonEntity, Roles> actorsAndRoles = new HashMap<>();

	// 导演
	@Relationship(type = "DIRECTED", direction = INCOMING)
	private List<PersonEntity> directors = new ArrayList<>();

	// 作者
	@Relationship(type = "PRODUCED", direction = INCOMING)
	private List<PersonEntity> produces = new ArrayList<>();

	// 影评
	@Relationship(type = "REVIEWED", direction = INCOMING)
	private List<PersonEntity> previews = new ArrayList<>();

	// 编剧
	@Relationship(type = "WROTE", direction = INCOMING)
	private List<PersonEntity> writes = new ArrayList<>();


	public Long getId() {
		return id;
	}

	public String getTitle() {
		return title;
	}

	public Integer getReleased(){
		return released;
	}

	public String getDescription() {
		return description;
	}

	public Map<PersonEntity, Roles> getActorsAndRoles() {
		return actorsAndRoles;
	}

	public List<PersonEntity> getDirectors() {
		return directors;
	}

	public List<PersonEntity> getProduces(){
		return produces;
	}

	public List<PersonEntity> getPreviews() {
		return previews;
	}

	public List<PersonEntity> getWrites() {
		return writes;
	}

}
