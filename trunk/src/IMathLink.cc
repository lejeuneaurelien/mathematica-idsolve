#include "IMathLink.h"

#include <iostream>
#include <sstream>
#include <typeinfo>
#include "vnode.h"
#include "UserFunction.h"
#include "MathExpression.h"

using namespace std;
using namespace vnodelp;

template <typename var_type> static void f(int n, var_type *yp, const var_type *y, var_type t, void *param) {
	UserFunction* F = (UserFunction*)param;
	F->f<var_type>(n, yp, y, t);
}

std::string intToString(const int i) {
    std::ostringstream oss;
    oss << i;
    return oss.str();
}

std::string doubleToString(const double d) {
    std::ostringstream oss;
    oss << d;
    return oss.str();
}

static IDSolveInterval intervalToIDSolve(const interval& i) {
	IDSolveInterval I;
	I.setBounds(inf(i), sup(i));
	return I;
}

static interval IDSolveToInterval(const IDSolveInterval& i) {
	return interval(i.min(), i.max());
}

/******************************************************************************/

IDExprTree createExprTreeFunc(string s) {
	return new ExpTree(ExpNode(FUNCTION, s));
}

IDExprTree createExprTreeVar(string s) {
	return new ExpTree(ExpNode(VARIABLE, s));
}

IDExprTree createExprTreeConst(int i) {
	return new ExpTree(ExpNode(CONSTANT, interval(i)));
}

IDExprTree createExprTreeConst(double d) {
	return new ExpTree(ExpNode(CONSTANT, interval(d)));
}

IDExprTree createExprTreeConst(double inf, double sup) {
	return new ExpTree(ExpNode(CONSTANT, interval(inf, sup)));
}

void exprTreeAddLeft(IDExprTree A, IDExprTree B) {
	(reinterpret_cast<ExpTree*>(A))->addLeft(reinterpret_cast<ExpTree*>(B));
}

void exprTreeAddRight(IDExprTree A, IDExprTree B) {
	(reinterpret_cast<ExpTree*>(A))->addRight(reinterpret_cast<ExpTree*>(B));
}

std::string exprTreeToString(IDExprTree expr) {
	return (reinterpret_cast<ExpTree*>(expr))->toString();
}

/******************************************************************************/

void IDSolveInterval::setBounds(const double min, const double max) {
	this->_min = min;
	this->_max = max;
}

double IDSolveInterval::min() const {
	return this->_min;
}

double IDSolveInterval::max() const {
	return this->_max;
}

std::string IDSolveInterval::toString() const {
	string s("[");
	s.append(doubleToString(this->_min));
	s.append(", ");
	s.append(doubleToString(this->_max));
	s.append("]");
	return s;
}

/******************************************************************************/

IDSolver::IDSolver() {
	this->mathExprMng = MathExpressionManager::getInstance();
}

void IDSolver::setN(const int n) {
	this->n = n;
}

int  IDSolver::getN() const { return this->n; }

void IDSolver::addFunction(const std::string& f) {
	this->functionName[f] = this->functionName.size();
	this->functionNameUnMap.push_back(f);
}

std::string IDSolver::functionToString() {
	vector<std::string>::iterator it;
	string s("{");
	for(it=this->functionNameUnMap.begin() ; it < this->functionNameUnMap.end(); it++) {
		if(it != this->functionNameUnMap.begin()) s.append(", ");
		s.append(*it);
	}
	s.append("}");
	return s;
}

void IDSolver::setVariableName(const std::string& s) {this->variableName = s; }
std::string IDSolver::getVariableName() const { return this->variableName; }

void IDSolver::setVarMin(const IDSolveInterval& i) { this->varMin = i; }
void IDSolver::setVarMax(const IDSolveInterval& i) { this->varMax = i; }
IDSolveInterval IDSolver::getVarMin() const { return this->varMin; }
IDSolveInterval IDSolver::getVarMax() const { return this->varMax; }

void IDSolver::addInitialValue(const IDSolveInterval& i) {
	this->initialValues.push_back(i);
}

std::string IDSolver::initialValuesToString() {
	vector<IDSolveInterval>::iterator it;
	string s("{");
	for(it=this->initialValues.begin() ; it < this->initialValues.end(); it++) {
		if(it != this->initialValues.begin()) s.append(", ");
		s.append(((IDSolveInterval)*it).toString());
	}
	s.append("}");
	return s;
}

std::vector<IDSolveInterval> IDSolver::getSolution() const {
	return this->solution;
}

string IDSolver::getFunction(const int i) const {
	return this->functionNameUnMap[i];
}

void IDSolver::addEquation(int i, IDExprTree expr) {
	MathExpressionManager* mem = ((MathExpressionManager*)(this->mathExprMng));
	mem->addExpression(this->functionNameUnMap[i], ((ExpTree*)(expr)));
}

int IDSolver::getNBEquations() {
	MathExpressionManager* mem = ((MathExpressionManager*)(this->mathExprMng));
	return mem->getNbExpression();
}

/******************************************************************************/

int IDSolver::solve() {
	
	interval t = IDSolveToInterval(this->varMin);
	interval tend = IDSolveToInterval(this->varMax);
	iVector x(this->n);
	std::map<std::string, int>::iterator it;
	int j = 0;
	string s;
	int error = -1;
	int tab[this->n];
	for(it=this->functionName.begin(); it != this->functionName.end(); it++, j++) {
		x[j] = IDSolveToInterval(this->initialValues[(*it).second - 1]);
		tab[(*it).second - 1] = j;
	}
	
	try {
		MathExpressionManager* mem = MathExpressionManager::getInstance();
		UserFunction *uf = new UserFunction(this->n, mem);
		for(int i = 0; i < this->n; i++) {
			mem->addVariable(this->functionNameUnMap[i]);
			uf->addDerivative(this->functionNameUnMap[i], this->functionNameUnMap[i]);
		}
		
		if(!mem->check()) error = 1;
		else {
			AD* ad = new FADBAD_AD(this->n, f, f, uf);
			VNODE*Solver= new VNODE(ad);
			Solver->integrate(t,x,tend);
			if(!Solver->successful()) error = 2;
			else {
				for(int i = 0; i < this->n; i++) {
					this->solution.push_back(intervalToIDSolve(x[tab[i]]));
				}
				error = 0;
			}
		}
	} catch ( exception &e ) { cerr << e.what() << endl; }
	return error;
}
