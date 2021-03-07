#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

/* where i can make the preprocessor (e.g: define Class struct) */

struct Person {
   char name[10];
   uint8_t age;
   uint8_t height;
   void(*show)(struct Person *);
};

struct Person *create_person(
      char name[10], 
      uint8_t age, 
      uint8_t height
);

void show_person(struct Person *person);

int
main() {
   struct Person *gabriel = (struct Person *) create_person("gabriel", 21, 174);
   gabriel->show(gabriel);

   return EXIT_SUCCESS;
}

/* person == self */
struct Person
*create_person(
      char name[10], 
      uint8_t age, 
      uint8_t height
) {
   struct Person *person = (struct Person *) malloc(sizeof(struct Person));

   strcpy(person->name, name);
   person->age = age;
   person->height = height;

   person->show = &show_person;

   return person;
};

/* person == self */
void
show_person(struct Person *person) {
   printf("name: %s, age: %d, height: %d\n", 
         person->name, 
         person->age, 
         person->height
   );
}
