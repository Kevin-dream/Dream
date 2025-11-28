#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

struct location{
	unsigned int line = 0;
	unsigned int column = 0;
};

enum class TokenType:int {
    END_OF_FILE = 0,

    IF,
    ELSE,
    FOR,
    WHILE,
    CLASS,

    IDENT,

    INT,
    FLOAT,
    STRING,
    BOOL,

    PLUS,
    MINUS,
    MULT,
    DIVISION,
    GREATER,
    LESS,
    EQUAL,
    NOT_EQUAL,
    GREATER_EQUAL,
    LESS_EQUAL,
    AND,
    OR,
    NOT,

    COMMA,
    SEMICOLON,
    ASSIGNMENT,
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_BRACE,
    RIGHT_BRACE
};

extern const char* TokenNames[];

class Token {
	public:
		Token();
		~Token();

		void toString() const;
	public:
		TokenType type;
		unsigned int length = 0;
		char* lexeme = nullptr;
		location loc;

		Token* next = nullptr;
};

#endif
