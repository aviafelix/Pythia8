// main13.cc is a part of the PYTHIA event generator.
// Copyright (C) 2007 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program. 
// It illustrates how Les Houches Event File input can be used in Pythia8.
// It uses two LHE files, ttbar.lhe and ttbar2.lhe, which are combined in
// the event generation by a pythia.init( fileName, true) call.
// In the first file top decays have been performed, in the second not,
// and are instead handled by the internal PYTHIA resonance-decay machinery. 
// Furthermore the internal top production processes are switched on and
// mixed in, giving a "double up" total top cross section.
// Much of this of course is not intended to be realistic, 
// but rather illustrates several tricks that can be useful.

#include "Pythia.h"
using namespace Pythia8; 
int main() {

  // Number of events to print: LHA and others.
  int nPrintLHA  = 1;             
  int nPrintRest = 1;             

  // Allow for possibility of a few faulty events.
  int nAbort     = 10;

  // Generator           
  Pythia pythia;                            

  // Switch on internal ttbar production.
  pythia.readString("Top:gg2ttbar = on");  
  pythia.readString("Top:qqbar2ttbar = on"); 

  // Use same top mass as in Pythia 6.4 to simplify comparison.
  pythia.readString("6:m0 = 175.");    

  // Initialize Les Houches Event File run. List initialization information.
  pythia.init("ttbar.lhe");   

  // List changed settings and particle data.
  pythia.settings.listChanged();   
  pythia.particleData.listChanged();   

  // Book histogram.
  Hist nCharged("charged particle multiplicity",100,-0.5,399.5); 

  // Set counters.
  int iPrintLHA  = 0;             
  int iPrintRest = 0;             
  int iAbort     = 0;
  int iFile      = 1;

  // Begin event loop   
  for (int iEvent = 0; ; ++iEvent) {

    // Generate until none left in input file.  
    if (!pythia.next()) {
      if (pythia.info.atEndOfFile()) {

        // First time open next file, second time stop event loop.
        if (iFile == 1) {
          pythia.init("ttbar2.lhe", true);   
          ++iFile;
          continue;
	} else break;          
      }

      // First few failures write off as "acceptable" errors, then quit.
      if (++iAbort < nAbort) continue;
      break;
    }
  
    // List first few Les Houches and other events.
    if (pythia.info.isLHA() && iPrintLHA < nPrintLHA) {     
      pythia.LHAevntList();               
      pythia.info.list();          
      pythia.process.list();          
      pythia.event.list();  
      ++iPrintLHA;         
    } else if (!pythia.info.isLHA() && iPrintRest < nPrintRest) {     
      pythia.info.list();          
      pythia.process.list();          
      pythia.event.list();           
      ++iPrintRest;         
    }                 

    // Sum up final charged multiplicity and fill in histogram.
    int nChg = 0;                 
    for (int i = 0; i < pythia.event.size(); ++i) 
    if (pythia.event[i].isFinal() && pythia.event[i].isCharged()) 
      ++nChg;
    nCharged.fill(nChg);               

  // End of event loop.        
  }                                           

  // Give statistics. Print histogram.
  pythia.statistics();
  cout << nCharged;  

  // Done.                           
  return 0;
}
