#include <stdlib.h>

#define token_IMPORT

#include "token.h"

void freeToken(Token *token)
{
  free(token->value);

  free(token);
}