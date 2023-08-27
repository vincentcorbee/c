#include <stdlib.h>

#define token_IMPORT

#include "token.h"

void freeToken(Token *token)
{
  if (token->value != NULL)
    free(token->value);

  free(token);
}