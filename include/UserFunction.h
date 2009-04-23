/**
 * \file UserFunction.h
 * \brief Structure and functions for creating a VNODE-LP function
 * \author Lejeune Aurelien
 * \version 1.0
 * \date 06/10/2008
 */



#ifndef USERFUNCTION_H_INCLUDED
#define USERFUNCTION_H_INCLUDED

#include <iostream>
#include <sstream>
#include <vnode.h>
#include <string>
#include <set>
#include <map>
#include "MathExpression.h"

class UserFunction {
  private:
	  unsigned int n;
	  map<string, string> derivatives;
	  MathExpressionManager* expressions;
  public:
	  UserFunction(int, MathExpressionManager*);
	  set<string> getVariables() const;
	  map<string, string> getDerivatives() const;
	  void addVariable(const string);
	  void addDerivative(const string, const string);
	  
	  template <typename var_type> void f(int, var_type*, const var_type*, var_type);
	 
	  friend std::ostream &operator << (std::ostream&, const UserFunction&);
};

#endif
