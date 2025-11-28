#include "Lexer.h"

void TokenPush(TokenType type,location loc) {
	list -> next = new Token;
	list -> next -> type = type;
	list -> next -> loc = loc;

	list = list -> next;
}

void TokenPush(TokenType type,unsigned int length,char* lexeme,location loc) {
	list -> next = new Token;
	list -> next -> type = type;
	list -> next -> length = length;
	list -> next -> loc = loc;
	list -> next -> lexeme = new char[length+1];
	strncpy(list -> next -> lexeme,lexeme,length);
	list -> next -> lexeme[length] = '\0';

	list = list -> next;
}

TokenType verify_keyword(char* buf,unsigned int length) {
	unsigned int index = 0;
	switch(buf[index++]) {
		case 'b':
			if(length!=4)return TokenType::IDENT;
			if(strcmp(buf,"bool"))return TokenType::IDENT;
			return TokenType::BOOL;
			break;
		case 'c':
			if(length!=5)return TokenType::IDENT;
			if(strcmp(buf,"class"))return TokenType::IDENT;
			return TokenType::CLASS;
		case 'e':
			if(length!=4)return TokenType::IDENT;
			if(strcmp(buf,"else"))return TokenType::IDENT;
			return TokenType::ELSE;
		case 'f':
			if(length>5)return TokenType::IDENT;
			if(buf[1]=='o') {
				if(!strcmp(buf,"for"))return TokenType::FOR;
			} else if(buf[1]=='l') {
				if(!strcmp(buf,"float"))return TokenType::FLOAT;
			}
			return TokenType::IDENT;
		case 'i':
			if(length>3)return TokenType::IDENT;
			if(buf[1]=='f') {
				if(!strcmp(buf,"if"))return TokenType::IF;
			} else if(buf[1]=='n') {
				if(!strcmp(buf,"int"))return TokenType::INT;
			}
			return TokenType::IDENT;
		case 's':
			if(length!=6)return TokenType::IDENT;
			if(strcmp(buf,"string"))return TokenType::IDENT;
			return TokenType::STRING;
		case 'w':
			if(length!=5)return TokenType::IDENT;
			if(strcmp(buf,"while"))return TokenType::IDENT;
			return TokenType::WHILE;
	}
	return TokenType::IDENT;
}

Token* Scanning(char* source) {
	list = new Token;
	Token* head = list;

	TokenType tokenType;
	char tokenBuffer[256] = {'\0'};
	unsigned int tokenLength = 0;
	location loc;
	loc.line = 1;
	loc.column = 0;

	char* start = &source[0];

	for(unsigned int i=0; source[i]!='\0'; i++,loc.column++) {
		bool isFloat = false;
		start = &source[i];
		switch(source[i]) {
			case 'a'...'z':
			case 'A'...'Z':
			case '_':
				while(source[i]>='a'&& source[i]<='z' || source[i]>='A'&& source[i]<='Z' || source[i]>='0'&& source[i]<='9' || source[i]=='_') {
					tokenBuffer[tokenLength] = source[i];
					tokenLength++;
					i++;
				}
				i--;
				tokenType = verify_keyword(tokenBuffer,tokenLength);
				TokenPush(tokenType,tokenLength,tokenBuffer,loc);
				break;
			case '0'...'9':
				while(source[i]>='0'&& source[i]<='9' || source[i]=='.') {
					if(source[i]=='.')isFloat = true;
					tokenBuffer[tokenLength] = source[i];
					tokenLength++;
					i++;
				}
				i--;
				if(isFloat) TokenPush(TokenType::FLOAT,tokenLength,tokenBuffer,loc);
				else TokenPush(TokenType::INT,tokenLength,tokenBuffer,loc);
				break;
			case '"':
				i++;
				while(source[i]!='"') {
					if(source[i]=='\n' || source[i]=='\0') {
						lex_error(loc,"Error: Missing terminating '\"' character");
						error_count++;
						goto error;
					}
					if(source[i] == '\\') {
						switch(source[++i]) {
							case 'a':
								tokenBuffer[tokenLength] = '\a';
								break;
							case 'b':
								tokenBuffer[tokenLength] = '\b';
								break;
							case 'n':
								tokenBuffer[tokenLength] = '\n';
								break;
							case 't':
								tokenBuffer[tokenLength] = '\t';
								break;
							case 'r':
								tokenBuffer[tokenLength] = '\r';
								break;
							case '\"':
								tokenBuffer[tokenLength] = '\"';
								break;
							case '\'':
								tokenBuffer[tokenLength] = '\'';
								break;
							case '\\':
								tokenBuffer[tokenLength] = '\\';
								break;
							default:
								tokenBuffer[tokenLength] = source[i];
						}
					} else {
						tokenBuffer[tokenLength] = source[i];
					}
					tokenLength++;
					i++;
				}
				TokenPush(TokenType::STRING,tokenLength,tokenBuffer,loc);
error:
				break;
			case '+':
				TokenPush(TokenType::PLUS,loc);
				break;
			case '-':
				TokenPush(TokenType::MINUS,loc);
				break;
			case '*':
				TokenPush(TokenType::MULT,loc);
				break;
			case '/':
				if(source[i+1] == '/') while(source[i]!='\n' || source[i]!='\0')i++;
				else TokenPush(TokenType::DIVISION,loc);
				break;
			case '>':
				if(source[i+1] == '=') {
					TokenPush(TokenType::GREATER_EQUAL,loc);
					i++;
				} else TokenPush(TokenType::GREATER,loc);
				break;
			case '<':
				if(source[i+1] == '=') {
					TokenPush(TokenType::LESS_EQUAL,loc);
					i++;
				} else TokenPush(TokenType::LESS,loc);
				break;
			case '=':
				if(source[i+1] == '=') {
					TokenPush(TokenType::EQUAL,loc);
					i++;
				} else TokenPush(TokenType::ASSIGNMENT,loc);
				break;
			case '!':
				if(source[i+1] == '=') {
					TokenPush(TokenType::NOT_EQUAL,loc);
					i++;
				} else TokenPush(TokenType::NOT,loc);
				break;
			case '&':
				TokenPush(TokenType::AND,loc);
				break;
			case '|':
				TokenPush(TokenType::OR,loc);
				break;
			case ',':
				TokenPush(TokenType::COMMA,loc);
				break;
			case ';':
				TokenPush(TokenType::SEMICOLON,loc);
				break;
			case '(':
				TokenPush(TokenType::LEFT_PARENTHESIS,loc);
				break;
			case ')':
				TokenPush(TokenType::RIGHT_PARENTHESIS,loc);
				break;
			case '[':
				TokenPush(TokenType::LEFT_BRACKET,loc);
				break;
			case ']':
				TokenPush(TokenType::RIGHT_BRACKET,loc);
				break;
			case '{':
				TokenPush(TokenType::LEFT_BRACE,loc);
				break;
			case '}':
				TokenPush(TokenType::RIGHT_BRACE,loc);
				break;
			case '\n':
				loc.line++;
				loc.column = 0;
				break;
			default:
				lex_error(loc,"Unknown characters");
				error_count++;
				break;
		}
		loc.column += &source[i]-start;
		memset(tokenBuffer,0,sizeof(tokenBuffer));
		tokenLength = 0;
	}
	TokenPush(TokenType::END_OF_FILE,loc);
	return head;
}
