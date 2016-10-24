// SigmaHiggs.h is a part of the PYTHIA event generator.
// Copyright (C) 2007 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Header file for Higgs process differential cross sections.
// Contains classes derived from SigmaProcess via Sigma2Process.

#ifndef Pythia8_SigmaHiggs_H
#define Pythia8_SigmaHiggs_H

#include "SigmaProcess.h"

namespace Pythia8 {
 
//**************************************************************************

// A derived class for f fbar -> H0 (Standard Model Higgs).

class Sigma1ffbar2H : public Sigma1Process {

public:

  // Constructor.
  Sigma1ffbar2H() {}

  // Initialize process. 
  virtual void initProc(); 

  // Calculate flavour-independent parts of cross section.
  virtual void sigmaKin();

  // Evaluate sigmaHat(sHat). 
  virtual double sigmaHat();

  // Select flavour, colour and anticolour.
  virtual void setIdColAcol();

  // Evaluate weight for decay angles.
  virtual double weightDecay( Event& process, int iResBeg, int iResEnd); 

  // Info on the subprocess.
  virtual string name()       const {return "f fbar -> H0 (SM)";}
  virtual int    code()       const {return 901;}
  virtual string inFlux()     const {return "ffbarSame";}
  virtual int    resonanceA() const {return 25;}

private:

  // A H0 resonance object provides coupling and propagator expressions.
  ResonanceSMH HRes;
  double mRes, GammaRes, m2Res, GamMRat, sigBW, widthOut;

};
 
//**************************************************************************

// A derived class for g g -> H0 (Standard Model Higgs).

class Sigma1gg2H : public Sigma1Process {

public:

  // Constructor.
  Sigma1gg2H() {}

  // Initialize process. 
  virtual void initProc(); 

  // Calculate flavour-independent parts of cross section.
  virtual void sigmaKin();

  // Evaluate sigmaHat(sHat). 
  virtual double sigmaHat() {return sigma;}

  // Select flavour, colour and anticolour.
  virtual void setIdColAcol();

  // Evaluate weight for decay angles.
  virtual double weightDecay( Event& process, int iResBeg, int iResEnd); 

  // Info on the subprocess.
  virtual string name()       const {return "g g -> H0 (SM)";}
  virtual int    code()       const {return 902;}
  virtual string inFlux()     const {return "gg";}
  virtual int    resonanceA() const {return 25;}

private:

  // A H0 resonance object provides coupling and propagator expressions.
  ResonanceSMH HRes;
  double mRes, GammaRes, m2Res, GamMRat, sigma;

};
 
//**************************************************************************

// A derived class for gamma gamma -> H0 (Standard Model Higgs).

class Sigma1gmgm2H : public Sigma1Process {

public:

  // Constructor.
  Sigma1gmgm2H() {}

  // Initialize process. 
  virtual void initProc(); 

  // Calculate flavour-independent parts of cross section.
  virtual void sigmaKin();

  // Evaluate sigmaHat(sHat). 
  virtual double sigmaHat() {return sigma;}

  // Select flavour, colour and anticolour.
  virtual void setIdColAcol();

  // Evaluate weight for decay angles.
  virtual double weightDecay( Event& process, int iResBeg, int iResEnd); 

  // Info on the subprocess.
  virtual string name()       const {return "gamma gamma -> H0 (SM)";}
  virtual int    code()       const {return 903;}
  virtual string inFlux()     const {return "gmgm";}
  virtual int    resonanceA() const {return 25;}

private:

  // A H0 resonance object provides coupling and propagator expressions.
  ResonanceSMH HRes;
  double mRes, GammaRes, m2Res, GamMRat, sigma;

};
 
//**************************************************************************

// A derived class for f fbar -> H0 Z0 (Standard Model Higgs).

class Sigma2ffbar2HZ : public Sigma2Process {

public:

  // Constructor.
  Sigma2ffbar2HZ() {}

  // Initialize process. 
  virtual void initProc(); 

  // Calculate flavour-independent parts of cross section.
  virtual void sigmaKin();

  // Evaluate d(sigmaHat)/d(tHat). 
  virtual double sigmaHat();

  // Select flavour, colour and anticolour.
  virtual void setIdColAcol();

  // Evaluate weight for decay angles.
  virtual double weightDecay( Event& process, int iResBeg, int iResEnd); 

  // Info on the subprocess.
  virtual string name()       const {return "f fbar -> H0 Z0 (SM)";}
  virtual int    code()       const {return 904;}
  virtual string inFlux()     const {return "ffbarSame";}
  virtual bool   isSChannel() const {return true;}
  virtual int    id3Mass()    const {return 25;}
  virtual int    id4Mass()    const {return 23;}
  virtual int    resonanceA() const {return 23;}
  virtual int    gmZmode()    const {return 2;}

private:

  // Store Z0 mass and width.
  double mZ, widZ, mZS, mwZS, thetaWRat, sigma0;

};
 
//**************************************************************************

// A derived class for f fbar -> H0 W+- (Standard Model Higgs).

class Sigma2ffbar2HW : public Sigma2Process {

public:

  // Constructor.
  Sigma2ffbar2HW() {}

  // Initialize process. 
  virtual void initProc(); 

  // Calculate flavour-independent parts of cross section.
  virtual void sigmaKin();

  // Evaluate d(sigmaHat)/d(tHat). 
  virtual double sigmaHat();

  // Select flavour, colour and anticolour.
  virtual void setIdColAcol();

  // Evaluate weight for decay angles.
  virtual double weightDecay( Event& process, int iResBeg, int iResEnd); 

  // Info on the subprocess.
  virtual string name()       const {return "f fbar -> H0 W+- (SM)";}
  virtual int    code()       const {return 905;}
  virtual string inFlux()     const {return "ffbarChg";}
  virtual bool   isSChannel() const {return true;}
  virtual int    id3Mass()    const {return 25;}
  virtual int    id4Mass()    const {return 24;}
  virtual int    resonanceA() const {return 24;}

private:

  // Store W+- mass and width, and couplings.
  double mW, widW, mWS, mwWS, thetaWRat, sigma0;

};
 
//**************************************************************************

// A derived class for f f' -> H0 f f' (Z0 Z0 fusion of SM Higgs).

class Sigma3ff2HfftZZ : public Sigma3Process {

public:

  // Constructor.
  Sigma3ff2HfftZZ() {}

  // Initialize process. 
  virtual void initProc(); 

  // Calculate flavour-independent parts of cross section.
  virtual void sigmaKin();

  // Evaluate sigmaHat(sHat). 
  virtual double sigmaHat();

  // Select flavour, colour and anticolour.
  virtual void setIdColAcol();

  // Evaluate weight for decay angles.
  virtual double weightDecay( Event& process, int iResBeg, int iResEnd); 

  // Info on the subprocess.
  virtual string name()    const {return "f f -> H0 f f (SM; Z0 Z0 fusion)";}
  virtual int    code()    const {return 906;}
  virtual string inFlux()  const {return "ff";}
  virtual int    id3Mass() const {return 25;}

  // Instructions for 3-body phase space with t-channel propagators.
  virtual int    idTchan1()        const {return 23;}
  virtual int    idTchan2()        const {return 23;}
  virtual double tChanFracPow1()   const {return 0.05;}
  virtual double tChanFracPow2()   const {return 0.9;}
  virtual bool   useMirrorWeight() const {return true;}

private:

  // Store standard factors.
  double mZS, prefac, sigma1, sigma2;

};
 
//**************************************************************************

// A derived class for f_1 f_2 -> H0 f_3 f_4 (W+ W- fusion of SM Higgs).

class Sigma3ff2HfftWW : public Sigma3Process {

public:

  // Constructor.
  Sigma3ff2HfftWW() {}

  // Initialize process. 
  virtual void initProc(); 

  // Calculate flavour-independent parts of cross section.
  virtual void sigmaKin();

  // Evaluate sigmaHat(sHat). 
  virtual double sigmaHat();

  // Select flavour, colour and anticolour.
  virtual void setIdColAcol();

  // Evaluate weight for decay angles.
  virtual double weightDecay( Event& process, int iResBeg, int iResEnd); 

  // Info on the subprocess.
  virtual string name()    const {return "f f -> H0 f f (SM; W+ W- fusion)";}
  virtual int    code()    const {return 907;}
  virtual string inFlux()  const {return "ff";}
  virtual int    id3Mass() const {return 25;}

  // Instructions for 3-body phase space with t-channel propagators.
  virtual int    idTchan1()        const {return 24;}
  virtual int    idTchan2()        const {return 24;}
  virtual double tChanFracPow1()   const {return 0.05;}
  virtual double tChanFracPow2()   const {return 0.9;}
  virtual bool   useMirrorWeight() const {return true;}

private:

  // Store standard prefactor.
  double mWS, prefac, sigma0;

};
 
//**************************************************************************

// A derived class for g g -> H0 Q Qbar (Q Qbar fusion of SM Higgs).

class Sigma3gg2HQQbar : public Sigma3Process {

public:

  // Constructor.
  Sigma3gg2HQQbar(int idIn, int codeIn, string nameIn) : idNew(idIn), 
    codeSave(codeIn), nameSave(nameIn) {}

  // Initialize process. 
  virtual void initProc(); 

  // Calculate flavour-independent parts of cross section.
  virtual void sigmaKin();

  // Evaluate sigmaHat(sHat). 
  virtual double sigmaHat() {return sigma;}

  // Select flavour, colour and anticolour.
  virtual void setIdColAcol();

  // Evaluate weight for decay angles.
  virtual double weightDecay( Event& process, int iResBeg, int iResEnd); 

  // Info on the subprocess.
  virtual string name()    const {return nameSave;}
  virtual int    code()    const {return codeSave;}
  virtual string inFlux()  const {return "gg";}
  virtual int    id3Mass() const {return 25;}
  virtual int    id4Mass() const {return idNew;}
  virtual int    id5Mass() const {return idNew;}

  // Instructions for 3-body phase space with t-channel propagators.
  virtual int    idTchan1()        const {return idNew;}
  virtual int    idTchan2()        const {return idNew;}
  virtual double tChanFracPow1()   const {return 0.4;}
  virtual double tChanFracPow2()   const {return 0.2;}
  virtual bool   useMirrorWeight() const {return false;}

private:

  // Store flavour-specific process information and standard prefactor.
  int    idNew, codeSave;
  string nameSave;
  double prefac, sigma;

};
 
//**************************************************************************

// A derived class for q qbar -> H0 Q Qbar (Q Qbar fusion of SM Higgs).

class Sigma3qqbar2HQQbar : public Sigma3Process {

public:

  // Constructor.
  Sigma3qqbar2HQQbar(int idIn, int codeIn, string nameIn) : idNew(idIn), 
    codeSave(codeIn), nameSave(nameIn) {}

  // Initialize process. 
  virtual void initProc(); 

  // Calculate flavour-independent parts of cross section.
  virtual void sigmaKin();

  // Evaluate sigmaHat(sHat). 
  virtual double sigmaHat() {return sigma;}

  // Select flavour, colour and anticolour.
  virtual void setIdColAcol();

  // Evaluate weight for decay angles.
  virtual double weightDecay( Event& process, int iResBeg, int iResEnd); 

  // Info on the subprocess.
  virtual string name()    const {return nameSave;}
  virtual int    code()    const {return codeSave;}
  virtual string inFlux()  const {return "qqbarSame";}
  virtual int    id3Mass() const {return 25;}
  virtual int    id4Mass() const {return idNew;}
  virtual int    id5Mass() const {return idNew;}

  // Instructions for 3-body phase space with t-channel propagators.
  virtual int    idTchan1()        const {return idNew;}
  virtual int    idTchan2()        const {return idNew;}
  virtual double tChanFracPow1()   const {return 0.4;}
  virtual double tChanFracPow2()   const {return 0.2;}
  virtual bool   useMirrorWeight() const {return false;}

private:

  // Store flavour-specific process information and standard prefactor.
  int    idNew, codeSave;
  string nameSave;
  double prefac, sigma;

};
 
//**************************************************************************

// A derived class for q g -> H0 q (Standard Model Higgs).

class Sigma2qg2Hq : public Sigma2Process {

public:

  // Constructor.
  Sigma2qg2Hq(int idIn, int codeIn, string nameIn) : idNew(idIn), 
    codeSave(codeIn), nameSave(nameIn) {}

  // Initialize process. 
  virtual void initProc(); 

  // Calculate flavour-independent parts of cross section.
  virtual void sigmaKin();

  // Evaluate sigmaHat(sHat). 
  virtual double sigmaHat();

  // Select flavour, colour and anticolour.
  virtual void setIdColAcol();

  // Evaluate weight for decay angles.
  virtual double weightDecay( Event& process, int iResBeg, int iResEnd); 

  // Info on the subprocess.
  virtual string name()    const {return nameSave;}
  virtual int    code()    const {return codeSave;}
  virtual string inFlux()  const {return "qg";}
  virtual int    id3Mass() const {return 25;}
  virtual int    id4Mass() const {return idNew;}

private:

  // Store flavour-specific process information and standard prefactor.
  int    idNew, codeSave;
  string nameSave;
  double m2Fix, m2W, thetaWRat, sigma;

};
 
//**************************************************************************

// A derived class for g g -> H0 g (Standard Model Higgs via heavy top loop).

class Sigma2gg2Hglt : public Sigma2Process {

public:

  // Constructor.
  Sigma2gg2Hglt() {}

  // Initialize process. 
  virtual void initProc(); 

  // Calculate flavour-independent parts of cross section.
  virtual void sigmaKin();

  // Evaluate d(sigmaHat)/d(tHat). 
  virtual double sigmaHat() {return sigma;}

  // Select flavour, colour and anticolour.
  virtual void setIdColAcol();

  // Evaluate weight for decay angles.
  virtual double weightDecay( Event& process, int iResBeg, int iResEnd); 

  // Info on the subprocess.
  virtual string name()    const {return "g g -> H0 g (SM; top loop)";}
  virtual int    code()    const {return 914;}
  virtual string inFlux()  const {return "gg";}
  virtual int    id3Mass() const {return 25;}

private:

  // Store standard prefactor.
  double widHgg, sigma;

};
 
//**************************************************************************

// A derived class for q g -> H0 q (Standard Model Higgs via heavy top loop).

class Sigma2qg2Hqlt : public Sigma2Process {

public:

  // Constructor.
  Sigma2qg2Hqlt() {}

  // Initialize process. 
  virtual void initProc(); 

  // Calculate flavour-independent parts of cross section.
  virtual void sigmaKin();

  // Evaluate d(sigmaHat)/d(tHat). 
  virtual double sigmaHat() {return sigma;}

  // Select flavour, colour and anticolour.
  virtual void setIdColAcol();

  // Evaluate weight for decay angles.
  virtual double weightDecay( Event& process, int iResBeg, int iResEnd); 

  // Info on the subprocess.
  virtual string name()    const {return "q g -> H0 q (SM; top loop)";}
  virtual int    code()    const {return 915;}
  virtual string inFlux()  const {return "qg";}
  virtual int    id3Mass() const {return 25;}

private:

  // Store standard prefactor.
  double widHgg, sigma;

};
 
//**************************************************************************

// A derived class for q qbar -> H0 g (Standard Model Higgs via heavy top loop).

class Sigma2qqbar2Hglt : public Sigma2Process {

public:

  // Constructor.
  Sigma2qqbar2Hglt() {}

  // Initialize process. 
  virtual void initProc(); 

  // Calculate flavour-independent parts of cross section.
  virtual void sigmaKin();

  // Evaluate d(sigmaHat)/d(tHat). 
  virtual double sigmaHat() {return sigma;}

  // Select flavour, colour and anticolour.
  virtual void setIdColAcol();

  // Evaluate weight for decay angles.
  virtual double weightDecay( Event& process, int iResBeg, int iResEnd); 

  // Info on the subprocess.
  virtual string name()    const {return "q qbar -> H0 g (SM; top loop)";}
  virtual int    code()    const {return 916;}
  virtual string inFlux()  const {return "qqbarSame";}
  virtual int    id3Mass() const {return 25;}

private:

  // Store standard prefactor.
  double widHgg, sigma;

};
  
//**************************************************************************

} // end namespace Pythia8

#endif // Pythia8_SigmaHiggs_H
