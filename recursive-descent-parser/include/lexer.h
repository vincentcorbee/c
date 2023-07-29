#ifndef lexer_H
#define lexer_H

#include "token.h"

/* Set EXTERN macro: */
#ifdef lexer_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif

/* Type declarations */

typedef struct Lexer Lexer;

typedef struct Lexer
{
  int col;
  int line;
  char **source;
  Token *(*next)(Lexer *self);
  Token *(*peek)(Lexer *self);
} Lexer;

/* Function prototypes */

Lexer *lexerFactory(char **source);

Token *peekToken(Lexer *self);

Token *nextToken(Lexer *self);

Token *tokenFactory(Lexer *self, TokenType type);

char *eatDoubleQuotedString(Lexer *self);

char *eatInteger(Lexer *self);

char *eatChar(Lexer *self, int count);

#undef lexer_IMPORT
#undef EXTERN
#endif