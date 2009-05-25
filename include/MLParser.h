/**
 * \file MLParser.h
 * \brief Parse MathLink data and send it to the VNode Interface 
 * \author Lejeune Aurelien
 * \version 1.0
 * \date 05/01/2009
 */

#ifndef MLPARSER_H_INCLUDED
#define MLPARSER_H_INCLUDED

#include "mathlink.h"
#include "IMathLink.h"
#include <vector>


/**
 * \enum IDS_ERROR_CODE
 * \brief error codes
 */
enum IDS_ERROR_CODE {
	IDS_ERROR_FCT,	/*!< A function is expected. */
	IDS_ERROR_LIST,	/*!< A List[] is expected. */
	IDS_ERROR_ARG,	/*!< There is a problem with the number of parameters. */
	IDS_ERROR_SYMB,	/*!< A Symbol is expected. */
	IDS_ERROR_SCLR,	/*!< A scalar is expected. */
	IDS_ERROR_REAL,	/*!< A Real is expected. */
	IDS_ERROR_ITRV	/*!< An Interval[] is expected. */
};

/******************************************************************************/
/** Sending data to MathLink */
/******************************************************************************/

void sendError(MLINK, IDS_ERROR_CODE);

void MLPutInterval(MLINK, const IDSolveInterval&);

void MLPutBoxe(MLINK, std::vector<IDSolveInterval>&);

/******************************************************************************/
/** Reading datatype from MathLink */
/******************************************************************************/

/**
 * \fn double numberToDouble(MLINK, int)
 * \brief tranform a MathLink number into a C++ double 
 * \param the ML link
 * \param the current packet
 * \return a double
 */
double numberToDouble(MLINK, int);

/**
 * \fn double readFraction(MLINK lp)
 * \brief transform a Mathematica Fraction[A,B] into a C++ double
 * \param the ML link
 * \return an approximation of A÷B
 */
double readFraction(MLINK lp);

/**
 * \fn IDSolveInterval parseScalar(MLINK lp)
 * \brief Tansform a Mathematica scalar (real or interval) S into an interval
 * \param the ML link
 * \return [S,S] ou [inf(S), sup(S)]
 */
IDSolveInterval parseScalar(MLINK lp);

/**
 * \fn std::string readExpression(MLINK lp)
 * \brief return the string of the ML expression
 * \param the ML link
 * \return the expression as a string
 */
std::string readExpression(MLINK);

/******************************************************************************/
/** Reading parameters from MathLink */
/******************************************************************************/

/**
 * \fn void readIDSolveParameters(MLINK lp, IDSolver &solver)
 * \brief read the parameters from Mathematica and store them in a Solver
 * \param the ML link
 * \param[in] An instance of an IODE solver
 */
void readIDSolveParameters(MLINK lp, IDSolver &solver);

/**
 * \fn void readFunctionsName(MLINK, IDSolver&)
 * \brief read the name of the function used in the ODE
 *		  and store them in the solver
 * \param the ML link
 * \param[in] An instance of an IODE solver
 */
void readFunctionsName(MLINK, IDSolver&);

/**
 * \fn void readVariableData(MLINK, IDSolver&)
 * \brief read the name of the time variable and its bounds
 *		  and store them in the solver
 * \param the ML link
 * \param[in] An instance of an IODE solver
 */
void readVariableData(MLINK, IDSolver&);

/**
 * \fn void readInitialValues(MLINK, IDSolver&)
 * \brief read the initial values and store them in the solver
 * \param the ML link
 * \param[in] An instance of an IODE solver
 */
void readInitialValues(MLINK, IDSolver&);

/**
 * \fn void readODE(MLINK, IDSolver&)
 * \brief read the ODE equations
 *		  and store them in the solver
 * \param the ML link
 * \param[in] An instance of an IODE solver
 */
void readODE(MLINK, IDSolver&);

#endif