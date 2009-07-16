(* ::Package:: *)

IDLink =.
IDSetLinkPath::usage = "IDSetLinkPath[str] is used to indicate the location of the IDSolve plug-in.";
IDSetLinkPath::error = "`1` is not a string.";
IDSetLinkPath[x_] := If[StringQ[x],IDLink = x,  Message[IDSetLinkPath::error, x]];


IDScalarQ::usage = "IDScalarQ[expr] gives True if expr is a scalar (number or interval), and False otherwise.";
IDScalarQ[x_] := Module[
{bool},
bool = If[MemberQ[{Interval}, Head[x]],Length[Level[x,1]]==1,False];
MemberQ[{Rational,Integer, Real}, Head[x]] || bool
];


IDSolve::error = "Segmentation Fault";
IDSolve::idlim = "Range specification `1` is not of the form {x,xmin, xmax}";
IDSolve::dsvar = "`1` cannot be used as a variable";
IDSolve::itcs = "Cannot solve constraint equations for initial conditions";
IDSolve::idep = "Endpoint `1` in `2` is not a scalar";
IDSolve::ivnq = "";
IDSolve::dsfun = "`1` cannot be used as functions";
IDSolve::dsdif ="There is a problem with the derivatives";
IDSolve::ivins ="The initial values must be instanciate as Number or Interval";
IDSolve::eval = "The ODE expression cannot be evaluated";
IDSolve::reach = "VNODE-LP could not reach `1` = `2`";
IDSolve[expr_,func_,var_] :=
Module[{iv, ode, link, result, x, e},
(* dependent variable testing *)
If[Length[var] != 3 ,
Message[IDSolve::idlim, var[[1]]];Abort[]];
If[Length[ToBoxes[var[[1]]]] != 0 || NumberQ[var[[1]]],
Message[IDSolve::dsvar, var[[1]]];Abort[]];
If[!IDScalarQ[var[[2]]] || !IDScalarQ[var[[3]]],
 Message[IDSolve::idep, var[[1]], var];Abort[]];
(* function name testing *)
If[!And@@(MemberQ[{Symbol},#]&/@Head/@func),
 Message[IDSolve::dsfun, func];Abort[]];
(* initial values testing *)
iv = Map[Cases[expr,#[var[[2]]]==e_:>e]&,func];
If[!Length/@iv == Table[1,{Length[func]}],Message[IDSolve::itcs];Abort[]];
If[!And@@(IDScalarQ/@Flatten[iv]),Message[IDSolve::ivins]; Abort[]];
(* initial function *)
ode = Map[Cases[expr,Derivative[1][#][var[[1]]]==e_:>e]&,func];
If[!Length/@ode == Table[1,{Length[func]}],Message[IDSolve::dsdif];Abort[]];
ode =Flatten[ode]/.x_[var[[1]]]:>x;
(* Call VNODE-LP trought MathLink *)
link = Install[IDLink];
result = VNodeSolve[{func, var,Flatten[iv], Flatten[ode]}];
Uninstall[link];
result
];
