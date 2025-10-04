#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <cstring>
#include "Lexer.h"
#include "Token.h"

static Token* token;

Token peek();
Token next();

class AST_Node {
	public:
		AST_Node() = default;
		~AST_Node() = default;

		void virtual toString();
};

class FunctionCallNode : public AST_Node {
	public:
		FunctionCallNode() = default;
		~FunctionCallNode() = default;

		FunctionCallNode(AST_Node* left,AST_Node* args);
		void toString()override;
	private:
		AST_Node* left;
		AST_Node* args;
};

class UnaryNode : public AST_Node {
	public:
		enum class Operator {
		    PLUS,
		    MINUS
		};
	public:
		UnaryNode() = default;
		~UnaryNode() = default;

		UnaryNode(Operator op,AST_Node* right);
		void toString()override;
	private:
		Operator op;
		AST_Node* right;
};

class BinaryNode : public AST_Node {
	public:
		enum class Operator {
		    ADD,
		    SUBTRACT,
		    MULTIPLY,
		    DIVISION,

		    GREATER,
		    LESS,
		    EQUAL,
		    NOT_EQUAL,
		    GREATER_EQUAL,
		    LESS_EQUAL,

		    COMMA,
		    SEMICOLON
		};
	public:
		BinaryNode() = default;
		~BinaryNode() = default;

		BinaryNode(Operator op,AST_Node* left,AST_Node* right);
		void toString()override;
	private:
		AST_Node* left;
		Operator op;
		AST_Node* right;
};

class IntNode : public AST_Node {
	public:
		IntNode() = default;
		~IntNode() = default;

		IntNode(Token tok);
		void toString()override;
	private:
		int value = 0;
};

class FloatNode : public AST_Node {
	public:
		FloatNode() = default;
		~FloatNode() = default;

		FloatNode(Token tok);
		void toString()override;
	private:
		double value = 0;
};

class StringNode : public AST_Node {
	public:
		StringNode();
		~StringNode();

		StringNode(Token tok);
		void toString()override;
	private:
		unsigned int length;
		char* value;
};

AST_Node* parse(Token* list);
AST_Node* parseExpr();
AST_Node* parseCommaExpr();
AST_Node* parseCommaTail(AST_Node* left);
AST_Node* parseAddExpr();
AST_Node* parseAddTail(AST_Node* left);
AST_Node* parseMultExpr();
AST_Node* parseMultTail(AST_Node* left);
AST_Node* parseUnaryExpr();
AST_Node* parseUnaryTail();
AST_Node* parsePostfixExpr();
AST_Node* parsePostfixTail(AST_Node* left);
AST_Node* parseTermExpr();

#endif
