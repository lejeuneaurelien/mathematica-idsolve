#include "MLParser.h"

static IDExprTree parseEquation(MLINK lp);
/******************************************************************************/
/** Sending data to MathLink */
/******************************************************************************/

void sendError(MLINK lp, IDS_ERROR_CODE e) {
	MLEndPacket(stdlink);
	MLPutFunction(stdlink, "Message", 1);
	switch(e) {
	case IDS_ERROR_LIST :
		MLPutString(stdlink, "A list was expected");
		break;
	case IDS_ERROR_ARG :
		MLPutString(stdlink, "More or less arguments was expected");
		break;
	case IDS_ERROR_FCT :
		MLPutString(stdlink, "A function was expected");
		break;
	case IDS_ERROR_SYMB :
		MLPutString(stdlink, "A symbol was expected");
		break;
	case IDS_ERROR_SCLR :
		MLPutString(stdlink, "A scalar was expected");
		break;
	case IDS_ERROR_REAL :
		MLPutString(stdlink, "A real was expected");
		break;
	case IDS_ERROR_ITRV :
		MLPutString(stdlink, "A interval was expected");
		break;
	default : MLPutString(stdlink, "Unreferenced error");
	}
	MLEndPacket(stdlink);
}

void MLPutInterval(MLINK lp, const IDSolveInterval& i) {
	MLPutFunction(lp, "Interval", 1); 
	  MLPutFunction(lp, "List", 2);
		MLPutDouble(lp, i.min());
		MLPutDouble(lp, i.max());
}

void MLPutBoxe(MLINK lp, std::vector<IDSolveInterval>& b) {
	std::vector<IDSolveInterval>::iterator it;
	MLPutFunction(lp, "List", b.size());
	  for(it = b.begin() ; it < b.end(); it++)
		MLPutInterval(lp, *it);
}

/******************************************************************************/
/** Reading datatype from MathLink */
/******************************************************************************/

double numberToDouble(MLINK lp, int packet) {
	double x = 0;
	int a; double d; const char *s;
	switch(packet) {
		case MLTKINT:
			MLGetInteger(lp, &a);
			x = (double)a;
			break;
		case MLTKREAL:
			MLGetReal(lp, &d);
			x = d;
			break;
		case MLTKFUNC: MLGetFunction(stdlink, &s, &a);
			if(std::string(s).compare("Rational") != 0) sendError(lp, IDS_ERROR_SCLR);
			x = readFraction(lp);
			break;
		default : sendError(lp, IDS_ERROR_SCLR);
	}
	return x;
}

double readFraction(MLINK lp) {
	double q, r;
	q = numberToDouble(lp, MLGetNext(lp));
	r = numberToDouble(lp, MLGetNext(lp));
	return q / r;
}

IDSolveInterval parseScalar(MLINK lp) {
	const char* s; int a; double d, f;
	IDSolveInterval i;
	i.setBounds(1, 1);
	switch(MLGetNext(lp)) {
		case MLTKINT:
			MLGetInteger(lp, &a);
			i.setBounds(a, a);
			break;
		case MLTKREAL:
			MLGetReal(lp, &d);
			i.setBounds(d, d);
			break;
		case MLTKFUNC: MLGetFunction(lp, &s, &a);
			if(std::string(s).compare("Interval") == 0) {
				MLGetNext(lp);
				MLGetFunction(lp, &s, &a);
				if(std::string(s).compare("List") != 0) sendError(lp, IDS_ERROR_LIST);
				d = numberToDouble(lp, MLGetNext(lp));
				f = numberToDouble(lp, MLGetNext(lp));
				i.setBounds(d, f);
			} else
			if(std::string(s).compare("Rational") == 0) {
				d = readFraction(lp);
				i.setBounds(d, d);
			} else
				sendError(lp, IDS_ERROR_SCLR);
			break;
		default : sendError(lp, IDS_ERROR_SCLR);
	}
	return i;
}

std::string readExpression(MLINK lp) {
	const char* s;
	int a;
	double d;
	std::string str;
	switch(MLGetNext(lp)) {
		case MLTKSYM:
			MLGetSymbol(lp, &s);
			str.append("#");
			str.append(s);
			MLDisownSymbol(lp, s);
			break;
		case MLTKINT:
			MLGetInteger(lp, &a);
			str.append(intToString(a));
			break;
		case MLTKREAL:
			MLGetReal(lp, &d);
			str.append(doubleToString(d));
			break;
		case MLTKFUNC:
			MLGetFunction(stdlink, &s, &a);
			str.append(s);
			str.append("@");
			str.append("[");
			for(int i = 1; i <= a; i++){
				str.append(readExpression(lp));
				if(i != a) str.append(", ");
			}
			str.append("]");
			break;
		default : ;
	}
	return str;
}

/******************************************************************************/
/** Reading parameters from MathLink */
/******************************************************************************/

void readIDSolveParameters(MLINK lp, IDSolver &solver) {
	const char* s;
	int a;
	if(MLGetNext(lp) != MLTKFUNC) sendError(lp, IDS_ERROR_FCT);
	MLGetFunction(stdlink, &s, &a);
	if(std::string(s).compare("List") != 0) sendError(lp, IDS_ERROR_LIST);
	if(a != 4) sendError(lp, IDS_ERROR_ARG);
	
	readFunctionsName(lp, solver);
	readVariableData(lp, solver);
	readInitialValues(lp, solver);
	readODE(lp, solver);
}

void readFunctionsName(MLINK lp, IDSolver &solver) {
	const char* s; int a;
	if(MLGetNext(lp) != MLTKFUNC) sendError(lp, IDS_ERROR_FCT);
	MLGetFunction(stdlink, &s, &a);
	if(std::string(s).compare("List") != 0) sendError(lp, IDS_ERROR_LIST);
	solver.setN(a);
	for(int i = 0; i < a; i++) {
		if(MLGetNext(lp) != MLTKSYM) sendError(lp, IDS_ERROR_SYMB);
		MLGetSymbol(lp, &s);
		solver.addFunction(std::string(s));
	}
}

void readVariableData(MLINK lp, IDSolver &solver) {
	const char* s; int a;
	if(MLGetNext(lp) != MLTKFUNC) sendError(lp, IDS_ERROR_FCT);
	MLGetFunction(stdlink, &s, &a);
	if(std::string(s).compare("List") != 0) sendError(lp, IDS_ERROR_LIST);
	if(MLGetNext(lp) != MLTKSYM) sendError(lp, IDS_ERROR_SYMB);
	MLGetSymbol(lp, &s);
	solver.setVariableName(std::string(s));
	solver.setVarMin(parseScalar(lp));
	solver.setVarMax(parseScalar(lp));
}

void readInitialValues(MLINK lp, IDSolver &solver) {
	const char* s; int a;
	if(MLGetNext(lp) != MLTKFUNC) sendError(lp, IDS_ERROR_FCT);
	MLGetFunction(stdlink, &s, &a);
	if(std::string(s).compare("List") != 0) sendError(lp, IDS_ERROR_LIST);
	for(int i = 0; i < a; i++) {
		solver.addInitialValue(parseScalar(lp));
	}
}

void readODE(MLINK lp, IDSolver &solver) {
	const char* s; int a;
	if(MLGetNext(lp) != MLTKFUNC) sendError(lp, IDS_ERROR_FCT);
	MLGetFunction(stdlink, &s, &a);
	if(std::string(s).compare("List") != 0) sendError(lp, IDS_ERROR_LIST);
	for(int i = 0; i < a; i++) {
		solver.addEquation(i, parseEquation(lp));
	}
}

/******************************************************************************/
/** Static functions */
/******************************************************************************/

static IDExprTree intervalToExprTree(MLINK lp) {
	const char* s; int a; double inf, sup;
	MLGetNext(lp);
	MLGetFunction(lp, &s, &a);
	if(std::string(s).compare("List") != 0) sendError(lp, IDS_ERROR_LIST);
	inf = numberToDouble(lp, MLGetNext(lp));
	sup = numberToDouble(lp, MLGetNext(lp));
	return createExprTreeConst(inf, sup);
}

static IDExprTree parseEquation(MLINK lp) {
	const char* s; int a, i; double d; IDExprTree expr = NULL;
	std::vector<IDExprTree> tab;
	switch(MLGetNext(lp)) {
		case MLTKSYM:
			MLGetSymbol(lp, &s);
			expr = createExprTreeVar(std::string(s));
			MLDisownSymbol(lp, s);
			break;
		case MLTKINT:
			MLGetInteger(lp, &a);
			expr = createExprTreeConst(a);
			break;
		case MLTKREAL:
			MLGetReal(lp, &d);
			expr = createExprTreeConst(d);
			break;
		case MLTKFUNC:
			MLGetFunction(stdlink, &s, &a);
			if(std::string(s).compare("Interval") == 0) {
				expr = intervalToExprTree(lp);
			} else {
			if(std::string(s).compare("Rational") == 0) {
				expr = createExprTreeConst(readFraction(lp));
			} else {
			if(a < 2) sendError(lp, IDS_ERROR_ARG);
				tab.push_back(createExprTreeFunc(std::string(s)));
				exprTreeAddLeft(tab[0],  parseEquation(lp));
				for(i = 1; i < a-1; i++) {
				tab.push_back(createExprTreeFunc(std::string(s)));
				exprTreeAddLeft(tab[i],  parseEquation(lp));
				exprTreeAddRight(tab[i-1],  tab[i]);
				}
				exprTreeAddRight(tab[i-1],  parseEquation(lp));
				expr = tab[0];
			}}
			break;
		default : expr = NULL;
	}
	return expr;
}