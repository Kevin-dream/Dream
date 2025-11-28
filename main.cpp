#include <iostream>
#include "Lexer.h"
#include "Parser.h"

unsigned int error_count = 0;

int main(int argc,char** argv) {
	char source[] = "if(1){\n"
	                "printf(\"hello world\");\n"
	                "}";
	Token* tokens = Scanning(source);
	AST_Node* ast = parse(tokens);
	ast->toString();
	return 0;
}
