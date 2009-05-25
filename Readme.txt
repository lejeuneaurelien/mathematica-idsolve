
 ******************************************************************************
 *                                                                            *
 *               _   _                          _   _                         *
 *   /\/\   __ _| |_| |__   ___ _ __ ___   __ _| |_(_) ___ __ _               *
 *  /    \ / _` | __| '_ \ / _ \ '_ ` _ \ / _` | __| |/ __/ _` |              *
 * / /\/\ \ (_| | |_| | | |  __/ | | | | | (_| | |_| | (_| (_| |              *
 * \/    \/\__,_|\__|_| |_|\___|_| |_| |_|\__,_|\__|_|\___\__,_|              *
 *                                          _____  ___  __       _            *
 *                                          \_   \/   \/ _\ ___ | |_   _____  *
 *                                           / /\/ /\ /\ \ / _ \| \ \ / / _ \ *
 *                                        /\/ /_/ /_// _\ \ (_) | |\ V /  __/ *
 *                                        \____/___,'  \__/\___/|_| \_/ \___| *
 *                                                                            *
 *              Rigorous ODE Solver plug-in for Mathematica                   *
 *                                                                            *
 *                                                                            *
 *                                                (c) Aurelien Lejeune 2009   *
 ******************************************************************************

Mathematica IDSolve Source Code

  This package contains the full source code of Mathematica IDSolve 1.2
  To compile it you need the VNode library.
  You can download it on Pr. Ned Nedialkov web page.
          http://www.cas.mcmaster.ca/~nedialk/
  If you are using Windows XP, there is a how to method to compile the library
  under this OS at this address :
          http://sites.google.com/site/lejeuneaurelien/

Compiling the sources

  For the compilation under Ms Windows, you will need MinGW and MSYS
    avaliable at :
          http://www.mingw.org/
    MinGW is a port of GNU applications and MSYS is a shell emulator.
  Run "make mrproper" in a shell
  Run "./configure" in a shell
    Indicate the location of VNODE-LP and Mathematica
  Run "make" in a shell

Using the plug-in

  Open "IDSolve.m" in Mathematica and run the package
  In your notebook include the line :
          IDSetLinkPath["IDSolveDirectory\IDSolve.exe"];
    where "IDSolveDirectory" is the directory where you have installed IDSolve
  Now you can use the function IDSolve[] to rigorously solve ODE
  Some exemples are include in the notebook "exemple.nb"

Copyright

  See License.txt for details about distribution and modification.

  If you have any comments or questions, please drop me a note:
  lejeune.aurelien@gmail.com

  (c) Aurelien Lejeune 2009