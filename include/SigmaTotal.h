// SigmaTotal.h is a part of the PYTHIA event generator.
// Copyright (C) 2008 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This file contains the class for cross section parametrizations.
// SigmaTotal: total and partial cross section in hadron-hadron collisions.

#ifndef Pythia8_SigmaTotal_H
#define Pythia8_SigmaTotal_H

#include "Info.h"
#include "ParticleData.h"
#include "PythiaStdlib.h"
#include "Settings.h"

namespace Pythia8 {
 
//**************************************************************************

// The SigmaTotal class contains parametrizations of total, elastic and 
// diffractive cross sections, and of the respective slope parameter.

class SigmaTotal {

public:

  // Constructor.
  SigmaTotal() : isCalc(false) {};

  // Storee pointer to Info and initialize data members.
  void init(Info* infoPtrIn);

  // Calculate, or recalculate for new beams or new energy.
  bool calc(int idA, int idB, double eCM); 

  // Confirm that initialization worked.
  bool   hasSigmaTot() const {return isCalc;}

  // Read out total and partial cross sections.
  double sigmaTot() const {return sigTot;}
  double sigmaEl() const {return sigEl;}
  double sigmaXB() const {return sigXB;}
  double sigmaAX() const {return sigAX;}
  double sigmaXX() const {return sigXX;}
  double sigmaND() const {return sigND;}

  // Read out slope b in exp(b*t) dependence.
  double bSlopeEl() const {return bEl;}
  double bSlopeXB(double sX) const { return 2.*bB + alP2 * log(s/sX) ;}
  double bSlopeAX(double sX) const { return 2.*bA + alP2 * log(s/sX) ;} 
  double bSlopeXX(double sX1, double sX2) const { 
    return alP2 * log( exp(4.) + s * s0 / (sX1 * sX2) ) ;}   

  // Read out parameters of diffractive mass spectra.
  double mMinXB() const {return mMinXBsave;}
  double mMinAX() const {return mMinAXsave;}
  double cRes() const {return CRES;}
  double mResXB() const {return mResXBsave;}
  double mResAX() const {return mResAXsave;}
  double sProton() const {return SPROTON;}

  // Read out parameters of trial t spectra.
  double bMinSlopeXB() const { return max(2., 2. * bB);} 
  double bMinSlopeAX() const { return max(2., 2. * bA);} 
  double bMinSlopeXX() const { return alP2 * 4.;} 

private:

  // Initialization data, normally only set once.
  bool   isCalc, setTotal, setElastic;
  double sigTotOwn, sigElOwn, sigXBOwn, sigAXOwn, sigXXOwn,
         bSlope, rho, lambda, tAbsMin, alphaEM0;

  // Constants: could only be changed in the code itself.
  static const int    IHADATABLE[], IHADBTABLE[], ISDTABLE[], IDDTABLE[];
  static const double MMIN, EPSILON, ETA, X[], Y[], BETA0[], BHAD[],
                      ALPHAPRIME, CONVERTEL, CONVERTSD, CONVERTDD, MMIN0, 
                      CRES, MRES0, CSD[10][8], CDD[10][9], SPROTON;

  // Pointer to various information on the generation.
  Info*  infoPtr;

  // Store values found by calc.
  double sigTot, sigEl, sigXB, sigAX, sigXX, sigND, bEl, s, bA, bB,
         alP2, s0, exp4, mMinXBsave, mMinAXsave, mResXBsave, mResAXsave;

};
 
//**************************************************************************

} // end namespace Pythia8

#endif // Pythia8_SigmaTotal_H
