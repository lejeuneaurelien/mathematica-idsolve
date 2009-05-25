/**
 * \file IMathLink.h
 * \brief Interface between IDSolve and MathLink
 * \author Lejeune Aurelien
 * \version 1.0
 * \date 06/10/2008
 */

#ifndef IMATHLINK_H_INCLUDED
#define IMATHLINK_H_INCLUDED

#include <string>
#include <vector>
#include <map>

std::string intToString(const int);
std::string doubleToString(const double);
int test(int);

/******************************************************************************/

typedef void* IDExprTree;

IDExprTree createExprTreeFunc(std::string);
IDExprTree createExprTreeVar(std::string);
IDExprTree createExprTreeConst(int);
IDExprTree createExprTreeConst(double);
IDExprTree createExprTreeConst(double, double);
void exprTreeAddLeft(IDExprTree, IDExprTree);
void exprTreeAddRight(IDExprTree , IDExprTree);
std::string exprTreeToString(IDExprTree);

/******************************************************************************/

class IDSolveInterval{
	private :
		double _min;
		double _max;
	public :
		double min() const;
		double max() const;
		void setBounds(const double, const double);
		std::string toString() const;
};

class IDSolver {
	private:
		int n;
		std::vector<IDSolveInterval> initialValues;
		std::map<std::string, int> functionName;
		std::vector<std::string> functionNameUnMap;
		std::string variableName;
		IDSolveInterval varMin;
		IDSolveInterval varMax;
		std::vector<IDSolveInterval> solution;
		void* mathExprMng;
		
	public:
		IDSolver();
		void addEquation(int i, IDExprTree);
		int getNBEquations();
		void setN(const int);
		int  getN() const;
		void addFunction(const std::string&);
		std::string functionToString();
		void setVariableName(const std::string&);
		std::string getVariableName() const;
		void setVarMin(const IDSolveInterval&);
		void setVarMax(const IDSolveInterval&);
		IDSolveInterval getVarMin() const;
		IDSolveInterval getVarMax() const;
		void addInitialValue(const IDSolveInterval&);
		std::string initialValuesToString();
		int solve();
		std::vector<IDSolveInterval> getSolution() const;
		std::string getFunction(const int) const;
};

#endif