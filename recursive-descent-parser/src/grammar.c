#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "helpers.h"
#include "lexer.h"

/* Including own header for checking by compiler */
#define grammar_IMPORT

#include "grammar.h"

/* Public functions */

/* Grammar */

/* Types */

/*
  TypeAnnotation ::=
    ":" Type
*/
Node *typeAnnotation(Lexer *lexer)
{
  if (lexer->peek(lexer) != TokenColon)
    return errorNodeFactory("Expected ':'", lexer);

  lexer->advance(lexer);

  Node *t = type(lexer);

  if (t->type == ErrorNodeType)
    return t;

  return typeAnnotationNodeFactory(t);
}

/*
  Type ::=
    UnionType
*/
Node *type(Lexer *lexer)
{
  return unionType(lexer);
}

/*
  UnionType ::=
    PrimaryType
  | UnionType "|" IntersectionType
*/
Node *unionType(Lexer *lexer)
{
  Node *node = primaryType(lexer);

  if (node->type == ErrorNodeType)
    return node;

  if (lexer->peek(lexer) != TokenBinaryOr)
    return node;

  LinkedList *list = linkedList_linkedListFactory();

  list->add(list, node);

  while (lexer->peek(lexer) == TokenBinaryOr)
  {
    lexer->advance(lexer);

    list->add(list, intersectionType(lexer));
  }

  return nodeListNodeFactory(list);
}

/*
  IntersectionType ::=
    UnionType
  | IntersectionType "&" PrimaryType
*/
Node *intersectionType(Lexer *lexer)
{
  Node *node = unionType(lexer);

  if (lexer->peek(lexer) != TokenBinaryAnd)
    return node;

  LinkedList *list = linkedList_linkedListFactory();

  list->add(list, node);

  while (lexer->peek(lexer) == TokenBinaryAnd)
  {
    lexer->advance(lexer);

    list->add(list, primaryType(lexer));
  }

  return nodeListNodeFactory(list);
}

/*
  PrimaryType ::=
    PredefinedType
  | TypeLiteral
  | ArrayType
  | TupleType
  | TypeReference
*/
Node *primaryType(Lexer *lexer)
{
  Node *pt = NULL;

  TokenType nextToken = lexer->peek(lexer);

  switch (nextToken)
  {
  case TokenLeftBracket:
    pt = tupleType(lexer);
    break;
  case TokenLeftCurlyBrace:
    pt = TypeLiteral(lexer);
    break;
  case TokenIdendifierStart:
    pt = typeReference(lexer);
    break;
  default:
    pt = predefinedType(lexer);
  }

  if (pt->type == ErrorNodeType)
    return pt;

  nextToken = lexer->peek(lexer);

  switch (nextToken)
  {
  case TokenLeftBracket:
    return arrayType(lexer, pt);
  default:
    return pt;
  }
}

/*
  PredefinedType ::=
    "string"
  | "number"
*/
Node *predefinedType(Lexer *lexer)
{
  TokenType nextToken = lexer->peek(lexer);

  switch (nextToken)
  {
  case TokenInteger:
  case TokenDoubleQuotedString:
    return literalNodeFactory(lexer->next(lexer));
    return literalNodeFactory(lexer->next(lexer));
  case TokenKeywordNumber:
  case TokenKeywordString:
    return literalNodeFactory(lexer->next(lexer));

  default:
    return errorNodeFactory("Unexpected token", lexer);
  }
}

/*
  ArrayType :
    PrimaryType "[" "]"
*/
Node *arrayType(Lexer *lexer, Node *pt)
{
  Node *at = pt;

  while (lexer->peek(lexer) == TokenLeftBracket)
  {
    lexer->advance(lexer);

    if (lexer->peek(lexer) != TokenRightBracket)
    {
      freeNode(at);

      return errorNodeFactory("Expected ']'", lexer);
    }

    lexer->advance(lexer);

    at = arrayTypeNodeFactory(at);
  }

  return at;
}

/*
  TupleType ::=
    "[" TupleList "]"
*/
Node *tupleType(Lexer *lexer)
{
  if (lexer->peek(lexer) != TokenLeftBracket)
    return errorNodeFactory("Expected '['", lexer);

  lexer->advance(lexer);

  Node *tl = tupleList(lexer);

  if (lexer->peek(lexer) != TokenRightBracket)
  {
    freeNode(tl);

    return errorNodeFactory("Expected ']'", lexer);
  }

  lexer->advance(lexer);

  return tupleTypeNodeFactory(tl);
}

/*
  TupleList ::=
    TupleItem
  | TupleList "," TupleItem
*/
Node *tupleList(Lexer *lexer)
{
  LinkedList *list = linkedList_linkedListFactory();

  Node *item = tupleItem(lexer);

  list->add(list, item);

  while (lexer->peek(lexer) == TokenComma)
  {
    lexer->advance(lexer);

    list->add(list, tupleItem(lexer));
  }

  Node *nodeList = nodeListNodeFactory(list);

  return nodeList;
}

/*
  TupleItem ::=
    Type
*/
Node *tupleItem(Lexer *lexer)
{
  return type(lexer);
}

/*
  TypeReference ::=
    Identifier TypeParameters?
*/
Node *typeReference(Lexer *lexer)
{
  Node *tn = identifier(lexer);

  Node *tp = NULL;

  if (lexer->peek(lexer) == TokenLeftAngleBracket)
  {
    tp = typeParameters(lexer);

    if (tp->type == ErrorNodeType)
    {
      freeNode(tn);

      return tp;
    }
  }

  return typeReferenceNodeFactory(tn, tp);
}

/*
  TypeLiteral ::=
    "{" TypeBody? "}"
*/
Node *TypeLiteral(Lexer *lexer)
{
  if (lexer->peek(lexer) != TokenLeftCurlyBrace)
    return errorNodeFactory("Expected '{'", lexer);

  lexer->advance(lexer);

  Node *body = typeBody(lexer);

  if (body->type == ErrorNodeType)
    return body;

  if (lexer->peek(lexer) != TokenRightCurlyBrace)
  {
    freeNode(body);

    return errorNodeFactory("Expected '}'", lexer);
  }

  lexer->advance(lexer);

  return TypeLiteralNodeFactory(body);
}

/*
  TypeBody ::=
    TypeMemberList ";"
  | TypeMemberList ","
*/
Node *typeBody(Lexer *lexer)
{
  Node *tml = typeMemberList(lexer);

  TokenType nextToken = lexer->peek(lexer);

  if (nextToken != TokenSemicolon && nextToken != TokenComma)
  {
    freeNode(tml);

    return errorNodeFactory("Expected ';' | ','", lexer);
  }

  lexer->advance(lexer);

  return tml;
}

/*
  TypeMemberList ::=
    TypeMember
  | TypeMemberList ";" TypeMember
  | TypeMemberList "," TypeMember
*/
Node *typeMemberList(Lexer *lexer)
{
  Node *tm = typeMember(lexer);

  if (tm->type == ErrorNodeType)
    return tm;

  Node *nl = nodeListNodeFactory(NULL);

  nl->data.list->add(nl->data.list, tm);

  TokenType peekedChar = lexer->peek(lexer);

  while (peekedChar == TokenSemicolon || peekedChar == TokenComma)
  {
    LexerState state = getLexerState(lexer);

    lexer->advance(lexer);

    Node *t = typeMember(lexer);

    if (t->type == ErrorNodeType)
    {
      freeNode(t);

      setLexerState(lexer, state);

      break;
    }

    nl->data.list->add(nl->data.list, t);

    peekedChar = lexer->peek(lexer);
  }

  return nl;
}

/*
  TypeMember ::=
    PropertySignature
  | CallSignature
*/
Node *typeMember(Lexer *lexer)
{
  Node *node = propertySignature(lexer);

  return node;
}

/*
  PropertySignature ::=
    PropertyName "?"? TypeAnnotation
*/
Node *propertySignature(Lexer *lexer)
{
  Node *key = propertyName(lexer);

  if (key->type == ErrorNodeType)
    return key;

  int optional = 0;

  if (lexer->peek(lexer) == TokenOptional)
  {
    lexer->advance(lexer);

    optional = 1;
  }

  Node *tp = typeAnnotation(lexer);

  if (tp->type == ErrorNodeType)
  {
    freeNode(key);

    return tp;
  }

  return propertySignatureNodeFactory(key, optional, tp);
}

/*
  CallSignature ::=
    TypeParameters? "(" ParameterList? ")" TypeAnnotation?
*/
Node *callSignature(Lexer *lexer)
{
  return NULL;
}

/*
  ParameterList ::=
    RequiredParameterList
*/
Node *parameterList(Lexer *lexer)
{
  return requiredParameterList(lexer);
}

/*
  RequiredParameterList ::=
    RequiredParameter
  | RequiredParameterList "," RequiredParameter
*/
Node *requiredParameterList(Lexer *lexer)
{
  LinkedList *list = linkedList_linkedListFactory();

  Node *item = requiredParameter(lexer);

  list->add(list, item);

  while (lexer->peek(lexer) == TokenComma)
  {
    lexer->advance(lexer);

    list->add(list, requiredParameter(lexer));
  }

  return nodeListNodeFactory(list);
}

/*
  RequiredParameter ::=
    AccessibiltyModifier? BindingIdentifier
*/
Node *requiredParameter(Lexer *lexer)
{
  return NULL;
}

/*
  PropertyName ::=
    Identifier
*/
Node *propertyName(Lexer *lexer)
{
  return identifier(lexer);
}

/*
  TypeParameters ::=
    "<" TypeParametersList? ">"
*/
Node *typeParameters(Lexer *lexer)
{
  if (lexer->peek(lexer) != TokenLeftAngleBracket)
    return errorNodeFactory("Expected '<'", lexer);

  lexer->advance(lexer);

  if (lexer->peek(lexer) == TokenRightAngleBracket)
  {
    lexer->advance(lexer);

    return nodeListNodeFactory(NULL);
  };

  Node *tp = typeParameterList(lexer);

  if (lexer->peek(lexer) != TokenRightAngleBracket)
  {
    freeNode(tp);

    return errorNodeFactory("Expected '>'", lexer);
  }

  lexer->advance(lexer);

  return tp;
}

/*
  TypeParameterList ::=
    TypeParameter
  | TypeParameterList "," TypeParameter
*/
Node *typeParameterList(Lexer *lexer)
{
  LinkedList *list = linkedList_linkedListFactory();

  Node *item = typeParameter(lexer);

  list->add(list, item);

  while (lexer->peek(lexer) == TokenComma)
  {
    lexer->advance(lexer);

    list->add(list, typeParameter(lexer));
  }

  Node *nodeList = nodeListNodeFactory(list);

  return nodeList;
}

/*
  TypeParameter ::= Type
*/
Node *typeParameter(Lexer *lexer)
{
  return type(lexer);
}

/*
  TypeAliasDeclaration ::=
    "type" BindingIdentifier TypeParameters? "=" Type ";"
*/
Node *typeAliasDeclaration(Lexer *lexer)
{
  if (lexer->peek(lexer) != TokenTypeAlias)
    return errorNodeFactory("Expected 'type'", lexer);

  lexer->advance(lexer);

  Node *id = bindingIdentifier(lexer);

  if (id->type == ErrorNodeType)
    return id;

  Node *tp = typeParameters(lexer);

  if (tp->type == ErrorNodeType)
  {
    tp = nodeListNodeFactory(NULL);
  }

  if (lexer->peek(lexer) != TokenEqual)
    return errorNodeFactory("Expected '='", lexer);

  lexer->advance(lexer);

  Node *t = type(lexer);

  if (tp->type == ErrorNodeType)
  {
    freeNode(id);
    freeNode(tp);

    return t;
  }

  if (t->type != NodeListNodeType)
  {
    LinkedList *list = linkedList_linkedListFactory();

    list->add(list, t);

    t = nodeListNodeFactory(list);
  }

  if (lexer->peek(lexer) != TokenSemicolon)
  {
    freeNode(id);
    freeNode(tp);

    return errorNodeFactory("Expected ';'", lexer);
  }

  lexer->advance(lexer);

  return typeAliasDeclarationNodeFactory(id, tp, t);
}

/*
  Program ::= Statements
*/
Node *program(Lexer *lexer)
{
  return programNodeFactory(statements(lexer));
}

/*
  Statements ::=
    StatementItem
  | Statements StatementItem
*/
Node *statements(Lexer *lexer)
{
  Node *node = nodeListNodeFactory(NULL);

  NodeList *statements = node->data.list;

  while (hasData(lexer->source) && lexer->peek(lexer) != TokenRightCurlyBrace)
  {
    Node *si = statementItem(lexer);

    statements->add(statements, si);

    if (si->type == ErrorNodeType)
    {
      // freeNode(node);

      // node = nodeListNodeFactory(linkedList_linkedListFactory());

      // node->data.list->add(node->data.list, si);

      break;
    }

    // statements->add(statements, si);
  }

  return node;
}

/*
  StatementItem ::=
    Statement
  | Declaration
*/
Node *statementItem(Lexer *lexer)
{
  Node *s = statement(lexer);

  if (s->type != ErrorNodeType)
    return s;

  // freeNode(s);

  return declaration(lexer);
}

/*
  Statement ::=
    ExpressionStatement ";"
    BlockStatement ";"
*/
Node *statement(Lexer *lexer)
{
  Node *st = NULL;

  switch (lexer->peek(lexer))
  {
  case TokenLeftCurlyBrace:
    st = blockStatement(lexer);
    break;
  default:
    st = expressionStatement(lexer);
  }

  if (st->type == ErrorNodeType)
    return st;

  if (lexer->peek(lexer) != TokenSemicolon)
    return errorNodeFactory("Expected ';'", lexer);

  lexer->advance(lexer);

  return st;
}

/*
  BlockStatement ::= "{" Statements? "}"
*/
Node *blockStatement(Lexer *lexer)
{
  if (lexer->peek(lexer) == TokenLeftCurlyBrace)
  {
    lexer->advance(lexer);

    Node *body = NULL;

    if (lexer->peek(lexer) == TokenRightCurlyBrace)
    {
      lexer->advance(lexer);
    }
    else
    {
      body = statements(lexer);

      if (lexer->peek(lexer) != TokenRightCurlyBrace)
      {
        freeNode(body);

        return errorNodeFactory("Expected '}'", lexer);
      }

      lexer->advance(lexer);
    }

    return blockStatementNodeFactory(body);
  }
  else
  {
    return errorNodeFactory("Expected '{'", lexer);
  }
}

/*
  Declaration ::=
    FunctionDeclaration
  | TypeAliasDeclaration
  | VariableDeclaration
*/
Node *declaration(Lexer *lexer)
{
  switch (lexer->peek(lexer))
  {
  case TokenFunc:
    return functionDeclaration(lexer);
  case TokenKeywordConst:
  case TokenKeywordLet:
    return variableDeclaration(lexer);
  case TokenTypeAlias:
    return typeAliasDeclaration(lexer);
  default:
    return errorNodeFactory("Unexpected token", lexer);
  }
}

/*
  VariableDeclaration ::=
    LetOrConst BindingList ";"
*/
Node *variableDeclaration(Lexer *lexer)
{

  Node *kind = letOrConst(lexer);

  if (kind->type == ErrorNodeType)
    return kind;

  Node *bl = bindingList(lexer);

  if (bl->type == ErrorNodeType)
  {
    freeNode(kind);

    return bl;
  }

  if (lexer->peek(lexer) != TokenSemicolon)
  {
    freeNode(kind);

    freeNode(bl);

    return errorNodeFactory("Expected ';'", lexer);
  }

  lexer->advance(lexer);

  return variableDeclarationNodeFactory(kind, bl);
}

/*
  LetOrConst ::=
    "let"
  | "const"
*/

Node *letOrConst(Lexer *lexer)
{
  switch (lexer->peek(lexer))
  {
  case TokenKeywordLet:
  case TokenKeywordConst:
    return literalNodeFactory(lexer->next(lexer));
  default:
    return errorNodeFactory("Expected 'let' | 'const'", lexer);
  }
}

/*
  BindingList ::=
    VariableBinding
  | BindingList "," VariableBinding
*/
Node *bindingList(Lexer *lexer)
{
  Node *vb = variableBinding(lexer);

  if (vb->type == ErrorNodeType)
    return vb;

  Node *bl = nodeListNodeFactory(NULL);

  NodeList *list = bl->data.list;

  list->add(list, vb);

  while (lexer->peek(lexer) == TokenComma)
  {
    lexer->advance(lexer);

    vb = variableBinding(lexer);

    if (vb->type == ErrorNodeType)
    {
      freeNode(bl);

      return vb;
    }

    list->add(list, vb);
  }

  return bl;
}
/*
  VariableBinding ::= BindingIdentifier Initializer?
*/
Node *variableBinding(Lexer *lexer)
{
  Node *bi = bindingIdentifier(lexer);

  if (bi->type == ErrorNodeType)
    return bi;

  Node *init = NULL;

  if (lexer->peek(lexer) == TokenEqual)
  {
    init = initializer(lexer);

    if (init->type == ErrorNodeType)
    {
      freeNode(bi);

      return init;
    }
  }

  return variableDeclaratorNodeFactory(bi, init);
}

/*
  Initializer ::=
    "=" AssignmentExpression
*/
Node *initializer(Lexer *lexer)
{
  if (lexer->peek(lexer) != TokenEqual)
    return errorNodeFactory("Expected '='", lexer);

  lexer->advance(lexer);

  return assignmentExpression(lexer);
}

/*
  `FunctionDeclaration ::=
    "func" BindingIdentifier TypeParameters? FunctionParameters "=>" ArrowFunctionBody ";"`
*/
Node *functionDeclaration(Lexer *lexer)
{
  if (lexer->peek(lexer) == TokenFunc)
  {
    lexer->advance(lexer);

    Node *id = bindingIdentifier(lexer);

    if (id->type == ErrorNodeType)
      return id;

    Node *tp = NULL;

    if (lexer->peek(lexer) == TokenLeftAngleBracket)
    {
      tp = typeParameters(lexer);

      if (tp->type == ErrorNodeType)
      {
        freeNode(id);

        return tp;
      }
    }

    Node *params = functionParameters(lexer);

    if (params->type == ErrorNodeType)
    {
      if (tp)
        freeNode(tp);

      freeNode(id);

      return params;
    }

    if (lexer->peek(lexer) != TokenFatArrow)
    {
      if (tp)
        freeNode(tp);

      freeNode(id);

      freeNode(params);

      return errorNodeFactory("Expected '=>'", lexer);
    }

    lexer->advance(lexer);

    Node *body = arrowFunctionBody(lexer);

    if (lexer->peek(lexer) != TokenSemicolon)
    {
      if (tp)
        freeNode(tp);

      freeNode(id);

      freeNode(params);

      freeNode(body);

      return errorNodeFactory("Expected ';'", lexer);
    }

    lexer->advance(lexer);

    return functionDeclarationNodeFactory(id, params, body, tp);
  }
  else
    return errorNodeFactory("Expected 'func'", lexer);
}

/*
  ArrowFunctionBody ::=
    AssignmentExpression
  | "{" FunctionBody "}"
*/

Node *arrowFunctionBody(Lexer *lexer)
{
  if (lexer->peek(lexer) == TokenLeftCurlyBrace)
  {
    lexer->advance(lexer);

    Node *body = functionBody(lexer);

    if (lexer->peek(lexer) != TokenRightCurlyBrace)
    {
      freeNode(body);

      return errorNodeFactory("ArrowFunctionBody expected }", lexer);
    }

    return blockStatementNodeFactory(body);
  }
  else
  {
    LinkedList *list = linkedList_linkedListFactory();

    Node *node = assignmentExpression(lexer);

    list->add(list, node);

    return nodeListNodeFactory(list);
  }
}

/*
  FunctionBody ::=
    Statements?
*/
Node *functionBody(Lexer *lexer)
{
  return statements(lexer);
}

/*
  FunctionParameters ::=
    "(" FunctionParametersList? ")"
*/
Node *functionParameters(Lexer *lexer)
{
  if (lexer->peek(lexer) != TokenLeftParen)
    return errorNodeFactory("Expected (", lexer);

  lexer->advance(lexer);

  Node *parameters;

  if (lexer->peek(lexer) == TokenRightParen)
  {
    parameters = nodeListNodeFactory(NULL);
  }
  else
  {
    parameters = functionParametersList(lexer);

    if (lexer->peek(lexer) != TokenRightParen)
    {
      freeNode(parameters);

      return errorNodeFactory("Expected )", lexer);
    }
  }

  lexer->advance(lexer);

  return parameters;
}

/*
  FunctionParametersList ::=
    FunctionParameter
  | FunctionParamtersList ',' FunctionParameter
*/
Node *functionParametersList(Lexer *lexer)
{
  LinkedList *list = linkedList_linkedListFactory();

  Node *item = functionParameter(lexer);

  list->add(list, item);

  while (lexer->peek(lexer) == TokenComma)
  {
    lexer->advance(lexer);

    list->add(list, functionParameter(lexer));
  }

  Node *nodeList = nodeListNodeFactory(list);

  return nodeList;
}

/*
  FunctionParameter ::= BindingIdentifier Initializer?
*/
Node *functionParameter(Lexer *lexer)
{
  Node *bi = bindingIdentifier(lexer);

  if (bi->type == ErrorNodeType)
    return bi;

  Node *init = NULL;

  if (lexer->peek(lexer) == TokenEqual)
  {
    init = initializer(lexer);

    if (init->type == ErrorNodeType)
    {
      freeNode(bi);

      return init;
    }

    return assignmentPatternNodeFactory(bi, init);
  }

  return bi;
}

/*
  ExpressionStatement ::= Expression
*/
Node *expressionStatement(Lexer *lexer)
{
  return expression(lexer);
}

/*
  Expression ::= AssignmentExpression
*/
Node *expression(Lexer *lexer)
{
  return assignmentExpression(lexer);
}

/*
  AssignmentExpression ::=
      ConditionalExpression
    | LeftHandSideExpression
    | LeftHandSideExpression "=" AssignmentExpression
*/
Node *assignmentExpression(Lexer *lexer)
{
  LexerState state = getLexerState(lexer);

  Node *ce = conditionalExpression(lexer);

  if (ce->type != ErrorNodeType)
    return ce;

  setLexerState(lexer, state);

  freeNode(ce);

  Node *lhse = leftHandSideExpression(lexer);

  if (lhse->type == ErrorNodeType)
    return lhse;

  while (lexer->peek(lexer) == TokenEqual)
  {
    Token *token = lexer->next(lexer);

    char *operator= token->value;

    free(token);

    Node *ae = assignmentExpression(lexer);

    lhse = binaryExpressionNodeFactory(lhse, operator, ae);
  }

  return lhse;
}

/*
  LeftHandSideExpression ::=
    CallExpression
  | MemberExpression
*/
Node *leftHandSideExpression(Lexer *lexer)
{
  return callExpression(lexer);
}

/*
  CallExpression ::= MemberExpression TypeParameters? Arguments
*/
Node *callExpression(Lexer *lexer)
{
  Node *me = memberExpression(lexer);

  if (me->type == ErrorNodeType)
    return me;

  Node *tp = NULL;

  if (lexer->peek(lexer) == TokenLeftAngleBracket)
  {
    tp = typeParameters(lexer);

    if (tp->type == ErrorNodeType)
    {
      freeNode(me);

      return tp;
    }
  }

  if (lexer->peek(lexer) != TokenLeftParen)
  {
    if (tp)
      freeNode(tp);

    return me;
  }

  Node *args = arguments(lexer);

  if (args->type == ErrorNodeType)
  {
    freeNode(me);

    if (tp)
      freeNode(tp);

    return args;
  }

  return callExpressionNodeFactory(me, args, tp);
}

/*
  Arguments ::= "(" ArgumentList? ")"
*/
Node *arguments(Lexer *lexer)
{
  lexer->advance(lexer);

  Node *al = NULL;

  if (lexer->peek(lexer) == TokenRightParen)
  {
    al = nodeListNodeFactory(NULL);
  }

  al = argumentsList(lexer);

  if (al->type == ErrorNodeType)
    return al;

  if (lexer->peek(lexer) != TokenRightParen)
  {
    freeNode(al);

    return errorNodeFactory("Expected ')'", lexer);
  }

  lexer->advance(lexer);

  return al;
}

/*
  ArgumentsList ::=
    AssignmentExpression
  | ArgumentList "," AssignmentExpression
*/
Node *argumentsList(Lexer *lexer)
{
  Node *ae = assignmentExpression(lexer);

  if (ae->type == ErrorNodeType)
    return ae;

  Node *al = nodeListNodeFactory(NULL);

  NodeList *list = al->data.list;

  list->add(list, ae);

  while (lexer->peek(lexer) == TokenComma)
  {
    lexer->advance(lexer);

    ae = assignmentExpression(lexer);

    if (ae->type == ErrorNodeType)
    {
      free(al);

      return ae;
    }

    list->add(list, ae);
  }

  return al;
}

/*
  MemberExpression ::=
      PrimaryExpression
*/
Node *memberExpression(Lexer *lexer)
{
  return primaryExpression(lexer);
}

/*
  ConditionalExpression ::= EqualityExpression
*/
Node *conditionalExpression(Lexer *lexer)
{
  return equalityExpression(lexer);
}

/*
  EqualityExpression ::=
      LogicalOrExpression
    | EqualityExpression "==" LogicalOrExpression
    | EqualityExpression "!=" LogicalOrExpression
*/
Node *equalityExpression(Lexer *lexer)
{
  Node *left = logicalOrExpression(lexer);

  if (lexer->peek(lexer) == TokenEqual || lexer->peek(lexer) == TokenNot)
  {
    freeNode(left);

    return errorNodeFactory("EqualityExpression expected '=' | '!'", lexer);
  };

  while (lexer->peek(lexer) == TokenEqualEqual || lexer->peek(lexer) == TokenNotEqual)
  {
    Token *token = lexer->next(lexer);

    char *operator= token->value;

    free(token);

    Node *right = logicalOrExpression(lexer);

    left = binaryExpressionNodeFactory(left, operator, right);
  }

  return left;
}

/*
  LogicalOrExpression ::=
      LogicalAndExpression
    | LogicalOrExpression "||" LogicalAndExpression
*/
Node *logicalOrExpression(Lexer *lexer)
{
  Node *left = logicalAndExpression(lexer);

  while (lexer->peek(lexer) == TokenLogicalOr)
  {
    Token *token = lexer->next(lexer);

    char *operator= token->value;

    free(token);

    Node *right = logicalAndExpression(lexer);

    left = binaryExpressionNodeFactory(left, operator, right);
  }

  return left;
}

/*
  LogicalAndExpression ::=
      LogicalAndExpression
    | LogicalAndExpression "&&" AdditiveExpression
*/
Node *logicalAndExpression(Lexer *lexer)
{
  Node *left = additiveExpression(lexer);

  while (lexer->peek(lexer) == TokenLogicalAnd)
  {
    Token *token = lexer->next(lexer);

    char *operator= token->value;

    free(token);

    Node *right = additiveExpression(lexer);

    left = binaryExpressionNodeFactory(left, operator, right);
  }

  return left;
}

/*
  PrimaryExpression ::=
      Literal
    | CoverParenthesizedExpression
*/
Node *primaryExpression(Lexer *lexer)
{
  if (lexer->peek(lexer) == TokenLeftParen)
    return coverParenthesizedExpression(lexer);

  return literal(lexer);
}

/*
  Literal ::=
      NumericLiteral
    | StringLiteral
    | Identifier
*/
Node *literal(Lexer *lexer)
{
  switch (lexer->peek(lexer))
  {
  case TokenInteger:
    return numericLiteral(lexer);
  case TokenDoubleQuotedString:
    return stringLiteral(lexer);
  case TokenIdendifierStart:
    return identifier(lexer);
  default:
  {
    char a[] = "Unexspected character";

    char b[3] = {' ', peek(lexer->source), '\0'};

    char message[strlen(a) + strlen(b) + 1];

    strcpy(message, a);

    strcat(message, b);

    return errorNodeFactory(message, lexer);
  }
  }
}

/*
  NumericLiteral ::= [0-9]+
*/
Node *numericLiteral(Lexer *lexer)
{
  return literalNodeFactory(lexer->next(lexer));
}

/*
  StringLiteral ::= '"' [^"]* '"'
*/
Node *stringLiteral(Lexer *lexer)
{
  return literalNodeFactory(lexer->next(lexer));
}

/*
  BindingIdentifier ::=
    Identifier TypeAnnotation?
*/
Node *bindingIdentifier(Lexer *lexer)
{
  Node *i = identifier(lexer);

  if (i->type == ErrorNodeType)
    return i;

  if (lexer->peek(lexer) == TokenColon)
  {
    Node *ta = typeAnnotation(lexer);

    if (ta->type == ErrorNodeType)
    {
      freeNode(i);

      return ta;
    }

    i->data.identifier.typeAnnotation = ta;
  }

  return i;
}

/*
  Identifier ::= IdentiferStart IdentifierPart?
*/
Node *identifier(Lexer *lexer)
{
  char *start = identifierStart(lexer);

  if (start == NULL)
    return errorNodeFactory("Expected identifier", lexer);

  Node *node = identifierNodeFactory(start, NULL);

  return node;
}

/*
  IdentifierStart ::= [_$A-z]+
*/
char *identifierStart(Lexer *lexer)
{
  TokenType nextToken = lexer->peek(lexer);

  if (nextToken == TokenIdendifierStart)
  {
    Token *token = lexer->next(lexer);

    char *value = token->value;

    free(token);

    return value;
  }

  return NULL;
}

/*
  CoverParenthesizedExpression ::= "(" Expression? ")"
*/
Node *coverParenthesizedExpression(Lexer *lexer)
{
  lexer->advance(lexer);

  Node *node = expression(lexer);

  if (lexer->peek(lexer) != TokenRightParen)
  {
    freeNode(node);

    return errorNodeFactory("CoverParenthesizedExpression expected )", lexer);
  }

  lexer->advance(lexer);

  return node;
}

/*
  AdditiveExpression ::=
      MultiplicativeExpression
    | MultiplicativeExpression [+-] MultiplicativeExpression
*/
Node *additiveExpression(Lexer *lexer)
{
  Node *left = multiplicativeExpression(lexer);

  if (left == NULL)
    return NULL;

  while (lexer->peek(lexer) == TokenPlus || lexer->peek(lexer) == TokenMinus)
  {
    Token *token = lexer->next(lexer);

    char *operator= token->value;

    free(token);

    Node *right = multiplicativeExpression(lexer);

    left = binaryExpressionNodeFactory(left, operator, right);
  }

  return left;
}

/*
  MultiplicativeExpression ::=
      LeftHandSideExpression
    | MultiplicativeExpression "*" LeftHandSideExpression
*/
Node *multiplicativeExpression(Lexer *lexer)
{
  Node *left = leftHandSideExpression(lexer);

  if (left == NULL)
    return NULL;

  while (lexer->peek(lexer) == TokenMultiply)
  {
    Token *token = lexer->next(lexer);

    char *operator= token->value;

    free(token);

    Node *right = leftHandSideExpression(lexer);

    left = binaryExpressionNodeFactory(left, operator, right);
  }

  return left;
}

/*
  AccessibiltyModifier ::=
      "public"
    | "private"
*/
Node *accessibiltyModifier(Lexer *lexer)
{
  TokenType nextToken = lexer->peek(lexer);

  if (nextToken != TokenKeywordPublic && nextToken != TokenKeywordPrivate)
    return NULL;

  return literalNodeFactory(lexer->next(lexer));
}