// BeamParticle.h is a part of the PYTHIA event generator.
// Copyright (C) 2007 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Header file for information on incoming beams.
// ResolvedParton: an initiator or remnant in beam.
// BeamParticle: contains partons, parton densities, etc.

#ifndef Pythia8_BeamParticle_H
#define Pythia8_BeamParticle_H

#include "Basics.h"
#include "Event.h"
#include "FragmentationFlavZpT.h"
#include "Information.h"
#include "ParticleData.h"
#include "PartonDistributions.h"
#include "PythiaStdlib.h"
#include "Settings.h"

namespace Pythia8 {

//**************************************************************************

// This class holds info on a parton resolved inside the incoming beam,
// i.e. either an initiator (part a hard scattering or multiple interaction)
// or a remnant (part of the beam remnant treatment).

// The companion code is -1 from onset and for g, is -2 for an unmatched 
// sea quark, is >= 0 for a matched sea quark, with the number giving the 
// companion position, and is -3 for a valence quark.

class ResolvedParton {

public:

  // Constructor.
  ResolvedParton( int iPosIn = 0, int idIn = 0, double xIn = 0., 
    int companionIn = -1) : iPosRes(iPosIn), idRes(idIn), xRes(xIn), 
    companionRes(companionIn), xqCompRes(0.), mRes(0.), colRes(0),
    acolRes(0) { } 

  // Set info on initiator or remnant parton.
  void iPos( int iPosIn) {iPosRes = iPosIn;} 
  void id( int idIn) {idRes = idIn;} 
  void x( double xIn) {xRes = xIn;} 
  void update( int iPosIn, int idIn, double xIn) {iPosRes = iPosIn;
    idRes = idIn; xRes = xIn;} 
  void companion( int companionIn) {companionRes = companionIn;} 
  void xqCompanion( double xqCompIn) {xqCompRes = xqCompIn;} 
  void p(Vec4 pIn) {pRes = pIn;}
  void px(double pxIn) {pRes.px(pxIn);}
  void py(double pyIn) {pRes.py(pyIn);}
  void pz(double pzIn) {pRes.pz(pzIn);}
  void e(double eIn) {pRes.e(eIn);}
  void m(double mIn) {mRes = mIn;}
  void col(int colIn) {colRes = colIn;}
  void acol(int acolIn) {acolRes = acolIn;}
  void cols(int colIn = 0,int acolIn = 0) 
    {colRes = colIn; acolRes = acolIn;}  

  // Get info on initiator or remnant parton.
  int iPos() const {return iPosRes;} 
  int id() const {return idRes;} 
  double x() const {return xRes;} 
  int companion() const {return companionRes;} 
  bool isValence() const {return (companionRes == -3);}
  bool isUnmatched() const {return (companionRes == -2);}
  bool isCompanion() const {return (companionRes >= 0);}
  double xqCompanion() const {return xqCompRes;} 
  Vec4 p() const {return pRes;}
  double px() const {return pRes.px();}
  double py() const {return pRes.py();}
  double pz() const {return pRes.pz();}
  double e() const {return pRes.e();}
  double m() const {return mRes;}
  double pT() const {return pRes.pT();}
  double mT2() const {return mRes*mRes + pRes.pT2();}
  int col() const {return colRes;}
  int acol() const {return acolRes;}
 
private:

  // Properties of a resolved parton. 
  int    iPosRes, idRes;
  double xRes;
  // Companion code and distribution value, if any.
  int    companionRes; 
  double xqCompRes;
  // Four-momentum and mass; for remnant kinematics construction.
  Vec4   pRes;
  double mRes;
  // Colour codes.
  int   colRes, acolRes;

};

//**************************************************************************

// This class holds info on a beam particle in the evolution of 
// initial-state radiation and multiple interactions.

class BeamParticle {

public:

  // Constructor.
  BeamParticle() {Q2ValFracSav = -1.;}  

  // Initialize static data members.
  static void initStatic();

  // Initialize. Possibility to force re-initialization by hand.
  void init( int idIn, double pzIn, double eIn, double mIn, PDF* pdfInPtr,
    PDF* pdfHardInPtr, bool isUnresolvedIn = false);

  // Member functions for output.
  int id() const {return idBeam;}
  Vec4 p() const {return pBeam;}
  double px() const {return pBeam.px();}
  double py() const {return pBeam.py();}
  double pz() const {return pBeam.pz();}
  double e() const {return pBeam.e();}
  double m() const {return mBeam;}
  bool isLepton() const {return isLeptonBeam;}
  bool isUnresolved() const {return isUnresolvedBeam;}
  // As hadrons here we only count those we know how to handle remnants for.
  bool isHadron() const {return isHadronBeam;}
  bool isMeson() const {return isMesonBeam;}
  bool isBaryon() const {return isBaryonBeam;}

  // Maximum x remaining after previous MI and ISR, plus safety margin.
  double xMax(int iSkip = -1);

  // Special hard-process parton distributions (can agree with standard ones).
  double xfHard(int id, double x, double Q2) 
    {return pdfHardBeamPtr->xf(id, x, Q2);}
   
  // Standard parton distributions.
  double xf(int id, double x, double Q2) 
    {return pdfBeamPtr->xf(id, x, Q2);}

  // Ditto, split into valence and sea parts (where gluon counts as sea).
  double xfVal(int id, double x, double Q2) 
    {return pdfBeamPtr->xfVal(id, x, Q2);}
  double xfSea(int id, double x, double Q2) 
    {return pdfBeamPtr->xfSea(id, x, Q2);}

  // Rescaled parton distributions, as needed for MI and ISR.
  // For ISR also allow split valence/sea, and only return relevant part.
  double xfMI(int id, double x, double Q2) 
    {return xfModified(-1, id, x, Q2);}
  double xfISR(int indexMI, int id, double x, double Q2) 
    {return xfModified( indexMI, id, x, Q2);}

  // Decide whether chosen quark is valence, sea or companion.
  void pickValSeaComp();

  // Initialize kind of incoming beam particle.
  void initBeamKind();

  // Overload index operator to access a resolved parton from the list.
  ResolvedParton& operator[](int i) {return resolved[i];}

  // Total number of partons extracted from beam, and initiators only.
  int size() const {return resolved.size();}
  int sizeInit() const {return nInit;}

  // Clear list of resolved partons. 
  void clear() {resolved.resize(0);}

  // Add a resolved parton to list. 
  int append( int iPos, int id, double x, int companion = -1)
    {resolved.push_back( ResolvedParton( iPos, id, x, companion) );
    return resolved.size() - 1;}

  // Print extracted parton list; for debug mainly.
  void list(ostream& os = cout); 

  // How many different flavours, and how many quarks of given flavour.
  int nValenceKinds() const {return nValKinds;}
  int nValence(int id) const {for (int i = 0; i < nValKinds; ++i) 
    if (id == idVal[i]) return nVal[i]; return 0;}

  // Test whether a lepton is to be considered as unresolved.
  bool isUnresolvedLepton();

  // Add extra remnant flavours to make valence and sea come out right. 
  bool remnantFlavours(Event& event); 

  // Correlate all initiators and remnants to make a colour singlet. 
  bool remnantColours(Event& event, vector<int>& colFrom,
    vector<int>& colTo); 

  // Pick unrescaled x of remnant parton (valence or sea).
  double xRemnant(int i);

  // Tell whether a junction has been resolved, and its junction colours.
  bool hasJunction() const {return hasJunctionBeam;}  
  int junctionCol(int i) const {return junCol[i];}
  void junctionCol(int i, int col) {junCol[i] = col;}

  // For a diffractive system, decide whether to kick out gluon or quark.
  bool pickGluon(double mDiff);

  // Pick a valence quark at random, and provide the remaining flavour.
  int pickValence();
  int pickRemnant() const {return idVal2;}

  // Share lightcone momentum between two remnants in a diffractive system.
  // At the same time generate a relative pT for the two.
  double zShare( double mDiff, double m1, double m2);
  double pxShare() const {return pxRel;}
  double pyShare() const {return pyRel;}
 
private: 

  // Static initialization data, normally only set once.
  static int    maxValQuark, companionPower;
  static double valencePowerMeson, valencePowerUinP,
         valencePowerDinP, valenceDiqEnhance;
  static bool   allowJunction;
  static double pickQuarkNorm, pickQuarkPower, diffPrimKTwidth,
         diffLargeMassSuppress;

  // Constants: could only be changed in the code itself.
  static const double XMINUNRESOLVED;

  // Basic properties of a beam particle.
  int    idBeam, idBeamAbs;  
  Vec4   pBeam;
  double mBeam;
  PDF*   pdfBeamPtr;
  PDF*   pdfHardBeamPtr;

  // Beam kind. Valence flavour content for hadrons.
  bool   isLeptonBeam, isUnresolvedBeam, isHadronBeam, isMesonBeam, 
         isBaryonBeam;
  int    nValKinds, idVal[3], nVal[3];

  // Current parton density, by valence, sea and companion.
  int    idSave, iSkipSave, nValLeft[3]; 
  double xqgTot, xqVal, xqgSea, xqCompSum;

  // The list of resolved partons.
  vector<ResolvedParton> resolved; 

  // Status after all initiators have been accounted for. Junction content.
  int    nInit;
  bool   hasJunctionBeam;
  int    junCol[3];

  // Routine to calculate pdf's given previous interactions.
  double xfModified( int iSkip, int id, double x, double Q2); 

  // Fraction of hadron momentum sitting in a valence quark distribution.
  double xValFrac(int j, double Q2);
  double Q2ValFracSav, uValInt, dValInt;

  // Fraction of hadron momentum sitting in a companion quark distribution.
  double xCompFrac(double xs);

  // Value of companion quark PDF, also given the sea quark x.
  double xCompDist(double xc, double xs);

  // Valence quark subdivision for diffractive systems.
  int    idVal1, idVal2, idVal3;
  double zRel, pxRel, pyRel;

};
 
//**************************************************************************

} // end namespace Pythia8

#endif // Pythia8_BeamParticle_H
