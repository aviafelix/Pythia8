// SusyLesHouches.h is a part of the PYTHIA event generator.
// Copyright (C) 2010 Peter Skands, Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Header file for SUSY Les Houches Accord Interface
// Is independent of the rest of the Pythia implementation and thus could
// be re-used stand-alone or merged into other applications, subject to 
// the MCnet Guidelines mentioned above.

#ifndef SLHA_H
#define SLHA_H

// Stdlib header files for string and character manipulation.
#include <string>
#include <cctype>
// Stdlib header files for containers.
#include <vector>
#include <map>
// Stdlib header files for input/output.
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
// Stdlib header files for mathematics.
#include <cmath>
#include <cstdlib>

// Stdlib namespace
using namespace std;

//==========================================================================

class SusyLesHouches {

public:

  //Constructor, with and without filename.
  SusyLesHouches(int verboseIn=1) : verbose(verboseIn), 
    headerPrinted(false), footerPrinted(false),
    slhaRead(false), lhefRead(false), lhefSlha(false), useDecay(true) {};
  SusyLesHouches(string filename, int verboseIn=1) : verbose(verboseIn), 
    headerPrinted(false), footerPrinted(false),
    slhaRead(true), lhefRead(false), lhefSlha(false), useDecay(true) 
    {readFile(filename);};

  //***************************** SLHA FILE I/O *****************************//
  // Read and write SLHA files 
  int readFile(string slhaFileIn="slha.spc",int verboseIn=1, bool useDecayIn=true); 
  //int writeFile(string filename): write SLHA file on filename

  //Output utilities
  void printHeader();   // print Header
  void printFooter();   // print Footer
  void printSpectrum(); // print Spectrum
  
  // Check spectrum and decays
  int checkSpectrum();
  int checkDecays();

  // File Name (can be either SLHA or LHEF)
  string slhaFile;

  // Class for SLHA data entry
  class Entry {
    
  public:
    //Constructor. 
    Entry() : isIntP(false), isDoubleP(false), 
      isStringP(false), n(0), d(0.0), s(""), commentP("") {}
    
    // Generic functions to inquire whether an int, double, or string
    bool isInt(){return isIntP;}
    bool isDouble(){return isDoubleP;}
    bool isString(){return isStringP;}

    // = Overloading: Set entry to int, double, or string
    Entry& operator=(double& val)  {
      d=val;isIntP=false;isDoubleP=true;isStringP=false;
      return *this;      
    };
    Entry& operator=(int& val)  {
      n=val;isIntP=true;isDoubleP=false;isStringP=false;
      return *this;
    };
    Entry& operator=(string& val)  {
      s=val;isIntP=false;isDoubleP=false;isStringP=true;
      return *this;
    };
    
    // Set and Get comment
    void setComment(string comment) {commentP=comment;}
    void getComment(string comment) {comment=commentP;}

    // Generic functions to get value
    bool get(int& val) {val=n; return isIntP;}
    bool get(double& val) {val=d; return isDoubleP;}
    bool get(string& val) {val=s; return isStringP;}

  private:
    bool isIntP, isDoubleP, isStringP;    
    int n;
    double d;
    string s;
    string commentP;
    
  };

  //***************************** SLHA CLASSES *****************************//

  //class Block: the generic SLHA block (see below for matrices)
  //Explicit typing required, e.g. block<double> minpar;
  template <class T> class Block {    

  public: 

    //Constructor. 
    Block<T>() : idnow(0) {} ;    

    //Does block exist?
    bool exists() { return int(entry.size()) == 0 ? false : true ; };
    //Clear block
    void clear() { entry.clear(); };    

    //set: set block entry values.
    //Possible return values from set:
    // 0: normal return. Entry did not previously exist and has been created.
    // 1: normal return. Entry did previously exist and has been overwritten.
    //-1: failure. 
    int set(int iIn,T valIn) { 
      int alreadyexisting=exists(iIn)?1:0;
      entry[iIn]=valIn; 
      return alreadyexisting;
    };
    // Read index and value from SLHA data line
    int set(istringstream& linestream) {
      linestream >> i >> val;
      return linestream ? set(i,val) : -1;
    };
    // With i already given, read value from remaining SLHA data line
    int set(int iIn,istringstream& linestream) {
      linestream >> val;
      return linestream ? set(iIn,val) : -1;
    };
    // Shorthand for entry[0]. Used e.g. for block ALPHA.
    void set(T valIn) { entry[0]=valIn; };

    // Does entry i already exist in this block?
    bool exists(int iIn) {return entry.find(iIn) != entry.end() 
      ? true : false;};

    // Indexing with (). Output only.
    T operator()(int iIn=0) {
      if (exists(iIn)) {return entry[iIn];} else {T dummy(0); return dummy;};
    };

    // Size of map
    int size() {return int(entry.size());};

    // First and next key code
    int first() { idnow = entry.begin()->first; return idnow; };
    int next() { 
      typename map<int,T>::iterator itnow;
      itnow = ++entry.find(idnow);
      if ( itnow == entry.end() ) itnow=entry.begin();
      return idnow = itnow->first;
    };

    // Simple print utility
    void print() {
      bool finished=false;
      int ibegin=first();
      i=ibegin;
      while (!finished) {
	cout << "  "<< i << " " << entry[i] <<endl;
	i=next();
	if (i == ibegin) finished=true;
      };       
    };

    // Special for DRbar running blocks.
    void setq(double qIn) { qDRbar=qIn; }
    double q() { return qDRbar; }
 
  protected: 
    map<int,T> entry;    

  private:
    int idnow;
    double qDRbar;
    //Auxiliary vars
    int i; 
    T val;
  };

  // Derived class for generic blocks containing vectors of strings.
  class GenericBlock : public Block<string> {    

  public:
    
    //Constructor. 
    GenericBlock() { } ;    

    // Read index and value from SLHA data line
    int set(string lineIn) {
      entry[entry.size()] = lineIn;
      return 0;
    };
    
  };

  // class MatrixBlock: the generic SLHA matrix 
  // Explicit sizing required, e.g. MatrixBlock<4> nmix;
  template <int size> class MatrixBlock {    
  public: 
    //Constructor. Set uninitialized and explicitly zero.
    MatrixBlock<size>() { 
      initialized=false; 
      for (i=1;i<=size;i++) {
	for (j=1;j<=size;j++) {
	  entry[i][j]=0.0;
	};
      };
    };    

    // Assignment
    MatrixBlock& operator=(const MatrixBlock& m) { 
      if (this != &m) { 
	for (i=0;i<size;i++) for (j=0;j<=size;j++) entry[i][j] = m(i,j);
	qDRbar = m.qDRbar; 
	initialized = m.initialized; 
      } 
      return *this; };

    // Does this matrix contain any entries?
    bool exists() { return initialized; };
    // Clear initialized flag
    void clear() { initialized=false; };

    // Set matrix entry
    int set(int iIn,int jIn, double valIn) { 
      if (iIn>0 && jIn>0 && iIn<=size && jIn<=size) {
	entry[iIn][jIn]=valIn;
	initialized=true;
	return 0;
      } else {
	return -1;
      };
    };

    // Set entry from linestream (used during file read)
    int set(istringstream& linestream) {
      linestream >> i >> j >> val;
      return linestream ? set(i,j,val) : -1;
    };

    // () Overloading: Get entry
    double operator()(int iIn, int jIn) const {
      return (iIn <= size && jIn <= size && iIn > 0 && jIn > 0) ? 
	entry[iIn][jIn] : 0.0;
    };

    // Set and get scale for DRbar running Blocks.
    void setq(double qIn) { qDRbar=qIn; }
    double q() { return qDRbar; }

    // Simple print utility, to be elaborated on.
    void print() {
      for (i=1;i<=size;i++) {
	cout << "   "<<i << " " ;
	for (j=1;j<=size;j++) cout << entry[i][j] << " ";
	cout << endl;
      };
    };

  private:
    bool initialized;
    double entry[size+1][size+1];
    double qDRbar;
    //Auxiliary vars
    int i,j; 
    double val;
  };

  // class tensorBlock: the generic SLHA tensor
  // Explicit sizing required, e.g. tensorBlock<3> rvlam;
  template <int size> class Tensor3Block {    
  public: 
    //Constructor. Set uninitialized and explicitly zero.
    Tensor3Block<size>() { 
      initialized=false; 
      for (i=1;i<=size;i++) {
	for (j=1;j<=size;j++) {
	  for (k=1;k<=size;k++) {
	    entry[i][j][k]=0.0;
	  };
	};
      };
    };    
    
    // Assignment
    Tensor3Block& operator=(const Tensor3Block& m) { 
      if (this != &m) { 
	for (i=0;i<size;i++) for (j=0;j<=size;j++) for (k=0;k<=size;k++) 
	  entry[i][j][k] = m(i,j,k);
	qDRbar = m.qDRbar; 
	initialized = m.initialized; 
      } 
      return *this; };
    
    // Does this matrix contain any entries?
    bool exists() { return initialized; };
    // Clear initialized flag
    void clear() { initialized=false; };
    
    // Set matrix entry
    int set(int iIn,int jIn, int kIn, double valIn) { 
      if (iIn>0 && jIn>0 && kIn>0 && iIn<=size && jIn<=size && kIn<=size) {
	entry[iIn][jIn][kIn]=valIn;
	initialized=true;
	return 0;
      } else {
	return -1;
      };
    };

    // Set entry from linestream (used during file read)
    int set(istringstream& linestream) {
      linestream >> i >> j >> k >> val;
      return linestream ? set(i,j,k,val) : -1;
    };

    // () Overloading: Get entry
    double operator()(int iIn, int jIn, int kIn) const {
      return (iIn <= size && jIn <= size && kIn <= size && iIn > 0 
	&& jIn > 0 && kIn > 0) ? entry[iIn][jIn][kIn] : 0.0;
    };

    // Set and get scale for DRbar running Blocks.
    void setq(double qIn) { qDRbar=qIn; }
    double q() { return qDRbar; }

    // Simple print utility, to be elaborated on.
    void print() {
      for (i=1;i<=size;i++) {	
	for (j=1;j<=size;j++) {
	  cout << "   "<<i << " "<<j << " " ;
	  for (k=1;k<=size;k++) {
	    cout << entry[i][j][k] << " ";	   
	    cout << endl; 
	  };
	};
      };
    };

  private:
    bool initialized;
    double entry[size+1][size+1][size+1];
    double qDRbar;
    //Auxiliary vars
    int i,j,k; 
    double val;
  };

  //*************************** DECAY TABLES ***************************//

  class decayChannel {
  public: 

    decayChannel() : brat(0.0) {};
    decayChannel(double bratIn, int nDaIn, vector<int> idDaIn, string cIn="") {
      setChannel(bratIn,nDaIn,idDaIn,cIn);
    }

    // Functions to set decay channel information
    void setChannel(double bratIn, int nDaIn, vector<int> idDaIn, string cIn="") {
      brat    = bratIn;
      for (int i=0; i<=nDaIn; i++) {
	if (i < int(idDaIn.size())) idDa.push_back(idDaIn[i]);
	comment = cIn;
      }
    }
    void setBrat(double bratIn) {brat=bratIn;}
    void setIdDa(vector<int> idDaIn) {idDa = idDaIn;}
    
    // Functions to get decay channel information
    double getBrat() {return brat;}
    int getNDa() {return int(idDa.size());}
    vector<int> getIdDa() {return idDa;}
    string getComment() {return comment;}
    
  private:
    double brat;
    vector<int> idDa;
    string comment;
      
  };

  class decayTable {        
  public: 
    
  decayTable() : id(0), width(0.0) {};
  decayTable(int idIn) : id(idIn), width(0.0) {};
  decayTable(int idIn, double widthIn) : id(idIn), width(widthIn) {};
    
    // Functions to get PDG code (id) and width
    int    getId() {return id;}
    double getWidth() {return width;} 
    
    // Functions to set PDG code (id) and width
    void setId(int idIn) {id = idIn;}
    void setWidth(double widthIn) {width=widthIn;}

    // Function to reset size and width (width -> 0 by default)
    void reset(double widthIn=0.0) {table.resize(0); width=widthIn;}
    
    // Function to add another decay channel
    void addChannel(decayChannel channelIn) {table.push_back(channelIn);}
    void addChannel(double bratIn, int nDaIn, vector<int> idDaIn, string cIn="") {
      decayChannel newChannel(bratIn, nDaIn, idDaIn, cIn);
      table.push_back(newChannel);
    }

    // Function to return number of decay channels
    int size() {return int(table.size());}

    // Function to return a branching ratio
    double getBrat(int iChannel) {
      if (iChannel >= 0 && iChannel < int(table.size())) {
	return table[iChannel].getBrat();
      } else {
	return 0.0;
      }
    }
    // Function to return daughter PDG codes 
    vector<int> getIdDa(int iChannel) {
      if (iChannel >= 0 && iChannel < int(table.size())) {
	return table[iChannel].getIdDa();
      } else {
	vector<int> dum;
	return dum;
      }
    }
    // Function to return a decay channel
    decayChannel getChannel(int iChannel) {
      if (iChannel >= 0 && iChannel < int(table.size())) {
	return table[iChannel];
      } else {
	decayChannel dum;
	return dum;
      }
    }
    
  private:
    int id;
    double width;
    vector<decayChannel> table;

  };

  //*************************** THE SLHA1 BLOCKS ***************************//
  //Blocks for model definition:
  Block<int> modsel;
  Block<int> modsel21;
  Block<double> modsel12;
  Block<double> minpar;
  Block<double> extpar;
  Block<double> sminputs;
  //Blocks for RGE program specific output
  Block<string> spinfo;
  Block<string> spinfo3;
  Block<string> spinfo4;
  //Blocks for DCY program specific output
  Block<string> dcinfo;
  Block<string> dcinfo3;
  Block<string> dcinfo4;
  //Blocks for mass and coupling spectrum
  Block<double> mass;
  MatrixBlock<4> nmix;
  MatrixBlock<2> umix;
  MatrixBlock<2> vmix;
  MatrixBlock<2> stopmix;
  MatrixBlock<2> sbotmix;
  MatrixBlock<2> staumix;
  Block<double> alpha;
  Block<double> hmix;
  Block<double> gauge;
  Block<double> msoft;
  MatrixBlock<3> au;
  MatrixBlock<3> ad;
  MatrixBlock<3> ae;
  MatrixBlock<3> yu;
  MatrixBlock<3> yd;
  MatrixBlock<3> ye;

  //************************ THE SLHA1 DECAY TABLES ************************//
  vector<decayTable> decays;
  map<int,int> decayIndices;

  //*************************** THE SLHA2 BLOCKS ***************************//
  //Additions to SLHA1
  Block<double> qextpar;  

  //FLV Input
  Block<double> vckmin;  // The input CKM Wolfenstein parms.
  Block<double> upmnsin; // The input PMNS PDG parms.
  MatrixBlock<3> msq2in; // The input upper off-diagonal msq2
  MatrixBlock<3> msu2in; // The input upper off-diagonal msu2
  MatrixBlock<3> msd2in; // The input upper off-diagonal msd2
  MatrixBlock<3> msl2in; // The input upper off-diagonal msl2
  MatrixBlock<3> mse2in; // The input upper off-diagonal mse2
  MatrixBlock<3> tuin;   // The input upper off-diagonal TU
  MatrixBlock<3> tdin;   // The input upper off-diagonal TD
  MatrixBlock<3> tein;   // The input upper off-diagonal TE
  //FLV Output
  MatrixBlock<3> vckm;    // The output DRbar running Re{VCKM} at Q
  MatrixBlock<3> upmns;   // The output DRbar running Re{UPMNS} at Q
  MatrixBlock<3> msq2;    // The output DRbar running msq2 at Q
  MatrixBlock<3> msu2;    // The output DRbar running msu2 at Q
  MatrixBlock<3> msd2;    // The output DRbar running msd2 at Q
  MatrixBlock<3> msl2;    // The output DRbar running msl2 at Q
  MatrixBlock<3> mse2;    // The output DRbar running mse2 at Q
  MatrixBlock<3> tu;      // The output DRbar running TU at Q
  MatrixBlock<3> td;      // The output DRbar running TD at Q
  MatrixBlock<3> te;      // The output DRbar running TE at Q
  MatrixBlock<6> usqmix;  // The Re{} up squark mixing matrix
  MatrixBlock<6> dsqmix;   // The Re{} down squark mixing matrix
  MatrixBlock<6> selmix;   // The Re{} selectron mixing matrix
  MatrixBlock<3> snumix;   // The Re{} sneutrino mixing matrix
  MatrixBlock<3> snsmix;   // The scalar sneutrino mixing matrix
  MatrixBlock<3> snamix;   // The pseudoscalar neutrino mixing matrix

  //RPV Input
  Tensor3Block<3> rvlamllein; // The input LNV lambda couplings
  Tensor3Block<3> rvlamlqdin; // The input LNV lambda' couplings
  Tensor3Block<3> rvlamuddin; // The input BNV lambda'' couplings
  Tensor3Block<3> rvtllein;   // The input LNV T couplings
  Tensor3Block<3> rvtlqdin;   // The input LNV T' couplings
  Tensor3Block<3> rvtuddin;   // The input BNV T'' couplings
  Block<double> rvkappain;    // The input LNV kappa couplings
  Block<double> rvdin;        // The input LNV D terms
  Block<double> rvm2lh1in;    // The input LNV m2LH1 couplings
  Block<double> rvsnvevin;    // The input LNV sneutrino vevs
  //RPV Output
  Tensor3Block<3> rvlamlle;   // The output LNV lambda couplings
  Tensor3Block<3> rvlamlqd;   // The output LNV lambda' couplings
  Tensor3Block<3> rvlamudd;   // The output BNV lambda'' couplings
  Tensor3Block<3> rvtlle;     // The output LNV T couplings
  Tensor3Block<3> rvtlqd;     // The output LNV T' couplings
  Tensor3Block<3> rvtudd;     // The output BNV T'' couplings
  Block<double> rvkappa;      // The output LNV kappa couplings
  Block<double> rvd;          // The output LNV D terms
  Block<double> rvm2lh1;      // The output LNV m2LH1 couplings
  Block<double> rvsnvev;      // The output LNV sneutrino vevs
  MatrixBlock<7> rvnmix;      // The RPV neutralino mixing matrix
  MatrixBlock<5> rvumix;      // The RPV chargino L mixing matrix
  MatrixBlock<5> rvvmix;      // The RPV chargino R mixing matrix
  MatrixBlock<5> rvhmix;      // The RPV neutral scalar mixing matrix
  MatrixBlock<5> rvamix;      // The RPV neutral pseudoscalar mixing matrix
  MatrixBlock<7> rvlmix;      // The RPV charged fermion mixing matrix

  //CPV Input
  Block<double> imminpar;
  Block<double> imextpar;
  //CPV Output
  MatrixBlock<4> cvhmix;   // The CPV Higgs mixing matrix
  MatrixBlock<4> imcvhmix; // Optional: imaginary components
  MatrixBlock<3> imau,imad,imae; // Im{} of AU, AD, AE
  Block<double> imhmix;
  Block<double> immsoft;

  //CPV + FLV Input
  MatrixBlock<3> immsq2in;  // The Im{} input upper off-diagonal msq2
  MatrixBlock<3> immsu2in;  // The Im{} input upper off-diagonal msu2
  MatrixBlock<3> immsd2in;  // The Im{} input upper off-diagonal msd2
  MatrixBlock<3> immsl2in;  // The Im{} input upper off-diagonal msl2
  MatrixBlock<3> immse2in;  // The Im{} input upper off-diagonal mse2
  MatrixBlock<3> imtuin,imtdin,imtein; //  The Im{} input upper off-diagonal T
  //CPV + FLV Output
  MatrixBlock<3> imvckm;  // The output DRbar running Im{VCKM} at Q
  MatrixBlock<3> imupmns; // The output DRbar running Im{UPMNS} at Q
  MatrixBlock<3> immsq2;  // The output DRbar running msq2 at Q
  MatrixBlock<3> immsu2;  // The output DRbar running msu2 at Q
  MatrixBlock<3> immsd2;  // The output DRbar running msd2 at Q
  MatrixBlock<3> immsl2;  // The output DRbar running msl2 at Q
  MatrixBlock<3> immse2;  // The output DRbar running mse2 at Q
  MatrixBlock<3> imtu,imtd,imte; // Im{} of TU, TD, TE
  MatrixBlock<6> imusqmix;// The Im{} up squark mixing matrix
  MatrixBlock<6> imdsqmix; // The Im{} down squark mixing matrix
  MatrixBlock<6> imselmix; // The Im{} selectron mixing matrix
  MatrixBlock<3> imsnumix; // The Im{} sneutrino mixing matrix
  MatrixBlock<4> imnmix;   // The Im{} neutralino mixing matrix
  MatrixBlock<4> imumix;   // The Im{} chargino L mixing matrix
  MatrixBlock<4> imvmix;   // The Im{} chargino R mixing matrix

  //NMSSM Input
  //    All input is in EXTPAR
  //NMSSM Output
  Block<double> nmssmrun;  // The Block of NMSSM running parameters
  MatrixBlock<3> nmhmix;   // The NMSSM scalar Higgs mixing
  MatrixBlock<3> nmamix;   // The NMSSM pseudoscalar Higgs mixing
  MatrixBlock<5> nmnmix;   // The NMSSM neutralino mixing
  MatrixBlock<5> imnmnmix; //   Im{} (for future use)

  //*************************** SET BLOCK VALUE ****************************//
  template <class T> int set(string,T);
  template <class T> int set(string,int,T);
  template <class T> int set(string,int,int,T);
  template <class T> int set(string,int,int,int,T);

  //********************* GENERIC/USER-DEFINED BLOCKS **********************//
  // bool getEntry(name, indices, value) 
  //      = true if Block and entry exists (value returned in value, typecast 
  //        by user in call)
  //      = false otherwise
  map<string, GenericBlock> genericBlocks;
  template <class T> bool getEntry(string, T&);
  template <class T> bool getEntry(string, int, T&);
  template <class T> bool getEntry(string, int, int, T&);
  template <class T> bool getEntry(string, vector<int>, T&);

  //***************************** SLHA PRIVATE *****************************//
private:
  //SLHA I/O
  void message(int, string,string ,int line=0);
  int verbose;
  bool headerPrinted, footerPrinted;
  bool slhaRead, lhefRead, lhefSlha, useDecay;

};

#endif


