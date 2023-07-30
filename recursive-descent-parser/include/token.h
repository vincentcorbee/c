#ifndef token_H
#define token_H

/* Set EXTERN macro: */
#ifdef token_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif

/* Type declarations */

typedef enum
{
  Plus,
  Minus,
  Multiply,
  LeftParen,
  RightParen,
  DoubleQuotedString,
  Integer,
  Semicolon,
  WhiteSpace,
  Newline,
  BinaryAnd,
  LogicalAnd,
  BinaryOr,
  LogicalOr,
  Not,
  Equal,
  NotEqual,
  EqualEqual,
  IdendifierStart
} TokenType;

typedef struct Token
{
  TokenType type;
  char *value;
} Token;

#undef token_IMPORT
#undef EXTERN
#endif