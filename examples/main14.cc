// File: main14.cc
// This is a simple test program. 
// It illustrates how Les Houches Event File input can be used in Pythia8.
// It uses the ttsample.lhe input file, the latter only with 100 events.
// Copyright C 2007 Torbjorn Sjostrand

#include "Pythia.h"
using namespace Pythia8; 
int main() {

  // Number of events to print.
  int nPrint = 1;             

  // Generator           
  Pythia pythia;                            

  // Stick with default values, so do not bother with a separate file
  // for changes. However, do one change, to show readString in action.
  pythia.readString("PartonLevel:MI = off"); 

  // Initialize Les Houches Event File run. List initialization information.
  pythia.init("ttbar.lhe");      

  // Book histogram.
  Hist nCharged("charged particle multiplicity",100,-0.5,999.5); 
  Hist thetaRatio("ratio of two cos(theta) calculations",100,0.5,1.5); 

  // Allow for possibility of a few faulty events.
  int nAbort = 10;
  int iAbort = 0;

  // Begin event loop; generate until none left in input file.     
  for (int iEvent = 0; ; ++iEvent) {

    // First few failures write off as potentially error, then quit.
    // (But probably end of file reached already first time.)
    if (!pythia.next()) {
      if (++iAbort < nAbort) continue;
      break;
    }
  
    // List first few events: Les Houches, hard process and complete.
    if (iEvent < nPrint) {     
      pythia.LHAevntList();               
      pythia.info.list();          
      pythia.process.list();          
      pythia.event.list();           
    }                           

    // Sum up final charged multiplicity and fill in histogram.
    int nChg = 0;                 
    for (int i = 0; i < pythia.event.size(); ++i) 
    if (pythia.event[i].isFinal() && pythia.event[i].isCharged()) 
      ++nChg;
    nCharged.fill(nChg);               

    // Compare cos(theta) = (tHat - uHat) / (beta_34 * sHat);
    double cosThe1 = cos(pythia.info.thetaHat());
    double sH = pythia.info.sHat();    
    double m3S = pow2( pythia.info.m3Hat() ); 
    double m4S = pow2( pythia.info.m4Hat() ); 
    double sHcorr = sqrt( pow2(sH - m3S - m4S) - 4. * m3S * m4S);
    double cosThe2 = (pythia.info.tHat() - pythia.info.uHat()) / sHcorr;
    thetaRatio.fill(cosThe2 / cosThe1);

  // End of event loop.        
  }                                           

  // Give statistics. Print histogram.
  pythia.statistics();
  cout << nCharged << thetaRatio;  

  // Done.                           
  return 0;
}
