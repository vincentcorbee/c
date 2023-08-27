#ifndef helpers_H
#define helpers_H

/* Set EXTERN macro: */
#ifdef helpers_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif

/* Function prototypes */

int isEOF(char c);

int hasData(char **input);

char eat(char **input);

char peek(char **input);

char peekAt(char **input, int pos);

char skip(char **input, int count);

int eatWhiteSpace(char **input);

int eatNewline(char **input);

void eatWhiteSpaceAndNewline(char **input);

/* Tokens */

int isInteger(char c);

int isNewline(char c);

int isAsciiAz(char c);

int isIdentifierStart(char c);

int isEqual(char c);

int isNot(char c);

int isMultiply(char c);

int isMinus(char c);

int isPlus(char c);

int isDivision(char c);

int isBinaryOr(char c);

int isBinaryAnd(char c);

int isDoubleQuotedString(char c);

int isWhiteSpace(char c);

int isRightAngleBracket(char c);

int isLeftAngleBracket(char c);

#undef helpers_IMPORT
#undef EXTERN
#endif