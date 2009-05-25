/**
 * \file MathExpression.h
 * \brief Structure and functions for creating an mathematic expression
 * \author Lejeune Aurelien
 * \version 1.0
 * \date 06/10/2008
 */

#ifndef MATHEXPRESSION_H_INCLUDED
#define MATHEXPRESSION_H_INCLUDED

#include <map>
#include <string>
#include "ExpTree.h"

class MathExpressionManager;

typedef T<interval> TInterval;
typedef T<F<interval> > TFInterval;
typedef TInterval (MathExpressionManager::*FPTR)(ExpTree*);
typedef TFInterval (MathExpressionManager::*FPTR_)(ExpTree*);
typedef map<string, FPTR> FctMap;
typedef map<string, FPTR_> FctMap_;
typedef map<string, const TInterval* > VarMap;
typedef map<string, const TFInterval* > VarMap_;
typedef map<string, ExpTree*> ExpMap;


/**
 * \class MathExpressionManager
 * \brief Manage a set of Mathematic Expressions
 * 
 */
class MathExpressionManager {
	private :
		ExpMap expressions;
		/**< Map of expression, the key is the name*/
		VarMap variables; VarMap_ variables_;
		/**< Maps of variables, the key is the name of the variable*/
		FctMap functions; FctMap_ functions_;
		/**< Maps of function, the key is the name of the function*/
		static MathExpressionManager* instance;
		/**< Singleton pattern*/
		
		/**
		 * \fn MathExpressionManager()
		 * \brief the constructor
		 */
		MathExpressionManager();
		
	public :
		
		/**
		 * \fn static MathExpressionManager* getInstance()
		 * \brief Singleton pattern
		 */
		static MathExpressionManager* getInstance();
		
		ExpMap getExpressions() const;
		VarMap getVariables() const; VarMap_ getVariables_() const;
		FctMap getFunctions() const;
		
		void addVariable(const string&);
		void setVariable(const string&, const TInterval*);
		void setVariable_(const string&, const TFInterval*);
		void removeVariable(const string&);
		void addExpression(const string&, ExpTree*);
		void removeExpression(const string&);
		int getNbExpression();
		
		TInterval evaluate(const string&);
		TInterval evaluate(ExpTree*);
		
		TFInterval evaluate_(const string&);
		TFInterval evaluate_(ExpTree*);
		
		bool check(const string&);
		bool check(const ExpTree*) const;
		bool check();
		
	protected :
		FctMap initFctMap(); FctMap_ initFctMap_();
		TInterval Plus(ExpTree*); TFInterval Plus_(ExpTree*);
		TInterval Times(ExpTree*); TFInterval Times_(ExpTree*);
		TInterval Power(ExpTree*); TFInterval Power_(ExpTree*);
};

#endif