#include "Parser.h"

Token peek() {
	return *(token->next);
}

Token next() {
	token = token->next;
	return *(token);
}

void AST_Node::toString() {

}

FunctionCallNode::FunctionCallNode(AST_Node* left,AST_Node* args) {
	this->left = left;
	this->args = args;
}

void FunctionCallNode::toString() {
	std::cout<<"{"<<std::endl;
	std::cout<<"\"type\":\"FunctionCallNode\","<<std::endl;
	std::cout<<"\"left\":";
	left->toString();
	std::cout<<","<<std::endl;
	std::cout<<"\"args\":";
	args->toString();
	std::cout<<std::endl;
	std::cout<<"}";
}

UnaryNode::UnaryNode(Operator op,AST_Node* right) {
	this->op = op;
	this->right=right;
}

void UnaryNode::toString() {
	std::cout<<"{"<<std::endl;
	std::cout<<"\"type\":\"UnaryNode\","<<std::endl;
	std::cout<<"\"operatorID\":";
	static_cast<int>(op);
	std::cout<<","<<std::endl;
	std::cout<<"\"right\":";
	right->toString();
	std::cout<<std::endl;
	std::cout<<"}";
}

BinaryNode::BinaryNode(Operator op,AST_Node* left,AST_Node* right) {
	this->op = op;
	this->left = left;
	this->right = right;
}

void BinaryNode::toString() {
	std::cout<<"{"<<std::endl;
	std::cout<<"\"type\":\"BinaryNode\","<<std::endl;
	std::cout<<"\"operatorID\":"<<static_cast<int>(op)<<","<<std::endl;
	std::cout<<"\"left\":";
	left->toString();
	std::cout<<","<<std::endl;
	std::cout<<"\"right\":";
	right->toString();
	std::cout<<std::endl;
	std::cout<<"}";
}

IntNode::IntNode(Token tok) {
	char* cur = tok.lexeme;
	while(*cur != '\0') {
		value = value * 10 + *cur - '0';
		cur++;
	}
}

void IntNode::toString() {
	std::cout<<"{"<<std::endl;
	std::cout<<"\"type\":\"IntNode\","<<std::endl;
	std::cout<<"\"value\":"<<value<<std::endl;
	std::cout<<"}";
}

FloatNode::FloatNode(Token tok) {
	char* cur = tok.lexeme;
	bool hasDecimal = false;
	double fraction = 1;
	while(*cur != '\0') {
		if(*cur == '.') {
			hasDecimal = true;
		} else {
			value = value * 10 + (*cur - '0');
			if(hasDecimal) {
				fraction *= 10;
			}
		}
		cur++;
	}
	value /= fraction;
}

void FloatNode::toString() {
	std::cout<<"{"<<std::endl;
	std::cout<<"\"type\":\"FloatNode\","<<std::endl;
	std::cout<<"\"value\":"<<value<<std::endl;
	std::cout<<"}";
}

StringNode::StringNode() {
	length = 0;
	value = nullptr;
}

void StringNode::toString() {
	std::cout<<"{"<<std::endl;
	std::cout<<"\"type\":\"StringNode\","<<std::endl;
	std::cout<<"\"value\":"<<value<<std::endl;
	std::cout<<"}"<<std::endl;
}

StringNode::~StringNode() {
	if(value != nullptr)delete value;
}

StringNode::StringNode(Token tok) {
	this->length = tok.length;
	this->value = tok.lexeme;
}

AST_Node* parse(Token* list) {
	token = list;
	return parseExpr();
}

AST_Node* parseExpr() {
	return parseCommaExpr();
}

AST_Node* parseCommaExpr() {
	AST_Node* expr = parseAddExpr();
	return parseCommaTail(expr);
}
AST_Node* parseCommaTail(AST_Node* left) {
	Token tok = peek();
	if(tok.type == TokenType::COMMA) {
		next();
		AST_Node* right = parseAddExpr();
		AST_Node* expr = new BinaryNode(BinaryNode::Operator::COMMA,left,right);
		return parseCommaTail(expr);
	} else {
		return left;
	}
}

AST_Node* parseAddExpr() {
	AST_Node* expr = parseMultExpr();
	return parseAddTail(expr);
}
AST_Node* parseAddTail(AST_Node* left) {
	Token tok = peek();
	if(tok.type == TokenType::PLUS) {
		next();
		AST_Node* right = parseMultExpr();
		AST_Node* expr = new BinaryNode(BinaryNode::Operator::ADD,left,right);
		return parseAddTail(expr);
	} else if(tok.type == TokenType::MINUS) {
		next();
		AST_Node* right = parseMultExpr();
		AST_Node* expr = new BinaryNode(BinaryNode::Operator::SUBTRACT,left,right);
		return parseAddTail(expr);
	} else {
		return left;
	}
}

AST_Node* parseMultExpr() {
	AST_Node* expr = parseUnaryExpr();
	return parseMultTail(expr);
}
AST_Node* parseMultTail(AST_Node* left) {
	Token tok = peek();
	if(tok.type == TokenType::MULT) {
		next();
		AST_Node* right = parseUnaryExpr();
		AST_Node* expr = new BinaryNode(BinaryNode::Operator::MULTIPLY,left,right);
		return parseMultTail(expr);
	} else if(tok.type == TokenType::DIVISION) {
		next();
		AST_Node* right = parseUnaryExpr();
		AST_Node* expr = new BinaryNode(BinaryNode::Operator::DIVISION,left,right);
		return parseMultTail(expr);
	} else {
		return left;
	}
}

AST_Node* parseUnaryExpr() {
	return parseUnaryTail();
}
AST_Node* parseUnaryTail() {
	Token tok = peek();
	if(tok.type == TokenType::PLUS) {
		next();
		AST_Node* left = parseUnaryTail();
		AST_Node* expr = new UnaryNode(UnaryNode::Operator::PLUS,left);
		return expr;
	} else if(tok.type == TokenType::MINUS) {
		next();
		AST_Node* left = parseUnaryTail();
		AST_Node* expr = new UnaryNode(UnaryNode::Operator::MINUS,left);
		return expr;
	} else {
		AST_Node* left = parsePostfixExpr();
		return left;
	}
}

AST_Node* parsePostfixExpr() {
	AST_Node* expr = parseTermExpr();
	return parsePostfixTail(expr);
}
AST_Node* parsePostfixTail(AST_Node* left) {
	Token tok = peek();
	if(tok.type == TokenType::LEFT_PARENTHESIS) {
		next();
		AST_Node* args = parseExpr();
		if(peek().type == TokenType::RIGHT_PARENTHESIS)next();
		else std::cerr<<"[line"<<peek().line<<"] Error: Expected ')'"<<std::endl;
		AST_Node* expr = new FunctionCallNode(left,args);
		return parsePostfixTail(expr);
	} else {
		return left;
	}
}

AST_Node* parseTermExpr() {
	Token tok = peek();
	switch(tok.type) {
		case TokenType::INT:
			next();
			return new IntNode(tok);
		case TokenType::FLOAT:
			next();
			return new FloatNode(tok);
		case TokenType::STRING:
			next();
			return new StringNode(tok);
		case TokenType::LEFT_PARENTHESIS: {
			next();
			AST_Node* expr = parseExpr();
			if(peek().type == TokenType::RIGHT_PARENTHESIS)next();
			else std::cerr<<"[line"<<peek().line<<"] Error: Expected ')'"<<std::endl;

			return expr;
		}
		default:
			std::cerr<<"[line"<<peek().line<<"] Error: Wrong token"<<std::endl;
			return nullptr;
	}
}
