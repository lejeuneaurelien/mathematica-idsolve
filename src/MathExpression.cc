#include "MathExpression.h"
#include <iostream>

/******************************************************************************/
/**	Static Functions														  */
/******************************************************************************/

static bool contain(VarMap m, string s) { return (m.count(s) != 0); }
static bool contain(VarMap_ m, string s) { return (m.count(s) != 0); }
static bool contain(FctMap m, string s) { return (m.count(s) != 0); }

/******************************************************************************/
/**	Manager Constructors													  */
/******************************************************************************/

MathExpressionManager* MathExpressionManager::instance = new MathExpressionManager();

MathExpressionManager::MathExpressionManager() {
	this->functions 	= this->initFctMap();
	this->functions_ 	= this->initFctMap_();
	this->expressions 	= ExpMap();
	this->variables 	= VarMap();
}

/******************************************************************************/
/**	Manager Getters															  */
/******************************************************************************/

MathExpressionManager* MathExpressionManager::getInstance() {
	return MathExpressionManager::instance;
}

ExpMap MathExpressionManager::getExpressions() const { return this->expressions; }
VarMap MathExpressionManager::getVariables() const { return this->variables; }
VarMap_ MathExpressionManager::getVariables_() const { return this->variables_; }
FctMap MathExpressionManager::getFunctions() const { return this->functions; }

/******************************************************************************/
/**	Manager Setters															  */
/******************************************************************************/

void MathExpressionManager::addVariable(const string& label) {
	if(!contain(this->variables,label)) {
		this->variables[label] = NULL;
		this->variables_[label] = NULL;
	}
}

void MathExpressionManager::setVariable(const string& label, const TInterval* i) {
	if(contain(this->variables,label))
		this->variables[label] = i;
}

void MathExpressionManager::setVariable_(const string& label, const TFInterval* i) {
	if(contain(this->variables_,label))
		this->variables_[label] = i;
}

void MathExpressionManager::removeVariable(const string& label) {
	this->variables.erase(label);
	this->variables_.erase(label);
}

void MathExpressionManager::addExpression(const string& label, ExpTree* t) {
	this->expressions[label] = t;
}

void MathExpressionManager::removeExpression(const string& label) {
	this->expressions.erase(label);
}

int MathExpressionManager::getNbExpression() {
	return this->expressions.size();
}

/******************************************************************************/
/**	Manager Evaluation Functions											  */
/******************************************************************************/

TInterval MathExpressionManager::evaluate(const string& label) {
	ExpTree* t = this->expressions[label];
	return (t)?this->evaluate(t):false;
}

TInterval MathExpressionManager::evaluate(ExpTree* tree) {
	switch(tree->getType()) {
	case CONSTANT  : return tree->node.getValue();
	case VARIABLE  : return *(this->variables[tree->node.getID()]);
	case FUNCTION  : return  (this->*functions[tree->node.getID()])(tree);
	default : return interval(0);
	}
}

TFInterval MathExpressionManager::evaluate_(const string& label) {
	ExpTree* t = this->expressions[label];
	return (t)?this->evaluate_(t):false;
}

TFInterval MathExpressionManager::evaluate_(ExpTree* tree) {
	switch(tree->getType()) {
	case CONSTANT  : return tree->node.getValue();
	case VARIABLE  : return *(this->variables_[tree->node.getID()]);
	case FUNCTION  : return  (this->*functions_[tree->node.getID()])(tree);
	default : return interval(0);
	}
}

bool MathExpressionManager::check(const string& label) {
	ExpTree* t = this->expressions[label];
	return (t)?this->check(t):false;
}

bool MathExpressionManager::check(const ExpTree* tree) const {	
	switch(tree->getType()) {
	case CONSTANT  : return true;
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
	f["Power"] = &MathExpressionManager::Power;
	return f;
}

FctMap_ MathExpressionManager::initFctMap_() {
	FctMap_ f = FctMap_();
	f["Plus"] = &MathExpressionManager::Plus_;
	f["Times"] = &MathExpressionManager::Times_;
	f["Power"] = &MathExpressionManager::Power_;
	return f;
}

TInterval MathExpressionManager::Plus(ExpTree* t) {
	return this->evaluate(t->left) + this->evaluate(t->right);
}

TFInterval MathExpressionManager::Plus_(ExpTree* t) {
	return this->evaluate_(t->left) + this->evaluate_(t->right);
}

TInterval MathExpressionManager::Times(ExpTree* t) {
	return this->evaluate(t->left) * this->evaluate(t->right);
}

TFInterval MathExpressionManager::Times_(ExpTree* t) {
	return this->evaluate_(t->left) * this->evaluate_(t->right);
}

TInterval MathExpressionManager::Power(ExpTree* t) {
	return pow(this->evaluate(t->left), this->evaluate(t->right));
}

TFInterval MathExpressionManager::Power_(ExpTree* t) {
	return pow(this->evaluate_(t->left), this->evaluate_(t->right));
}

