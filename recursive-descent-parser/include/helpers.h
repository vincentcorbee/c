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

int isInteger(char c);

int isNewline(char c);

int isAsciiAz(char c);

int isIdentifierStart(char c);

int isPlus(char c);

int isMinus(char c);

int isMultiply(char c);

int isLeftParen(char c);

int isRightParen(char c);

int isEqual(char c);

int isNot(char c);

int isSemicolon(char c);

int isDoubleQuotedString(char c);

char eat(char **input);

char peek(char **input);

char peekAt(char **input, int pos);

int isWhiteSpace(char c);

void eatWhiteSpace(char **input);

void eatNewline(char **input);

void eatWhiteSpaceAndNewline(char **input);

#undef helpers_IMPORT
#undef EXTERN
#endif