#ifndef node_H
#define node_H

/* Set EXTERN macro */
#ifdef node_IMPORT
#define NODE_EXTERN
#else
#define NODE_EXTERN extern
#endif

#include "linked-list.h"

/* Type declarations */

typedef enum
{
  ErrorNode = -1,
  Program = 0,
  LiteralNode = 1,
  BinaryExpressionNode = 2
} NodeType;

typedef struct Node Node;

typedef char *LiteralValue;

typedef char *ErrorValue;

typedef struct ProgramNode
{
  LinkedList *body;
} ProgramNode;

typedef struct BinaryNode
{
  char *operator;
  Node *left;
  Node *right;
} BinaryNode;

typedef union
{
  ProgramNode program;
  BinaryNode binary;
  LiteralValue literal;
  ErrorValue error;
} NodeData;

typedef struct Node
{
  NodeType type;
  NodeData data;
} Node;

/* Function prototypes */

Node *nodeFactory(NodeType type);

Node *programNodeFactory();

Node *numericLiteralNodeFactory(char **input);

Node *stringLiteralNodeFactory(char **input);

Node *binaryExpressionNodeFactory(Node *left, char *operator, Node * right);

Node *errorNodeFactory(char *message);

void freeNode(Node *node);

#undef node_IMPORT
#undef NODE_EXTERN
#endif