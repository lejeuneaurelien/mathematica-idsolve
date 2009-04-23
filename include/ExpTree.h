/**
 * \file ExpTree.h
 * \brief Structure and functions for creating an expression tree
 * \author Lejeune Aurelien
 * \version 1.0
 * \date 06/10/2008
 */

#ifndef EXPTREE_H_INCLUDED
#define EXPTREE_H_INCLUDED

#undef NULL
#define NULL 0

#include <iostream>
#include <vnode.h>
#include <string>
#include <stdexcept>

enum NodeType { FUNCTION, CONSTANT, VARIABLE, PARAMETER, EXPRESSION };

class ExpNode {
	private :
		NodeType type;
		string id;
		interval value;
	
	public :
		ExpNode();
		ExpNode(const NodeType&, const string&) throw(string&);
		ExpNode(const NodeType&, const interval&) throw(string&);
		NodeType getType() const;
		string getID() const;
		interval getValue() const;
		friend std::ostream &operator << (std::ostream&, const ExpNode&);
};

class ExpTree {
	friend class MathExpressionManager;
	private :
		ExpNode node;
		ExpTree* left;
		ExpTree* right;
		void print(int);
		
	public :
		ExpTree();
		ExpTree(const ExpNode&);
		NodeType getType() const;
		void addLeft(ExpTree*);
		void addRight(ExpTree*);
		void print();
};


#endif