#include "MathExpression.h"
#include <iostream>

/******************************************************************************/
/**	Static Functions														  */
/******************************************************************************/

static bool contain(ParMap m, string s) { return (m.count(s) != 0); }
static bool contain(VarMap m, string s) { return (m.count(s) != 0); }
static bool contain(FctMap m, string s) { return (m.count(s) != 0); }

/******************************************************************************/
/**	Manager Constructors													  */
/******************************************************************************/

MathExpressionManager* MathExpressionManager::instance = new MathExpressionManager();

MathExpressionManager::MathExpressionManager() {
	this->functions 	= this->initFctMap();
	this->expressions 	= ExpMap();
	this->variables 	= VarMap();
	this->parameters 	= ParMap();
}

/******************************************************************************/
/**	Manager Getters															  */
/******************************************************************************/

MathExpressionManager* MathExpressionManager::getInstance() {
	return MathExpressionManager::instance;
}

ExpMap MathExpressionManager::getExpressions() const { return this->expressions; }
ParMap MathExpressionManager::getParameters() const { return this->parameters; }
VarMap MathExpressionManager::getVariables() const { return this->variables; }
FctMap MathExpressionManager::getFunctions() const { return this->functions; }

/******************************************************************************/
/**	Manager Setters															  */
/******************************************************************************/

void MathExpressionManager::addParameter(const string& label) {
	if(!contain(this->parameters,label))
		this->parameters[label] = interval(0);
}

void MathExpressionManager::addParameter(const string& label, const interval& i) {
	this->parameters[label] = i;
}

void MathExpressionManager::setParameter(const string& label, const interval& i) {
	if(contain(this->parameters,label))
		this->parameters[label] = i;
}

void MathExpressionManager::removeParameter(const string& label) {
	this->parameters.erase(label);
}

void MathExpressionManager::addVariable(const string& label) {
	if(!contain(this->parameters,label))
		this->variables[label] = 0;
}

void MathExpressionManager::setVariable(const string& label, const T<interval>* i) {
	if(contain(this->variables,label))
		this->variables[label] = i;
}

void MathExpressionManager::removeVariable(const string& label) {
	this->variables.erase(label);
}

void MathExpressionManager::addExpression(const string& label, ExpTree* t) {
	this->expressions[label] = t;
}

void MathExpressionManager::removeExpression(const string& label) {
	this->expressions.erase(label);
}

/******************************************************************************/
/**	Manager Evaluation Functions											  */
/******************************************************************************/

T<interval> MathExpressionManager::evaluate(const string& label) {
	ExpTree* t = this->expressions[label];
	return (t)?this->evaluate(t):false;
}

T<interval> MathExpressionManager::evaluate(ExpTree* tree) {
	switch(tree->getType()) {
	case CONSTANT  : return tree->node.getValue();
	case PARAMETER : return this->parameters[tree->node.getID()];
	case VARIABLE  : return *(this->variables[tree->node.getID()]);
	case FUNCTION  : return  (this->*functions[tree->node.getID()])(tree);
	default : return interval(0);
	}
}
/*
iVector MathExpressionManager::evaluate() {
	ExpMap::iterator it;
	int i;
	iVector iv(this->expressions.size());
	for ( it=this->expressions.begin(), i = 0 ;
				it != this->expressions.end(); it++, i++ )
		iv[i] = this->evaluate((*it).second);
	return iv;
}
*/
bool MathExpressionManager::check(const string& label) {
	ExpTree* t = this->expressions[label];
	return (t)?this->check(t):false;
}

bool MathExpressionManager::check(const ExpTree* tree) const {
	switch(tree->getType()) {
	case CONSTANT  : return true;
	case PARAMETER : return (contain(this->parameters,tree->node.getID()))?true:false;
	case VARIABLE  : return (contain(this->variables, tree->node.getID()))?true:false;
	case FUNCTION  : return (contain(this->functions, tree->node.getID()) && check(tree->left) && ((tree->right)?check(tree->right):true))?true:false;
	default : return false;
	}
}

bool MathExpressionManager::check() {
	ExpMap::iterator it;
	for ( it=this->expressions.begin() ; it != this->expressions.end(); it++ )
		if(!this->check((*it).second))
			return false;
	return true;
}

/******************************************************************************/
/**	Manager Protected Arithmetical Functions								  */
/******************************************************************************/

FctMap MathExpressionManager::initFctMap() {
	FctMap f = FctMap();
	f["Plus"] = &MathExpressionManager::Plus;
	f["Times"] = &MathExpressionManager::Times;
	return f;
}

T<interval> MathExpressionManager::Plus(ExpTree* t) {
	return this->evaluate(t->left) + this->evaluate(t->right);
}

T<interval> MathExpressionManager::Times(ExpTree* t) {
	return this->evaluate(t->left) * this->evaluate(t->right);
}
