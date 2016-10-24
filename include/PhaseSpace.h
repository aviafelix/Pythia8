// Header file for phase space generators in kinematics selection.
// PhaseSpace: base class for phase space generators.
// PhaseSpace2to2tauyz: 2 -> 2 processes in tau, y, z = cos(theta).
// Copyright C 2007 Torbjorn Sjostrand

#ifndef Pythia8_PhaseSpace_H
#define Pythia8_PhaseSpace_H

#include "Basics.h"
#include "BeamParticle.h"
#include "Information.h"
#include "MultipleInteractions.h"
#include "ParticleData.h"
#include "PartonDistributions.h"
#include "PythiaStdlib.h"
#include "SigmaProcess.h"
#include "SigmaTotal.h"
#include "Settings.h"
#include "UserHooks.h"

namespace Pythia8 {

//**************************************************************************

// Forward reference to the UserHooks class.
class UserHooks;
 
//**************************************************************************

// PhaseSpace is a base class for  phase space generators 
// used in the selection of hard-process kinematics.

class PhaseSpace {

public:

  // Destructor.
  virtual ~PhaseSpace() {}

  // Initialize static data members.
  static void initStatic();

  // Store static pointers to beams, SigmaTotal and user hooks.
  static void setStaticPtrs( BeamParticle* beamAPtrIn, 
    BeamParticle* beamBPtrIn, SigmaTotal* sigmaTotPtrIn,
    UserHooks* userHooksPtrIn = 0);

  // Give in pointer to cross section and cm energy.
  void initInfo(SigmaProcess* sigmaProcessPtrIn, double eCMIn);

  // A pure virtual method, wherein an optimization procedure
  // is used to determine how phase space should be sampled.
  virtual bool setupSampling() = 0; 

  // A pure virtual method, wherein a trial event kinematics 
  // is to be selected in the derived class
  virtual bool trialKin(bool inEvent = true) = 0; 

  // A pure virtual method, wherein the accepted event kinematics 
  // is to be constructed in the derived class
  virtual bool finalKin() = 0; 

  // Allow for nonisotropic decays when ME's available.
  void decayKinematics( Event& process);

  // Give back current or maximum cross section, or set latter.
  double sigmaNow() const {return sigmaNw;}
  double sigmaMax() const {return sigmaMx;}
  void   setSigmaMax(double sigmaMaxIn) {sigmaMx = sigmaMaxIn;}

  // Give back constructed four-vectors and known masses.
  Vec4   p(int i)   const {return pH[i];} 
  double m(int i)   const {return mH[i];}

  // Give back other event properties.
  double ecm()      const {return eCM;}
  double x1()       const {return x1H;}
  double x2()       const {return x2H;}
  double sHat()     const {return sH;}
  double tHat()     const {return tH;}
  double uHat()     const {return uH;}
  double pTHat()    const {return pTH;}
  double thetaHat() const {return theta;}
  double phiHat()   const {return phi;}
  double runBW3()   const {return runBW3H;}
  double runBW4()   const {return runBW4H;}

  // Are beam particles resolved in partons or scatter directly?
  virtual bool isResolved() const {return true;}

protected:

  // Constructor.
  PhaseSpace() {}

  // Static initialization data, normally only set once.
  static double mHatGlobalMin, mHatGlobalMax, pTHatMin, pTHatMax, 
                pTHatMinDiverge, minWidthBreitWigners, pT2HatMin, 
                pT2HatMax;
  static bool   useBreitWigners, showSearch, showViolation;
  static int    gmZmodeGlob;

  // Constants: could only be changed in the code itself.
  static const double SAFETYMARGIN, TINY, EVENFRAC, SAMESIGMA, WIDTHMARGIN, 
                      SAMEMASS, MASSMARGIN, EXTRABWWTMAX, THRESHOLDSIZE, 
                      THRESHOLDSTEP, LEPTONXMIN, LEPTONXMAX, LEPTONXLOGMIN, 
                      LEPTONXLOGMAX, LEPTONTAUMIN;
  static const int    NMAXTRY;

  // Static information on incoming beams.
  static BeamParticle* beamAPtr;
  static BeamParticle* beamBPtr;
  static int    idA, idB;
  static double mA, mB; 
  static bool   hasLeptonBeams, hasPointLeptons;
  
  // Static pointer to the total/elastic/diffractive cross section object.
  static SigmaTotal* sigmaTotPtr;

  // Static pointer to userHooks object for user interaction with program.
  static UserHooks* userHooksPtr;
  static bool canModifySigma;

  // Center-of-mass energy.
  double eCM, s; 

  // Cross section information.
  int    gmZmode;
  double wtBW, sigmaNw, sigmaMx, sigmaNeg;

  // Pointer to cross section. 
  SigmaProcess* sigmaProcessPtr; 

  // Process-specific kinematics properties, almost always available.
  double mHatMin, mHatMax, sHatMin, sHatMax;

  // Event-specific kinematics properties, almost always available.
  double x1H, x2H, m3, m4, s3, s4, mHat, sH, tH, uH, pAbs, p2Abs, 
         pTH, theta, phi, betaZ, pTHatMinNow, pT2HatMinNow,
         runBW3H, runBW4H;
  Vec4   pH[6];
  double mH[6];

  // Much common code for normal 2 -> 1 and 2 -> 2 (= 2 -> 1/2) cases:

  // Determine how phase space should be sampled.
  bool setupSampling1or2(bool is2, ostream& os = cout); 

  // Select a trial kinematics phase space point.
  bool trialKin1or2(bool is2, bool inEvent = true, ostream& os = cout); 

  // Presence and properties of any s-channel resonances.
  int    idResA, idResB;
  double mResA, mResB, GammaResA, GammaResB, tauResA, tauResB, widResA, 
         widResB;
  bool   sameResMass;

  // Kinematics properties specific to 2 -> 1/2.
  double tau, y, z, tauMin, tauMax, yMax, zMin, zMax, ratio34, unity34, 
         zNeg, zPos, wtTau, wtY, wtZ, intTau0, intTau1, intTau2, intTau3, 
         intTau4, intTau5, intTau6, intY01, intY2, intY34; 

  // Coefficients for optimized selection in 2 -> 1/2.
  int    nTau, nY, nZ;
  double tauCoef[8], yCoef[8], zCoef[8], tauCoefSum[8], yCoefSum[8], 
         zCoefSum[8];

  // Calculate kinematical limits for 2 -> 1/2.
  bool limitTau(bool is2);
  bool limitY();
  bool limitZ();

  // Select kinematical variable between defined limits for 2 -> 1/2.
  void selectTau(int iTau, double tauVal, bool is2);
  void selectY(int iY, double yVal);
  void selectZ(int iZ, double zVal);

  // Solve equation system for better phase space coefficients in 2 -> 1/2.
  void solveSys( int n, int bin[8], double vec[8], double mat[8][8],
    double coef[8], ostream& os = cout); 

};
 
//**************************************************************************

// A derived class with 2 -> 1 kinematics set up in tau, y.

class PhaseSpace2to1tauy : public PhaseSpace {

public:

  // Constructor.
  PhaseSpace2to1tauy() {}

  // Construct the trial kinematics, using methods in base class.
  virtual bool setupSampling() {if (!setupMass()) return false;
    return setupSampling1or2(false);} 
  virtual bool trialKin(bool inEvent = true) {wtBW = 1.; 
    return trialKin1or2(false, inEvent);}

  // Construct the final event kinematics.
  virtual bool finalKin();

private:

  // Set up allowed mass range.
  bool setupMass();

};
 
//**************************************************************************

// A derived class with 2 -> 2 kinematics set up in tau, y, z = cos(theta).

class PhaseSpace2to2tauyz : public PhaseSpace {

public:

  // Constructor.
  PhaseSpace2to2tauyz() {}

  // Construct the trial kinematics, using methods in base class.
  virtual bool setupSampling() {if (!setupMasses()) return false; 
    return setupSampling1or2(true);} 
  virtual bool trialKin(bool inEvent = true) {
    if (!trialMasses()) return false; 
    return trialKin1or2(true, inEvent);}

  // Construct the final event kinematics.
  virtual bool finalKin();

private:

  // Set up for fixed or Breit-Wigner mass selection.
  bool setupMasses();

  // Select fixed or Breit-Wigner-distrubuted masses.
  bool trialMasses();

  // Evaluate Breit-Wigner correction factor.
  void weightMasses();

  // Pick off-shell initialization masses when on-shell not allowed.
  bool constrainedM3M4();
  bool constrainedM3();
  bool constrainedM4();

  // Properties specific to mass selection in 2 -> 2.
  int    id3Mass, id4Mass;
  bool   useBW3, useBW4; 
  double m34Max, 
         m3Peak, s3Peak, m3Width, m3Min, m3Max, mw3, wm3Rat, m3Lower, 
         m3Upper, s3Lower, s3Upper, frac3Flat, frac3Inv, frac3Inv2, 
         atan3Lower, atan3Upper, int3BW, int3Flat, int3Inv, int3Inv2, 
         m4Peak, s4Peak, m4Width, m4Min, m4Max, mw4, wm4Rat, m4Lower, 
         m4Upper, s4Lower, s4Upper, frac4Flat, frac4Inv, frac4Inv2, 
         atan4Lower, atan4Upper, int4BW, int4Flat, int4Inv, int4Inv2;

};
 
//**************************************************************************

// A derived class with 2 -> 2 kinematics set up for elastic or 
// diffractive scattering.

class PhaseSpace2to2eldiff : public PhaseSpace {

public:

  // Constructor.
  PhaseSpace2to2eldiff(bool diffAin = false, bool diffBin = false)
    : diffA(diffAin), diffB(diffBin) {}

  // Construct the trial or final event kinematics.
  virtual bool setupSampling(); 
  virtual bool trialKin(bool inEvent = true); 
  virtual bool finalKin(); 

  // Are beam particles resolved in partons or scatter directly?
  virtual bool isResolved() const {return false;}

private:

  // Constants: could only be changed in the code itself.
  static const int    NTRY;
  static const double EXPMAX, DIFFMASSMAX;

  // Kinematics properties specific to 2 -> 2.
  bool   diffA, diffB;
  double m3ElDiff, m4ElDiff, cRes, sResXB, sResAX, sProton,
         s1, s2, bMin, lambda12, lambda34, tLow, tUpp, tAux;

};
 
//**************************************************************************

// A derived class for minumum bias event. Hardly does anything, since
// the real action is taken care of by the MultipleInteractions class.

class PhaseSpace2to2minbias : public PhaseSpace {

public:

  // Constructor.
  PhaseSpace2to2minbias() {}

  // Construct the trial or final event kinematics.
  virtual bool setupSampling() {sigmaNw = sigmaProcessPtr->sigmaHat();
    sigmaMx = sigmaNw; return true;}
  virtual bool trialKin( bool ) {return true;}  
  virtual bool finalKin() {return true;}

private:

};

//**************************************************************************

} // end namespace Pythia8

#endif // Pythia8_PhaseSpace_H
 
