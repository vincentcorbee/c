#ifndef grammar_H
#define grammar_H

#ifdef grammar_IMPORT
#define GRAMMAR_EXTERN
#else
#define GRAMMAR_EXTERN extern
#endif

#include "node.h"
#include "lexer.h"

/* Types */

Node *typeAnnotation(Lexer *lexer);
Node *type(Lexer *lexer);
Node *unionType(Lexer *lexer);
Node *intersectionType(Lexer *lexer);
Node *primaryType(Lexer *lexer);
Node *predefinedType(Lexer *lexer);
Node *arrayType(Lexer *lexer, Node *pt);
Node *tupleType(Lexer *lexer);
Node *tupleList(Lexer *lexer);
Node *tupleItem(Lexer *lexer);
Node *typeReference(Lexer *lexer);
Node *TypeLiteral(Lexer *lexer);
Node *typeBody(Lexer *lexer);
Node *typeMemberList(Lexer *lexer);
Node *typeMember(Lexer *lexer);
Node *propertySignature(Lexer *lexer);
Node *callSignature(Lexer *lexer);
Node *parameterList(Lexer *lexer);
Node *requiredParameterList(Lexer *lexer);
Node *requiredParameter(Lexer *lexer);
Node *propertyName(Lexer *lexer);
Node *typeParameters(Lexer *lexer);
Node *typeParameterList(Lexer *lexer);
Node *typeParameter(Lexer *lexer);

Node *variableDeclaration(Lexer *lexer);
Node *letOrConst(Lexer *lexer);
Node *variableBinding(Lexer *lexer);
Node *initializer(Lexer *lexer);

Node *program(Lexer *lexer);

Node *statements(Lexer *lexer);

Node *statementItem(Lexer *lexer);

Node *blockStatement(Lexer *lexer);

Node *declaration(Lexer *lexer);

Node *functionDeclaration(Lexer *lexer);

Node *arrowFunctionBody(Lexer *lexer);

Node *functionBody(Lexer *lexer);

Node *functionParameters(Lexer *lexer);

Node *functionParametersList(Lexer *lexer);

Node *bindingList(Lexer *lexer);

Node *lexicalBinding(Lexer *lexer);

Node *statement(Lexer *lexer);

Node *expressionStatement(Lexer *lexer);

Node *primaryExpression(Lexer *lexer);

Node *expression(Lexer *lexer);

Node *assignmentExpression(Lexer *lexer);

Node *leftHandSideExpression(Lexer *lexer);

Node *memberExpression(Lexer *lexer);

Node *conditionalExpression(Lexer *lexer);

Node *equalityExpression(Lexer *lexer);

Node *logicalAndExpression(Lexer *lexer);

Node *logicalOrExpression(Lexer *lexer);

Node *additiveExpression(Lexer *lexer);

Node *multiplicativeExpression(Lexer *lexer);

Node *numericLiteral(Lexer *lexer);

Node *stringLiteral(Lexer *lexer);

Node *literal(Lexer *lexer);

Node *lexicalBinding(Lexer *lexer);

Node *bindingIdentifier(Lexer *lexer);

Node *identifier(Lexer *lexer);

char *identifierStart(Lexer *lexer);

Node *coverParenthesizedExpression(Lexer *lexer);

Node *accessibiltyModifier(Lexer *lexer);

Node *functionParameter(Lexer *lexer);

Node *arguments(Lexer *lexer);
Node *argumentsList(Lexer *lexer);
Node *callExpression(Lexer *lexer);

#undef grammar_IMPORT
#undef GRAMMAR_EXTERN
#endif