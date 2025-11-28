#ifndef PARSER_H
#define PARSER_H

#include <cstring>
#include <vector>
#include "AstNode.h"
#include "Token.h"
#include "Error.h"

extern unsigned int error_count;

static Token* token;

Token* peek();
Token* next();
bool match(TokenType type);

AST_Node* parse(Token* list);
AST_Node* parseStatement();
AST_Node* parseStatementsBlock();
AST_Node* parseIfStatement();
AST_Node* parseForStatement();
AST_Node* parseWhileStatement();
AST_Node* parseExprStatement();
AST_Node* parseExpr();
AST_Node* parseCommaExpr();
AST_Node* parseCommaTail(AST_Node* left);
AST_Node* parseAddExpr();
AST_Node* parseAddTail(AST_Node* left);
AST_Node* parseMultExpr();
AST_Node* parseMultTail(AST_Node* left);
AST_Node* parseUnaryExpr();
AST_Node* parseUnaryTail();
AST_Node* parsePostfixExpr();
AST_Node* parsePostfixTail(AST_Node* left);
AST_Node* parseParenthesesPair();
AST_Node* parseTermExpr();

#endif
