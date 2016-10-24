// Header file for functionality pulled in from Stdlib,
// plus a few useful utilities (small powers).
// Copyright C 2007 Torbjorn Sjostrand

#ifndef Pythia8_PythiaStdlib_H
#define Pythia8_PythiaStdlib_H

// Stdlib header files for mathematics.
#include <cmath>

// Stdlib header files for strings and containers.
#include <string>
#include <vector>
#include <map>

// Stdlib header file for input and output.
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

// Define pi if not yet done.
#ifndef M_PI
#define M_PI 3.1415926535897932385
#endif

namespace Pythia8 {

// Generic utilities and mathematical functions.
using std::swap;
using std::max;
using std::min; 
using std::abs; 

// Strings and containers.
using std::string; 
using std::vector; 
using std::map; 

// Input/output streams.
using std::cin; 
using std::cout; 
using std::cerr; 
using std::istream; 
using std::ostream; 
using std::ifstream; 
using std::ofstream; 
using std::istringstream; 
using std::ostringstream; 

// Input/output formatting.
using std::endl; 
using std::fixed; 
using std::scientific; 
using std::left; 
using std::right; 
using std::setw; 
using std::setprecision; 

// Powers of small integers - for balance speed/code clarity.
inline double pow2(const double& x) {return x*x;}
inline double pow3(const double& x) {return x*x*x;}
inline double pow4(const double& x) {return x*x*x*x;}
inline double pow5(const double& x) {return x*x*x*x*x;}

// Avoid problem with negative square root argument (from roundoff).
inline double sqrtpos(const double& x) {return sqrt( max( 0., x));}

} // end namespace Pythia8

#endif // Pythia8_PythiaStdlib_H
