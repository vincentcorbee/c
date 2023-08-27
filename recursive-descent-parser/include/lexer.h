#ifndef lexer_H
#define lexer_H

#include "token.h"

#ifdef lexer_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif

/* Type declarations */

typedef struct Lexer Lexer;

typedef struct Lexer
{
  int index;
  int col;
  int line;
  Token *current;
  char **source;
  Token *(*next)(Lexer *self);
  TokenType (*peek)(Lexer *self);
  void (*skip)(Lexer *self, int count);
  void (*advance)(Lexer *self);
  char *(*eatChar)(Lexer *self, int count);
} Lexer;

typedef const char *Keywords[];

typedef struct LexerState
{
  int index;
  int col;
  int line;
  char *source;
} LexerState;

/* Function prototypes */

Lexer *lexerFactory(char **source);

Token *tokenFactory(Lexer *self, TokenType type, const char *value);

TokenType peekToken(Lexer *self);

Token *nextToken(Lexer *self);

void skipToken(Lexer *self, int count);

void advanceToken(Lexer *self);

LexerState getLexerState(Lexer *self);

void setLexerState(Lexer *self, LexerState state);

#undef lexer_IMPORT
#undef EXTERN
#endif