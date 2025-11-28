#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <cstring>
#include "Token.h"
#include "Error.h"

extern unsigned int error_count;

static Token* list;

void TokenPush(TokenType type);
void TokenPush(TokenType type,unsigned int length,char* lexeme,unsigned int line,unsigned int column);

TokenType verify_keyword(char* buf,unsigned int length);

Token* Scanning(char* source);

#endif
