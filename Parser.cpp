#include "Parser.h"

Token* peek() {
	return token->next;
}

Token* next() {
	token = token->next;
	return token;
}

bool match(TokenType type) {
	if(token->next->type == type) {
		token = token->next;
		return true;
	}
	return false;
}

AST_Node* parse(Token* list) {
	token = list;
	std::vector<AST_Node*> statements;
	AST_Node* statement = nullptr;
	while(peek()->type!=TokenType::END_OF_FILE) {
		statement = parseStatement();
		if(!statement)break;
		statements.push_back(statement);
	}
	return new StatementsNode(statements);
}

AST_Node* parseStatement() {
	Token* tok = peek();
	switch(tok->type) {
		case TokenType::IF:
			next();
			return parseIfStatement();
		case TokenType::FOR:
			next();
			return parseForStatement();
		case TokenType::WHILE:
			next();
			return parseWhileStatement();
		case TokenType::LEFT_BRACE:
			do {
				next();
				AST_Node* expr = parseStatementsBlock();
				if(!match(TokenType::RIGHT_BRACE)) {
					parse_error(peek(),"Expected '}'");
					error_count++;
					return nullptr;
				}
				return expr;
			} while(0);
		default:
			return parseExprStatement();
	}
}

AST_Node* parseStatementsBlock() {
	std::vector<AST_Node*> statements;
	while(peek()->type!=TokenType::RIGHT_BRACE) {
		if(peek()->type==TokenType::END_OF_FILE) {
			parse_error(peek(),"Expected '}' at the end");
			error_count++;
			break;
		}
		statements.push_back(parseStatement());
	}
	AST_Node* statement = new StatementsNode(statements);
	return statement;
}

AST_Node* parseIfStatement() {
	AST_Node* condition = parseParenthesesPair();
	AST_Node* ifClause = parseStatement();
	if(peek()->type == TokenType::ELSE) {
		AST_Node* elseClause = parseStatement();
		AST_Node* statement = new IfStatementNode(condition,ifClause,elseClause);
		return statement;
	}
	AST_Node* statement = new IfStatementNode(condition,ifClause);
	return statement;
}

AST_Node* parseForStatement() {
	AST_Node* condition = parseParenthesesPair();
	AST_Node* clause = parseStatement();
	AST_Node* statement = new ForStatementNode(condition,clause);
	return statement;
}

AST_Node* parseWhileStatement() {
	AST_Node* condition = parseParenthesesPair();
	AST_Node* clause = parseStatement();
	AST_Node* statement = new WhileStatementNode(condition,clause);
	return statement;
}

AST_Node* parseExprStatement() {
	AST_Node* statement = parseExpr();
	if(!match(TokenType::SEMICOLON)) {
		parse_error(peek(),"Expected ';'");
		error_count++;
		return statement;
	}
	return statement;
}

AST_Node* parseExpr() {
	return parseCommaExpr();
}

AST_Node* parseCommaExpr() {
	AST_Node* expr = parseAddExpr();
	return parseCommaTail(expr);
}
AST_Node* parseCommaTail(AST_Node* left) {
	Token* tok = peek();
	if(tok->type == TokenType::COMMA) {
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
	Token* tok = peek();
	if(match(TokenType::PLUS)) {
		AST_Node* right = parseMultExpr();
		AST_Node* expr = new BinaryNode(BinaryNode::Operator::ADD,left,right);
		return parseAddTail(expr);
	} else if(match(TokenType::MINUS)) {
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
	if(match(TokenType::MULT)) {
		AST_Node* right = parseUnaryExpr();
		AST_Node* expr = new BinaryNode(BinaryNode::Operator::MULTIPLY,left,right);
		return parseMultTail(expr);
	} else if(match(TokenType::DIVISION)) {
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
	if(match(TokenType::PLUS)) {
		AST_Node* left = parseUnaryTail();
		AST_Node* expr = new UnaryNode(UnaryNode::Operator::PLUS,left);
		return expr;
	} else if(match(TokenType::MINUS)) {
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
	if(match(TokenType::LEFT_PARENTHESIS)) {
		AST_Node* args = parseExpr();
		if(!match(TokenType::RIGHT_PARENTHESIS)) {
			parse_error(peek(),"Expected ')'");
			error_count++;
		}
		AST_Node* expr = new FunctionCallNode(left,args);
		return parsePostfixTail(expr);
	} else {
		return left;
	}
}

AST_Node* parseParenthesesPair() {
	if(!match(TokenType::LEFT_PARENTHESIS)){
		parse_error(peek(),"Expected '('");
		error_count++;
	}
	AST_Node* expr = parseExpr();
	if(!match(TokenType::RIGHT_PARENTHESIS)){
		parse_error(peek(),"Expected ')'");
		error_count++;
	}
	return expr;
}

AST_Node* parseTermExpr() {
	Token* tok = peek();
	switch(tok->type) {
		case TokenType::INT:
			next();
			return new IntNode(tok);
		case TokenType::FLOAT:
			next();
			return new FloatNode(tok);
		case TokenType::IDENT:
			next();
			return new IdentNode(tok);
		case TokenType::STRING:
			next();
			return new StringNode(tok);
		case TokenType::LEFT_PARENTHESIS:
			return parseParenthesesPair();
		default:
			parse_error(peek(),"Wrong token");
			error_count++;
			return nullptr;
	}
}
