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

/**
 * \enum NodeType
 * \brief The different kind of Node used in the expression tree
 */
enum NodeType { FUNCTION, CONSTANT, VARIABLE, PARAMETER, EXPRESSION };

/**
 * \class ExpNode
 * \brief A node of the expression tree
 *
 * A node could be an interior node or a leaf. Up to now only the type FUNCTION,
 * VARIABLE or EXPRESSION are used. Leaf are FUNCTION (a label) or VARIABLE (a
 * value ie an interval). Interior nodes are typed by EXPRESSION and labeled by
 * a well defined operator.
 */
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


/**
 * \class ExpTree
 * \brief An expression tree is a binary parse tree for arithmetic expression.
 *
 * By convention for unary operator like Sin(x) the left part of the tree will
 * represent the operand (ad hoc x) and the right part is empty (null pointer).
 */
class ExpTree {
	friend class MathExpressionManager;
	private :
		ExpNode node;
		ExpTree* left;
		ExpTree* right;
		void print(int);
		std::string toString(int i);
		
	public :
		ExpTree(const ExpNode&);
		NodeType getType() const;
		
		/**
		 * \fn void addLeft(ExpTree*)
		 * \brief Link two expression tree
		 * \param[in] an expression tree representing the first operand
		 */
		void addLeft(ExpTree*);
		
		/**
		 * \fn void addRight(ExpTree*)
		 * \brief Link two expression tree
		 * \param[in] an expression tree representing the second operand
		 */
		void addRight(ExpTree*);
		
		/**
		 * \fn void print()
		 * \brief Display an expression tree on the standard output
		 */
		void print();
		
		/**
		 * \fn std::string toString()
		 * \brief return a string representation of an expression tree
		 * \return a multiline string
		 */
		std::string toString();
};


#endif