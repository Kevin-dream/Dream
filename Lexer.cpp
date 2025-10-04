#include "Lexer.h"

void TokenPush(TokenType type,unsigned int line) {
	list -> next = new Token;
	list -> next -> type = type;
	list -> next -> line = line;

	list = list -> next;
}

void TokenPush(TokenType type,unsigned int length,char* lexeme,unsigned int line) {
	list -> next = new Token;
	list -> next -> type = type;
	list -> next -> length = length;
	list -> next -> line = line;
	list -> next -> lexeme = new char[length+1];
	strncpy(list -> next -> lexeme,lexeme,length);
	list -> next -> lexeme[length] = '\0';

	list = list -> next;
}

Token* Scanning(char* source) {
	list = new Token;
	Token* head = list;

	TokenType tokenType;
	char tokenBuffer[256] = {'\0'};
	unsigned int tokenLength = 0;
	unsigned int line = 1;

	for(unsigned int i=0; source[i]!='\0'; i++) {
		if(source[i]>='a'&& source[i]<='z' || source[i]>='A'&& source[i]<='Z' || source[i]=='_') {
			while(source[i]>='a'&& source[i]<='z' || source[i]>='A'&& source[i]<='Z' || source[i]>='0'&& source[i]<='9' || source[i]=='_') {
				tokenBuffer[tokenLength] = source[i];
				tokenLength++;
				i++;
			}
			i--;
			TokenPush(TokenType::IDENT,tokenLength,tokenBuffer,line);
		} else if(source[i]>='0'&& source[i]<='9') {
			bool isFloat = false;
			while(source[i]>='0'&& source[i]<='9' || source[i]=='.') {
				if(source[i]=='.')isFloat = true;
				tokenBuffer[tokenLength] = source[i];
				tokenLength++;
				i++;
			}
			i--;
			if(isFloat) TokenPush(TokenType::FLOAT,tokenLength,tokenBuffer,line);
			else TokenPush(TokenType::INT,tokenLength,tokenBuffer,line);
		} else if(source[i]=='"') {
			i++;
			while(source[i]!='"') {
				if(source[i]=='\n' || source[i]=='\0')std::cerr<<"[line"<<line<<"] Error: Missing terminating '\"' character"<<std::endl;
				if(source[i] == '\\') {
					switch(source[i++]) {
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
			TokenPush(TokenType::STRING,tokenLength,tokenBuffer,line);
		} else {
			switch(source[i]) {
				case '+':
					TokenPush(TokenType::PLUS,line);
					break;
				case '-':
					TokenPush(TokenType::MINUS,line);
					break;
				case '*':
					TokenPush(TokenType::MULT,line);
					break;
				case '/':
					if(source[i+1] == '/') while(source[i++]!='\n' || source[i++]!='\0');
					else TokenPush(TokenType::DIVISION,line);
					break;
				case '>':
					if(source[i+1] == '=') {
						TokenPush(TokenType::GREATER_EQUAL,line);
						i++;
					} else TokenPush(TokenType::GREATER,line);
					break;
				case '<':
					if(source[i+1] == '=') {
						TokenPush(TokenType::LESS_EQUAL,line);
						i++;
					} else TokenPush(TokenType::LESS,line);
					break;
				case '=':
					if(source[i+1] == '=') {
						TokenPush(TokenType::EQUAL,line);
						i++;
					} else TokenPush(TokenType::ASSIGNMENT,line);
					break;
				case '!':
					if(source[i+1] == '=') {
						TokenPush(TokenType::NOT_EQUAL,line);
						i++;
					}
					TokenPush(TokenType::NOT,line);
					break;
				case '&':
					TokenPush(TokenType::AND,line);
					break;
				case '|':
					TokenPush(TokenType::OR,line);
					break;
				case ',':
					TokenPush(TokenType::COMMA,line);
					break;
				case ';':
					TokenPush(TokenType::SEMICOLON,line);
					break;
				case '(':
					TokenPush(TokenType::LEFT_PARENTHESIS,line);
					break;
				case ')':
					TokenPush(TokenType::RIGHT_PARENTHESIS,line);
					break;
				case '[':
					TokenPush(TokenType::LEFT_BRACKET,line);
					break;
				case ']':
					TokenPush(TokenType::RIGHT_BRACKET,line);
					break;
				case '{':
					TokenPush(TokenType::LEFT_BRACE,line);
					break;
				case '}':
					TokenPush(TokenType::RIGHT_BRACE,line);
					break;
				case '\n':
					line++;
					break;
				default:
					std::cerr<<"[line "<<line<<"] Error: Unknown characters '"<<source[i]<<"'"<<std::endl;
					break;
			}
		}
		memset(tokenBuffer,0,sizeof(tokenBuffer));
		tokenLength = 0;
	}
	TokenPush(TokenType::END_OF_FILE,line);
	return head;
}
