#include "Token.h"

Token::Token() {
	lexeme = nullptr;
}

Token::~Token() {
	if(lexeme!=nullptr)delete lexeme;
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
	"LEFT_PARENTHESIS",
	"RIGHT_PARENTHESIS",
	"LEFT_BRACKET",
	"RIGHT_BRACKET",
	"LEFT_BRACE",
	"RIGHT_BRACE"
};
