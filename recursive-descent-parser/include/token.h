#ifndef token_H
#define token_H

/* Set EXTERN macro */
#ifdef token_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif

/* Type declarations */

typedef enum
{
  TokenKeywordPublic,
  TokenKeywordPrivate,
  TokenTypeAlias,
  TokenKeywordString,
  TokenKeywordNumber,
  TokenFunc,
  TokenPlus,
  TokenMinus,
  TokenMultiply,
  TokenLeftParen,
  TokenRightParen,
  TokenDoubleQuotedString,
  TokenInteger,
  TokenSemicolon,
  TokenWhiteSpace,
  TokenFatArrow,
  TokenLeftAngleBracket,
  TokenRightAngleBracket,
  TokenLeftCurlyBrace,
  TokenRightCurlyBrace,
  TokenLeftBracket,
  TokenRightBracket,
  TokenComma,
  TokenColon,
  TokenOptional,
  TokenNewline,
  TokenBinaryAnd,
  TokenLogicalAnd,
  TokenBinaryOr,
  TokenLogicalOr,
  TokenNot,
  TokenEqual,
  TokenNotEqual,
  TokenEqualEqual,
  TokenIdendifierStart,
  TokenKeywordLet,
  TokenKeywordConst
} TokenType;

typedef enum
{
  KeywordFunc,
  keywordConst,
  KeywordLet,
  KeywordPublic,
  KeywordPrivate,
  KeywordTypeAlias,
  KeywordString,
  KeywordNumber
} Keyword;

typedef struct Token
{
  TokenType type;
  char *value;
} Token;

/* Function prototypes */

void freeToken(Token *token);

#undef token_IMPORT
#undef EXTERN
#endif