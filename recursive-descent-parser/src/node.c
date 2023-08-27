#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define node_IMPORT

#include "helpers.h"
#include "node.h"
#include "token.h"

/* Private functions */

unsigned int getCharacterCountInt(int number)
{
  if (number == 0)
    return 1;

  return (int)log10(number) + 1;
}

static void freeList(NodeList *list)
{
  if (list != NULL)
  {
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
}

/* Public functions */

Node *nodeFactory(NodeType type)
{
  Node *node = malloc(sizeof(Node));

  node->type = type;

  return node;
}

Node *programNodeFactory(Node *body)
{
  Node *node = nodeFactory(ProgramNodeType);

  node->data.program.body = body;

  return node;
}

Node *returnStatementNodeFactory(Node *argument)
{
  Node *node = nodeFactory(ReturnStatementNodeType);

  node->data.returnStatement.argument = argument;

  return node;
}

Node *typeAliasDeclarationNodeFactory(Node *id, Node *typeParameters, Node *typeAnnotation)
{
  Node *node = nodeFactory(TypeAliasDeclarationNodeType);

  node->data.typeAliasDeclaration.id = id;

  node->data.typeAliasDeclaration.typeParameters = typeParameters;

  node->data.typeAliasDeclaration.typeAnnotation = typeAnnotation;

  return node;
}

Node *tupleTypeNodeFactory(Node *elementTypes)
{
  Node *node = nodeFactory(TupleTypeNodeType);

  node->data.tupleType.elementTypes = elementTypes;

  return node;
}

Node *arrayTypeNodeFactory(Node *elementType)
{
  Node *node = nodeFactory(ArrayTypeNodeType);

  node->data.arrayType.elementType = elementType;

  return node;
}

Node *typeReferenceNodeFactory(Node *typeName, Node *typeParameters)
{
  Node *node = nodeFactory(TypeReferenceNodeType);

  if (typeParameters == NULL)
    typeParameters = nodeListNodeFactory(NULL);

  node->data.typeReference.typeName = typeName;

  node->data.typeReference.typeParameters = typeParameters;

  return node;
}

Node *propertySignatureNodeFactory(Node *key, int optional, Node *typeAnnotation)
{

  Node *node = nodeFactory(PropertySignatureNodetype);

  node->data.propertySignature.key = key;

  node->data.propertySignature.optional = optional;

  node->data.propertySignature.typeAnnotation = typeAnnotation;

  return node;
}

Node *TypeLiteralNodeFactory(Node *members)
{
  Node *node = nodeFactory(TypeLiteralNodeType);

  node->data.TypeLiteral.members = members;

  return node;
}

Node *typeAnnotationNodeFactory(Node *typeAnnotation)
{
  Node *node = nodeFactory(TypeAnnotationNodeType);

  node->data.typeAnnotation.typeAnnotation = typeAnnotation;

  return node;
}

Node *literalNodeFactory(Token *token, NodeType type)
{
  Node *node = nodeFactory(type);

  char *value = token->value;

  node->data.literal = value;

  free(token);

  return node;
}

Node *identiferNodeFactory(Token *token)
{
  Node *node = nodeFactory(IdentifierNodeType);

  char *value = token->value;

  node->data.identifier.name = value;

  node->data.identifier.typeAnnotation = NULL;

  free(token);

  return node;
}

Node *nodeListNodeFactory(NodeList *list)
{
  Node *node = nodeFactory(NodeListNodeType);

  node->data.list = list == NULL ? linkedList_linkedListFactory() : list;

  return node;
}

Node *variableDeclarationNodeFactory(Node *kind, Node *declarations)
{
  Node *node = nodeFactory(VariableDeclarationNodeType);

  node->data.variableDeclaration.kind = kind->data.literal;

  node->data.variableDeclaration.declarations = declarations;

  return node;
}

Node *assignmentPatternNodeFactory(Node *left, Node *right)
{
  Node *node = nodeFactory(AssignmentPatternNodeType);

  node->data.assignmentPattern.left = left;

  node->data.assignmentPattern.right = right;

  return node;
}

Node *callExpressionNodeFactory(Node *callee, Node *arguments, Node *typeParameters)
{
  Node *node = nodeFactory(CallExpressionNodeType);

  node->data.callExpression.callee = callee;

  node->data.callExpression.arguments = arguments;

  node->data.callExpression.typeParameters = typeParameters;

  return node;
}

Node *variableDeclaratorNodeFactory(Node *id, Node *init)
{
  Node *node = nodeFactory(VariableDeclaratorNodeType);

  node->data.variableDeclarator.id = id;

  node->data.variableDeclarator.init = init;

  return node;
}

Node *functionDeclarationNodeFactory(Node *id, Node *params, Node *body, Node *typeParameters, Node *returnType)
{
  Node *node = nodeFactory(FunctionDeclarationNodeType);

  node->data.functionDeclaration.id = id;

  node->data.functionDeclaration.params = params;

  node->data.functionDeclaration.body = body;

  node->data.functionDeclaration.typeParameters = typeParameters;

  node->data.functionDeclaration.returnType = returnType;

  return node;
}

Node *binaryExpressionNodeFactory(Node *left, Token *operator, Node * right)
{
  Node *node = nodeFactory(BinaryExpressionNodeType);

  char *value = operator->value;

  node->data.binary.left = left;

  node->data.binary.operator= value;

  node->data.binary.right = right;

  free(operator);

  return node;
}

Node *updateExpressionNodeFactory(Node *argument, Token *operator, int prefix)
{
  Node *node = nodeFactory(UpdateExpressionNodeType);

  char *value = operator->value;

  node->data.updateExpression.argument = argument;

  node->data.updateExpression.operator= value;

  node->data.updateExpression.prefix = prefix;

  free(operator);

  return node;
}

Node *blockStatementNodeFactory(Node *body)
{
  Node *node = nodeFactory(BlockStatementNodeType);

  node->data.block.body = body;

  return node;
}

Node *identifierNodeFactory(char *name, Node *typeAnnotation)
{
  Node *node = nodeFactory(IdentifierNodeType);

  node->data.identifier.name = name;

  node->data.identifier.typeAnnotation = typeAnnotation;

  return node;
}

Node *ifStatementNodeFactory(Node *test, Node *consequent, Node *alternate)
{
  Node *node = nodeFactory(IfStatementNodeType);

  node->data.ifStatement.test = test;

  node->data.ifStatement.consequent = consequent;

  node->data.ifStatement.alternate = alternate;

  return node;
}

Node *whileStatementNodeFactory(Node *test, Node *body)
{
  Node *node = nodeFactory(WhileStatementNodeType);

  node->data.whileStatement.test = test;

  node->data.whileStatement.body = body;

  return node;
}

Node *forStatementNodeFactory(Node *test, Node *init, Node *update, Node *body)
{
  Node *node = nodeFactory(ForStatementNodeType);

  node->data.forStatement.test = test;

  node->data.forStatement.init = init;

  node->data.forStatement.update = update;

  node->data.forStatement.body = body;

  return node;
}

Node *expressionStatementNodeFactory(Node *expression)
{
  Node *node = nodeFactory(ExpressionStatementNodeType);

  node->data.expressionStatement.expression = expression;

  return node;
}

Node *emptyStatementNodeFactory(void)
{
  Node *node = nodeFactory(EmptyStatementNodeType);

  return node;
}

Node *errorNodeFactory(char *message, Lexer *lexer, ErrorType errorType)
{
  Node *node = nodeFactory(ErrorNodeType);

  size_t length = getCharacterCountInt(lexer->line) + getCharacterCountInt(lexer->col) + 1;

  char line[length + 1];

  char suffix[] = " on line ";

  char result[strlen(suffix) + length + strlen(message) + 1];

  sprintf(line, "%d:%d", lexer->line, lexer->col);

  strcpy(result, message);

  strcat(result, suffix);

  strcat(result, line);

  node->data.error.msg = strdup(result);
  node->data.error.type = !errorType ? SyntacticalError : errorType;

  return node;
}

void freeNode(Node *node)
{
  if (!node)
    return;

  switch (node->type)
  {
  case ProgramNodeType:
  {
    freeNode(node->data.program.body);

    free(node);

    break;
  }
  case BlockStatementNodeType:
  {
    freeNode(node->data.block.body);

    free(node);

    break;
  }
  case NodeListNodeType:
  {
    freeList(node->data.list);

    free(node);

    break;
  }
  case FunctionDeclarationNodeType:
  {
    freeNode(node->data.functionDeclaration.body);

    freeNode(node->data.functionDeclaration.params);

    free(node);

    break;
  }
  case BinaryExpressionNodeType:
  {
    freeNode(node->data.binary.left);

    freeNode(node->data.binary.right);

    free(node->data.binary.operator);

    free(node);

    break;
  }
  case ErrorNodeType:
    free(node->data.error.msg);

    free(node);

    break;
  case IdentifierNodeType:
  {
    free(node->data.identifier.name);

    if (node->data.identifier.typeAnnotation)
      freeNode(node->data.identifier.typeAnnotation);

    free(node);

    break;
  }
  case LiteralNodeType:
  {
    free(node->data.literal);

    free(node);

    break;
  }
  case TypeAliasDeclarationNodeType:
  case ArrayTypeNodeType:
  case TupleTypeNodeType:
  case TypeReferenceNodeType:
  case TypeParameterDeclarationNodeType:
  case TypeParameterInstantiationNodeType:
  case PropertySignatureNodetype:
  case TypeLiteralNodeType:
  case TypeAnnotationNodeType:
  {
    break;
  }
  }
}