#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token.h"

#define helpers_IMPORT

#include "helpers.h"

/* Public functions */

int isEOF(char c)
{
  return c == EOF || c == '\0';
}

int hasData(char **input)
{
  return strlen(*input) > 0;
}

int isInteger(char c)
{
  return c >= '0' && c <= '9';
}

int isAsciiAz(char c)
{
  return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

int isIdentifierStart(char c)
{
  return isAsciiAz(c) || c == '$';
}

int isNewline(char c)
{
  return c == '\n';
}

int isEqual(char c)
{
  return c == '=';
}

int isRightAngleBracket(char c)
{
  return c == '>';
}

int isLeftAngleBracket(char c)
{
  return c == '<';
}

int isNot(char c)
{
  return c == '!';
}

int isPlus(char c)
{
  return c == '+';
}

int isMinus(char c)
{
  return c == '-';
}

int isMultiply(char c)
{
  return c == '*';
}

int isBinaryAnd(char c)
{
  return c == '&';
}

int isBinaryOr(char c)
{
  return c == '|';
}

int isDoubleQuotedString(char c)
{
  return c == '"';
}

int isWhiteSpace(char c)
{
  return c == ' ';
}

int eatWhiteSpace(char **input)
{
  int count = 0;

  while (isWhiteSpace(**input))
  {
    eat(input);

    count++;
  }

  return count;
}

int eatNewline(char **input)
{
  int count = 0;

  while (isNewline(**input))
  {
    eat(input);

    count++;
  }

  return count;
}

void eatWhiteSpaceAndNewline(char **input)
{
  while (isWhiteSpace(**input) || isNewline(**input))
    eat(input);
}

char eat(char **input)
{
  return *(*input)++;
}

char peek(char **input)
{
  return **input;
}

char peekAt(char **input, int pos)
{
  return *(*input + pos);
}

char skip(char **input, int count)
{
  return **input + count;
}
