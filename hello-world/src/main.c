#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <string.h>

// typedef struct Person
//   {
//     char * name;
//     int age;
//   } Person;

// void displayPerson(Person person);

// void displayPerson(Person person) {
//   printf("Hello %s your age is %d\n", person.name, person.age);
// }

// char chars[] = "abcdefg";

// int main() {
//   int num = 5;
//   int index = 0;

//   printf("Date :%s\n", __DATE__ );
//    printf("Time :%s\n", __TIME__ );

//   Person * persons = NULL;

//   while (index < num) {
//     Person person;

//     char name[] = { chars[index], '\0' };

//     person.name = malloc(1 + sizeof(char));

//     strcpy(person.name, name);

//     person.age = 25 + index;

//     persons = realloc(persons, (index + 1) * sizeof(Person));

//     persons[index++] = person;
//   }

//   index = 0;

//   while (index < num) {
//     Person person = persons[index++];

//     // printf("%p\n", &person);

//     displayPerson(person);

//     free(person.name);
//   }

//   printf("%s\n", chars);

//   free(persons);

//   return 0;
// }

#define INITIAL_SIZE 10

int statement(char ** input);

int expression(char ** input);

int additiveExpression(char ** input);

int numericLiteral(char ** input);

int isInteger(char ** c);

void eat(char ** input);

int isWhiteSpace(char ** c);

void eatWhiteSpace(char ** input);


int isInteger(char ** c) {
  return **c >= '0' && **c <= '9';
}

int isWhiteSpace(char ** c) {
  return **c == ' ';
}

void eatWhiteSpace(char ** input) {
  while(isWhiteSpace(input)) eat(input);
}

void eat(char ** input) {
  (*input)++;
}

/*
  Statement: Expression
*/

int statement(char ** input) {
  if (expression(input)) return 1;

  return 0;
}

/*
  Expression : AdditiveExpression
*/
int expression(char ** input) {
  if (additiveExpression(input)) return 1;

  return 0;
}

/*
  AdditiveExpression : NumericLiteral "+" NumericLiteral
*/
int additiveExpression(char ** input) {
  if (numericLiteral(input)) {
    if(**input == '+') {
      eat(input);

      if(numericLiteral(input)) return 1;
    }
  }

  return 0;
}

/*
  NumericLiteral : [0-9]+
*/

int numericLiteral(char ** input) {
  if (!isInteger(input)) return 0;

  while(isInteger(input)) eat(input);

  return 1;
}

typedef struct Node {
  void * data;
} Node;

int main() {
  char * input = malloc(INITIAL_SIZE * sizeof(char));

  if (input == NULL) return 1;

  int c;

  size_t length = 0;
  size_t capacity = INITIAL_SIZE;

  while((c = getchar()) != EOF && c != '\n') {
    if(c != ' ') input[length++] = c;

    if (length >= capacity) {
      capacity *= 2;

      char * newInput = realloc(input, capacity * sizeof(char));

      if (newInput == NULL) {
        free(input);

        return 1;
      }

      input = newInput;
    }
  }

  input[length] = '\0';

  char *originalInput = input;

  // printf("%s\n", input);

  int result = statement(&input);

  // printf("%d\n", result);

  if(result) printf("input is a statement: %s\n", originalInput);

  free(originalInput);

  input = NULL;

  return 0;
}