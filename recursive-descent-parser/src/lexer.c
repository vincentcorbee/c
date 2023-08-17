#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "helpers.h"

/* Including own header for checking by compiler */
#define lexer_IMPORT

#include "lexer.h"

/* Public functions */

Keywords keywords = {"func", "const", "let", "public", "private", "type", "string", "number"};

Lexer *lexerFactory(char **source)
{
  Lexer *lexer = malloc(sizeof(Lexer));

  lexer->index = 0;
  lexer->col = 1;
  lexer->line = 1;

  lexer->source = source;

  lexer->next = nextToken;

  lexer->peek = peekToken;

  lexer->eatChar = eatChar;

  lexer->skip = skipToken;

  lexer->advance = advanceToken;

  return lexer;
}

LexerState getLexerState(Lexer *self)
{
  LexerState state = {.source = *self->source, .line = self->line, .col = self->col, .index = self->index};

  return state;
}

void setLexerState(Lexer *self, LexerState state)
{
  self->col = state.col;
  self->line = state.line;
  self->index = state.index;
  *self->source = state.source;
}

char *eatChar(Lexer *self, int count)
{
  char *buffer = malloc(count * sizeof(char) + 1);

  int index = 0;

  while (hasData(self->source) && index < count)
  {
    buffer[index++] = eat(self->source);

    self->col++;

    self->index++;
  }

  buffer[index] = '\0';

  return buffer;
}

char *eatIdentiferStart(Lexer *self)
{
  int index = 0;

  char *source = *self->source;

  int col = self->col;

  int previousIndex = self->index;

  char *idenifier = malloc(sizeof(char));

  while (hasData(self->source) && isAsciiAz(**self->source))
  {
    idenifier = realloc(idenifier, (index + 1) * sizeof(char) + 1);

    idenifier[index++] = eat(self->source);

    self->col++;

    self->index++;
  }

  if (index == 0)
  {
    free(idenifier);

    self->col = col;

    self->index = previousIndex;

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

  int previousIndex = self->index;

  char *source = *self->source;

  char *string = malloc(sizeof(char));

  string[index++] = eat(self->source);

  while (hasData(self->source) && !isDoubleQuotedString(**self->source))
  {
    string = realloc(string, (index + 1) * sizeof(char) + 2);

    string[index++] = eat(self->source);

    self->col++;

    self->index++;
  };

  if (!isDoubleQuotedString(**self->source))
  {
    free(string);

    self->col = col;

    self->index = previousIndex;

    *self->source = source;

    return NULL;
  }

  string[index++] = eat(self->source);

  string[index] = '\0';

  return string;
}

char *eatKeyword(Lexer *self, Keyword keywordType)
{
  const char *keyword = keywords[keywordType];

  return eatChar(self, strlen(keyword));
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

    self->index++;
  };

  value[index] = '\0';

  return value;
}

Token *tokenFactory(Lexer *self, TokenType type, char *value)
{
  Token *token = malloc(sizeof(Token));

  token->type = type;

  switch (type)
  {
  case TokenFunc:
  case TokenKeywordPublic:
  case TokenKeywordPrivate:
  case TokenTypeAlias:
  case TokenKeywordString:
  case TokenKeywordNumber:
  case TokenIdendifierStart:
  case TokenKeywordLet:
  case TokenKeywordConst:
    token->value = value;
    return token;
  case TokenInteger:
    token->value = eatInteger(self);
    return token;
  case TokenDoubleQuotedString:
    token->value = eatDoubleQuotedString(self);
    return token;
  case TokenLogicalAnd:
  case TokenLogicalOr:
  case TokenEqualEqual:
  case TokenNotEqual:
  case TokenFatArrow:
    token->value = eatChar(self, 2);
    return token;
  case TokenLeftParen:
  case TokenRightParen:
  case TokenMultiply:
  case TokenPlus:
  case TokenMinus:
  case TokenEqual:
  case TokenNot:
  case TokenSemicolon:
  case TokenBinaryOr:
  case TokenBinaryAnd:
  case TokenColon:
  case TokenComma:
  case TokenLeftAngleBracket:
  case TokenRightAngleBracket:
  case TokenLeftCurlyBrace:
  case TokenRightCurlyBrace:
  case TokenLeftBracket:
  case TokenRightBracket:
  case TokenOptional:
    token->value = eatChar(self, 1);
    return token;
  default:
    free(token);

    return NULL;
  }
}

TokenType peekToken(Lexer *self)
{
  if (!hasData(self->source))
    return -1;

  char *source = *self->source;
  int col = self->col;

  Token *token = nextToken(self);

  *self->source = source;

  self->col = col;

  if (!token)
    return -1;

  TokenType type = token->type;

  freeToken(token);

  return type;
}

void skipToken(Lexer *self, int count)
{
  while (count)
  {
    Token *token = nextToken(self);

    if (token == NULL)
      break;

    count--;

    freeToken(token);
  }
}

void advanceToken(Lexer *self)
{
  self->skip(self, 1);
}

int isKeyword(Lexer *self, Keyword keywordType)
{
  const char *keyword = keywords[keywordType];

  size_t length = strlen(keyword);

  int i;

  for (i = 0; i < length; i++)
  {
    if (!hasData(self->source) || peekAt(self->source, i) != keyword[i])
      return 0;
  }

  return 1;
}

int isKeywordTypeAlias(Lexer *self)
{
  return isKeyword(self, KeywordTypeAlias);
}

int isKeywordFunc(Lexer *self)
{
  return isKeyword(self, KeywordFunc);
}

int isKeywordPublic(Lexer *self)
{
  return isKeyword(self, KeywordPublic);
}

int isKeywordPrivate(Lexer *self)
{
  return isKeyword(self, KeywordPrivate);
}

int isKeywordString(Lexer *self)
{
  return isKeyword(self, KeywordString);
}

int isKeywordNumber(Lexer *self)
{
  return isKeyword(self, KeywordNumber);
}

Keyword getKeyword(char *input)
{
  int i = 0;
  int length = sizeof(keywords) / sizeof(keywords[0]);

  while (i < length)
  {
    if (strcmp(input, keywords[i]) == 0)
      return i;

    i++;
  }

  return -1;
}

TokenType getKeywordOrIdentifierTokenType(char *value)
{
  Keyword keyword = getKeyword(value);

  if (keyword == -1)
    return TokenIdendifierStart;

  switch (keyword)
  {
  case KeywordFunc:
    return TokenFunc;
  case KeywordTypeAlias:
    return TokenTypeAlias;
  case KeywordLet:
    return TokenKeywordLet;
  case KeywordNumber:
    return TokenKeywordNumber;
  case KeywordPrivate:
    return TokenKeywordPrivate;
  case KeywordPublic:
    return TokenKeywordPublic;
  case KeywordString:
    return TokenKeywordString;
  default:
    return -1;
  }
}

Token *nextToken(Lexer *self)
{
  if (!hasData(self->source))
    return NULL;

  char nextChar = peek(self->source);

  switch (nextChar)
  {
  case ' ':
  case '\t':
  case '\r':
  {
    int count = eatWhiteSpace(self->source);

    self->col += count;
    self->index += count;

    return nextToken(self);
  }
  case '\n':
  {
    int count = eatNewline(self->source);

    self->line += count;
    self->col = 1;
    self->index += count;

    return nextToken(self);
  }
  case '&':
  {
    if (isBinaryAnd(peekAt(self->source, 1)))
      return tokenFactory(self, TokenLogicalAnd, NULL);

    return tokenFactory(self, TokenBinaryAnd, NULL);
  }
  case '|':
  {
    if (isBinaryOr(peekAt(self->source, 1)))
      return tokenFactory(self, TokenLogicalOr, NULL);

    return tokenFactory(self, TokenBinaryOr, NULL);
  }
  case '!':
  {
    if (isEqual(peekAt(self->source, 1)))
      return tokenFactory(self, TokenNotEqual, NULL);

    return tokenFactory(self, TokenNot, NULL);
  }
  case '=':
  {
    if (isEqual(peekAt(self->source, 1)))
      return tokenFactory(self, TokenEqualEqual, NULL);

    if (isRightAngleBracket(peekAt(self->source, 1)))
      return tokenFactory(self, TokenFatArrow, NULL);

    return tokenFactory(self, TokenEqual, NULL);
  }
  case '+':
    return tokenFactory(self, TokenPlus, NULL);
  case '-':
    return tokenFactory(self, TokenMinus, NULL);
  case '*':
    return tokenFactory(self, TokenMultiply, NULL);
  case ',':
    return tokenFactory(self, TokenComma, NULL);
  case ':':
    return tokenFactory(self, TokenColon, NULL);
  case '(':
    return tokenFactory(self, TokenLeftParen, NULL);
  case ')':
    return tokenFactory(self, TokenRightParen, NULL);
  case '{':
    return tokenFactory(self, TokenLeftCurlyBrace, NULL);
  case '}':
    return tokenFactory(self, TokenRightCurlyBrace, NULL);
  case '<':
    return tokenFactory(self, TokenLeftAngleBracket, NULL);
  case '>':
    return tokenFactory(self, TokenRightAngleBracket, NULL);
  case '[':
    return tokenFactory(self, TokenLeftBracket, NULL);
  case ']':
    return tokenFactory(self, TokenRightBracket, NULL);
  case '?':
    return tokenFactory(self, TokenOptional, NULL);
  case ';':
    return tokenFactory(self, TokenSemicolon, NULL);
  case '"':
    return tokenFactory(self, TokenDoubleQuotedString, NULL);
  default:
  {
    if (isInteger(nextChar))
      return tokenFactory(self, TokenInteger, NULL);

    if (isIdentifierStart(nextChar))
    {
      char *value = eatIdentiferStart(self);

      return tokenFactory(self, getKeywordOrIdentifierTokenType(value), value);
    }
  }
  }

  return NULL;
}
