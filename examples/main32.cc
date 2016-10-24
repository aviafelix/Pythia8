// main32.cc is a part of the PYTHIA event generator.
// Copyright (C) 2009 Mikhail Kirsanov, Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program.
// It illustrates how a file with HepMC events can be generated by Pythia8.
// Input and output files are specified on the command line, e.g. like
// ./main32.exe main32.cmnd hepmcout32.dat > out
// The main program contains no analysis; this is intended to happen later.
// It therefore "never" has to be recompiled to handle different tasks.

#include "Pythia.h"

#include "HepMCInterface.h"

#include "HepMC/GenEvent.h"
#include "HepMC/IO_Ascii.h"
//#include "HepMC/IO_AsciiParticles.h"

using namespace Pythia8; 

int main(int argc, char* argv[]) {

  // Check that correct number of command-line arguments
  if (argc != 3) {
    cerr << " Unexpected number of command-line arguments. \n You are"
         << " expected to provide one input and one output file name. \n"
         << " Program stopped! " << endl;
    return 1;
  }

  // Check that the provided input name corresponds to an existing file.
  ifstream is(argv[1]);  
  if (!is) {
    cerr << " Command-line file " << argv[1] << " was not found. \n"
         << " Program stopped! " << endl;
    return 1;
  }

  // Confirm that external files will be used for input and output.
  cout << " PYTHIA settings will be read from file " << argv[1] << endl;
  cout << " HepMC events will be written to file " << argv[2] << endl;

  // Interface for conversion from Pythia8::Event to HepMC one. 
  HepMC::I_Pythia8 ToHepMC;
  //  ToHepMC.set_crash_on_problem();

  // Specify file where HepMC events will be stored.
  HepMC::IO_Ascii ascii_io(argv[2],std::ios::out);
  // HepMC::IO_AsciiParticles ascii_io(argv[2],std::ios::out);

  // Generator. 
  Pythia pythia;

  // Read in commands from external file.
  pythia.readFile(argv[1]);    

  // Extract settings to be used in the main program.
  int    nEvent    = pythia.mode("Main:numberOfEvents");
  int    nShow     = pythia.mode("Main:timesToShow");
  int    nAbort    = pythia.mode("Main:timesAllowErrors");
  bool   showCS    = pythia.flag("Main:showChangedSettings");
  bool   showAS    = pythia.flag("Main:showAllSettings");
  bool   showCPD   = pythia.flag("Main:showChangedParticleData");
  bool   showAPD   = pythia.flag("Main:showAllParticleData");
 
  // Initialization. Beam parameters set in .cmnd file.
  pythia.init();

  // List settings.
  if (showCS) pythia.settings.listChanged();
  if (showAS) pythia.settings.listAll();

  // List particle data.  
  if (showCPD) pythia.particleData.listChanged();
  if (showAPD) pythia.particleData.listAll();

  // Begin event loop.
  int nShowPace = max(1,nEvent/nShow); 
  int iAbort = 0; 
  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {
    if (iEvent%nShowPace == 0) cout << " Now begin event " 
      << iEvent << endl;

    // Generate event. Skip if erroneous. Quit if too many failures.   
    if (!pythia.next()) {
      if (++iAbort < nAbort) continue;
      cout << " Event generation aborted prematurely, owing to error!\n"; 
      break;
    }

    // Convert event record to HepMC format and output to file.
    HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
    ToHepMC.fill_next_event( pythia.event, hepmcevt );
    ascii_io << hepmcevt;
    delete hepmcevt;

  // End of event loop. Statistics. 
  }
  pythia.statistics();

  // Done.
  return 0;
}
