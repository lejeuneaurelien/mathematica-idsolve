#include "UserFunction.h"

UserFunction::UserFunction(int n, MathExpressionManager* mem) {
	this->n = n;
	this->derivatives = map<string, string>();
	this->expressions = mem;
}

map<string, string> UserFunction::getDerivatives() const { return this->derivatives; }

void UserFunction::addDerivative(const string var, const string derivative) {
	if(this->derivatives.size() < n)
		this->derivatives[var] = derivative;
}

std::ostream & operator << (std::ostream & flux, const UserFunction & uf){
	flux << uf.n;
	return flux;
}

template <> void UserFunction::f(int n, T<F<interval> > *yp, const T<F<interval> > *y, T<F<interval> > t) {
	map<string, string>::iterator it;
	int i;
	for (it=this->derivatives.begin(), i = 0;
			it != this->derivatives.end(); it++, i++) {
		this->expressions->setVariable_((*it).first, &y[i]);
	}
	
	for (it=this->derivatives.begin(), i = 0;
			it != this->derivatives.end(); it++, i++) {
		yp[i] = this->expressions->evaluate_((*it).second);
	}
	
}

template <> void UserFunction::f(int n, T<interval> *yp, const T<interval> *y, T<interval> t) {
	map<string, string>::iterator it;
	int i;
	for (it=this->derivatives.begin(), i = 0;
			it != this->derivatives.end(); it++, i++) {
		this->expressions->setVariable((*it).first, &y[i]);
	}
	
	for (it=this->derivatives.begin(), i = 0;
			it != this->derivatives.end(); it++, i++) {
		yp[i] = this->expressions->evaluate((*it).second);
	}
}