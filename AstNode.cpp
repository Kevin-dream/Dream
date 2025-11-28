#include "AstNode.h"

void newline_t::Indent(){
	indentLevel++;
}

void newline_t::unIndent(){
	indentLevel--;
}

std::ostream& operator<<(std::ostream& os, const newline_t& obj) {
    os<<'\n';
    for(int i=0;i<obj.indentLevel;i++){
    	os<<"  ";
	}
    return os;
}

newline_t newline;

void AST_Node::toString() {

}

StatementsNode::StatementsNode(std::vector<AST_Node*>& clauses){
	this->length = clauses.size();
	this->clauses = new AST_Node*[clauses.size()];
	for(int i=0;i<clauses.size();i++)this->clauses[i] = clauses[i];
}

void StatementsNode::toString(){
	newline.Indent();
	std::cout<<"{";
	std::cout<<newline<<"\"type\":\"StatementsNode\",";
	std::cout<<newline<<"\"condition\":[";
	newline.Indent();
	for(int i=0;i<length-1;i++){
		std::cout<<newline;
		clauses[i]->toString();
		std::cout<<",";
	}
	std::cout<<newline;
	clauses[length-1]->toString();
	newline.unIndent();
	std::cout<<newline<<"]";
	newline.unIndent();
	std::cout<<newline;
	std::cout<<"}";
	
}

IfStatementNode::IfStatementNode(AST_Node* condition,AST_Node* if_clause,AST_Node* else_clause) {
	this->condition = condition;
	this->if_clause = if_clause;
	this->else_clause = else_clause;
}

void IfStatementNode::toString() {
	newline.Indent();
	std::cout<<"{";
	std::cout<<newline<<"\"type\":\"IfStatementNode\",";
	std::cout<<newline<<"\"condition\":";
	condition->toString();
	std::cout<<",";
	std::cout<<newline<<"\"if_clause\":";
	if_clause->toString();
	if(else_clause != nullptr) {
		std::cout<<",";
		std::cout<<newline<<"\"else_clause\":";
		else_clause->toString();
	}
	newline.unIndent();
	std::cout<<newline<<"}";
}

ForStatementNode::ForStatementNode(AST_Node* condition,AST_Node* clause) {
	this->condition = condition;
	this->clause = clause;
}

void ForStatementNode::toString() {
	newline.Indent();
	std::cout<<"{";
	std::cout<<newline<<"\"type\":\"ForStatementNode\",";
	std::cout<<newline<<"\"condition\":";
	condition->toString();
	std::cout<<",";
	std::cout<<newline<<"\"clause\":";
	clause->toString();
	newline.unIndent();
	std::cout<<newline<<"}";
}

WhileStatementNode::WhileStatementNode(AST_Node* condition,AST_Node* clause) {
	this->condition = condition;
	this->clause = clause;
}

void WhileStatementNode::toString() {
	newline.Indent();
	std::cout<<"{";
	std::cout<<newline<<"\"type\":\"WhileStatementNode\",";
	std::cout<<newline<<"\"condition\":";
	condition->toString();
	std::cout<<",";
	std::cout<<newline<<"\"clause\":";
	clause->toString();
	newline.unIndent();
	std::cout<<newline<<"}";
}

BinaryNode::BinaryNode(Operator op,AST_Node* left,AST_Node* right) {
	this->op = op;
	this->left = left;
	this->right = right;
}

void BinaryNode::toString() {
	newline.Indent();
	std::cout<<"{";
	std::cout<<newline<<"\"type\":\"BinaryNode\",";
	std::cout<<newline<<"\"operatorID\":"<<static_cast<int>(op)<<",";
	std::cout<<newline<<"\"left\":";
	left->toString();
	std::cout<<",";
	std::cout<<newline<<"\"right\":";
	right->toString();
	newline.unIndent();
	std::cout<<newline<<"}";
}

FunctionCallNode::FunctionCallNode(AST_Node* left,AST_Node* args) {
	this->left = left;
	this->args = args;
}

void FunctionCallNode::toString() {
	newline.Indent();
	std::cout<<"{";
	std::cout<<newline<<"\"type\":\"FunctionCallNode\",";
	std::cout<<newline<<"\"left\":";
	left->toString();
	std::cout<<",";
	std::cout<<newline<<"\"args\":";
	args->toString();
	newline.unIndent();
	std::cout<<newline<<"}";
}

UnaryNode::UnaryNode(Operator op,AST_Node* right) {
	this->op = op;
	this->right=right;
}

void UnaryNode::toString() {
	newline.Indent();
	std::cout<<"{";
	std::cout<<newline<<"\"type\":\"UnaryNode\",";
	std::cout<<newline<<"\"operatorID\":";
	static_cast<int>(op);
	std::cout<<",";
	std::cout<<newline<<"\"right\":";
	right->toString();
	newline.unIndent();
	std::cout<<newline<<"}";
}

IntNode::IntNode(Token* tok) {
	char* cur = tok->lexeme;
	while(*cur != '\0') {
		value = value * 10 + *cur - '0';
		cur++;
	}
}

void IntNode::toString() {
	newline.Indent();
	std::cout<<"{";
	std::cout<<newline<<"\"type\":\"IntNode\",";
	std::cout<<newline<<"\"value\":"<<value;
	newline.unIndent();
	std::cout<<newline<<"}";
}

FloatNode::FloatNode(Token* tok) {
	char* cur = tok->lexeme;
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
	newline.Indent();
	std::cout<<"{";
	std::cout<<newline<<"\"type\":\"FloatNode\",";
	std::cout<<newline<<"\"value\":"<<value;
	newline.unIndent();
	std::cout<<newline<<"}";
}

IdentNode::IdentNode(Token* tok) {
	value = tok;
}

void IdentNode::toString() {
	newline.Indent();
	std::cout<<"{";
	std::cout<<newline<<"\"type\":\"IdentNode\",";
	std::cout<<newline<<"\"value\":\""<<value->lexeme<<"\"";
	newline.unIndent();
	std::cout<<newline<<"}";
}

StringNode::StringNode(Token* tok) {
	this->length = tok->length;
	this->value = tok->lexeme;
}

StringNode::~StringNode() {
	if(value != nullptr)delete value;
}

void StringNode::toString() {
	newline.Indent();
	std::cout<<"{";
	std::cout<<newline<<"\"type\":\"StringNode\",";
	std::cout<<newline<<"\"value\":"<<value;
	newline.unIndent();
	std::cout<<newline<<"}";
}
