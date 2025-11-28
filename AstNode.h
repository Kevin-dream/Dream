#ifndef AST_NODE_H
#define AST_NODE_H

#include <vector>
#include "Token.h"

class newline_t{
	public:
		newline_t() = default;
		~newline_t() = default;
		
		void Indent();
		void unIndent();
		
		friend std::ostream& operator<<(std::ostream& os, const newline_t& obj);
	private:
		unsigned int indentLevel = 0;
}; 

extern newline_t newline;

class AST_Node {
	public:
		AST_Node() = default;
		~AST_Node() = default;

		void virtual toString();
};

class StatementsNode : public AST_Node {
	public:
		StatementsNode() = default;
		~StatementsNode() = default;

		StatementsNode(std::vector<AST_Node*>& clauses);
		void toString()override;
	private:
		unsigned int length;
		AST_Node** clauses;
};

class IfStatementNode : public AST_Node {
	public:
		IfStatementNode() = default;
		~IfStatementNode() = default;

		IfStatementNode(AST_Node* condition,AST_Node* if_clause,AST_Node* else_clause = nullptr);
		void toString()override;
	private:
		AST_Node* condition;
		AST_Node* if_clause;
		AST_Node* else_clause;
};

class ForStatementNode : public AST_Node {
	public:
		ForStatementNode() = default;
		~ForStatementNode() = default;

		ForStatementNode(AST_Node* condition,AST_Node* clause);
		void toString()override;
	private:
		AST_Node* condition;
		AST_Node* clause;
};

class WhileStatementNode : public AST_Node {
	public:
		WhileStatementNode() = default;
		~WhileStatementNode() = default;

		WhileStatementNode(AST_Node* condition,AST_Node* clause);
		void toString()override;
	private:
		AST_Node* condition;
		AST_Node* clause;
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

		    COMMA
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

class IntNode : public AST_Node {
	public:
		IntNode() = default;
		~IntNode() = default;

		IntNode(Token* tok);
		void toString()override;
	private:
		int value = 0;
};

class FloatNode : public AST_Node {
	public:
		FloatNode() = default;
		~FloatNode() = default;

		FloatNode(Token* tok);
		void toString()override;
	private:
		double value = 0;
};

class IdentNode : public AST_Node {
	public:
		IdentNode() = default;
		~IdentNode() = default;

		IdentNode(Token* tok);
		void toString()override;
	private:
		Token* value;
};

class StringNode : public AST_Node {
	public:
		StringNode();
		~StringNode();

		StringNode(Token* tok);
		void toString()override;
	private:
		unsigned int length;
		char* value;
};

#endif
