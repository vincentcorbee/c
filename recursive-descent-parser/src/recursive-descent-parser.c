#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <string.h>

#define INITIAL_SIZE 10

/* Typedef linked list */

typedef struct LinkedList LinkedList;

typedef struct LinkedListNode LinkedListNode;

typedef struct LinkedListNode {
  void *data;
  LinkedListNode *next;
} LinkedListNode;

typedef struct LinkedList {
  LinkedListNode *head;
  LinkedListNode *tail;
  int count;
  void (*add)(LinkedList *self, void *data);
  void (*free)(LinkedList *self);
} LinkedList;

void linkedListAddNode(LinkedList *self, void *data);

LinkedListNode* linkedListNodeFactory(void *data);

LinkedList* linkedListFactory();

/* Linked list */

LinkedList* linkedListFactory() {
  LinkedList *linkedList = malloc(sizeof(LinkedList));

  linkedList->head = NULL;

  linkedList->tail = NULL;

  linkedList->add = linkedListAddNode;

  linkedList->count = 0;

  return linkedList;
}

LinkedListNode* linkedListNodeFactory(void *data) {
  LinkedListNode *node = malloc(sizeof(LinkedListNode));

  node->next = NULL;

  node->data = data;

  return node;
}

void linkedListAddNode(LinkedList *self, void *data) {
  LinkedListNode *node = linkedListNodeFactory(data);

  if (self->head == NULL) self->head = node;

  LinkedListNode *tail = self->tail;

  if (tail != NULL) tail->next = node;

  self->tail = node;

  self->count++;
}


/* Typedef Nodes */

typedef enum {
  ErrorNode = -1,
  Program = 0,
  LiteralNode = 1,
  BinaryExpressionNode = 2
} NodeType;

typedef struct Node Node;

typedef char *LiteralValue;

typedef char *ErrorValue;

typedef struct ProgramNode {
  LinkedList *body;
} ProgramNode;

typedef struct BinaryNode {
  char *operator;
  Node *left;
  Node *right;
} BinaryNode;

typedef union {
  ProgramNode program;
  BinaryNode binary;
  LiteralValue literal;
  ErrorValue error;
} NodeData;

typedef struct Node {
  NodeType type;
  NodeData data;
} Node;

/* Typedef Grammar rules */

Node* program(char **input);

LinkedList* statements(char **input);

Node* statement(char **input);

Node* expressionStatement(char **input);

Node* primaryExpression(char **input);

Node* expression(char **input);

Node* assignmentExpression(char **input);

Node* leftHandSideExpression(char **input);

Node* memberExpression(char **input);

Node* conditionalExpression(char **input);

Node* equalityExpression(char **input);

Node* logicalAndExpression(char **input);

Node* logicalOrExpression(char **input);

Node* additiveExpression(char **input);

Node* multiplicativeExpression(char **input);

Node* numericLiteral(char **input);

Node* stringLiteral(char **input);

Node* literal(char **input);

Node* identifier(char **input);

char* identifierStart(char **input);

Node* coverParenthesizedExpression(char **input);

/* Typedef helper functions */

int isEOF(char c);

int hasData(char **input);

int isInteger(char c);

int isNewline(char c);

int isDoubleQuotedString(char c);

char eat(char **input);

char peek(char **input);

int isWhiteSpace(char c);

void eatWhiteSpace(char **input);

void eatNewline(char **input);

void eatWhiteSpaceAndNewline(char **input);

/* Typedef Factory functions */

Node *nodeFactory(NodeType type);

Node* programNodeFactory(char **input);

Node* numericLiteralNodeFactory(char **input);

Node *stringLiteralNodeFactory(char **input);

Node* binaryExpressionNodeFactory(Node *left, char *operator, Node *right);

Node* errorNodeFactory(char *message);


void visitNode(Node *node, size_t indent);

void createIdentation(size_t indent);

void freeNode(Node *node);

/* Typedef Lexer */

typedef struct Lexer Lexer;

typedef struct Token Token;

typedef enum {
  Plus = 0,
  Minus = 1,
  Multiply = 2,
  LParen = 3,
  RParen = 4,
  DoubleQuotedString = 5,
  Integer = 6,
  Semicolon = 7,
  WhiteSpace = 8,
  Newline = 9,
} TokenType;

typedef struct Token {
  TokenType type;
  char * value;
} Token;

typedef struct Lexer {
  int col;
  int line;
  char **source;
  Token* (*next)(Lexer *self);
  Token* (*peek)(Lexer *self);
} Lexer;

Token* peekToken(Lexer *self);

Token* nextToken(Lexer *self);

Token* tokenFactory(Lexer *self, TokenType type);

Lexer* lexerFactory(char **source) {
  Lexer *lexer = malloc(sizeof(Lexer));

  lexer->col = 0;
  lexer->line = 0;
  lexer->source = source;

  lexer->next = nextToken;

  lexer->peek = peekToken;

  return lexer;
}

char* eatDoubleQuotedString(Lexer *self) {
  int index = 0;

  int col = self->col;
  char *source = *self->source;

  char *string = malloc(sizeof(char));

  string[index++] = eat(self->source);

  while(hasData(self->source) && !isDoubleQuotedString(**self->source)) {
    string = realloc(string, (index + 1) * sizeof(char) + 2);

    string[index++] = eat(self->source);

    col++;
  };

  if (!isDoubleQuotedString(**self->source)) {
    free(string);

    self->col = col;
    *self->source = source;

    return NULL;
  }

  string[index++] = eat(self->source);

  string[index] = '\0';

  return string;
}

char* eatInteger(Lexer *self) {
  char *value = malloc(sizeof(char));

  int index = 0;

  while(hasData(self->source) && isInteger(**self->source)) {
    value = realloc(value, (index + 1) * sizeof(char) + 1);

    value[index++] = eat(self->source);

    self->col++;
  };

  value[index] = '\0';

  return value;
}

Token* tokenFactory(Lexer *self, TokenType type) {
  int index = 0;

  Token *token = malloc(sizeof(Token));

  token->type = type;

  switch (type) {
    case Integer:
      token->value = eatInteger(self);

      return token;
    case DoubleQuotedString:
      token->value = eatDoubleQuotedString(self);

      return token;
    default:
      free(token);

      return NULL;
  }
}

Token* nextToken(Lexer *self) {
  if (isInteger(**self->source)) return tokenFactory(self, Integer);

  if(isDoubleQuotedString(**self->source)) return tokenFactory(self, DoubleQuotedString);

  return NULL;
}

Token* peekToken(Lexer *self) {
  char *source = *self->source;
  int col = self->col;

  Token *token = nextToken(self);

  *self->source = source;

  self->col = col;

  return token;
}

/* Function declartions */

void createIdentation(size_t indent) {
  for (int i = 0; i < indent; i++) putchar(' ');
}

void freeNode(Node *node) {
  switch(node->type) {
    case Program: {
        LinkedList *list = node->data.program.body;

        LinkedListNode *current = list->head;

        while(current) {
          freeNode((Node*)current->data);

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

void visitNode(Node *node, size_t indent) {
  createIdentation(indent);

  switch(node->type) {
    case Program:
      printf("Program(\n");

      LinkedListNode * current = node->data.program.body->head;

      while(current) {
        visitNode((Node*)current->data, indent + 1);

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

/* Helpers */

int isEOF(char c) {
  return c == EOF || c == '\0';
}

int hasData(char **input) {
  return strlen(*input) > 0;
}

int isInteger(char c) {
  return c >= '0' && c <= '9';
}

int isAsciiAz(char c) {
  return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

int isIdentifier(char c) {
  return isAsciiAz(c) || c == '$';
}

int isNewline(char c) {
  return c == '\n';
}

int isDoubleQuotedString(char c) {
  return c == '"';
}

int isWhiteSpace(char c) {
  return c == ' ';
}

void eatWhiteSpace(char **input) {
  while(isWhiteSpace(**input)) eat(input);
}

void eatNewline(char **input) {
  while(isNewline(**input)) eat(input);
}

void eatWhiteSpaceAndNewline(char **input) {
  while(isWhiteSpace(**input) || isNewline(**input)) eat(input);
}

char eat(char **input) {
  return *(*input)++;
}

char peek(char **input) {
  return **input;
}

/* Node factory functions */

Node *nodeFactory(NodeType type) {
  Node *node = malloc(sizeof(Node));

  node->type = type;

  return node;
}

Node *programNodeFactory(char **input) {
  return nodeFactory(Program);
}

Node *errorNodeFactory(char *message) {
  Node *node = nodeFactory(ErrorNode);

  node->data.error = message;

  return node;
}

Node *numericLiteralNodeFactory(char **input) {
  Node *node = nodeFactory(LiteralNode);

  node->data.literal = malloc(sizeof(char));

  int index = 0;

  while(hasData(input) && isInteger(**input)) {
    node->data.literal = realloc(node->data.literal, (index + 1) * sizeof(char) + 1);

    node->data.literal[index++] = eat(input);
  };

  node->data.literal[index] = '\0';

  return node;
}

Node *stringLiteralNodeFactory(char **input) {
  int index = 0;

  char *literal = malloc(sizeof(char));

  literal[index++] = eat(input);

  while(hasData(input) && !isDoubleQuotedString(**input)) {
    literal = realloc(literal, (index + 1) * sizeof(char) + 2);

    literal[index++] = eat(input);
  };

  if (!isDoubleQuotedString(**input)) {
    free(literal);

    return errorNodeFactory("StringLiteral expected \"");
  }

  Node *node = nodeFactory(LiteralNode);

  literal[index++] = eat(input);

  literal[index] = '\0';

  node->data.literal = literal;

  return node;
}

Node *binaryExpressionNodeFactory(Node *left, char *operator, Node *right) {
  Node * node = malloc(sizeof(Node));

  node->type = BinaryExpressionNode;

  node->data.binary.left = left;

  node->data.binary.operator = operator;

  node->data.binary.right = right;

  return node;
}

/* Grammar */

/*
  Program ::= Statements
*/

Node* program(char **input) {
  Node *node = programNodeFactory(input);

  node->data.program.body = statements(input);

  return node;
}

/*
  Statements ::=
    Statement
  | Statement Statements
*/

LinkedList* statements(char **input) {
  LinkedList *statements = linkedListFactory();

  int index = 0;

  while(hasData(input)) {
    Node *node = statement(input);

    statements->add(statements, node);

    if (node->type == ErrorNode) break;
  }

  return statements;
}

/*
  Statement ::= ExpressionStatement ";"
*/

Node* statement(char **input) {
  Node *node = expressionStatement(input);

  if (node->type == ErrorNode) return node;

  if (**input != ';') return errorNodeFactory("Statement expected ;");

  eat(input);

  return node;
}

/*
  ExpressionStatement ::= Expression
*/
Node* expressionStatement(char **input) {
  return expression(input);
}

/*
  Expression ::= AssignmentExpression
*/
Node* expression(char **input) {
  return assignmentExpression(input);
}

/*
  AssignmentExpression ::=
      ConditionalExpression
    | LeftHandSideExpression
    | LeftHandSideExpression "=" AssignmentExpression
*/
Node* assignmentExpression(char **input) {
  char *curInput = *input;

  Node *expression = conditionalExpression(input);

  if(expression->type != ErrorNode) return expression;

  *input = curInput;

  freeNode(expression);

  Node *left = leftHandSideExpression(input);

  if (left->type == ErrorNode) return left;

  while(peek(input) == '=') {
    eat(input);

    char *operator = "=";

    Node *right = assignmentExpression(input);

    left = binaryExpressionNodeFactory(left, operator, right);
  }

  return left;
}

/*
  LeftHandSideExpression ::=
      MemberExpression
*/
Node* leftHandSideExpression(char **input) {
  return memberExpression(input);
}

/*
  MemberExpression ::=
      PrimaryExpression
*/
Node* memberExpression(char **input) {
  return primaryExpression(input);
}

/*
  ConditionalExpression ::= EqualityExpression
*/
Node* conditionalExpression(char **input) {
  return equalityExpression(input);
}

/*
  EqualityExpression ::=
      logicalOrExpression
    | EqualityExpression "==" logicalOrExpression
*/
Node* equalityExpression(char **input) {
  Node *left = logicalOrExpression(input);


  while(peek(input) == '=' || peek(input) == '!') {
    char startOperator = eat(input);

    char peekedChar = peek(input);

    if (peekedChar != '=' && peekedChar != '!') {
      freeNode(left);

      return errorNodeFactory("EqualityExpression expected = | !");
    };

    eat(input);

    char *operator = startOperator == '=' ? "==" : "!=";

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
Node* logicalOrExpression(char **input) {
  Node *left = logicalAndExpression(input);

  while(peek(input) == '|') {
    eat(input);

    if (peek(input) != '|') {
      freeNode(left);

      return errorNodeFactory("LogicalOrExpression expected |");
    };

    eat(input);

    char *operator = "||";

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
Node* logicalAndExpression(char **input) {
  Node *left = additiveExpression(input);

  while(peek(input) == '&') {
    eat(input);

    if (peek(input) != '&') {
      freeNode(left);

      return errorNodeFactory("LogicalAndExpression expected &");
    };

    eat(input);

    char *operator = "&&";

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
Node* primaryExpression(char **input){
  if(peek(input) == '(') return coverParenthesizedExpression(input);

  return literal(input);
}

/*
  Literal ::=
      NumericLiteral
    | StringLiteral
    | Identifier
*/
Node* literal(char **input) {
  if(isInteger(**input)) return numericLiteral(input);

  if(isDoubleQuotedString(**input)) return stringLiteral(input);

  if(isIdentifier(**input)) return identifier(input);

  return errorNodeFactory("Literal could not parse");
}

/*
  NumericLiteral ::= [0-9]+
*/
Node* numericLiteral(char **input) {
  return numericLiteralNodeFactory(input);
}

/*
  StringLiteral ::= '"' [^"]* '"'
*/
Node* stringLiteral(char **input) {
  return stringLiteralNodeFactory(input);
}

/*
  Identifier ::= IdentiferStart IdentifierPart?
*/
Node* identifier(char **input) {
  char *start = identifierStart(input);

  if (start == NULL) return errorNodeFactory("Expected identifier");

  Node *node = nodeFactory(LiteralNode);

  node->data.literal = start;

  return node;
}

/*
  IdentifierStart ::= [$A-z]+
*/
char* identifierStart(char **input) {
  char *value = malloc(sizeof(char));

  int index = 0;

  while(hasData(input) && isAsciiAz(**input)) {
    value = realloc(value, (index + 2) * sizeof(char));

    value[index++] = eat(input);
  }

  if (index == 0) {
    free(value);

    return NULL;
  }

  value[index] = '\0';

  return value;
}

/*
  CoverParenthesizedExpression ::= "(" Expression? ")"
*/
Node* coverParenthesizedExpression(char **input) {
  eat(input);

  Node *node = expression(input);

  if (peek(input) != ')') {
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
Node* additiveExpression(char **input) {
  Node *left = multiplicativeExpression(input);

  if (left == NULL) return NULL;

  while(peek(input) == '+' || peek(input) == '-') {
    char *operator = peek(input) == '+' ? "+" : "-";

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
Node* multiplicativeExpression(char **input) {
  Node *left = primaryExpression(input);

  if (left == NULL) return NULL;

  while(peek(input) == '*') {
    eat(input);

    char *operator = "*";

    Node *right = primaryExpression(input);

    left = binaryExpressionNodeFactory(left, operator, right);
  }

  return left;
}


/* Arguments */


typedef struct Arguments {
  char * file;
} Arguments;

Arguments* getArguments(int argc, char *argv[]);


Arguments* getArguments(int argc, char *argv[]) {
  Arguments *args = malloc(sizeof(Arguments));

  int index = 1;

  while(index < argc) {
    char *arg = argv[index++];

    if (strcmp(arg,"--file") == 0) {
      char *value = argv[index++];

      args->file = value;
    }
  }

  return args;
}

/* Main function */

int main(int argc, char *argv[]) {
  char *input = malloc(INITIAL_SIZE * sizeof(char));

  if (input == NULL) return 1;

  Arguments *args = getArguments(argc, argv);

  FILE *ptr = args->file == NULL ? stdin : fopen(args->file, "r");

  size_t length = 0;
  size_t capacity = INITIAL_SIZE;

  int c;

  while((c = fgetc(ptr)) != EOF) {
    if(!isWhiteSpace(c) && !isNewline(c)) input[length++] = c;

    if (length >= capacity) {
      capacity *= 2;

      char *newInput = realloc(input, capacity * sizeof(char));

      if (newInput == NULL) {
        free(input);

        free(args);

        fclose(ptr);

        return 1;
      }

      input = newInput;
    }
  }

  fclose(ptr);

  input[length] = '\0';

  char *originalInput = input;

  Node * result = program(&input);

  visitNode(result, 0);

  freeNode(result);

  free(originalInput);

  free(args);

  input = NULL;

  char *test = "1234\"abcd\"98";

  Lexer *lexer = lexerFactory(&test);

  Token *token = NULL;

  // token = peekToken(lexer);

  // if(token != NULL) {
  //   printf("token: %s\n", token->value);
  //   free(token->value);
  //   free(token);
  // }



  while ((token = lexer->next(lexer)) != NULL) {
    printf("token: %s\n", token->value);
    free(token->value);
    free(token);
  }

  free(lexer);

  printf("column %d\n", lexer->col);

  return 0;
}
