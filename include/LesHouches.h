// LesHouches.h is a part of the PYTHIA event generator.
// Copyright (C) 2007 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Header file for Les Houches Accord user process information.
// LHAinit: base class for initialization information.
// LHEevnt: Base class for event information. 
// LHAinitLHEF: derived class for initilization from Les Houches Event File.
// LHAevntLHEF: derived class for events from Les Houches Evewnt File.
// Code for interfacing with Fortran commonblocks is found in LHAFortran.h.

#ifndef Pythia8_LesHouches_H
#define Pythia8_LesHouches_H

#include "PythiaStdlib.h"

namespace Pythia8 {

//**************************************************************************

// LHAinit is base class for initialization information 
// from an external parton-level generator.

class LHAinit {

public:

  // Destructor.
  virtual ~LHAinit() {}

  // A pure virtual method set, wherein all initialization information 
  // is supposed to be set in the derived class. Can do this by reading a 
  // file or some other way, as desired. Returns false if it did not work. 
  virtual bool set() = 0; 

  // Give back info on beams.
  int    idBeamA()       const {return idBeamASave;}
  int    idBeamB()       const {return idBeamBSave;}
  double eBeamA()        const {return eBeamASave;}
  double eBeamB()        const {return eBeamBSave;}
  int    pdfGroupBeamA() const {return pdfGroupBeamASave;}
  int    pdfGroupBeamB() const {return pdfGroupBeamBSave;}
  int    pdfSetBeamA()   const {return pdfSetBeamASave;}
  int    pdfSetBeamB()   const {return pdfSetBeamBSave;}
    
  // Give back weight strategy.
  int    strategy()      const {return strategySave;}

  // Give back info on processes.
  int    size()          const {return processes.size();} 
  int    idProcess(int proc) const {return processes[proc].idProc;} 
  double xSec(int proc)  const {return processes[proc].xSecProc;}    
  double xErr(int proc)  const {return processes[proc].xErrProc;}    
  double xMax(int proc)  const {return processes[proc].xMaxProc;} 
   
  // Print the info; useful to check that setting it worked.
  void   list(ostream& os = cout);  

protected:

  // Constructor. Sets default to be that events come with unit weight.
  LHAinit(int strategyIn = 3) : strategySave(strategyIn) 
    { processes.reserve(10);} 

  // Input beam info.
  void beamA(int idIn, double eIn, int pdfGroupIn = 0, int pdfSetIn = 0) 
    { idBeamASave = idIn; eBeamASave = eIn; pdfGroupBeamASave = pdfGroupIn;  
    pdfSetBeamASave = pdfSetIn;} 
  void beamB(int idIn, double eIn, int pdfGroupIn = 0, int pdfSetIn = 0) 
    { idBeamBSave = idIn; eBeamBSave = eIn; pdfGroupBeamBSave = pdfGroupIn;  
    pdfSetBeamBSave = pdfSetIn;} 

  // Input process weight strategy.
  void strategy(int strategyIn) {strategySave = strategyIn;} 

  // Input process info.
  void process(int idProcessIn, double xSecIn = 1., double xErrIn = 0., 
    double xMaxIn = 1.) { processes.push_back( Process( idProcessIn, 
    xSecIn, xErrIn, xMaxIn)); }

private:

  // Event weighting and mixing strategy.
  int strategySave;

  // Beam particle properties.
  int idBeamASave, idBeamBSave;
  double eBeamASave, eBeamBSave;
  int pdfGroupBeamASave, pdfGroupBeamBSave, pdfSetBeamASave, pdfSetBeamBSave;

  // A nested class for processes...
  class Process {
  public:
    Process() : idProc(0), xSecProc(0.), xErrProc(0.), xMaxProc(0.) { }
    Process(int idProcIn, double xSecIn, double xErrIn, double xMaxIn) :
      idProc(idProcIn), xSecProc(xSecIn), xErrProc(xErrIn), 
      xMaxProc(xMaxIn) { }
    int idProc;
    double xSecProc, xErrProc, xMaxProc;
  } ;

  // ...so that the process list can be kept as a vector.
  vector<Process> processes;
};

//**************************************************************************

// LHAevnt is base class for event information 
// from an external parton-level generator.

class LHAevnt {

public:

  // Destructor.
  virtual ~LHAevnt() {}

  // A pure virtual method set, wherein information on the next event
  // is supposed to be set in the derived class. 
  // Strategies +-1 and +-2: idProcIn is the process type, selected by PYTHIA.
  // Strategies +-3 and +-4: idProcIn is dummy; process choice is made locally. 
  // The method can find the next event by a runtime interface to another
  // program, or by reading a file, as desired. 
  // The method should return false if it did not work.
  virtual bool set(int idProcIn = 0) = 0; 

  // Give back process number, weight, scale, alpha_em, alpha_s.
  int    idProcess()       const {return idProc;} 
  double weight()          const {return weightProc;} 
  double scale()           const {return scaleProc;} 
  double alphaQED()        const {return alphaQEDProc;} 
  double alphaQCD()        const {return alphaQCDProc;} 

  // Give back info on separate particle.
  int    size()            const {return particles.size();}
  int    id(int part)      const {return particles[part].idPart;}
  int    status(int part)  const {return particles[part].statusPart;}
  int    mother1(int part) const {return particles[part].mother1Part;}
  int    mother2(int part) const {return particles[part].mother2Part;}
  int    col1(int part)    const {return particles[part].col1Part;}
  int    col2(int part)    const {return particles[part].col2Part;}
  double px(int part)      const {return particles[part].pxPart;}
  double py(int part)      const {return particles[part].pyPart;}
  double pz(int part)      const {return particles[part].pzPart;}
  double e(int part)       const {return particles[part].ePart;}
  double m(int part)       const {return particles[part].mPart;}
  double tau(int part)     const {return particles[part].tauPart;}
  double spin(int part)    const {return particles[part].spinPart;}

  // Optional: give back info on parton density values of event.
  bool   pdfIsSet()        const {return pdfIsSetSave;}
  int    id1()             const {return id1Save;}
  int    id2()             const {return id2Save;}
  double x1()              const {return x1Save;}
  double x2()              const {return x2Save;}
  double scalePDF()        const {return scalePDFSave;}
  double xpdf1()           const {return xpdf1Save;}
  double xpdf2()           const {return xpdf2Save;}

  // Print the info; useful to check that reading an event worked.
  void   list(ostream& os = cout);  

protected:

  // Constructor.
  LHAevnt() { particles.reserve(20); }
 
  // Input info on the selected process.
  void process(int idProcIn = 0, double weightIn = 1., double scaleIn = 0.,
    double alphaQEDIn = 0.0073, double alphaQCDIn = 0.12) 
    { idProc = idProcIn; weightProc = weightIn; scaleProc = scaleIn; 
    alphaQEDProc = alphaQEDIn; alphaQCDProc = alphaQCDIn; 
    // Clear particle list. Add empty zeroth particle for correct indices.
    particles.clear(); particle(0); pdfIsSetSave = false;}

  // Input particle info, one particle at the time.
  void particle(int idIn, int statusIn = 0, int mother1In = 0, 
    int mother2In = 0, int col1In = 0, int col2In = 0, double pxIn = 0., 
    double pyIn = 0., double pzIn = 0., double eIn = 0., double mIn = 0., 
    double tauIn = 0., double spinIn = 9.) { 
    particles.push_back( Particle( idIn, statusIn, mother1In, mother2In, 
    col1In, col2In, pxIn, pyIn, pzIn, eIn, mIn, tauIn, spinIn)); }

  // Optionally input info on parton density values of event.
  void pdf(int id1In, int id2In, double x1In, double x2In, 
    double scalePDFIn, double xpdf1In, double xpdf2In) 
    { id1Save = id1In; id2Save = id2In; x1Save = x1In; x2Save = x2In;
    scalePDFSave = scalePDFIn; xpdf1Save = xpdf1In; xpdf2Save = xpdf2In;
    pdfIsSetSave = true;}

private:

  // Store info on the selected process. 
  int idProc;
  double weightProc, scaleProc, alphaQEDProc, alphaQCDProc;

  // A nested class for particles...
  class Particle {
  public:
    Particle() : idPart(0), statusPart(0), mother1Part(0), mother2Part(0),
      col1Part(0), col2Part(0), pxPart(0.), pyPart(0.), pzPart(0.), 
      ePart(0.), mPart(0.), tauPart(0.), spinPart(9.) { }
    Particle(int idIn, int statusIn, int mother1In, int mother2In,
      int col1In, int col2In, double pxIn, double pyIn, double pzIn, 
      double eIn, double mIn, double tauIn, double spinIn) :
      idPart(idIn), statusPart(statusIn), mother1Part(mother1In), 
      mother2Part(mother2In), col1Part(col1In), col2Part(col2In), 
      pxPart(pxIn), pyPart(pyIn), pzPart(pzIn), ePart(eIn), mPart(mIn), 
      tauPart(tauIn), spinPart(spinIn) { }
    int idPart, statusPart, mother1Part, mother2Part, col1Part, col2Part;
    double pxPart, pyPart, pzPart, ePart, mPart, tauPart, spinPart;
  } ;

  // ...so that the particle list can be kept as a vector.
  vector<Particle> particles;

  // Optional info on parton density values of event.
  bool   pdfIsSetSave;
  int    id1Save, id2Save;
  double x1Save, x2Save, scalePDFSave, xpdf1Save, xpdf2Save;

};

//**************************************************************************

// A derived class with initialization information from Les Houches Event File.

class LHAinitLHEF : public LHAinit {

public:

  // Constructor.
  LHAinitLHEF(const char* fileIn) : is(fileIn) {}

  // Destructor.
  ~LHAinitLHEF() {}

  // Routine for doing the job of reading and setting initialization info.  
  bool set(); 

private:
 
  // File from which to read.
  ifstream is;

};

//**************************************************************************

// A derived class with event information from Les Houches Event File.

class LHAevntLHEF : public LHAevnt {

public:

  // Constructor.
  LHAevntLHEF(const char* fileIn) : is(fileIn) {}

  // Destructor.
  ~LHAevntLHEF() {}

  // Routine for doing the job of reading and setting info on next event.  
  bool set(int = 0); 

private:
 
  // File from which to read.
  ifstream is;

};

//**************************************************************************

} // end namespace Pythia8

#endif // Pythia8_LesHouches_H
