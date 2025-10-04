#include <iostream>
#include "Lexer.h"
#include "Parser.h"
using namespace std;

int main() {
	char source[] = "(1+2)*3";
	Token* tokens = Scanning(source);
	AST_Node* ast = parse(tokens);
	ast->toString();
	return 0;
}
