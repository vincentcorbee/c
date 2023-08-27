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
  NumericLiteralNodeType,
  StringLiteralNodeType,
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
  CallExpressionNodeType,
  IfStatementNodeType,
  WhileStatementNodeType,
  ForStatementNodeType,
  ExpressionStatementNodeType,
  UpdateExpressionNodeType,
  EmptyStatementNodeType,
  ReturnStatementNodeType
} NodeType;

typedef enum
{
  LexicalError = 1,
  SyntacticalError = 2
} ErrorType;

typedef struct Node Node;

typedef char *LiteralValue;

typedef char *ErrorValue;

typedef LinkedList NodeList;

typedef struct ErrorNode
{
  char *msg;
  ErrorType type;
} ErrorNode;

typedef struct ProgramNode
{
  Node *body;
} ProgramNode;

typedef struct BlockStatementNode
{
  Node *body;
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
  Node *typeParameters;
} TypeAliasDeclarationNode;

typedef struct ArrayTypeNode
{
  Node *elementType;
} ArrayTypeNode;

typedef struct TupleTypeNode
{
  Node *elementTypes;
} TupleTypeNode;

typedef struct TypeLiteralNode
{
  Node *members;
} TypeLiteralNode;

typedef struct TypeReferenceNode
{
  Node *typeName;
  Node *typeParameters;
} TypeReferenceNode;

typedef struct TypeParameterDeclartionNode
{
  Node *typeName;
  Node *params;
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
  Node *params;
  Node *body;
  Node *typeParameters;
  Node *returnType;
} FunctionDeclarationNode;

typedef struct IdentifierNode
{
  char *name;
  Node *typeAnnotation;
} IdentifierNode;

typedef struct VariableDeclarationNode
{
  char *kind;
  Node *declarations;
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
  Node *arguments;
  Node *typeParameters;
} CallExpressionNode;

typedef struct IfStatementNode
{
  Node *test;
  Node *consequent;
  Node *alternate;
} IfStatementNode;

typedef struct WhileStatementNode
{
  Node *test;
  Node *body;
} WhileStatementNode;

typedef struct ForStatementNode
{
  Node *init;
  Node *test;
  Node *update;
  Node *body;
} ForStatementNode;

typedef struct ExpressionStatementNode
{
  Node *expression;
} ExpressionStatementNode;

typedef struct UpdateExpressionNode
{
  Node *argument;
  char *operator;
  int prefix;
} UpdateExpressionNode;

typedef struct ReturnStatementNode
{
  Node *argument;
} ReturnStatementNode;

typedef union
{
  ProgramNode program;
  BinaryNode binary;
  LiteralValue literal;
  ErrorNode error;
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
  IfStatementNode ifStatement;
  WhileStatementNode whileStatement;
  ForStatementNode forStatement;
  ExpressionStatementNode expressionStatement;
  UpdateExpressionNode updateExpression;
  ReturnStatementNode returnStatement;
} NodeData;

typedef struct Node
{
  NodeType type;
  NodeData data;
} Node;

/* Function prototypes */

Node *nodeFactory(NodeType type);
Node *errorNodeFactory(char *message, Lexer *lexer, ErrorType errorType);

Node *programNodeFactory(Node *body);
Node *literalNodeFactory(Token *token, NodeType type);
Node *identifierNodeFactory(char *name, Node *typeAnnotation);
Node *binaryExpressionNodeFactory(Node *left, Token *operator, Node * right);
Node *updateExpressionNodeFactory(Node *argument, Token *operator, int prefix);
Node *blockStatementNodeFactory(Node *body);
Node *nodeListNodeFactory(NodeList *list);
Node *functionDeclarationNodeFactory(Node *id, Node *params, Node *body, Node *typeParameters, Node *returnType);
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
Node *ifStatementNodeFactory(Node *test, Node *consequent, Node *alternate);
Node *whileStatementNodeFactory(Node *test, Node *body);
Node *forStatementNodeFactory(Node *init, Node *test, Node *update, Node *body);
Node *expressionStatementNodeFactory(Node *expression);
Node *emptyStatementNodeFactory(void);
Node *returnStatementNodeFactory(Node *argument);

void freeNode(Node *node);

#undef node_IMPORT
#undef NODE_EXTERN
#endif