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

/* Node print functions */

static void printEmptyStatement(Node *node, size_t indent)
{
  printf("EmptyStatement\n");
}

static void returnEmptyStatement(Node *node, size_t indent)
{
  printf("ReturnStatement {\n");

  createIdentation(indent + 1);

  printf("argument: {\n");

  visitNode(node->data.returnStatement.argument, indent + 2);

  createIdentation(indent + 1);

  printf("}\n");

  createIdentation(indent);

  printf("}\n");
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

  printf("typeParameters: {");

  visitNode(node->data.functionDeclaration.typeParameters, indent + 1);

  printf("}\n");

  createIdentation(indent + 1);

  printf("params: [");

  visitNode(node->data.functionDeclaration.params, indent + 1);

  printf("]\n");

  createIdentation(indent + 1);

  printf("returnType: { \n");

  visitNode(node->data.functionDeclaration.returnType, indent + 2);

  createIdentation(indent + 1);

  printf("}\n");

  createIdentation(indent + 1);

  printf("body: {\n");

  visitNode(node->data.functionDeclaration.body, indent + 2);

  createIdentation(indent + 1);

  printf("}\n");

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

  visitNode(node->data.typeAliasDeclaration.typeParameters, indent + 1);

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

  visitNode(node->data.typeReference.typeParameters, indent + 1);

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

static void printUpdateExpression(Node *node, size_t indent)
{
  printf("UpdateExpression {\n");

  createIdentation(indent + 1);

  printf("argument: {\n");

  visitNode(node->data.updateExpression.argument, indent + 2);

  createIdentation(indent + 1);

  printf("}\n");

  createIdentation(indent + 1);

  printf("operator: %s\n", node->data.updateExpression.operator);

  createIdentation(indent + 1);

  printf("prefix: %d\n", node->data.updateExpression.prefix);

  createIdentation(indent);

  printf("}\n");
}

static void printTupleType(Node *node, size_t indent)
{
  printf("TupleType [");

  visitNode(node->data.tupleType.elementTypes, indent + 1);

  printf("]\n");
}

static void printArrayType(Node *node, size_t indent)
{
  printf("ArrayType {\n");

  visitNode(node->data.arrayType.elementType, indent + 1);

  createIdentation(indent);

  printf("}\n");
}

static void printProgram(Node *node, size_t indent)
{
  printf("Program {\n");

  createIdentation(indent + 1);

  printf("body: [");

  visitNode(node->data.program.body, indent + 1);

  printf("]\n");

  createIdentation(indent);

  printf("}\n");
}

static void printIfStatement(Node *node, size_t indent)
{
  printf("IfStatement {\n");

  createIdentation(indent + 1);

  printf("test: {\n");

  visitNode(node->data.ifStatement.test, indent + 2);

  createIdentation(indent + 1);

  printf("}\n");

  createIdentation(indent + 1);

  printf("consequent: {\n");

  visitNode(node->data.ifStatement.consequent, indent + 2);

  createIdentation(indent + 1);

  printf("}\n");

  createIdentation(indent + 1);

  printf("alternate: {");

  Node *alternate = node->data.ifStatement.alternate;

  if (alternate)
  {
    printf("\n");

    visitNode(node->data.ifStatement.alternate, indent + 2);

    createIdentation(indent + 1);
  }

  printf("}\n");

  createIdentation(indent);

  printf("}\n");
}

static void printWhileStatement(Node *node, size_t indent)
{
  printf("WhileStatement {\n");

  createIdentation(indent + 1);

  printf("test: {\n");

  visitNode(node->data.whileStatement.test, indent + 2);

  createIdentation(indent + 1);

  printf("}\n");

  createIdentation(indent + 1);

  printf("body: {\n");

  visitNode(node->data.whileStatement.body, indent + 2);

  createIdentation(indent + 1);

  printf("}\n");

  createIdentation(indent);

  printf("}\n");
}

static void printForStatement(Node *node, size_t indent)
{
  printf("ForStatement {\n");

  createIdentation(indent + 1);

  printf("init: {\n");

  visitNode(node->data.forStatement.init, indent + 2);

  createIdentation(indent + 1);

  printf("}\n");

  createIdentation(indent + 1);

  printf("test: {\n");

  visitNode(node->data.forStatement.test, indent + 2);

  createIdentation(indent + 1);

  printf("}\n");

  createIdentation(indent + 1);

  printf("test: {\n");

  visitNode(node->data.forStatement.update, indent + 2);

  createIdentation(indent + 1);

  printf("}\n");

  createIdentation(indent + 1);

  printf("body: {\n");

  visitNode(node->data.forStatement.body, indent + 2);

  createIdentation(indent + 1);

  printf("}\n");

  createIdentation(indent);

  printf("}\n");
}

static void printAssignmentPattern(Node *node, size_t indent)
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
}

static void printExpressionStatement(Node *node, size_t indent)
{
  printf("ExpressionStatement {\n");

  createIdentation(indent + 1);

  printf("expression: {\n");

  visitNode(node->data.expressionStatement.expression, indent + 2);

  createIdentation(indent + 1);

  printf("}\n");

  createIdentation(indent);

  printf("}\n");
}

static void printTypeAnnotation(Node *node, size_t indent)
{
  printf("TypeAnnotation {\n");

  visitNode(node->data.typeAnnotation.typeAnnotation, indent + 1);

  createIdentation(indent);

  printf("}\n");
}

static void printIdentifierNode(Node *node, size_t indent)
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
}

static void printCallExpression(Node *node, size_t indent)
{
  printf("CallExpression {\n");

  createIdentation(indent + 1);

  printf("callee: {\n");

  visitNode(node->data.callExpression.callee, indent + 2);

  createIdentation(indent + 1);

  printf("}\n");

  createIdentation(indent + 1);

  printf("arguments: [");

  visitNode(node->data.callExpression.arguments, indent + 1);

  createIdentation(0);

  printf("]\n");

  if (node->data.callExpression.typeParameters)
  {
    visitNode(node->data.callExpression.typeParameters, indent + 2);
  }

  createIdentation(indent);

  printf("}\n");
}

static void printBlockStatement(Node *node, size_t indent)
{
  printf("BlockStatement {\n");

  Node *body = node->data.block.body;

  createIdentation(indent + 1);

  printf("body: [");

  visitNode(body, indent + 1);

  printf("]\n");

  createIdentation(indent);

  printf("}\n");
}

static void printLiteral(Node *node, size_t indent)
{
  printf("Literal: %s\n", node->data.literal);
}

static void printStringLiteral(Node *node, size_t indent)
{
  printf("StringLiteral: %s\n", node->data.literal);
}

static void printNumericLiteral(Node *node, size_t indent)
{
  printf("NumericLiteral: %s\n", node->data.literal);
}

static void printVariableDeclarator(Node *node, size_t indent)
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
}

void printVariableDeclaration(Node *node, size_t indent)
{
  printf("VariableDeclaration {\n");

  createIdentation(indent + 1);

  printf("kind: %s\n", node->data.variableDeclaration.kind);

  createIdentation(indent + 1);

  printf("declarations: [");

  visitNode(node->data.variableDeclaration.declarations, indent + 1);

  printf("]\n");

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
    return printProgram(node, indent);
  case ReturnStatementNodeType:
    return returnEmptyStatement(node, indent);
  case EmptyStatementNodeType:
    return printEmptyStatement(node, indent);
  case LiteralNodeType:
    return printLiteral(node, indent);
  case NumericLiteralNodeType:
    return printNumericLiteral(node, indent);
  case StringLiteralNodeType:
    return printStringLiteral(node, indent);
  case IdentifierNodeType:
    return printIdentifierNode(node, indent);
  case TypeAnnotationNodeType:
    return printTypeAnnotation(node, indent);
  case ExpressionStatementNodeType:
    return printExpressionStatement(node, indent);
  case IfStatementNodeType:
    return printIfStatement(node, indent);
  case WhileStatementNodeType:
    return printWhileStatement(node, indent);
  case ForStatementNodeType:
    return printForStatement(node, indent);
  case AssignmentPatternNodeType:
    return printAssignmentPattern(node, indent);
  case CallExpressionNodeType:
    return printCallExpression(node, indent);
  case BlockStatementNodeType:
    return printBlockStatement(node, indent);
  case VariableDeclaratorNodeType:
    return printVariableDeclarator(node, indent);
  case VariableDeclarationNodeType:
    return printVariableDeclaration(node, indent);
  case NodeListNodeType:
  {
    if (node->data.list->count)
    {
      printf("\n");

      printList(node->data.list, indent + 1);

      createIdentation(indent);
    }

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

    visitNode(node->data.TypeLiteral.members, indent + 1);

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
  case UpdateExpressionNodeType:
    return printUpdateExpression(node, indent);
  case ErrorNodeType:
  {

    printf("Error: %s\n", node->data.error.msg);

    break;
  }
  }
}

typedef struct Parser
{
  Token *current;
  Token *previous;
  void (*advance)(void);
} Parser;

Parser parser;

static void advance()
{
  parser.previous = parser.current;

  while (1)
  {
    // parser.current = scanToken();

    if (parser.current->type != TokenError)
      break;

    // errorAtCurrent(parser.current.start);
  }
}

Node *parse(char **source)
{
  Parser parser = {.current = NULL, .previous = NULL, .advance = advance};

  Lexer *lexer = lexerFactory(source);

  Node *tree = program(lexer);

  free(lexer);

  return tree;
}