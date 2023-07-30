#include <stdlib.h>
#include <string.h>

#define parser_IMPORT

#include "lexer.h"
#include "parser.h"

/* Private functions */

static void createIdentation(size_t indent)
{
  for (int i = 0; i < indent; i++)
    putchar(' ');
}

/* Public functions */

void visitNode(Node *node, size_t indent)
{
  createIdentation(indent);

  switch (node->type)
  {
  case Program:
    printf("Program(\n");

    LinkedListNode *current = node->data.program.body->head;

    while (current)
    {
      visitNode((Node *)current->data, indent + 1);

      current = current->next;
    }

    createIdentation(indent);

    printf(")\n");
    break;
  case LiteralNode:
    printf("Literal(%s)\n", node->data.literal);
    break;
  case BinaryExpressionNode:
    printf("BinaryExpression(\n");

    visitNode(node->data.binary.left, indent + 1);

    createIdentation(indent + 1);

    printf("%s\n", node->data.binary.operator);

    visitNode(node->data.binary.right, indent + 1);

    createIdentation(indent);

    printf(")\n");

    break;
  case ErrorNode:
    createIdentation(indent);

    printf("Error: %s\n", node->data.error);

    break;
  }
}

Node *parse(char **source)
{
  Lexer *lexer = lexerFactory(source);

  Node *tree = program(source);

  free(lexer);

  return tree;
}