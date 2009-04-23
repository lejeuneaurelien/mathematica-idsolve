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

class MathExpression;
class MathExpressionManager;

typedef T<interval> (MathExpressionManager::*FPTR)(ExpTree*);
typedef map<string, FPTR> FctMap;
typedef map<string, T<interval> > ParMap;
typedef map<string, const T<interval>* > VarMap;
typedef map<string, ExpTree*> ExpMap;

class MathExpressionManager {
	private :
		ExpMap expressions;
		ParMap parameters;
		VarMap variables;
		FctMap functions;
		static MathExpressionManager* instance;
		MathExpressionManager();
		
	public :
		static MathExpressionManager* getInstance();
		ExpMap getExpressions() const;
		ParMap getParameters() const;
		VarMap getVariables() const;
		FctMap getFunctions() const;
		
		void addParameter(const string&);
		void addParameter(const string&, const interval&);
		void setParameter(const string&, const interval&);
		void removeParameter(const string&);
		void addVariable(const string&);
		void setVariable(const string&, const T<interval>*);
		void removeVariable(const string&);
		void addExpression(const string&, ExpTree*);
		void removeExpression(const string&);
		
		T<interval> evaluate(const string&);
		T<interval> evaluate(ExpTree*);
		bool check(const string&);
		bool check(const ExpTree*) const;
		bool check();
		
	protected :
		FctMap initFctMap();
		T<interval> Plus(ExpTree*);
		T<interval> Times(ExpTree*);
};

#endif