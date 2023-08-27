#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "helpers.h"

/* Including own header for checking by compiler */
#define lexer_IMPORT

#include "lexer.h"

/* Private functions */

static Keywords keywords = {"func", "const", "let", "public", "private", "type", "string", "number", "void", "if", "else", "while", "for", "return"};

static char *eatChar(Lexer *self, int count)
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

static char *eatIdentiferStart(Lexer *self)
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

static char *eatDoubleQuotedString(Lexer *self)
{
  int index = 0;

  int col = self->col;

  int previousIndex = self->index;

  char *source = *self->source;

  char *string = malloc(sizeof(char));

  string[index++] = eat(self->source);

  self->col++;

  while (hasData(self->source) && !isDoubleQuotedString(**self->source))
  {
    string = realloc(string, (index + 1) * sizeof(char) + 1);

    self->col++;

    if (isNewline(**self->source))
    {
      self->col = 1;
      self->line++;
    }

    string[index++] = eat(self->source);

    self->index++;
  };

  if (!isDoubleQuotedString(**self->source))
  {
    free(string);

    return NULL;
  }

  string[index++] = eat(self->source);

  self->col++;

  string[index] = '\0';

  return string;
}

static char *eatKeyword(Lexer *self, Keyword keywordType)
{
  const char *keyword = keywords[keywordType];

  return eatChar(self, strlen(keyword));
}

static char *eatInteger(Lexer *self)
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

static Keyword getKeyword(char *input)
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

static TokenType getKeywordOrIdentifierTokenType(char *value)
{
  Keyword keyword = getKeyword(value);

  if (keyword == -1)
    return TokenIdendifierStart;

  switch (keyword)
  {
  case KeywordFunc:
    return TokenKeywordFunc;
  case KeywordTypeAlias:
    return TokenTypeAlias;
  case KeywordLet:
    return TokenKeywordLet;
  case keywordConst:
    return TokenKeywordConst;
  case KeywordNumber:
    return TokenKeywordNumber;
  case KeywordPrivate:
    return TokenKeywordPrivate;
  case KeywordPublic:
    return TokenKeywordPublic;
  case KeywordString:
    return TokenKeywordString;
  case KeywordVoid:
    return TokenKeywordVoid;
  case KeywordIf:
    return TokenKeywordIf;
  case KeywordElse:
    return TokenKeywordElse;
  case KeywordWhile:
    return TokenKeywordWhile;
  case KeywordFor:
    return TokenKeywordFor;
  case KeywordReturn:
    return TokenKeywordReturn;
  default:
    return -1;
  }
}

static void eatMultiLineComment(Lexer *self)
{
  self->eatChar(self, 2);

  while (!isMultiply(peekAt(self->source, 0)) && !isDivision(peekAt(self->source, 1)))
  {
    self->index++;
    self->col++;

    if (isNewline(**self->source))
    {
      self->line++;
      self->col = 1;
    }

    self->eatChar(self, 1);
  }

  self->eatChar(self, 2);
}

static void eatSingleLineComment(Lexer *self)
{
  self->eatChar(self, 2);

  while (!isNewline(**self->source))
  {
    self->index++;
    self->col++;

    self->eatChar(self, 1);
  }
}

/* Public functions */

Lexer *
lexerFactory(char **source)
{
  Lexer *lexer = malloc(sizeof(Lexer));

  lexer->index = 0;
  lexer->col = 1;
  lexer->line = 1;

  lexer->current = NULL;

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

Token *tokenFactory(Lexer *self, TokenType type, const char *value)
{
  Token *token = malloc(sizeof(Token));

  token->type = type;

  self->current = token;

  switch (type)
  {
  case TokenKeywordFunc:
  case TokenKeywordPublic:
  case TokenKeywordPrivate:
  case TokenTypeAlias:
  case TokenKeywordString:
  case TokenKeywordNumber:
  case TokenKeywordVoid:
  case TokenIdendifierStart:
  case TokenKeywordLet:
  case TokenKeywordConst:
  case TokenKeywordIf:
  case TokenKeywordElse:
  case TokenKeywordWhile:
  case TokenKeywordFor:
  case TokenKeywordReturn:
  case TokenEOF:
  case TokenDoubleQuotedString:
  case TokenInteger:
    token->value = value;
    return token;
  case TokenLogicalAnd:
  case TokenLogicalOr:
  case TokenEqualEqual:
  case TokenNotEqual:
  case TokenFatArrow:
  case TokenLesserThenOrEqual:
  case TokenGreaterThenOrEqual:
  case TokenIncrement:
  case TokenDecrement:
    token->value = eatChar(self, 2);
    return token;
  case TokenLeftParen:
  case TokenRightParen:
  case TokenMultiply:
  case TokenDivide:
  case TokenPlus:
  case TokenMinus:
  case TokenEqual:
  case TokenNot:
  case TokenSemicolon:
  case TokenBinaryOr:
  case TokenBinaryXOr:
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
  case TokenError:
    token->value = malloc(sizeof(char) * strlen(value));

    strcpy(token->value, value);

    return token;
  default:
    free(token);

    return NULL;
  }
}

TokenType peekToken(Lexer *self)
{

  if (isEOF(**self->source))
    return TokenEOF;

  LexerState state = getLexerState(self);

  Token *token = nextToken(self);

  setLexerState(self, state);

  TokenType type = token->type;

  freeToken(token);

  return type;
}

void skipToken(Lexer *self, int count)
{
  while (count)
  {
    Token *token = nextToken(self);

    if (token->type == TokenEOF)
      break;

    count--;

    if (count)
      freeToken(token);
  }
}

void advanceToken(Lexer *self)
{
  self->skip(self, 1);
}

Token *nextToken(Lexer *self)
{
  if (isEOF(**self->source))
    return tokenFactory(self, TokenEOF, NULL);

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
  case '^':
    return tokenFactory(self, TokenBinaryXOr, NULL);
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
  {
    if (isPlus(peekAt(self->source, 1)))
      return tokenFactory(self, TokenIncrement, NULL);
    return tokenFactory(self, TokenPlus, NULL);
  }
  case '-':
  {
    if (isMinus(peekAt(self->source, 1)))
      return tokenFactory(self, TokenDecrement, NULL);
    return tokenFactory(self, TokenMinus, NULL);
  }
  case '*':
    return tokenFactory(self, TokenMultiply, NULL);
  case '/':
  {
    if (isMultiply(peekAt(self->source, 1)))
    {
      eatMultiLineComment(self);

      return self->next(self);
    }
    else if (isDivision(peekAt(self->source, 1)))
    {
      eatSingleLineComment(self);

      return self->next(self);
    }

    return tokenFactory(self, TokenDivide, NULL);
  }
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
  {
    if (isEqual(peekAt(self->source, 1)))
      return tokenFactory(self, TokenLesserThenOrEqual, NULL);
    return tokenFactory(self, TokenLeftAngleBracket, NULL);
  }
  case '>':
  {
    if (isEqual(peekAt(self->source, 1)))
      return tokenFactory(self, TokenGreaterThenOrEqual, NULL);

    return tokenFactory(self, TokenRightAngleBracket, NULL);
  }
  case '[':
    return tokenFactory(self, TokenLeftBracket, NULL);
  case ']':
    return tokenFactory(self, TokenRightBracket, NULL);
  case '?':
    return tokenFactory(self, TokenOptional, NULL);
  case ';':
    return tokenFactory(self, TokenSemicolon, NULL);
  case '"':
  {
    char *str = eatDoubleQuotedString(self);

    if (str == NULL)
      return tokenFactory(self, TokenError, "Unterminated string literal");

    return tokenFactory(self, TokenDoubleQuotedString, str);
  }

  default:
  {
    if (isInteger(nextChar))
      return tokenFactory(self, TokenInteger, eatInteger(self));

    if (isIdentifierStart(nextChar))
    {
      char *value = eatIdentiferStart(self);

      return tokenFactory(self, getKeywordOrIdentifierTokenType(value), value);
    }
  }
  }

  return tokenFactory(self, TokenError, "Unexpected character");
}
