#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "helpers.h"

/* Including own header for checking by compiler */
#define lexer_IMPORT

#include "lexer.h"

/* Public functions */

Lexer *lexerFactory(char **source)
{
  Lexer *lexer = malloc(sizeof(Lexer));

  lexer->col = 0;
  lexer->line = 0;
  lexer->source = source;

  lexer->next = nextToken;

  lexer->peek = peekToken;

  return lexer;
}

char *eatChar(Lexer *self, int count)
{
  char *string = malloc(count * sizeof(char) + 1);

  int index = 0;

  while (hasData(self->source) && index < count)
  {
    string[index++] = eat(self->source);

    self->col++;
  }

  string[index] = '\0';

  return string;
}

char *eatIdentiferStart(Lexer *self)
{
  int index = 0;

  char *source = *self->source;

  int col = self->col;

  char *idenifier = malloc(sizeof(char));

  while (hasData(self->source) && isAsciiAz(**self->source))
  {
    idenifier = realloc(idenifier, (index + 2) * sizeof(char));

    idenifier[index++] = eat(self->source);

    self->col++;
  }

  if (index == 0)
  {
    free(idenifier);

    self->col = col;

    *self->source = source;

    return NULL;
  }

  idenifier[index] = '\0';

  return idenifier;
}

char *eatDoubleQuotedString(Lexer *self)
{
  int index = 0;

  int col = self->col;

  char *source = *self->source;

  char *string = malloc(sizeof(char));

  string[index++] = eat(self->source);

  while (hasData(self->source) && !isDoubleQuotedString(**self->source))
  {
    string = realloc(string, (index + 1) * sizeof(char) + 2);

    string[index++] = eat(self->source);

    self->col++;
  };

  if (!isDoubleQuotedString(**self->source))
  {
    free(string);

    self->col = col;

    *self->source = source;

    return NULL;
  }

  string[index++] = eat(self->source);

  string[index] = '\0';

  return string;
}

char *eatInteger(Lexer *self)
{
  char *value = malloc(sizeof(char));

  int index = 0;

  while (hasData(self->source) && isInteger(**self->source))
  {
    value = realloc(value, (index + 1) * sizeof(char) + 1);

    value[index++] = eat(self->source);

    self->col++;
  };

  value[index] = '\0';

  return value;
}

Token *tokenFactory(Lexer *self, TokenType type)
{
  Token *token = malloc(sizeof(Token));

  token->type = type;

  switch (type)
  {
  case Integer:
    token->value = eatInteger(self);

    return token;
  case DoubleQuotedString:
    token->value = eatDoubleQuotedString(self);

    return token;
  case IdendifierStart:
    token->value = eatIdentiferStart(self);

    return token;
  case EqualEqual:
  case NotEqual:
    token->value = eatChar(self, 2);

    return token;
  case LeftParen:
  case RightParen:
  case Multiply:
  case Plus:
  case Minus:
  case Equal:
  case Not:
  case Semicolon:
    token->value = eatChar(self, 1);

    return token;
  default:
    free(token);

    return NULL;
  }
}

Token *nextToken(Lexer *self)
{
  if (!hasData(self->source))
    return NULL;

  eatWhiteSpaceAndNewline(self->source);

  char nextChar = peek(self->source);

  if (isInteger(nextChar))
    return tokenFactory(self, Integer);

  if (isDoubleQuotedString(nextChar))
    return tokenFactory(self, DoubleQuotedString);

  if (isIdentifierStart(nextChar))
    return tokenFactory(self, IdendifierStart);

  if (isLeftParen(nextChar))
    return tokenFactory(self, LeftParen);

  if (isLeftParen(nextChar))
    return tokenFactory(self, RightParen);

  if (isPlus(nextChar))
    return tokenFactory(self, Plus);

  if (isMinus(nextChar))
    return tokenFactory(self, Minus);

  if (isMultiply(nextChar))
    return tokenFactory(self, Multiply);

  if (isNot(nextChar))
  {
    if (isEqual(peekAt(self->source, 1)))
      return tokenFactory(self, NotEqual);

    return tokenFactory(self, Not);
  }

  if (isEqual(nextChar))
  {
    if (isEqual(peekAt(self->source, 1)))
      return tokenFactory(self, EqualEqual);

    return tokenFactory(self, Equal);
  }

  if (isSemicolon(nextChar))
    return tokenFactory(self, Semicolon);

  return NULL;
}

Token *peekToken(Lexer *self)
{
  char *source = *self->source;
  int col = self->col;

  Token *token = nextToken(self);

  *self->source = source;

  self->col = col;

  return token;
}
