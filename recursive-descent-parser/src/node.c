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

Node *programNodeFactory(Node *list)
{
  Node *node = nodeFactory(ProgramNodeType);

  node->data.program.body = list->data.list;

  free(list);

  return node;
}

Node *errorNodeFactory(char *message, Lexer *lexer)
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

  node->data.error = strdup(result);

  return node;
}

Node *typeAliasDeclarationNodeFactory(Node *id, Node *typeParameters, Node *typeAnnotation)
{
  Node *node = nodeFactory(TypeAliasDeclarationNodeType);

  node->data.typeAliasDeclaration.id = id;

  node->data.typeAliasDeclaration.typeParameters = typeParameters->data.list;

  node->data.typeAliasDeclaration.typeAnnotation = typeAnnotation;

  free(typeParameters);

  return node;
}

Node *tupleTypeNodeFactory(Node *elementTypes)
{
  Node *node = nodeFactory(TupleTypeNodeType);

  node->data.tupleType.elementTypes = elementTypes->data.list;

  free(elementTypes);

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

  node->data.typeReference.typeParameters = typeParameters->data.list;

  free(typeParameters);

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

  node->data.TypeLiteral.members = members->data.list;

  free(members);

  return node;
}

Node *typeAnnotationNodeFactory(Node *typeAnnotation)
{
  Node *node = nodeFactory(TypeAnnotationNodeType);

  node->data.typeAnnotation.typeAnnotation = typeAnnotation;

  return node;
}

Node *literalNodeFactory(Token *token)
{
  Node *node = nodeFactory(LiteralNodeType);

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

  node->data.variableDeclaration.declarations = declarations->data.list;

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

  node->data.callExpression.arguments = arguments->data.list;

  node->data.callExpression.typeParameters = typeParameters;

  free(arguments);

  return node;
}

Node *variableDeclaratorNodeFactory(Node *id, Node *init)
{
  Node *node = nodeFactory(VariableDeclaratorNodeType);

  node->data.variableDeclarator.id = id;

  node->data.variableDeclarator.init = init;

  return node;
}

Node *functionDeclarationNodeFactory(Node *id, Node *params, Node *body, Node *typeParameters)
{
  Node *node = nodeFactory(FunctionDeclarationNodeType);

  node->data.functionDeclaration.id = id;

  node->data.functionDeclaration.params = params->data.list;

  node->data.functionDeclaration.body = body->data.list;

  node->data.functionDeclaration.typeParameters = typeParameters;

  free(params);

  free(body);

  return node;
}

Node *binaryExpressionNodeFactory(Node *left, char *operator, Node * right)
{
  Node *node = nodeFactory(BinaryExpressionNodeType);

  node->data.binary.left = left;

  node->data.binary.operator= operator;

  node->data.binary.right = right;

  return node;
}

Node *blockStatementNodeFactory(Node *list)
{
  Node *node = nodeFactory(BlockStatementNodeType);

  node->data.block.body = list != NULL ? list->data.list : NULL;

  free(list);

  return node;
}

Node *identifierNodeFactory(char *name, Node *typeAnnotation)
{
  Node *node = nodeFactory(IdentifierNodeType);

  node->data.identifier.name = name;

  node->data.identifier.typeAnnotation = typeAnnotation;

  return node;
}

void freeNode(Node *node)
{
  switch (node->type)
  {
  case ProgramNodeType:
  {
    freeList(node->data.program.body);

    free(node);

    break;
  }
  case BlockStatementNodeType:
  {
    freeList(node->data.block.body);

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
    freeList(node->data.functionDeclaration.body);

    freeList(node->data.functionDeclaration.params);

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
    free(node->data.error);

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