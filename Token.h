#ifndef TOKEN_H
#define TOKEN_H

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
	public:
		TokenType type;
		char* lexeme;
};

#endif
