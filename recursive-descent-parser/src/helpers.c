#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Including own header for checking by compiler */
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

int isLeftParen(char c)
{
  return c == '(';
}

int isRightParen(char c)
{
  return c == ')';
}

int isEqual(char c)
{
  return c == '=';
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

int isSemicolon(char c)
{
  return c == ';';
}

int isDoubleQuotedString(char c)
{
  return c == '"';
}

int isWhiteSpace(char c)
{
  return c == ' ';
}

void eatWhiteSpace(char **input)
{
  while (isWhiteSpace(**input))
    eat(input);
}

void eatNewline(char **input)
{
  while (isNewline(**input))
    eat(input);
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
  return *(*input + 1);
}
