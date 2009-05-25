#include "mathlink.h"
#include "MLParser.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


void bidon() {
	std::string s = readExpression(stdlink);
	MLPutString(stdlink, s.c_str());
}

void intervalList(double x, double y, int n) {
	MLPutFunction(stdlink, "List", n);
	for(int i = 0; i < n; i++) {
		MLPutFunction(stdlink, "Interval", 1); 
		 MLPutFunction(stdlink, "List", 2);
		  MLPutDouble(stdlink, x);
		  MLPutDouble(stdlink, y);
	}
	MLEndPacket(stdlink);
}

void VNodeSolve() {
	IDSolver solver;
	std::vector<IDSolveInterval> solution;
	std::string message;
	readIDSolveParameters(stdlink, solver);
	MLEndPacket(stdlink);
	switch(solver.solve()) {
	case 0 :
		solution = solver.getSolution();
		MLPutBoxe(stdlink, solution);
		break;
	case 1 :
		MLPutFunction(stdlink, "Message", 1);
		MLPutString(stdlink, "The ODE expression cannot be evaluated");
		break;
	case 2 :
		MLPutFunction(stdlink, "Message", 3);
		MLPutString(stdlink, "IDSolve::reach");
		MLPutString(stdlink, solver.getVariableName().c_str());
		MLPutInterval(stdlink, solver.getVarMax());
		break;
	}
	MLEndPacket(stdlink);
}

int main(int argc, char *argv[]) {
	return MLMain(argc, argv);
}
