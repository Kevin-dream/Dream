#include "Error.h"

void error(unsigned int line,unsigned int column,const char* msg){
	std::printf("line %d:column %d:Error: %s\n",line,column,msg);
}

void lex_error(location loc,const char* msg){
	error(loc.line,loc.column,msg);
}

void parse_error(Token* tok,const char* msg){
	error(tok->loc.line,tok->loc.column,msg);
}
