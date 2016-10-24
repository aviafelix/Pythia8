// File: main11.cc
// This is a simple test program linking to Pythia6 for hard processes. 
// Copyright C 2007 Torbjorn Sjostrand

#include "Pythia.h"
#include "LHAFortran.h"
#include "Pythia6Interface.h"

using namespace Pythia8; 

//**************************************************************************

// Implement initialization fillHepRup method for Pythia6 example.

bool LHAinitFortran::fillHepRup() { 

  // Set process to generate.
  // Example 1: QCD production; must set pTmin.  
  //Pythia6Interface::pygive("msel = 1"); 
  //Pythia6Interface::pygive("ckin(3) = 20.");
  // Example 2: t-tbar production.  
  //Pythia6Interface::pygive("msel = 6"); 
  // Example 3: Z0 production; must set mMin.
  Pythia6Interface::pygive("msel = 11"); 
  Pythia6Interface::pygive("ckin(1) = 50."); 

  // Speed up initialization: multiple interactions only in C++ code.
  Pythia6Interface::pygive("mstp(81)=0");
    
  // Initialize for 14 TeV pp collider.
  Pythia6Interface::pyinit("cms","p","p",14000.);   

  // Fill initialization information in HEPRUP.
  Pythia6Interface::pyupin();

  // Done.
  return true;

}

//**************************************************************************

// Implement event generation fillHepEup method for Pythia6 example.

bool LHAevntFortran::fillHepEup() { 

  // Generate and fill the next Pythia6 event in HEPEUP.
  Pythia6Interface::pyupev();

  // Done.
  return true;

}

//**************************************************************************

int main() {

  // Generator. Shorthand for the event and for settings.
  Pythia pythia;
  Event& event = pythia.event;
  Settings& settings = pythia.settings;

  // Set Pythia8 generation aspects.
  pythia.readString("Beams:primordialKThard = 2.");    
  pythia.readString("MultipleInteractions:bProfile = 3");    

  // Initialize to access Pythia6 generator by Les Houches interface.
  LHAinitFortran pythia6Init;
  LHAevntFortran pythia6Evnt;
  pythia.init(&pythia6Init, &pythia6Evnt);    

  // Set some generation values.
  int nEvent = 100;
  int nList = 1;
  int nAbort = 10;

  // List changed settings data.
  settings.listChanged();

  // Histograms.
  double eCM = 14000.;
  double epTol = 1e-7 * eCM;
  Hist epCons("deviation from energy-momentum conservation",100,0.,epTol);
  Hist nFinal("final particle multiplicity",100,-0.5,1599.5);
  Hist nChg("final charged multiplicity",100,-0.5,799.5);

  // Begin event loop.
  int iAbort = 0; 
  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {

    // Generate events. Quit if too many failures.
    if (!pythia.next()) {
      if (++iAbort < nAbort) continue;
      cout << " Event generation aborted prematurely, owing to error!\n"; 
      break;
    }
 
    // List first few events, both hard process and complete events.
    if (iEvent < nList) { 
      pythia.info.list();
      // This call to Pythia6 is superfluous, but shows it can be done.
      Pythia6Interface::pylist(1);
      pythia.process.list();
      event.list();
    }

    // Reset quantities to be summed over event.
    int nfin = 0;
    int nch = 0;
    Vec4 pSum = - (event[1].p() + event[2].p());

    // Loop over final particles in the event. 
    for (int i = 0; i < event.size(); ++i) 
    if (event[i].isFinal()) {
      ++nfin;
      if (event[i].isCharged()) ++nch;
      pSum += event[i].p();
    }

    // Fill summed quantities. 
    double epDev = abs(pSum.e()) + abs(pSum.px()) + abs(pSum.py())
      + abs(pSum.pz());
    epCons.fill(epDev);
    nFinal.fill(nfin);
    nChg.fill(nch);

  // End of event loop.
  }

  // Final statistics. Must do call to Pythia6 explicitly.
  pythia.statistics();
  Pythia6Interface::pystat(1);  

  // Histogram output.
  cout << epCons << nFinal<< nChg; 

  // Done.
  return 0;
}
