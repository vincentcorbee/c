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
  TokenError = -1,
  TokenKeywordPublic,
  TokenKeywordPrivate,
  TokenTypeAlias,
  TokenKeywordString,
  TokenKeywordNumber,
  TokenKeywordVoid,
  TokenKeywordFunc,
  TokenPlus,
  TokenMinus,
  TokenMultiply,
  TokenDivide,
  TokenIncrement,
  TokenDecrement,
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
  TokenKeywordIf,
  TokenKeywordElse,
  TokenKeywordWhile,
  TokenKeywordFor,
  TokenComma,
  TokenColon,
  TokenOptional,
  TokenNewline,
  TokenBinaryAnd,
  TokenLogicalAnd,
  TokenBinaryOr,
  TokenBinaryXOr,
  TokenLogicalOr,
  TokenNot,
  TokenEqual,
  TokenNotEqual,
  TokenEqualEqual,
  TokenLesserThenOrEqual,
  TokenGreaterThenOrEqual,
  TokenIdendifierStart,
  TokenKeywordLet,
  TokenKeywordConst,
  TokenKeywordReturn,
  TokenEOF
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
  KeywordNumber,
  KeywordVoid,
  KeywordIf,
  KeywordElse,
  KeywordWhile,
  KeywordFor,
  KeywordReturn
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