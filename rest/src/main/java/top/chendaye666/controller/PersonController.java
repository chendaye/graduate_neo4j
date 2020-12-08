package top.chendaye666.controller;

import org.springframework.web.bind.annotation.*;
import reactor.core.publisher.Flux;
import reactor.core.publisher.Mono;
import top.chendaye666.domain.PersonEntity;
import top.chendaye666.repositories.PersonRepository;

@RestController
@RequestMapping("/persons")
public class PersonController {
    private final PersonRepository personRepository;

    public PersonController(PersonRepository personRepository){
        this.personRepository = personRepository;
    }

    @GetMapping("/getPersonsWhoActAndDirect")
    Flux<PersonEntity> getPersonsWhoActAndDirect() {
        return personRepository.getPersonsWhoActAndDirect();
    }

    @GetMapping("/{id}")
    Mono<PersonEntity> findById(@PathVariable Long id) {
        return personRepository.findById(id);
    }

    @PostMapping("/setBorn")
    Mono<PersonEntity> setBorn(@RequestBody PersonEntity personEntity) {
        return personRepository.setBorn(personEntity.getId(), personEntity.getBorn());
    }
}
