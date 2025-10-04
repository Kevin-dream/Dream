#include "Token.h"

Token::Token() {

}

Token::~Token() {

}

void Token::toString() const {
	std::cout<<TokenNames[static_cast<int>(type)]<<' '<<(lexeme?lexeme:"null")<<std::endl;
}
const char* TokenNames[] = {
	"END_OF_FILE",

	"IF",
	"ELSE",
	"FOR",
	"WHILE",
	"CLASS",

	"IDENT",

	"INT",
	"FLOAT",
	"STRING",
	"BOOL",

	"PLUS",
	"MINUS",
	"MULT",
	"DIVISION",
	"GREATER",
	"LESS",
	"EQUAL",
	"NOT_EQUAL",
	"GREATER_EQUAL",
	"LESS_EQUAL",
	"AND",
	"OR",
	"NOT",

	"COMMA",
	"SEMICOLON",
	"ASSIGNMENT",
	"LEFT_PARENTHESIS",
	"RIGHT_PARENTHESIS",
	"LEFT_BRACKET",
	"RIGHT_BRACKET",
	"LEFT_BRACE",
	"RIGHT_BRACE"
};
