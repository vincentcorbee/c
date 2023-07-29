#ifndef parser_H
#define parser_H

#include <stdio.h>

#include "linked-list.h"
#include "node.h"
#include "grammar.h"
#include "helpers.h"
#include "lexer.h"

/* Set EXTERN macro: */
#ifdef parser_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif

/* Function prototypes */

EXTERN void visitNode(Node *node, size_t indent);

EXTERN Node *parse(char **input);

#undef parser_IMPORT
#undef EXTERN
#endif