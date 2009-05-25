#include "ExpTree.h"

#include <iostream>
#include <sstream>

/******************************************************************************/
/**	Expression Node Functions												  */
/******************************************************************************/

ExpNode::ExpNode() {}

ExpNode::ExpNode(const NodeType& type, const string& id) throw(string&) {
	if(type == CONSTANT)
		throw (std::logic_error("This Node Type need a value : \"" +id+ "\""));
	this->id = id;
	this->type = type;
}

ExpNode::ExpNode(const NodeType& type, const interval& value) throw(string&) {
	if(type != CONSTANT)
		throw (std::logic_error(
			"This Node Type is not compatible with a string value"));
	this->value = value;
	this->type = type;
}

NodeType ExpNode::getType() const { return this->type; }
string ExpNode::getID() const { return this->id; }
interval ExpNode::getValue() const { return this->value; }

std::ostream & operator << (std::ostream& str, const ExpNode& en){
	if(en.type == CONSTANT) str << en.value;
	else str << en.id;
	return str;
}

/******************************************************************************/
/**	Expression Tree Functions												  */
/******************************************************************************/

ExpTree::ExpTree() {
	this->left  = NULL;
	this->right = NULL;
}

ExpTree::ExpTree(const ExpNode& node) {
	this->node  = node;
	this->left  = NULL;
	this->right = NULL;
}

NodeType ExpTree::getType() const { return this->node.getType(); }

void ExpTree::addLeft(ExpTree* e) { this->left  = e; }
void ExpTree::addRight(ExpTree* e) { this->right  = e; }

void ExpTree::print(int i) {
	string s = "";
	for(int j = 0; j < i; j++) s += " ";
	std::cout << s << "-- " << this->node << std::endl;
	if(this->left) this->left->print(i+1);
	if(this->right) this->right->print(i+1);
}

void ExpTree::print() {
	this->print(0);
}

string ExpTree::toString(int i) {
	string s = "";
	std::ostringstream str(std::ostringstream::out);
    str << this->node << endl;
	for(int j = 0; j < i; j++) s.append(" ");
	s.append("-- ");
	s.append(str.str());
	if(this->left) s.append(this->left->toString(i+1));
	if(this->right) s.append(this->right->toString(i+1));
	return s;
}

string ExpTree::toString() {
	return this->toString(0);
}