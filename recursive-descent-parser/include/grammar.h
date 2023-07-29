#ifndef grammar_H
#define grammar_H

/* Set EXTERN macro */
#ifdef grammar_IMPORT
#define GRAMMER_EXTERN
#else
#define GRAMMER_EXTERN extern
#endif

#include "node.h"

Node *program(char **input);

LinkedList *statements(char **input);

Node *statement(char **input);

Node *expressionStatement(char **input);

Node *primaryExpression(char **input);

Node *expression(char **input);

Node *assignmentExpression(char **input);

Node *leftHandSideExpression(char **input);

Node *memberExpression(char **input);

Node *conditionalExpression(char **input);

Node *equalityExpression(char **input);

Node *logicalAndExpression(char **input);

Node *logicalOrExpression(char **input);

Node *additiveExpression(char **input);

Node *multiplicativeExpression(char **input);

Node *numericLiteral(char **input);

Node *stringLiteral(char **input);

Node *literal(char **input);

Node *identifier(char **input);

char *identifierStart(char **input);

Node *coverParenthesizedExpression(char **input);

#undef gramma_IMPORT
#undef GRAMMER_EXTERN
#endif