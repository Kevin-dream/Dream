#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <cstring>
#include "Token.h"

static Token* list;

void TokenPush(TokenType type);
void TokenPush(TokenType type,unsigned int length,char* lexeme);

Token* Scanning(char* source);

#endif
