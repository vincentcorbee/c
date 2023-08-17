#ifndef node_H
#define node_H

/* Set EXTERN macro */
#ifdef node_IMPORT
#define NODE_EXTERN
#else
#define NODE_EXTERN extern
#endif

#include "linked-list.h"
#include "token.h"
#include "lexer.h"

/* Type declarations */

typedef enum
{
  ErrorNodeType = -1,
  ProgramNodeType,
  LiteralNodeType,
  IdentifierNodeType,
  BinaryExpressionNodeType,
  BlockStatementNodeType,
  NodeListNodeType,
  FunctionDeclarationNodeType,
  TypeAliasDeclarationNodeType,
  ArrayTypeNodeType,
  TupleTypeNodeType,
  TypeReferenceNodeType,
  TypeParameterDeclarationNodeType,
  TypeParameterInstantiationNodeType,
  PropertySignatureNodetype,
  TypeLiteralNodeType,
  TypeAnnotationNodeType,
  VariableDeclarationNodeType,
  VariableDeclaratorNodeType,
  AssignmentPatternNodeType,
  CallExpressionNodeType
} NodeType;

typedef struct Node Node;

typedef char *LiteralValue;

typedef char *ErrorValue;

typedef LinkedList NodeList;

typedef struct ProgramNode
{
  NodeList *body;
} ProgramNode;

typedef struct BlockStatementNode
{
  NodeList *body;
} BlockStatementNode;

typedef struct PropertySignatureNode
{
  Node *key;
  int optional;
  Node *typeAnnotation;
} PropertySignatureNode;

typedef struct TypeAnnotationNode
{
  Node *typeAnnotation;
} TypeAnnotationNode;

typedef struct TypeAliasDeclarationNode
{
  Node *id;
  Node *typeAnnotation;
  NodeList *typeParameters;
} TypeAliasDeclarationNode;

typedef struct ArrayTypeNode
{
  Node *elementType;
} ArrayTypeNode;

typedef struct TupleTypeNode
{
  NodeList *elementTypes;
} TupleTypeNode;

typedef struct TypeLiteralNode
{
  NodeList *members;
} TypeLiteralNode;

typedef struct TypeReferenceNode
{
  Node *typeName;
  NodeList *typeParameters;
} TypeReferenceNode;

typedef struct TypeParameterDeclartionNode
{
  Node *typeName;
  NodeList *params;
} TypeParameterDeclartionNode;

typedef struct BinaryNode
{
  char *operator;
  Node *left;
  Node *right;
} BinaryNode;

typedef struct FunctionDeclarationNode
{
  Node *id;
  NodeList *params;
  NodeList *body;
  Node *typeParameters
} FunctionDeclarationNode;

typedef struct IdentifierNode
{
  char *name;
  Node *typeAnnotation;
} IdentifierNode;

typedef struct VariableDeclarationNode
{
  char *kind;
  NodeList *declarations;
} VariableDeclarationNode;

typedef struct AssignmentPatternNode
{
  Node *left;
  Node *right;
} AssignmentPatternNode;

typedef struct VariableDeclaratorNode
{
  Node *id;
  Node *init;
} VariableDeclaratorNode;

typedef struct CallExpressionNode
{
  Node *callee;
  NodeList *arguments;
  Node *typeParameters;
} CallExpressionNode;

typedef union
{
  ProgramNode program;
  BinaryNode binary;
  LiteralValue literal;
  ErrorValue error;
  NodeList *list;
  BlockStatementNode block;
  FunctionDeclarationNode functionDeclaration;
  TypeAliasDeclarationNode typeAliasDeclaration;
  ArrayTypeNode arrayType;
  TupleTypeNode tupleType;
  TypeReferenceNode typeReference;
  IdentifierNode identifier;
  PropertySignatureNode propertySignature;
  TypeLiteralNode TypeLiteral;
  TypeAnnotationNode typeAnnotation;
  VariableDeclarationNode variableDeclaration;
  VariableDeclaratorNode variableDeclarator;
  AssignmentPatternNode assignmentPattern;
  CallExpressionNode callExpression;
} NodeData;

typedef struct Node
{
  NodeType type;
  NodeData data;
} Node;

/* Function prototypes */

Node *nodeFactory(NodeType type);

Node *programNodeFactory(Node *list);

Node *literalNodeFactory(Token *token);

Node *identifierNodeFactory(char *name, Node *typeAnnotation);

Node *binaryExpressionNodeFactory(Node *left, char *operator, Node * right);

Node *blockStatementNodeFactory(Node *list);

Node *nodeListNodeFactory(NodeList *list);

Node *functionDeclarationNodeFactory(Node *id, Node *params, Node *body, Node *typeParameters);

Node *variableDeclaratorNodeFactory(Node *id, Node *init);

Node *variableDeclarationNodeFactory(Node *kind, Node *declarations);

Node *assignmentPatternNodeFactory(Node *left, Node *right);

Node *typeAliasDeclarationNodeFactory(Node *id, Node *typeAnnotation, Node *typeParameters);

Node *arrayTypeNodeFactory(Node *elementType);

Node *tupleTypeNodeFactory(Node *elementTypes);

Node *typeReferenceNodeFactory(Node *typeName, Node *typeParametes);

Node *propertySignatureNodeFactory(Node *key, int optiona, Node *typeAnnotation);

Node *TypeLiteralNodeFactory(Node *members);

Node *typeAnnotationNodeFactory(Node *typeAnnotation);

Node *callExpressionNodeFactory(Node *callee, Node *arguments, Node *typeParameters);

Node *errorNodeFactory(char *message, Lexer *lexer);

void freeNode(Node *node);

#undef node_IMPORT
#undef NODE_EXTERN
#endif