// HadronLevel.h is a part of the PYTHIA event generator.
// Copyright (C) 2007 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This file contains the main class for hadron-level generation.
// HadronLevel: handles administration of fragmentation and decay.

#ifndef Pythia8_HadronLevel_H
#define Pythia8_HadronLevel_H

#include "Basics.h"
#include "Event.h"
#include "FragmentationSystems.h"
#include "Information.h"
#include "MiniStringFragmentation.h"
#include "ParticleData.h"
#include "ParticleDecays.h"
#include "PythiaStdlib.h"
#include "Settings.h"
#include "StringFragmentation.h"
#include "TimeShower.h"

namespace Pythia8 {
 
//**************************************************************************

// The HadronLevel class contains the top-level routines to generate
// the transition from the partonic to the hadronic stage of an event.

class HadronLevel {

public:

  // Constructor. 
  HadronLevel() {}

  // Possibility to pass in pointer for external handling of some decays.
  bool decayPtr( DecayHandler* decayHandlePtrIn, 
    vector<int> handledParticles) 
    { return decays.decayPtr( decayHandlePtrIn, handledParticles);}  

  // Initialize static data members.
  static void initStatic();

  // Save pointer. Initialize alphaStrong in ParticleDecays.
  bool init(Info* infoPtrIn, TimeShower* timesDecPtrIn) {
    infoPtr = infoPtrIn; decays.init(timesDecPtrIn); return true;}
 
  // Generate the next event.
  bool next(Event& event); 

private: 

  // Static initialization data, normally only set once.
  static bool   Hadronize, Decay;
  static double mStringMin, eNormJunction, mThad;

  // Constants: could only be changed in the code itself.
  static const int    NTRYJNREST;
  static const double JJSTRINGM2MAX, JJSTRINGM2FRAC, CONVJNREST, MTHAD;

  // Pointer to various information on the generation.
  Info* infoPtr;

  // The main generator classes for hadronization and decay.
  StringFragmentation stringFrag;
  MiniStringFragmentation ministringFrag;
  ParticleDecays decays;

  // Special case: colour-octet onium decays, to be done initially.
  void  decayOctetOnia(Event& event);

  // Configuration of colour-singlet systems.
  ColConfig colConfig;   
 
  // Trace colour flow in the event to form colour singlet subsystems.
  bool findSinglets(Event& event);
 
  // Trace a colour line, from a colour, from an anticolour, or in loop.
  bool traceFromCol(int indxCol, Event& event, int iJun = -1, int iCol = -1);
  bool traceFromAcol(int indxCol, Event& event, int iJun = -1, int iCol = -1); 
  bool traceInLoop(int indxCol, int indxAcol, Event& event);

  // Split junction-antijunction system into two, or simplify other way.
  bool splitJunctionPair(Event& event);

  // Colour information.
  vector<int>    iColEnd, iAcolEnd, iColAndAcol, iParton, iPartonJun, 
                 iPartonAntiJun, iJunLegA, iJunLegB, iJunLegC,  
                 iAntiLegA, iAntiLegB, iAntiLegC, iGluLeg;
  vector<double> m2Pair; 
  
};
 
//**************************************************************************

} // end namespace Pythia8

#endif // Pythia8_HadronLevel_H
