#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

struct Person {
   char name[10];
   uint8_t age;
   uint8_t height;
};

void create_person(
      char name[10], 
      uint8_t age, 
      uint8_t height, 
      void(*build_person)(struct Person)
);

void show_person(struct Person person);

int
main() {
   create_person("gabriel", 21, 174, &show_person);

   return EXIT_SUCCESS;
}

void
create_person(
      char name[10], 
      uint8_t age, 
      uint8_t height, 
      void(*build_person)(struct Person)
) {
   struct Person person;
   strcpy(person.name, name);
   person.age = age;
   person.height = height;
   (*build_person)(person);
};

void
show_person(struct Person person) {
   printf("name: %s, age: %d, height: %d\n", 
         person.name, 
         person.age, 
         person.height
   );
}
