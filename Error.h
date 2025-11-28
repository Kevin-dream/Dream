#ifndef ERROR_H
#define ERROR_H

#include <iostream> 
#include "Token.h"

void error(unsigned int line,unsigned int column,const char* msg);

void lex_error(location loc,const char* msg);
void parse_error(Token* tok,const char* msg);

#endif
