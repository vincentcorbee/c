#include <stdlib.h>

#include "helpers.h"

/* Including own header for checking by compiler */
#define grammar_IMPORT

#include "grammar.h"

/* Public functions */

/* Grammar */

/*
  Program ::= Statements
*/
Node *program(char **input)
{
  Node *node = programNodeFactory();

  node->data.program.body = statements(input);

  return node;
}

/*
  Statements ::=
    Statement
  | Statement Statements
*/
LinkedList *statements(char **input)
{
  LinkedList *statements = linkedList_linkedListFactory();

  while (hasData(input))
  {
    Node *node = statement(input);

    statements->add(statements, node);

    if (node->type == ErrorNode)
      break;
  }

  return statements;
}

/*
  Statement ::= ExpressionStatement ";"
*/
Node *statement(char **input)
{
  Node *node = expressionStatement(input);

  if (node->type == ErrorNode)
    return node;

  if (**input != ';')
    return errorNodeFactory("Statement expected ;");

  eat(input);

  return node;
}

/*
  ExpressionStatement ::= Expression
*/
Node *expressionStatement(char **input)
{
  return expression(input);
}

/*
  Expression ::= AssignmentExpression
*/
Node *expression(char **input)
{
  return assignmentExpression(input);
}

/*
  AssignmentExpression ::=
      ConditionalExpression
    | LeftHandSideExpression
    | LeftHandSideExpression "=" AssignmentExpression
*/
Node *assignmentExpression(char **input)
{
  char *curInput = *input;

  Node *expression = conditionalExpression(input);

  if (expression->type != ErrorNode)
    return expression;

  *input = curInput;

  freeNode(expression);

  Node *left = leftHandSideExpression(input);

  if (left->type == ErrorNode)
    return left;

  while (peek(input) == '=')
  {
    eat(input);

    char *operator= "=";

    Node *right = assignmentExpression(input);

    left = binaryExpressionNodeFactory(left, operator, right);
  }

  return left;
}

/*
  LeftHandSideExpression ::=
      MemberExpression
*/
Node *leftHandSideExpression(char **input)
{
  return memberExpression(input);
}

/*
  MemberExpression ::=
      PrimaryExpression
*/
Node *memberExpression(char **input)
{
  return primaryExpression(input);
}

/*
  ConditionalExpression ::= EqualityExpression
*/
Node *conditionalExpression(char **input)
{
  return equalityExpression(input);
}

/*
  EqualityExpression ::=
      logicalOrExpression
    | EqualityExpression "==" logicalOrExpression
*/
Node *equalityExpression(char **input)
{
  Node *left = logicalOrExpression(input);

  while (peek(input) == '=' || peek(input) == '!')
  {
    char startOperator = eat(input);

    char peekedChar = peek(input);

    if (peekedChar != '=' && peekedChar != '!')
    {
      freeNode(left);

      return errorNodeFactory("EqualityExpression expected = | !");
    };

    eat(input);

    char *operator= startOperator == '=' ? "==" : "!=";

    Node *right = logicalOrExpression(input);

    left = binaryExpressionNodeFactory(left, operator, right);
  }

  return left;
}

/*
  LogicalOrExpression ::=
      LogicalAndExpression
    | LogicalOrExpression "||" LogicalAndExpression
*/
Node *logicalOrExpression(char **input)
{
  Node *left = logicalAndExpression(input);

  while (peek(input) == '|')
  {
    eat(input);

    if (peek(input) != '|')
    {
      freeNode(left);

      return errorNodeFactory("LogicalOrExpression expected |");
    };

    eat(input);

    char *operator= "||";

    Node *right = logicalAndExpression(input);

    left = binaryExpressionNodeFactory(left, operator, right);
  }

  return left;
}

/*
  LogicalAndExpression ::=
      LogicalAndExpression
    | LogicalAndExpression "||" AdditiveExpression
*/
Node *logicalAndExpression(char **input)
{
  Node *left = additiveExpression(input);

  while (peek(input) == '&')
  {
    eat(input);

    if (peek(input) != '&')
    {
      freeNode(left);

      return errorNodeFactory("LogicalAndExpression expected &");
    };

    eat(input);

    char *operator= "&&";

    Node *right = additiveExpression(input);

    left = binaryExpressionNodeFactory(left, operator, right);
  }

  return left;
}

/*
  PrimaryExpression ::=
      Literal
    | CoverParenthesizedExpression
*/
Node *primaryExpression(char **input)
{
  if (peek(input) == '(')
    return coverParenthesizedExpression(input);

  return literal(input);
}

/*
  Literal ::=
      NumericLiteral
    | StringLiteral
    | Identifier
*/
Node *literal(char **input)
{
  if (isInteger(**input))
    return numericLiteral(input);

  if (isDoubleQuotedString(**input))
    return stringLiteral(input);

  if (isIdentifierStart(**input))
    return identifier(input);

  return errorNodeFactory("Literal could not parse");
}

/*
  NumericLiteral ::= [0-9]+
*/
Node *numericLiteral(char **input)
{
  return numericLiteralNodeFactory(input);
}

/*
  StringLiteral ::= '"' [^"]* '"'
*/
Node *stringLiteral(char **input)
{
  return stringLiteralNodeFactory(input);
}

/*
  Identifier ::= IdentiferStart IdentifierPart?
*/
Node *identifier(char **input)
{
  char *start = identifierStart(input);

  if (start == NULL)
    return errorNodeFactory("Expected identifier");

  Node *node = nodeFactory(LiteralNode);

  node->data.literal = start;

  return node;
}

/*
  IdentifierStart ::= [$A-z]+
*/
char *identifierStart(char **input)
{
  char *value = malloc(sizeof(char));

  int index = 0;

  while (hasData(input) && isAsciiAz(**input))
  {
    value = realloc(value, (index + 2) * sizeof(char));

    value[index++] = eat(input);
  }

  if (index == 0)
  {
    free(value);

    return NULL;
  }

  value[index] = '\0';

  return value;
}

/*
  CoverParenthesizedExpression ::= "(" Expression? ")"
*/
Node *coverParenthesizedExpression(char **input)
{
  eat(input);

  Node *node = expression(input);

  if (peek(input) != ')')
  {
    freeNode(node);

    return errorNodeFactory("CoverParenthesizedExpression expected )");
  }

  eat(input);

  return node;
}

/*
  AdditiveExpression ::=
      MultiplicativeExpression
    | MultiplicativeExpression [+-] MultiplicativeExpression
*/
Node *additiveExpression(char **input)
{
  Node *left = multiplicativeExpression(input);

  if (left == NULL)
    return NULL;

  while (peek(input) == '+' || peek(input) == '-')
  {
    char *operator= peek(input) == '+' ? "+" : "-";

    eat(input);

    Node *right = multiplicativeExpression(input);

    left = binaryExpressionNodeFactory(left, operator, right);
  }

  return left;
}

/*
  MultiplicativeExpression ::=
      PrimaryExpression
    | PrimaryExpression "*" PrimaryExpression
*/
Node *multiplicativeExpression(char **input)
{
  Node *left = primaryExpression(input);

  if (left == NULL)
    return NULL;

  while (peek(input) == '*')
  {
    eat(input);

    char *operator= "*";

    Node *right = primaryExpression(input);

    left = binaryExpressionNodeFactory(left, operator, right);
  }

  return left;
}
