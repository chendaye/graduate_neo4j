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
package top.chendaye666.support;

import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.core.JsonGenerator;
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.*;
import com.fasterxml.jackson.databind.annotation.JsonDeserialize;
import com.fasterxml.jackson.databind.annotation.JsonSerialize;
import com.fasterxml.jackson.databind.module.SimpleModule;
import org.springframework.stereotype.Component;
import top.chendaye666.domain.MovieEntity;
import top.chendaye666.domain.PersonEntity;
import top.chendaye666.domain.Roles;

import java.io.IOException;
import java.util.List;
import java.util.Map;

/**
 * A jackson module supporting our domain package.
 *
 * @author chendaye
 */
@Component
public class MovieModule extends SimpleModule {

	public MovieModule() {

		setMixInAnnotation(MovieEntity.class, MovieEntityMixin.class);
		setMixInAnnotation(PersonEntity.class, PersonEntityMixin.class);

		addDeserializer(Roles.class, new RoleDeserializer());
	}

	@JsonInclude(JsonInclude.Include.NON_EMPTY)
	abstract static class MovieEntityMixin {
		@JsonCreator MovieEntityMixin(@JsonProperty("title") final String title,
			@JsonProperty("description") final String description) {
		}

		@JsonDeserialize(keyUsing = PersonEntityAsKeyDeSerializer.class)
		@JsonSerialize(keyUsing = PersonEntityAsKeySerializer.class, contentUsing = RolesAsContentSerializer.class)
		abstract Map<PersonEntity, Roles> getActorsAndRoles();

	}

	@JsonInclude(JsonInclude.Include.NON_EMPTY)
	abstract static class PersonEntityMixin {
		@JsonCreator PersonEntityMixin(@JsonProperty("name") final String name, @JsonProperty("born") final Long born) {
		}
	}

	static class RoleDeserializer extends JsonDeserializer<Roles> {

		@Override
		public Roles deserialize(JsonParser jsonParser,
			DeserializationContext deserializationContext) throws IOException {

			return new Roles(jsonParser.readValueAs(new TypeReference<List<String>>() {
			}));
		}
	}

	static class PersonEntityAsKeyDeSerializer extends KeyDeserializer {

		@Override
		public Object deserializeKey(String key, DeserializationContext ctxt) {
			return new PersonEntity(null, key);
		}
	}

	static class PersonEntityAsKeySerializer extends JsonSerializer<PersonEntity> {

		@Override
		public void serialize(PersonEntity personEntity, JsonGenerator jsonGenerator,
			SerializerProvider serializerProvider) throws IOException {
			jsonGenerator.writeFieldName(personEntity.getName());
		}
	}

	static class RolesAsContentSerializer extends JsonSerializer<Roles> {
		@Override
		public void serialize(Roles roles, JsonGenerator jsonGenerator,
			SerializerProvider serializerProvider) throws IOException {
			jsonGenerator.writeObject(roles.getRoles());
		}
	}
}
