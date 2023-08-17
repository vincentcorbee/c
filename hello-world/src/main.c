#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <string.h>

typedef struct Person
{
  char *name;
  int age;
} Person;

void displayPerson(Person person);

void displayPerson(Person person)
{
  printf("Hello %s your age is %d\n", person.name, person.age);
}

char chars[] = "abcdefg";

int main()
{
  int num = 5;
  int index = 0;

  printf("Date :%s\n", __DATE__);
  printf("Time :%s\n", __TIME__);

  Person *persons = NULL;

  while (index < num)
  {
    Person person;

    char name[] = {chars[index], '\0'};

    person.name = malloc(1 + sizeof(char));

    strcpy(person.name, name);

    person.age = 25 + index;

    persons = realloc(persons, (index + 1) * sizeof(Person));

    persons[index++] = person;
  }

  index = 0;

  while (index < num)
  {
    Person person = persons[index++];

    // printf("%p\n", &person);

    displayPerson(person);

    free(person.name);
  }

  printf("%s\n", chars);

  free(persons);

  return 0;
}
