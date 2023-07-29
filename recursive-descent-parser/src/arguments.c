#include <stdlib.h>
#include <string.h>

/* Including own header for checking by compiler */
#define arguments_IMPORT

#include "arguments.h"

/* Public functions */

Arguments *getArguments(int argc, char *argv[])
{
  Arguments *args = malloc(sizeof(Arguments));

  int index = 1;

  while (index < argc)
  {
    char *arg = argv[index++];

    if (strcmp(arg, "--file") == 0)
    {
      char *value = argv[index++];

      args->file = value;
    }
  }

  return args;
}
