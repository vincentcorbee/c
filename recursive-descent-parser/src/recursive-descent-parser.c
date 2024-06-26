#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <string.h>

#include "parser.h"
#include "arguments.h"

#define INITIAL_SIZE 10

void tokenize(Lexer *lexer)
{
  Token *token = lexer->next(lexer);

  // token = peekToken(lexer);

  // if(token != NULL) {
  //   printf("token: %s\n", token->value);
  //   free(token->value);
  //   free(token);
  // }

  while (token && token->type != TokenEOF)
  {
    if (token)
    {
      freeToken(token);
      printf("token: %d\n", token->type);
    }

    token = lexer->next(lexer);
  }

  printf("column %d\n", lexer->col);

  free(lexer);
}

int main(int argc, char *argv[])
{
  char *input = malloc(INITIAL_SIZE * sizeof(char));

  if (input == NULL)
    return 1;

  Arguments *args = getArguments(argc, argv);

  FILE *ptr = args->file == NULL ? stdin : fopen(args->file, "r");

  size_t length = 0;
  size_t capacity = INITIAL_SIZE;

  int c;

  while ((c = fgetc(ptr)) != EOF)
  {
    input[length++] = c;

    if (length >= capacity)
    {
      capacity *= 2;

      char *newInput = realloc(input, capacity * sizeof(char));

      if (newInput == NULL)
      {
        free(input);

        free(args);

        fclose(ptr);

        return 1;
      }

      input = newInput;
    }
  }

  fclose(ptr);

  input[length] = '\0';

  char *originalInput = input;

  Node *result = parse(&input);

  visitNode(result, 0);

  freeNode(result);

  free(originalInput);

  free(args);

  input = NULL;

  // char *test = "\n\n\n\n\n\n\n\n";

  // Lexer *lexer = lexerFactory(&test);

  // tokenize(lexer);

  return 0;
}
