#include <stdlib.h>

#include "helpers.h"

/* Including own header for checking by compiler */
#define node_IMPORT

#include "node.h"

/* Public functions */

Node *nodeFactory(NodeType type)
{
  Node *node = malloc(sizeof(Node));

  node->type = type;

  return node;
}

Node *programNodeFactory()
{
  return nodeFactory(Program);
}

Node *errorNodeFactory(char *message)
{
  Node *node = nodeFactory(ErrorNode);

  node->data.error = message;

  return node;
}

Node *numericLiteralNodeFactory(char **input)
{
  Node *node = nodeFactory(LiteralNode);

  node->data.literal = malloc(sizeof(char));

  int index = 0;

  while (hasData(input) && isInteger(**input))
  {
    node->data.literal = realloc(node->data.literal, (index + 1) * sizeof(char) + 1);

    node->data.literal[index++] = eat(input);
  };

  node->data.literal[index] = '\0';

  return node;
}

Node *stringLiteralNodeFactory(char **input)
{
  int index = 0;

  char *literal = malloc(sizeof(char));

  literal[index++] = eat(input);

  while (hasData(input) && !isDoubleQuotedString(**input))
  {
    literal = realloc(literal, (index + 1) * sizeof(char) + 2);

    literal[index++] = eat(input);
  };

  if (!isDoubleQuotedString(**input))
  {
    free(literal);

    return errorNodeFactory("StringLiteral expected \"");
  }

  Node *node = nodeFactory(LiteralNode);

  literal[index++] = eat(input);

  literal[index] = '\0';

  node->data.literal = literal;

  return node;
}

Node *binaryExpressionNodeFactory(Node *left, char *operator, Node * right)
{
  Node *node = malloc(sizeof(Node));

  node->type = BinaryExpressionNode;

  node->data.binary.left = left;

  node->data.binary.operator= operator;

  node->data.binary.right = right;

  return node;
}

void freeNode(Node *node)
{
  switch (node->type)
  {
  case Program:
  {
    LinkedList *list = node->data.program.body;

    LinkedListNode *current = list->head;

    while (current)
    {
      freeNode((Node *)current->data);

      LinkedListNode *next = current->next;

      free(current);

      current = next;
    }

    free(list);
  }

    free(node);

    break;
  case BinaryExpressionNode:
    freeNode(node->data.binary.left);

    freeNode(node->data.binary.right);

    free(node);

    break;
  case ErrorNode:
    free(node);

    break;
  case LiteralNode:
    free(node->data.literal);

    free(node);

    break;
  }
}
