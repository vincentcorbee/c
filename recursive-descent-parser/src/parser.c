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
static void printList(NodeList *list, size_t indent)
{
  LinkedListNode *current = list->head;

  while (current)
  {
    visitNode((Node *)current->data, indent);

    current = current->next;
  }
}
static void printFunctionDeclaration(Node *node, size_t indent)
{
  printf("FunctionDeclaration {\n");

  createIdentation(indent + 1);

  printf("id: {\n");

  visitNode(node->data.functionDeclaration.id, indent + 2);

  createIdentation(indent + 1);

  printf("}\n");

  createIdentation(indent + 1);

  printf("params: [");

  NodeList *params = node->data.functionDeclaration.params;

  if (params->count > 0)
  {
    printf("\n");

    printList(node->data.functionDeclaration.params, indent + 2);

    createIdentation(indent + 1);
  }

  printf("]\n");

  createIdentation(indent + 1);

  printf("body: [\n");

  printList(node->data.functionDeclaration.body, indent + 2);

  createIdentation(indent + 1);

  printf("]\n");

  createIdentation(indent);

  printf("}\n");
}

static void printTypeAliasDeclaration(Node *node, size_t indent)
{
  printf("TypeAliasDeclaration {\n");

  createIdentation(indent + 1);

  printf("id: \n");

  visitNode(node->data.typeAliasDeclaration.id, indent + 1);

  createIdentation(indent + 1);

  printf("typeParameters: [");

  if (node->data.typeAliasDeclaration.typeParameters->count != 0)
  {
    printf("\n");

    printList(node->data.typeAliasDeclaration.typeParameters, indent + 2);

    createIdentation(indent + 1);
  }

  printf("]\n");

  createIdentation(indent + 1);

  printf("typeAnnotation: {\n");

  visitNode(node->data.typeAliasDeclaration.typeAnnotation, indent + 2);

  createIdentation(indent + 1);

  printf("}\n");

  createIdentation(indent);

  printf("}\n");
}

static void printTypeReference(Node *node, size_t indent)
{
  printf("TypeReference {\n");

  createIdentation(indent + 1);

  printf("typeName: {\n");

  visitNode(node->data.typeReference.typeName, indent + 2);

  createIdentation(indent + 1);

  printf("}\n");

  createIdentation(indent + 1);

  printf("typeParameters: [");

  if (node->data.typeReference.typeParameters->count != 0)
  {
    printf("\n");

    printList(node->data.typeReference.typeParameters, indent + 2);

    createIdentation(indent + 1);
  }

  printf("]\n");

  createIdentation(indent);

  printf("}\n");
}

static void printBinaryExpression(Node *node, size_t indent)
{
  printf("BinaryExpression {\n");

  createIdentation(indent + 1);

  printf("left: {\n");

  visitNode(node->data.binary.left, indent + 2);

  createIdentation(indent + 1);

  printf("}\n");

  createIdentation(indent + 1);

  printf("operator: %s\n", node->data.binary.operator);

  createIdentation(indent + 1);

  printf("right: {\n");

  visitNode(node->data.binary.right, indent + 2);

  createIdentation(indent + 1);

  printf("}\n");

  createIdentation(indent);

  printf("}\n");
}

static void printTupleType(Node *node, size_t indent)
{
  printf("TupleType [\n");

  printList(node->data.tupleType.elementTypes, indent + 1);

  createIdentation(indent);

  printf("]\n");
}

static void printArrayType(Node *node, size_t indent)
{
  printf("ArrayType {\n");

  visitNode(node->data.arrayType.elementType, indent + 1);

  createIdentation(indent);

  printf("}\n");
}

/* Public functions */

void visitNode(Node *node, size_t indent)
{
  if (node == NULL)
    return;

  createIdentation(indent);

  switch (node->type)
  {
  case ProgramNodeType:
  {
    printf("Program {\n");

    createIdentation(indent + 1);

    printf("body: [");

    NodeList *body = node->data.program.body;

    if (body->count > 0)
    {
      printf("\n");

      printList(node->data.program.body, indent + 2);

      createIdentation(indent + 1);
    }
    printf("]\n");

    createIdentation(indent);

    printf("}\n");

    break;
  }
  case LiteralNodeType:
    printf("Literal: %s\n", node->data.literal);

    break;
  case IdentifierNodeType:
  {
    printf("Identifer {\n");

    createIdentation(indent + 1);

    printf("name: %s\n", node->data.identifier.name);

    Node *typeAnnotation = node->data.identifier.typeAnnotation;

    if (typeAnnotation != NULL)
    {
      createIdentation(indent + 1);

      printf("typeAnnotation: {\n");

      visitNode(node->data.identifier.typeAnnotation, indent + 2);

      createIdentation(indent + 1);

      printf("}\n");
    }

    createIdentation(indent);

    printf("}\n");

    break;
  }
  case TypeAnnotationNodeType:
  {
    printf("TypeAnnotation {\n");

    visitNode(node->data.typeAnnotation.typeAnnotation, indent + 1);

    createIdentation(indent);

    printf("}\n");

    break;
  }
  case AssignmentPatternNodeType:
  {
    printf("AssignmentPattern {\n");

    createIdentation(indent + 1);

    printf("left: {\n");

    visitNode(node->data.assignmentPattern.left, indent + 2);

    createIdentation(indent + 1);

    printf("}\n");

    createIdentation(indent + 1);

    printf("right: {\n");

    visitNode(node->data.assignmentPattern.right, indent + 2);

    createIdentation(indent + 1);

    printf("}\n");

    createIdentation(indent);

    printf("}\n");

    break;
  }
  case CallExpressionNodeType:
  {
    printf("CallExpression {\n");

    createIdentation(indent + 1);

    printf("callee: {\n");

    visitNode(node->data.callExpression.callee, indent + 2);

    createIdentation(indent + 1);

    printf("}\n");

    createIdentation(indent + 1);

    printf("arguments: [");

    NodeList *args = node->data.callExpression.arguments;

    if (args && args->count > 0)
    {
      printf("\n");
      printList(args, indent + 2);
    }

    createIdentation(indent + 1);

    printf("]\n");

    if (node->data.callExpression.typeParameters)
    {
      visitNode(node->data.callExpression.typeParameters, indent + 2);
    }

    createIdentation(indent);

    printf("}\n");

    break;
  }
  case BlockStatementNodeType:
  {
    printf("BlockStatement {\n");

    LinkedList *list = node->data.block.body;

    createIdentation(indent + 1);

    printf("body: [");

    if (list)
    {
      printf("\n");

      printList(list, indent + 2);

      createIdentation(indent + 1);
    }

    printf("]\n");

    createIdentation(indent);

    printf("}\n");

    break;
  }
  case VariableDeclaratorNodeType:
  {
    printf("VariableDeclarator {\n");

    createIdentation(indent + 1);

    printf("id: {\n");

    visitNode(node->data.variableDeclarator.id, indent + 2);

    createIdentation(indent + 1);

    printf("}\n");

    createIdentation(indent + 1);

    printf("init: {");

    Node *init = node->data.variableDeclarator.init;

    if (init)
    {
      printf("\n");

      visitNode(init, indent + 2);

      createIdentation(indent + 1);
    }

    printf("}\n");

    createIdentation(indent);

    printf("}\n");

    break;
  }
  case VariableDeclarationNodeType:
  {
    printf("VariableDeclaration {\n");

    createIdentation(indent + 1);

    printf("kind: %s\n", node->data.variableDeclaration.kind);

    createIdentation(indent + 1);

    printf("declarations: [");

    NodeList *declarations = node->data.variableDeclaration.declarations;

    if (declarations)
    {

      printf("\n");

      printList(declarations, indent + 2);

      createIdentation(indent + 1);
    }

    printf("]\n");

    createIdentation(indent);

    printf("}\n");

    break;
  }
  case NodeListNodeType:
  {
    printf("NodeList [\n");

    printList(node->data.list, indent + 1);

    createIdentation(indent);

    printf("]\n");

    break;
  }
  case ArrayTypeNodeType:
    return printArrayType(node, indent);
  case PropertySignatureNodetype:
  {
    printf("PropertySignature {\n");

    createIdentation(indent + 1);

    printf("key: {\n");

    visitNode(node->data.propertySignature.key, indent + 2);

    createIdentation(indent + 1);

    printf("}\n");

    createIdentation(indent + 1);

    printf("optional: %s\n", node->data.propertySignature.optional == 0 ? "false" : "true");

    visitNode(node->data.propertySignature.typeAnnotation, indent + 1);

    createIdentation(indent);

    printf("}\n");

    break;
  }
  case TypeLiteralNodeType:
  {
    printf("TypeLiteral {\n");

    createIdentation(indent + 1);

    printf("members: [");

    if (node->data.TypeLiteral.members->count > 0)
    {
      printf("\n");

      printList(node->data.TypeLiteral.members, indent + 2);
    }

    createIdentation(indent + 1);

    printf("]\n");

    createIdentation(indent);

    printf("}\n");

    break;
  }
  case FunctionDeclarationNodeType:
    return printFunctionDeclaration(node, indent);
  case TypeAliasDeclarationNodeType:
    return printTypeAliasDeclaration(node, indent);
  case TupleTypeNodeType:
    return printTupleType(node, indent);
  case TypeReferenceNodeType:
    return printTypeReference(node, indent);
  case BinaryExpressionNodeType:
    return printBinaryExpression(node, indent);
  case ErrorNodeType:
  {
    createIdentation(indent);

    printf("Error: %s\n", node->data.error);

    break;
  }
  }
}

Node *parse(char **source)
{
  Lexer *lexer = lexerFactory(source);

  Node *tree = program(lexer);

  free(lexer);

  return tree;
}