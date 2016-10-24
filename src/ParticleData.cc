// Function definitions (not found in the header) for the
// DecayChannel, ParticleDataEntry and ParticleDataTable classes.
// Copyright C 2007 Torbjorn Sjostrand

#include "ParticleData.h"

// Allow string and character manipulation.
#include <cctype>

namespace Pythia8 {

//**************************************************************************

// DecayChannel class.
// This class holds info on a single decay channel.

//*********

// Check whether id1 occurs anywhere in product list.

bool DecayChannel::contains(int id1) const {
  
  bool found1 = false;
  for (int i = 0; i < nProd; ++i) if (prod[i] == id1) found1 = true;
  return found1;

}

//*********

// Check whether id1 and id2 occur anywhere in product list.
// iF ID1 == ID2 then two copies of this particle must be present.

bool DecayChannel::contains(int id1, int id2) const {
  
  bool found1 = false;
  bool found2 = false;
  for (int i = 0; i < nProd; ++i) {
    if (!found1 && prod[i] == id1) {found1 = true; continue;}
    if (!found2 && prod[i] == id2) {found2 = true; continue;}
  }
  return found1 && found2;

}

//*********

// Check whether id1, id2 and id3 occur anywhere in product list.
// iF ID1 == ID2 then two copies of this particle must be present, etc.

bool DecayChannel::contains(int id1, int id2, int id3) const {
  
  bool found1 = false;
  bool found2 = false;
  bool found3 = false;
  for (int i = 0; i < nProd; ++i) {
    if (!found1 && prod[i] == id1) {found1 = true; continue;}
    if (!found2 && prod[i] == id2) {found2 = true; continue;}
    if (!found3 && prod[i] == id3) {found3 = true; continue;}
  }
  return found1 && found2 && found3;

}

//**************************************************************************

// DecayTable class.
// This class holds info on all decay channels of a particle. 

//*********

// Rescale all branching ratios to assure normalization to unity.

void DecayTable::rescaleBR(double newSumBR) {

  // Sum up branching ratios. Find rescaling factor. Rescale.
  double oldSumBR = 0.;
  for ( int i = 0; i < size(); ++ i) 
    oldSumBR += channel[i].bRatio(); 
  double rescaleFactor = newSumBR / oldSumBR;
  for ( int i = 0; i < size(); ++ i) channel[i].rescaleBR(rescaleFactor); 

}

//*********

// Prepare to pick a decay channel.

bool DecayTable::preparePick(int idIn) {

  // Find sum of allowed branching ratios, in case it is not unity.
  idSave = idIn;
  sumBR = 0.;
  int onModeNow = 0;
  for ( int i = 0; i < size(); ++ i) {
    onModeNow = channel[i].onMode();
    if (onModeNow == 1) sumBR += channel[i].bRatio(); 
    if (onModeNow == 2 && idSave > 0) sumBR += channel[i].bRatio(); 
    if (onModeNow == 3 && idSave < 0) sumBR += channel[i].bRatio(); 
  }

  // Failure if no channels found with positive branching ratios.
  return (sumBR > 0.);

}

//*********

// Pick a decay channel according to branching ratios.

DecayChannel& DecayTable::pickChannel() {

  // Find channel in table.
  double randBR = sumBR * Rndm::flat();
  int onModeNow = 0;
  int i = 0;
  do {
    onModeNow = channel[i].onMode();
    if (onModeNow == 1) randBR -= channel[i].bRatio(); 
    if (onModeNow == 2 && idSave > 0) randBR -= channel[i].bRatio(); 
    if (onModeNow == 3 && idSave < 0) randBR -= channel[i].bRatio(); 
    ++i;
  } while (randBR > 0.);
  return channel[i - 1];

}

//*********

// Pick a decay channel according to dynamically calculated branching ratios.

DecayChannel& DecayTable::dynamicPick(int idIn) {

  // Find sum of branching ratios, in case it is not unity.
  int onModeNow = 0;
  double sumBR = 0.;
  for ( int i = 0; i < size(); ++ i) {
    onModeNow = channel[i].onMode();
    if (onModeNow == 1) sumBR += channel[i].dynamicBR(); 
    if (onModeNow == 2 && idIn > 0) sumBR += channel[i].dynamicBR(); 
    if (onModeNow == 3 && idIn < 0) sumBR += channel[i].dynamicBR(); 
  }

  // If vanishing then default back to normal branching ratios. (??)
  if (sumBR <= 0.) {
    preparePick(idIn);
    return pickChannel();
  }

  // Find channel in table, assuming normalization to unity.
  double rand = Rndm::flat() * sumBR;
  int i = 0;
  do {
    onModeNow = channel[i].onMode();
    if (onModeNow == 1) rand -= channel[i].dynamicBR(); 
    if (onModeNow == 2 && idIn > 0) rand -= channel[i].dynamicBR(); 
    if (onModeNow == 3 && idIn < 0) rand -= channel[i].dynamicBR(); 
    ++i;
  } while (rand > 0.);
  return channel[i - 1];

}

//**************************************************************************

// ParticleDataEntry class.
// This class holds info on a single particle species.

//*********
 
// Definitions of static variables.
// (Values will be overwritten in initStatic call, so are purely dummy.)

int    ParticleDataEntry::modeBreitWigner = 1;
double ParticleDataEntry::maxEnhanceBW    = 2.5;

// Constants: could be changed here if desired, but normally should not.
// These are of technical nature, as described for each.

// Particles with a read-in tau0 (in mm/c) below this mayDecay by default.
const double ParticleDataEntry::MAXTAU0FORDECAY = 1000.;

// Particles with a read-in m0 above this isResonance by default.
const double ParticleDataEntry::MINMASSRESONANCE = 20.;

// Narrow states are assigned nominal mass.
const double ParticleDataEntry::NARROWMASS       = 1e-6;

//*********

// Initialize static data members.

void ParticleDataEntry::initStatic() {

  // Mass generation: fixed mass or linear/quadratic Breit-Wigner.
  modeBreitWigner = Settings::mode("ParticleData:modeBreitWigner");

  // Maximum tail enhancement when adding threshold factor to Breit-Wigner.
  maxEnhanceBW    = Settings::parm("ParticleData:maxEnhanceBW");

}

//*********

// Set initial default values for some quantities. 

void ParticleDataEntry::setDefaults() {

  // A particle is a resonance if it is heavy enough.
  isResonanceSave = (m0Save > MINMASSRESONANCE);

  // A particle may decay if it is shortlived enough.
  mayDecaySave = (tau0Save < MAXTAU0FORDECAY); 

  // A particle by default has no external decays.
  externalDecaySave = false;

  // A particle is invisible if it has neither strong nor electric charge,
  // and is not made up of constituents that have it. Only relevant for
  // long-lived particles. This list may need to be extended.
  isVisibleSave = true;
  const int invisibleTable[29] = { 12, 14, 16, 18, 23, 25, 32, 33, 35, 
    36, 39, 41, 1000012, 1000014, 1000016, 1000018, 1000022, 1000023, 
    1000025, 1000035, 1000039, 2000012, 2000014, 2000016, 2000018,
    9900012, 9900014, 9900016, 9900023};     
  for (int i = 0; i < 29; ++i) 
    if (idSave == invisibleTable[i]) isVisibleSave = false;   

  // Set up constituent masses.
  constituentMassCalc();

  // No Breit-Wigner mass selection before initialized.
  modeBWnow = 0;

}

//*********

// Constituent masses for (d, u, s, c, b) quarks and diquarks.
// Hardcoded here so that they are not overwritten by mistake,
// and separated from the "normal" masses. 
  
void ParticleDataEntry::constituentMassCalc() {

  // Equate with the normal masses as default guess.
  constituentMassSave = m0Save;

  // The constituent masses.
  const double constituentMassTable[6] 
    = {0., 0.325, 0.325, 0.50, 1.60, 5.00};

  // Quark masses trivial.
  if (idSave < 6) constituentMassSave = constituentMassTable[idSave];
 
  // Diquarks as simple sum of constituent quarks.  
  if (idSave > 1000 && idSave < 10000 && (idSave/10)%10 == 0) {
    int id1 = idSave/1000;
    int id2 = (idSave/100)%10;
    if (id1 <6 && id2 < 6) constituentMassSave = constituentMassTable[id1] 
      + constituentMassTable[id2];
  }

}

//*********

// Prepare the Breit-Wigner mass selection by precalculating 
// frequently-used expressions.

void ParticleDataEntry::initBWmass() {

  // Find Breit-Wigner mode for current particle.
  modeBWnow = modeBreitWigner;
  if ( mWidthSave < NARROWMASS || (mMaxSave > mMinSave      
    && mMaxSave - mMinSave < NARROWMASS) ) modeBWnow = 0;
  if (modeBWnow == 0) return;

  // Find atan expressions to be used in random mass selection.
  if (modeBWnow < 3) { 
    atanLow = atan( 2. * (mMinSave - m0Save) / mWidthSave );
    double atanHigh = (mMaxSave > mMinSave) 
      ? atan( 2. * (mMaxSave - m0Save) / mWidthSave ) : 0.5 * M_PI;
    atanDif = atanHigh - atanLow;
  } else {
    atanLow = atan( (pow2(mMinSave) - pow2(m0Save))
      / (m0Save * mWidthSave) );
    double atanHigh = (mMaxSave > mMinSave)
      ? atan( (pow2(mMaxSave) - pow2(m0Save)) / (m0Save * mWidthSave) )
      : 0.5 * M_PI;
    atanDif = atanHigh - atanLow;
  }

  // Done if no threshold factor.
  if (modeBWnow%2 == 1) return;

  // Find average mass threshold for threshold-factor correction.
  double bRatSum = 0.;
  double mThrSum = 0;
  for (int i = 0; i < decay.size(); ++i) 
  if (decay[i].onMode() >= 0) {
    bRatSum += decay[i].bRatio();
    double mChannelSum = 0.;
    for (int j = 0; j < decay[i].multiplicity(); ++j)
      mChannelSum += ParticleDataTable::m0( decay[i].product(j) );
    mThrSum += decay[i].bRatio() * mChannelSum;
  }
  mThr = mThrSum / bRatSum;

  // Switch off Breit-Wigner if very close to threshold.
  if (mThr + NARROWMASS > m0Save) {
    ostringstream osWarn;
    osWarn << "for id = " << idSave;
    ErrorMsg::message("Warning in ParticleDataEntry::initBWmass:"
    " switching off width", osWarn.str());
    modeBWnow = 0;
  }

}

//*********

// Function to give mass of a particle, either at the nominal value
// or picked according to a (linear or quadratic) Breit-Wigner. 

double ParticleDataEntry::mass() {

  // Nominal value.
  if (modeBWnow == 0) return m0Save;
  double mNow, m2Now;

  // Mass according to a Breit-Wigner linear in m.
  if (modeBWnow == 1) {
     mNow = m0Save + 0.5 * mWidthSave 
       * tan( atanLow + atanDif * Rndm::flat() );

  // Ditto, but make Gamma proportional to sqrt(m^2 - m_threshold^2).
  } else if (modeBWnow == 2) {
    double mWidthNow, fixBW, runBW;
    double m0ThrS = m0Save*m0Save - mThr*mThr;
    do {
      mNow = m0Save + 0.5 * mWidthSave 
        * tan( atanLow + atanDif * Rndm::flat() );
      mWidthNow = mWidthSave * sqrtpos( (mNow*mNow - mThr*mThr) / m0ThrS ); 
      fixBW = mWidthSave / (pow2(mNow - m0Save) + pow2(0.5 * mWidthSave));
      runBW = mWidthNow / (pow2(mNow - m0Save) + pow2(0.5 * mWidthNow));  
    } while (runBW < Rndm::flat() * maxEnhanceBW * fixBW);
       
  // Mass according to a Breit-Wigner quadratic in m.
  } else if (modeBWnow == 3) {
    m2Now = m0Save*m0Save + m0Save * mWidthSave 
      * tan( atanLow + atanDif * Rndm::flat() );
    mNow = sqrtpos( m2Now);
       
  // Ditto, but m_0 Gamma_0 -> m Gamma(m) with threshold factor as above.
  } else {
    double mwNow, fixBW, runBW;
    double m2Ref = m0Save * m0Save; 
    double mwRef = m0Save * mWidthSave; 
    double m2Thr = mThr * mThr;
    do {
      m2Now = m2Ref + mwRef * tan( atanLow + atanDif * Rndm::flat() );
      mNow = sqrtpos( m2Now);
      mwNow = mNow * mWidthSave 
        * sqrtpos( (m2Now - m2Thr) / (m2Ref - m2Thr) );
      fixBW = mwRef / (pow2(m2Now - m2Ref) + pow2(mwRef));
      runBW = mwNow / (pow2(m2Now - m2Ref) + pow2(mwNow));
    } while (runBW < Rndm::flat() * maxEnhanceBW * fixBW);
  }

  // Done.
  return mNow;
}

//*********

// Find out if a particle is a hadron.
// Only covers normal hadrons, not e.g. R-hadrons.

bool ParticleDataEntry::isHadron() const {

  if (idSave <= 100 || (idSave >= 1000000 && idSave <= 9000000)
    || idSave >= 9900000) return false;
  if (idSave == 130 || idSave == 310) return true;  
  if (idSave%10 == 0 || (idSave/10)%10 == 0 || (idSave/100)%10 == 0) 
    return false;
  return true;

}

//*********

// Find out if a particle is a meson.
// Only covers normal hadrons, not e.g. R-hadrons.

bool ParticleDataEntry::isMeson() const {

  if (idSave <= 100 || (idSave >= 1000000 && idSave <= 9000000)
    || idSave >= 9900000) return false;
  if (idSave == 130 || idSave == 310) return true;  
  if (idSave%10 == 0 || (idSave/10)%10 == 0 || (idSave/100)%10 == 0 
    || (idSave/1000)%10 != 0) return false;
  return true;

}

//*********

// Find out if a particle is a baryon.
// Only covers normal hadrons, not e.g. R-hadrons.

bool ParticleDataEntry::isBaryon() const {

  if (idSave <= 1000 || (idSave >= 1000000 && idSave <= 9000000)
    || idSave >= 9900000) return false;
  if (idSave%10 == 0 || (idSave/10)%10 == 0 || (idSave/100)%10 == 0 
    || (idSave/1000)%10 == 0) return false;
  return true;
  

}

//*********

// Extract the heaviest (= largest id)  quark in a hadron.

int ParticleDataEntry::heaviestQuark(int idIn) const {

  if (!isHadron()) return 0;
  int hQ = 0;
  
  // Meson.
  if ( (idSave/1000)%10 == 0 ) {
    hQ = (idSave/100)%10;
    if (idSave == 130) hQ = 3;
    if (hQ%2 == 1) hQ = -hQ;

  // Baryon.
  } else hQ = (idSave/1000)%10;

  // Done.
  return (idIn > 0) ? hQ : -hQ;

}

//*********

// Calculate three times baryon number, i.e. net quark - antiquark number.

int ParticleDataEntry::baryonNumberType(int idIn) const {

  // Quarks.
  if (isQuark()) return (idIn > 0) ? 1 : -1; 

  // Diquarks
  if (isDiquark()) return (idIn > 0) ? 2 : -2;  
  
  // Baryons.
  if (isBaryon()) return (idIn > 0) ? 3 : -3; 

  // Done.
  return 0;

}

//*********

// Convert string to lowercase for case-insensitive comparisons.

string ParticleDataEntry::toLower(const string& name) { 

  string temp(name);
  for (int i = 0; i < int(temp.length()); ++i) 
    temp[i] = std::tolower(temp[i]); 
  return temp; 

}

//**************************************************************************

// ParticleDataTable class.
// This class holds a map of all ParticleDataEntries,
// each entry containing info on a particle species.

//*********

// Definitions of static variables. 

map<int, ParticleDataEntry> ParticleDataTable::pdt;
bool ParticleDataTable::isInit = false;
ParticleDataEntry* ParticleDataTable::particlePtr = 0;

//*********

// Read in database from specific XML file (which may refer to others).

bool ParticleDataTable::readXML(string inFile, bool reset) {

  // Normally reset whole database before beginning.
  if (reset) {pdt.clear(); isInit = false;}
 
  // List of files to be checked.
  vector<string> files;
  files.push_back(inFile);

  // Loop over files. Open them for read.
  for (int i = 0; i < int(files.size()); ++i) {
    const char* cstring = files[i].c_str();
    ifstream is(cstring);  

    // Check that instream is OK.
    if (!is) {
      ErrorMsg::message("Error in ParticleDataTable::readXML:"
        " did not find file", files[i]);
      return false;
    }

    // Read in one line at a time.
    particlePtr = 0;
    string line;
    while ( getline(is, line) ) {

      // Get first word of a line.
      istringstream getfirst(line);
      string word1;
      getfirst >> word1;
    
      // Check for occurence of a particle. Add any continuation lines.
      if (word1 == "<particle") {
        while (line.find(">") == string::npos) {   
          string addLine;
          getline(is, addLine);
          line += addLine;
        } 

        // Read in particle properties.
        int id = intAttributeValue( line, "id");
        string name = attributeValue( line, "name");
        string antiName = attributeValue( line, "antiName");
        if (antiName == "") antiName = "void";
        int spinType = intAttributeValue( line, "spinType");
        int chargeType = intAttributeValue( line, "chargeType");
        int colType = intAttributeValue( line, "colType");
        double m0 = doubleAttributeValue( line, "m0");
        double mWidth = doubleAttributeValue( line, "mWidth");
        double mMin = doubleAttributeValue( line, "mMin");
        double mMax = doubleAttributeValue( line, "mMax");
        double tau0 = doubleAttributeValue( line, "tau0");

        // Erase if particle already exists.
        if (isParticle(id)) pdt.erase(id);

        // Store new particle. Save pointer, to be used for decay channels.
        addParticle( id, name, antiName, spinType, chargeType, colType, 
          m0, mWidth, mMin, mMax, tau0);
        particlePtr = particleDataPtr(id);
 
      // Check for occurence of a decay channel. Add any continuation lines. 
      } else if (word1 == "<channel") {
        while (line.find(">") == string::npos) {   
          string addLine;
          getline(is, addLine);
          line += addLine;
        } 

        // Read in channel properties - products so far only as a string.
        int onMode = intAttributeValue( line, "onMode");
        double bRatio = doubleAttributeValue( line, "bRatio");
        int meMode = intAttributeValue( line, "meMode");
        string products = attributeValue( line, "products");
 
        // Read in decay products from stream. Must have at least one.
        istringstream prodStream(products);
        int prod0 = 0; int prod1 = 0; int prod2 = 0; int prod3 = 0;
        int prod4 = 0; int prod5 = 0; int prod6 = 0; int prod7 = 0;
        prodStream >> prod0 >> prod1 >> prod2 >> prod3 >> prod4 >> prod5 
                   >> prod6 >> prod7;   
        if (prod0 == 0) {
          ErrorMsg::message("Error in ParticleDataTable::readXML:"
            " incomplete decay channel", line);
          return false;
        }

        // Store new channel (if particle already known).
        if (particlePtr == 0) {
          ErrorMsg::message("Error in ParticleDataTable::readXML:"
            " orphan decay channel", line);
          return false;
        }
        particlePtr->decay.addChannel(onMode, bRatio, meMode, prod0, prod1, 
          prod2, prod3, prod4, prod5, prod6, prod7);  
    
      // Check for occurence of a file also to be read.
      } else if (word1 == "<file") {
        string file = attributeValue(line, "name");
        if (file == "") {
          ErrorMsg::message("Warning in ParticleDataTable::readXML:"
            " skip unrecognized file name", line);
        } else files.push_back(file);
      }

    // End of loop over lines in input file and loop over files.
    };
  };

  // All particle data at this stage defines baseline original.
  if (reset) for (map<int, ParticleDataEntry>::iterator pdtEntry 
    = pdt.begin(); pdtEntry != pdt.end(); ++pdtEntry) {
    ParticleDataEntry* particlePtr = &pdtEntry->second;
    particlePtr->setHasChanged(false);
  }

  // Done.
  isInit = true;
  return true;

}

//*********
 
// Print out complete database in numerical order as an XML file.

void ParticleDataTable::listXML(string outFile) {

  // Convert file name to ofstream.
    const char* cstring = outFile.c_str();
    ofstream os(cstring);  

  // Iterate through the particle data table.
  for (map<int, ParticleDataEntry>::const_iterator pdtEntry 
    = pdt.begin(); pdtEntry != pdt.end(); ++pdtEntry) {
    const ParticleDataEntry* particlePtr = &pdtEntry->second;

    // Print particle properties.
    os << "<particle id=\"" << particlePtr->id() << "\""
       << " name=\"" << particlePtr->name() << "\"";
    if (particlePtr->hasAnti()) 
      os << " antiName=\"" << particlePtr->name(-1) << "\"";
    os << " spinType=\"" << particlePtr->spinType() << "\"" 
       << " chargeType=\"" << particlePtr->chargeType() << "\""
       << " colType=\"" << particlePtr->colType() << "\"\n";
    // Pick format for mass and width based on mass value.
    double m0Now = particlePtr->m0();
    if (m0Now == 0 || (m0Now > 0.1 && m0Now < 1000.)) 
      os << fixed << setprecision(5);
    else  os << scientific << setprecision(3); 
    os << "          m0=\"" << m0Now << "\"";
    if (particlePtr->mWidth() > 0.) 
      os << " mWidth=\"" << particlePtr->mWidth() << "\""
         << " mMin=\"" << particlePtr->mMin() << "\""
         << " mMax=\"" << particlePtr->mMax() << "\"";
    if (particlePtr->tau0() > 0.) os << scientific << setprecision(5) 
         << " tau0=\"" << particlePtr->tau0() << "\"";
    os << ">\n";

    // Loop through the decay channel table for each particle.
    if (particlePtr->decay.size() > 0) {
      for (int i = 0; i < int(particlePtr->decay.size()); ++i) {
        const DecayChannel& channel = particlePtr->decay[i];
        int mult = channel.multiplicity();

        // Print decay channel properties.
        os << " <channel onMode=\"" << channel.onMode() << "\"" 
           << fixed << setprecision(7)
           << " bRatio=\"" << channel.bRatio() << "\"";
        if (channel.meMode() > 0) 
          os << " meMode=\"" << channel.meMode() << "\"";  
        os << " products=\"";
	for (int j = 0; j < mult; ++j) {
          os << channel.product(j);
          if (j < mult - 1) os << " ";
	}

        // Finish off line and loop over allowed decay channels.
        os  << "\"/>\n";
      }
    }
        
    // Finish off existing particle.
    os << "</particle>\n\n";   

  } 

}

//*********

// Read in database from specific free format file.

bool ParticleDataTable::readFF(string inFile, bool reset) {

  // Normally reset whole database before beginning.
  if (reset) {pdt.clear(); isInit = false;}

  // Open file for read and check that instream is OK. 
  const char* cstring = inFile.c_str();
  ifstream is(cstring);  
  if (!is) {
    ErrorMsg::message("Error in ParticleDataTable::readFF:"
      " did not find file", inFile);
    return false;
  }

  // Read in one line at a time.
  particlePtr = 0;
  string line;
  bool readParticle = false;
  while ( getline(is, line) ) {

    // Empty lines begins new particle. 
    if (line.find_first_not_of(" ") == string::npos) {
      readParticle = true;
      continue;
    } 
 
    // Prepare to use standard read from line.
    istringstream readLine(line);

    // Read in a line with particle information.
    if (readParticle) {

      // Properties to be read. 
      int id;
      string name, antiName;
      int spinType, chargeType, colType;
      double m0, mWidth, mMin, mMax, tau0;
      string may;

      // Do the reading.
      readLine >> id >> name >> antiName >> spinType >> chargeType 
        >> colType >> m0 >> mWidth >> mMin >> mMax >> tau0;          

      // Error printout if something went wrong.
      if (!readLine) {
        ErrorMsg::message("Error in ParticleDataTable::readFF:"
          " incomplete particle", line);
        return false;
      }

      // Erase if particle already exists.
      if (isParticle(id)) pdt.erase(id);

      // Store new particle. Save pointer, to be used for decay channels.
      addParticle( id, name, antiName, spinType, chargeType, colType, 
        m0, mWidth, mMin, mMax, tau0);
      particlePtr = particleDataPtr(id);
      readParticle = false;

    // Read in a line with decay channel information.
    } else {
       
      // Properties to be read. 
      int onMode = 0;
      double bRatio = 0.;
      int meMode = 0;
      int prod0 = 0; int prod1 = 0; int prod2 = 0; int prod3 = 0;
      int prod4 = 0; int prod5 = 0; int prod6 = 0; int prod7 = 0;
  
      // Read in data from stream. Need at least one decay product.
      readLine >> onMode >> bRatio >> meMode >> prod0;
      if (!readLine) {
        ErrorMsg::message("Error in ParticleDataTable::readFF:"
          " incomplete decay channel", line);
        return false;
      }
      readLine >> prod1 >> prod2 >> prod3 >> prod4 >> prod5 
        >> prod6  >> prod7;   

      // Store new channel.
      if (particlePtr == 0) {
        ErrorMsg::message("Error in ParticleDataTable::readFF:"
          " orphan decay channel", line);
        return false;
      }
      particlePtr->decay.addChannel(onMode, bRatio, meMode, prod0, prod1, 
        prod2, prod3, prod4, prod5, prod6, prod7); 

    }

  // End of while loop over lines in the file.
  }


  // Done.
  isInit = true;
  return true;
   
}  

//*********
  
// Print out complete database in numerical order as a free format file.

void ParticleDataTable::listFF(string outFile) {

  // Convert file name to ofstream.
    const char* cstring = outFile.c_str();
    ofstream os(cstring);  

  // Iterate through the particle data table. 
  for (map<int, ParticleDataEntry>::const_iterator pdtEntry 
    = pdt.begin(); pdtEntry != pdt.end(); ++pdtEntry) {
    const ParticleDataEntry* particlePtr = &pdtEntry->second;

    // Pick format for mass and width based on mass value.
    double m0Now = particlePtr->m0();
    if (m0Now == 0 || (m0Now > 0.1 && m0Now < 1000.)) 
      os << fixed << setprecision(5);
    else os << scientific << setprecision(3); 

    // Print particle properties.
    os << "\n" << setw(8) << particlePtr->id() << "  " 
       << left << setw(16) << particlePtr->name() << " " 
       << setw(16) << particlePtr->name(-1) << "  " 
       << right << setw(2) << particlePtr->spinType() << "  " 
       << setw(2) << particlePtr->chargeType() << "  " 
       << setw(2) << particlePtr->colType() << " " 
       << setw(10) << particlePtr->m0() << " " 
       << setw(10) << particlePtr->mWidth() << " " 
       << setw(10) << particlePtr->mMin() << " "
       << setw(10) << particlePtr->mMax() << " " 
       << scientific << setprecision(5) 
       << setw(12) << particlePtr->tau0() << "\n";

    // Loop through the decay channel table for each particle.
    if (particlePtr->decay.size() > 0) {
      for (int i = 0; i < int(particlePtr->decay.size()); ++i) {
        const DecayChannel& channel = particlePtr->decay[i];
        os << "               " << setw(6) << channel.onMode()
           << "  " << fixed << setprecision(7) << setw(10) 
           << channel.bRatio() << "  " 
           << setw(3) << channel.meMode() << " ";
        for (int j = 0; j < channel.multiplicity(); ++j) 
          os << setw(8) << channel.product(j) << " ";
        os << "\n";  
      }
    }  

  } 

}

//*********

// Read in updates from a character string, like a line of a file. 
// Is used by readString (and readFile) in Pythia.

bool ParticleDataTable::readString(string lineIn, bool warn,
  ostream& os) {

  // Take copy that will be modified.
  string line = lineIn;

  // If first character is not a digit then taken to be a comment.
  int firstChar = line.find_first_not_of(" ");
  if (!isdigit(line[firstChar])) return true; 

  // Replace colons and equal signs by blanks to make parsing simpler.
  for ( int j = 0; j < int(line.size()); ++ j) 
     if (line[j] == ':' || line[j] == '=') line[j] = ' ';

  // Get particle id and property name.
  int id;
  string property;
  istringstream getWord(line);
  getWord >> id >> property;
  property = toLower(property);
  
  // Check that valid particle.
  if ( (!isParticle(id) && property  != "all" && property  != "new") 
  || id <= 0) {
    if (warn) os << "\n Warning: input particle not found in Particle"
      << " Data Table; skip:\n   " << lineIn << "\n";
    return false;
  }

  // Identify particle property and read + set its value, case by case.
  if (property == "name") {
    string name;
    getWord >> name;    
    pdt[id].setName(name);
    return true; 
  }
  if (property == "antiname") {
    string antiName;
    getWord >> antiName;    
    pdt[id].setAntiName(antiName);
    return true; 
  }
  if (property == "names") {
    string name, antiName; 
    getWord >> name >> antiName;
    pdt[id].setNames(name, antiName); 
    return true; 
  }
  if (property == "spintype") {
    int spinType;
    getWord >> spinType;
    pdt[id].setSpinType(spinType); 
    return true; 
  }
  if (property == "chargetype") {
    int chargeType;
    getWord >> chargeType;
    pdt[id].setChargeType(chargeType); 
    return true; 
  }
  if (property == "coltype") {
    int colType;
    getWord >> colType;
    pdt[id].setColType(colType); 
    return true; 
  }
  if (property == "m0") {
    double m0;
    getWord >> m0;
    pdt[id].setM0(m0); 
    return true; 
  }
  if (property == "mwidth") {
    double mWidth;
    getWord >> mWidth;
    pdt[id].setMWidth(mWidth); 
    return true; 
  }
  if (property == "mmin") {
    double mMin;
    getWord >> mMin;
    pdt[id].setMMin(mMin); 
    return true; 
  }
  if (property == "mmax") {
    double mMax;
    getWord >> mMax;
    pdt[id].setMMax(mMax); 
    return true; 
  }
  if (property == "tau0") {
    double tau0;
    getWord >> tau0;
    pdt[id].setTau0(tau0); 
    return true; 
  }
  if (property == "isresonance") {
    string isres;
    getWord >> isres;
    bool isResonance = boolString(isres);
    pdt[id].setIsResonance(isResonance);
    return true; 
  }
  if (property == "maydecay") {
    string may;
    getWord >> may;
    bool mayDecay = boolString(may);
    pdt[id].setMayDecay(mayDecay);
    return true; 
  }  
  if (property == "externaldecay") {
    string extdec;
    getWord >> extdec;
    bool externalDecay = boolString(extdec);
    pdt[id].setExternalDecay(externalDecay);
    return true; 
  }
  if (property == "isvisible") {
    string isvis;
    getWord >> isvis;
    bool isVisible = boolString(isvis);
    pdt[id].setIsVisible(isVisible);
    return true; 
  }       
   
  // Addition or complete replacement of a particle.
  if (property == "all" || property == "new") {

    // Properties to be read. 
    string name, antiName;
    int spinType, chargeType, colType;
    double m0, mWidth, mMin, mMax, tau0;

    // Read in data from stream.
    getWord >> name >> antiName >> spinType >> chargeType >> colType 
            >> m0 >> mWidth >> mMin >> mMax >> tau0;   

    // Error printout if something went wrong.
    if (!getWord) {
      os << "Error: incomplete particle " << id << "\n";
      return false;
    }

    // To keep existing decay channels, only overwrite particle data.
    if (property == "all" && isParticle(id)) {
      setAll( id, name, antiName, spinType, chargeType, colType, 
        m0, mWidth, mMin, mMax, tau0);   

    // Else start over completely from scratch.
    } else {
      if (isParticle(id)) pdt.erase(id);
      addParticle( id, name, antiName, spinType, chargeType, colType, 
        m0, mWidth, mMin, mMax, tau0);   
    }
    return true;
  }

  // Set onMode of all decay channels in one go.
  if (property == "onmode") {
      int onMode = 0;
      string onModeIn;
      getWord >> onModeIn;
      // For onMode allow the optional possibility of Bool input.
      if (isdigit(onModeIn[0])) {
        istringstream getOnMode(onModeIn);
        getOnMode >> onMode;
      } else onMode = (boolString(onModeIn)) ? 1 : 0;
    for (int i = 0; i < pdt[id].decay.size(); ++i) 
      pdt[id].decay[i].onMode(onMode);
    return true; 
  } 

  // Selective search for matching decay channels: read in particles to match.
  if (property == "onifany" || property == "offifany"
    || property == "onifall" || property == "offifall") {
    int onMode = (property == "onifany" || property == "onifall") ? 1 : 0;
    vector<int> idToMatch;
    int idRead;
    for ( ; ; ) {
      getWord >> idRead;
      if (!getWord) break;
      idToMatch.push_back(abs(idRead));
    }   
    int nToMatch = idToMatch.size();
    for (int i = 0; i < pdt[id].decay.size(); ++i) {

      // Look for any match at all.
      if (property == "onifany" || property == "offifany") {      
        bool foundMatch = false;
        for (int j = 0; j < pdt[id].decay[i].multiplicity(); ++j) {        
          int idNow =  abs(pdt[id].decay[i].product(j));
          for (int k = 0; k < nToMatch; ++k) 
  	  if (idNow == idToMatch[k]) {foundMatch = true; break;}
          if (foundMatch) break;
        }
        if (foundMatch) pdt[id].decay[i].onMode(onMode); 

      // Look for match of all products provided.
      } else {
        vector<int> idUnmatched;
        for (int k = 0; k < nToMatch; ++k) 
          idUnmatched.push_back(idToMatch[k]);
        int nUnmatched = nToMatch;
        for (int j = 0; j < pdt[id].decay[i].multiplicity(); ++j) {        
          int idNow =  abs(pdt[id].decay[i].product(j));
          for (int k = 0; k < nUnmatched; ++k) 
	  if (idNow == idUnmatched[k]) {
            idUnmatched[k] = idUnmatched[--nUnmatched];
            break;
	  }
          if (nUnmatched == 0) break;
	}
        if (nUnmatched == 0) pdt[id].decay[i].onMode(onMode); 
      }
    }
    return true;
  }

  // Rescale all branching ratios by common factor.
  if (property == "rescalebr") {
    double factor;
    getWord >> factor;
    pdt[id].rescaleBR(factor); 
    return true; 
  }   

  // Reset decay table in preparation for new input.
  if (property == "onechannel") pdt[id].decay.clear();

  // Add or change a decay channel: get channel number and new property.
  if (property == "addchannel" || property == "onechannel" 
    || isdigit(property[0])) {
    int channel;
    if (property == "addchannel" || property == "onechannel") {
      pdt[id].decay.addChannel(); 
      channel = pdt[id].decay.size() - 1; 
      property = "all"; 
    } else{ 
      istringstream getChannel(property);
      getChannel >> channel;
      getWord >> property;
      property = toLower(property);
    }

    // Check that channel exists.
    if (channel < 0 || channel >= pdt[id].decay.size()) return false;   
  
    // Find decay channel property and value, case by case.
    // At same time also do case where all should be replaced.
    if (property == "onmode" || property == "all") {
      int onMode = 0;
      string onModeIn;
      getWord >> onModeIn;
      // For onMode allow the optional possibility of Bool input.
      if (isdigit(onModeIn[0])) {
        istringstream getOnMode(onModeIn);
        getOnMode >> onMode;
      } else onMode = (boolString(onModeIn)) ? 1 : 0;
      pdt[id].decay[channel].onMode(onMode); 
      if (property == "onmode") return true; 
    }
    if (property == "bratio" || property == "all") {
      double bRatio;
      getWord >> bRatio;
      pdt[id].decay[channel].bRatio(bRatio); 
      if (property == "bratio") return true; 
    }
    if (property == "memode" || property == "all") {
      int meMode;
      getWord >> meMode;
      pdt[id].decay[channel].meMode(meMode); 
      if (property == "memode") return true; 
    }    

    // Scan for products until end of line.  
    if (property == "products" || property == "all") {
      int nProd = 0;
      for (int iProd = 0; iProd < 8; ++iProd) {
        int idProd;
        getWord >> idProd;
        if (!getWord) break;
        pdt[id].decay[channel].product(iProd, idProd);
        ++nProd;
      }   
      for (int iProd = nProd; iProd < 8; ++iProd) 
        pdt[id].decay[channel].product(iProd, 0);
      pdt[id].decay[channel].multiplicity(nProd);
      return true; 
    }

    // Rescale an existing branching ratio.
    if (property == "rescalebr") {
      double factor;
      getWord >> factor;
      pdt[id].decay[channel].rescaleBR(factor); 
      return true; 
    }        
  }

  // Return false if failed to recognize property.
  if (warn) os << "\n Warning: input property not found in Particle"
    << " Data Table; skip:\n   " << lineIn << "\n";
  return false;

}

//*********
 
// Print out complete or changed table of database in numerical order.

void ParticleDataTable::list(bool changedOnly, ostream& os) {

  // Table header; output for bool as off/on.
  if (!changedOnly) {
    os << "\n --------  PYTHIA Particle Data Table (complete)  --------"
       << "------------------------------------------------------------"
       << "----------\n \n";

  } else { 
    os << "\n --------  PYTHIA Particle Data Table (changed only)  ----"
       << "------------------------------------------------------------" 
       << "----------\n \n";
  }
  os << "      id   name            antiName         spn chg col      m0"
     << "        mWidth      mMin       mMax       tau0    res dec ext "
     << "vis\n             no onMode   bRatio   meMode     products \n";

  // Iterate through the particle data table. Option to skip unchanged.
  int nList = 0;
  for (map<int, ParticleDataEntry>::const_iterator pdtEntry 
    = pdt.begin(); pdtEntry != pdt.end(); ++pdtEntry) {
    const ParticleDataEntry* particlePtr = &pdtEntry->second;
    if (changedOnly && !particlePtr->hasChanged()) continue;

    // Pick format for mass and width based on mass value.
    double m0Now = particlePtr->m0();
    if (m0Now == 0 || (m0Now > 0.1 && m0Now < 1000.)) 
      os << fixed << setprecision(5);
    else os << scientific << setprecision(3); 

    // Print particle properties.
    ++nList;
    string antiName = particlePtr->name(-1);
    if (antiName == "void") antiName = " ";
    os << "\n" << setw(8) << particlePtr->id() << "  " 
       << left << setw(16) << particlePtr->name() << " " 
       << setw(16) << antiName << "  " 
       << right << setw(2) << particlePtr->spinType() << "  " 
       << setw(2) << particlePtr->chargeType() << "  " 
       << setw(2) << particlePtr->colType() << " " 
       << setw(10) << particlePtr->m0() << " " 
       << setw(10) << particlePtr->mWidth() << " " 
       << setw(10) << particlePtr->mMin() << " " 
       << setw(10) << particlePtr->mMax() << " "
       << scientific << setprecision(5) 
       << setw(12) << particlePtr->tau0() << "  " << setw(2)
       << particlePtr->isResonance() << "  " << setw(2) 
       << (particlePtr->mayDecay() && particlePtr->canDecay()) 
       << "  " << setw(2) << particlePtr->externalDecay() << "  "
       << setw(2) << particlePtr->isVisible() << "\n";

    // Loop through the decay channel table for each particle.
    if (particlePtr->decay.size() > 0) {
      for (int i = 0; i < int(particlePtr->decay.size()); ++i) {
        const DecayChannel& channel = particlePtr->decay[i];
        os << "          "  << setprecision(7) 
           << setw(5) << i 
           << setw(6) << channel.onMode() 
           << fixed<< setw(12) << channel.bRatio() 
           << setw(5) << channel.meMode() << " ";
        for (int j = 0; j < channel.multiplicity(); ++j) 
          os << setw(8) << channel.product(j) << " ";
        os << "\n";  
      }
    }  

  } 

  // End of loop over database contents.
  if (changedOnly && nList == 0) os << "\n no particle data has been "
    << "changed from its default value \n";
  os << "\n --------  End PYTHIA Particle Data Table  -----------------"
     << "--------------------------------------------------------------"
     << "------\n" << endl;

}

//*********
 
// Print out partial table of database in input order.

void ParticleDataTable::list(vector<int> idList, ostream& os) {

  // Table header; output for bool as off/on.
  os << "\n --------  PYTHIA Particle Data Table (partial)  ---------"
     << "------------------------------------------------------------"
     << "----------\n \n";
  os << "      id   name            antiName         spn chg col      m0"
     << "        mWidth      mMin       mMax       tau0    res dec ext "
     << "vis\n             no onMode   bRatio   meMode     products \n";

  // Iterate through the given list of input particles.
  for (int i = 0; i < int(idList.size()); ++i) {
    const ParticleDataEntry* particlePtr = particleDataPtr(idList[i]);

    // Pick format for mass and width based on mass value.
    double m0Now = particlePtr->m0();
    if (m0Now == 0 || (m0Now > 0.1 && m0Now < 1000.)) 
      os << fixed << setprecision(5);
    else os << scientific << setprecision(3); 

    // Print particle properties.
    string antiName = particlePtr->name(-1);
    if (antiName == "void") antiName = " ";
    os << "\n" << setw(8) << particlePtr->id() << "  " 
       << left << setw(16) << particlePtr->name() << " " 
       << setw(16) << antiName << "  " 
       << right << setw(2) << particlePtr->spinType() << "  " 
       << setw(2) << particlePtr->chargeType() << "  " 
       << setw(2) << particlePtr->colType() << " " 
       << setw(10) << particlePtr->m0() << " " 
       << setw(10) << particlePtr->mWidth() << " " 
       << setw(10) << particlePtr->mMin() << " " 
       << setw(10) << particlePtr->mMax() << " "
       << scientific << setprecision(5) 
       << setw(12) << particlePtr->tau0() << "  " << setw(2)
       << particlePtr->isResonance() << "  " << setw(2) 
       << (particlePtr->mayDecay() && particlePtr->canDecay()) 
       << "  " << setw(2) << particlePtr->externalDecay() << "  "
       << setw(2) << particlePtr->isVisible() << "\n";

    // Loop through the decay channel table for each particle.
    if (particlePtr->decay.size() > 0) {
      for (int i = 0; i < int(particlePtr->decay.size()); ++i) {
        const DecayChannel& channel = particlePtr->decay[i];
        os << "          "  << setprecision(7) 
           << setw(5) << i 
           << setw(6) << channel.onMode() 
           << fixed<< setw(12) << channel.bRatio() 
           << setw(5) << channel.meMode() << " ";
        for (int j = 0; j < channel.multiplicity(); ++j) 
          os << setw(8) << channel.product(j) << " ";
        os << "\n";  
      }
    }  

  } 

  // End of loop over database contents.
  os << "\n --------  End PYTHIA Particle Data Table  -----------------"
     << "--------------------------------------------------------------"
     << "------\n" << endl;

}

//*********
 
// Check that table makes sense: e.g. consistent names and mass ranges, 
// that branching ratios sum to unity, that charge is conserved and 
// that phase space is open in each channel.
// verbosity = 0: mimimal amount of checks, e.g. that no channels open.
//           = 1: further warning if individual channels closed
//                (except for resonances). 
//           = 2:  also print branching-ratio-averaged threshold mass.
//      = 11, 12: as 1, 2, but include resonances in detailed checks.


void ParticleDataTable::checkTable(int verbosity, ostream& os) {

  // Header.
  os << "\n --------  PYTHIA Check of Particle Data Table  ------------"
     <<"------\n\n";
  int nErr = 0;

  // Loop through all particles.
  for (map<int, ParticleDataEntry>::const_iterator pdtEntry 
  = pdt.begin(); pdtEntry != pdt.end(); ++pdtEntry) {
    const ParticleDataEntry* particlePtr = &pdtEntry->second;
  
    // Extract some particle properties. Set some flags;
    int id = particlePtr->id();
    bool hasAnti = particlePtr->hasAnti();
    int spinTypeNow = particlePtr->spinType();
    int chargeTypeNow = particlePtr->chargeType();
    int baryonTypeNow = particlePtr->baryonNumberType();
    double m0Now = particlePtr->m0();
    double mMinNow = particlePtr->mMin();
    double mMaxNow = particlePtr->mMax();
    double mWidthNow = particlePtr->mWidth();
    double tau0Now = particlePtr->tau0();
    bool isResonanceNow = particlePtr->isResonance();
    bool hasPrinted = false;
    bool studyCloser = verbosity > 10 || !isResonanceNow;

    // Check that particle name consistent with charge information.
    string particleName = particlePtr->name(1);
    if (particleName.size() > 16) {
      os << " Warning: particle " << id << " has name " << particleName 
         << " of length " << particleName.size() << "\n"; 
      hasPrinted = true;
      ++nErr;
    }   
    int nPlus = 0;
    int nMinus = 0;
    for (int i = 0; i < int(particleName.size()); ++i) {
      if (particleName[i] == '+') ++nPlus;
      if (particleName[i] == '-') ++nMinus;
    }
    if ( (nPlus > 0 && nMinus > 0) || ( nPlus + nMinus > 0 
      && 3 * (nPlus - nMinus) != chargeTypeNow )) {
      os << " Warning: particle " << id << " has name " << particleName 
         << " inconsistent with charge type " << chargeTypeNow << "\n"; 
      hasPrinted = true;
      ++nErr;
    }  

    // Check that antiparticle name consistent with charge information.
    if (hasAnti) {
      particleName = particlePtr->name(-1);
      if (particleName.size() > 16) {
        os << " Warning: particle " << id << " has name " << particleName 
           << " of length " << particleName.size() << "\n"; 
        hasPrinted = true;
        ++nErr;
      }   
      nPlus = 0;
      nMinus = 0;
      for (int i = 0; i < int(particleName.size()); ++i) {
        if (particleName[i] == '+') ++nPlus;
        if (particleName[i] == '-') ++nMinus;
      }
      if ( (nPlus > 0 && nMinus > 0) || ( nPlus + nMinus > 0 
        && 3 * (nPlus - nMinus) != -chargeTypeNow )) {
        os << " Warning: particle " << -id << " has name " 
           << particleName << " inconsistent with charge type " 
           << -chargeTypeNow << "\n"; 
        hasPrinted = true;
        ++nErr;
      }  
    }

    // Check that mass, mass range and width are consistent.
    if (particlePtr->useBreitWigner()) {
      if (mMinNow > m0Now) {
        os << " Error: particle " << id << " has mMin " 
           << fixed << setprecision(5) << mMinNow 
           << " larger than m0 " << m0Now << "\n"; 
        hasPrinted = true;
        ++nErr;
      }  
      if (mMaxNow > mMinNow && mMaxNow < m0Now) {
        os << " Error: particle " << id << " has mMax " 
           << fixed << setprecision(5) << mMaxNow 
           << " smaller than m0 " << m0Now << "\n"; 
        hasPrinted = true;
        ++nErr;
      }  
      if (mMaxNow > mMinNow && mMaxNow - mMinNow < mWidthNow) {
        os << " Warning: particle " << id << " has mMax - mMin " 
           << fixed << setprecision(5) << mMaxNow - mMinNow
           << " smaller than mWidth " << mWidthNow << "\n"; 
        hasPrinted = true;
        ++nErr;
      }  
    }  

    // Check that particle does not both have width and lifetime.
    if (mWidthNow > 0. && tau0Now > 0.) {
      os << " Warning: particle " << id << " has both nonvanishing width " 
         << scientific << setprecision(5) << mWidthNow << " and lifetime " 
         << tau0Now << "\n"; 
      hasPrinted = true;
      ++nErr;  
    }

    // Begin study decay channels.
    if (particlePtr->decay.size() > 0) {  
 
      // Loop through all decay channels.
      double bRatioSum = 0.;
      double bRatioPos = 0.;
      double bRatioNeg = 0.;
      bool hasPosBR = false;
      bool hasNegBR = false;
      double threshMass = 0.;
      bool openChannel = false;
      for (int i = 0; i < particlePtr->decay.size(); ++i) {

        // Extract channel properties.
        int onMode = particlePtr->decay[i].onMode();
        double bRatio = particlePtr->decay[i].bRatio();
        int meMode = particlePtr->decay[i].meMode();
        int mult = particlePtr->decay[i].multiplicity();
        int prod[8];
        for (int j = 0; j < 8; ++j) 
          prod[j] = particlePtr->decay[i].productForce(j);

        // Sum branching ratios. Include off-channels.
        if (onMode == 0 || onMode == 1) bRatioSum += bRatio;
        else if (onMode == 2) {bRatioPos += bRatio; hasPosBR = true;}
        else if (onMode == 3) {bRatioNeg += bRatio; hasNegBR = true;}

        // Error printout when unknown decay product code.
        for (int j = 0; j < 8; ++j) {
          if ( prod[j] != 0 && !isParticle(prod[j]) ) {
            os << " Error: unknown decay product for " << id 
               << " -> " << prod[j] << "\n";
            hasPrinted = true;
            ++nErr;
            continue;
	  }
	}

        // Error printout when no decay products or 0 interspersed.
        int nLast = 0;
        for (int j = 0; j < 8; ++j)
          if (prod[j] != 0) nLast = j + 1;
        if (mult == 0 || mult != nLast) {
          os << " Error: corrupted decay product list for "
             <<  particlePtr->id() << " -> ";
          for (int j = 0; j < 8; ++j) os << prod[j] << " ";
          os << "\n";  
          hasPrinted = true;
          ++nErr;
          continue;
        }

        // Check charge conservation and open phase space in decay channel.
        int chargeTypeSum = -chargeTypeNow;
        int baryonTypeSum = -baryonTypeNow;
        double avgFinalMass = 0.;
        double minFinalMass = 0.;
        bool canHandle = true;
        for (int j = 0; j < mult; ++j) {
          chargeTypeSum += chargeType( prod[j] );
          baryonTypeSum += baryonNumberType( prod[j] );
          avgFinalMass += m0( prod[j] );
          minFinalMass += m0Min( prod[j] );
          if (prod[j] == 81 || prod[j] == 82 || prod[j] == 83) 
            canHandle = false;
	}
        threshMass += bRatio * avgFinalMass;

        // Error printout when charge or baryon number not conserved.
        if (chargeTypeSum != 0 && canHandle) {
          os << " Error: 3*charge changed by " << chargeTypeSum
             << " in " << id << " -> ";
          for (int j = 0; j < mult; ++j) os << prod[j] << " ";
          os << "\n";  
          hasPrinted = true;
          ++nErr;
          continue;
	}
        if ( baryonTypeSum != 0 && canHandle && particlePtr->isHadron() ) {
          os << " Error: 3*baryon number changed by " << baryonTypeSum
             << " in " << id << " -> ";
          for (int j = 0; j < mult; ++j) os << prod[j] << " ";
          os << "\n";  
          hasPrinted = true;
          ++nErr;
          continue;
	}

        // Begin check that some matrix elements are used correctly.
        bool correctME = true;

        // Check matrix element mode 0: recommended not into partons.
        if (meMode == 0 && !isResonanceNow) {
          bool hasPartons = false;
          for (int j = 0; j < mult; ++j) {
            int idAbs = abs(prod[j]);
            if ( idAbs < 10 || idAbs == 21 || idAbs == 81 || idAbs == 82
            || idAbs == 83 || (idAbs > 1000 && idAbs < 10000 
            && (idAbs/10)%10 == 0) ) hasPartons = true;
	  }
          if (hasPartons) correctME = false;
	}

        // Check matrix element mode 1: rho/omega -> pi+ pi- pi0.
        bool useME1 = ( mult == 3 && spinTypeNow == 3 && id > 100 
          && id < 1000 && particlePtr->decay[i].contains(211, -211, 111) );
        if ( meMode == 1 && !useME1 ) correctME = false;
        if ( meMode != 1 &&  useME1 ) correctME = false;

        // Check matrix element mode 2: polarization in V -> PS + PS.
        bool useME2 = ( mult == 2 && spinTypeNow == 3  && id > 100 
          && id < 1000 && spinType(prod[0]) == 1 
          && spinType(prod[1]) == 1 );   
        if ( meMode == 2 && !useME2 ) correctME = false;
        if ( meMode != 2 &&  useME2 ) correctME = false;

        // Check matrix element mode 11, 12 and 13: Dalitz decay with
        // one or more particles in addition to the lepton pair,
        // or double Dalitz decay.
        bool useME11 = ( mult == 3 && !isResonanceNow
          && (prod[1] == 11 || prod[1] == 13 || prod[1] == 15) 
          && prod[2] == -prod[1] );   
        bool useME12 = ( mult > 3 && !isResonanceNow
          && (prod[mult - 2] == 11 || prod[mult - 2] == 13 
          || prod[mult - 2] == 15) && prod[mult - 1] == -prod[mult - 2] );   
        bool useME13 = ( mult == 4 && !isResonanceNow
          && (prod[0] == 11 || prod[0] == 13) && prod[1] == -prod[0]
          && (prod[2] == 11 || prod[2] == 13) && prod[3] == -prod[2] );
        if (useME13) useME12 = false;
        if ( meMode == 11 && !useME11 ) correctME = false;
        if ( meMode != 11 &&  useME11 ) correctME = false;
        if ( meMode == 12 && !useME12 ) correctME = false;
        if ( meMode != 12 &&  useME12 ) correctME = false;
        if ( meMode == 13 && !useME13 ) correctME = false;
        if ( meMode != 13 &&  useME13 ) correctME = false;

        // Check matrix element mode 21: tau -> nu_tau hadrons.
        bool useME21 = (id == 15 && mult > 2 && prod[0] == 16 
          && abs(prod[1]) > 100);
        if ( meMode == 21 && !useME21 ) correctME = false;
        if ( meMode != 21 &&  useME21 ) correctME = false;  

        // Check matrix element mode 22, but only for semileptonic decay.
        // For a -> b c d require types u = 2, ubar = -2, d = 1, dbar = -1.
        if ( isLepton(prod[0]) && isLepton(prod[1]) ) {
          bool useME22 = false;
          int typeA = 0;
          int typeB = 0;
          int typeC = 0;
          if (particlePtr->isLepton()) {
            typeA = (id > 0) ? 1 + (id-1)%2 : -1 - (1-id)%2;
          } else if (particlePtr->isHadron()) {
            int hQ = particlePtr->heaviestQuark();
            // Special case: for B_c either bbar or c decays.
            if (id == 541 && heaviestQuark(prod[2]) == -5) hQ = 4;
            typeA = (hQ > 0) ? 1 + (hQ-1)%2 : -1 - (1-hQ)%2;
  	  }
          typeB = (prod[0] > 0) ? 1 + (prod[0]-1)%2 : -1 - (1-prod[0])%2;
          typeC = (prod[1] > 0) ? 1 + (prod[1]-1)%2 : -1 - (1-prod[1])%2;
          // Special cases.
          if ( (id == 130 || id == 310) && typeC * typeA < 0) typeA = -typeA;
          if (mult == 3 && id == 2112 && prod[2] == 2212) typeA = 3 - typeA;
          if (mult == 3 && id == 3222 && prod[2] == 3122) typeA = 3 - typeA;
          if (mult > 2 && typeC == typeA && typeB * typeC < 0 
            && (typeB + typeC)%2 != 0) useME22 = true;
          if ( meMode == 22 && !useME22 ) correctME = false;
          if ( meMode != 22 &&  useME22 ) correctME = false;  
	}

        // Check for matrix element mode 31.
        if (meMode == 31) {
          int nGamma = 0;
          for (int j = 0; j < mult; ++j) if (prod[j] == 22) ++nGamma;
          if (nGamma != 1) correctME = false; 
	}   

        // Check for unknown mode, or resonance-only mode.
        if ( !isResonanceNow && (meMode < 0 || (meMode > 2 && meMode <= 10) 
          || (meMode > 13 && meMode <= 20) || (meMode > 23 && meMode <= 30)
          || (meMode > 31 && meMode <= 41) || meMode == 51 || meMode == 61
          || meMode == 71 || (meMode > 80 && meMode <= 90) 
          || (!particlePtr->isOctetHadron() && meMode > 92) ) )
          correctME = false;

        // Print if incorrect matrix element mode.
        if ( !correctME ) {
          os << " Warning: meMode " << meMode << " used for "
             << id << " -> ";
          for (int j = 0; j < mult; ++j) os << prod[j] << " ";
          os << "\n";  
          hasPrinted = true;
          ++nErr;
	}

        // Warning printout when no phase space for decay.
        if ( studyCloser && verbosity > 0  && canHandle && onMode > 0
          && particlePtr->m0Min() - minFinalMass < 0. ) {
          if (particlePtr->m0Max() - minFinalMass < 0.)
            os << " Error: decay never possible for ";
          else  os << " Warning: decay sometimes not possible for ";
          os << id << " -> ";
          for (int j = 0; j < mult; ++j) os << prod[j] << " ";
          os << "\n";  
          hasPrinted = true;
          ++nErr;
	}

        // End loop over decay channels. 
        if (onMode > 0 && bRatio > 0.) openChannel = true;
      }

      // Optional printout of threshold.
      if (verbosity%10 > 1 && particlePtr->useBreitWigner()) {
        threshMass /= bRatioSum;
        cout << " Info: particle " << id << fixed << setprecision(5)  
             << " has average mass threshold " << threshMass 
             << " while mMin is " << mMinNow << "\n"; 
        hasPrinted = true;
      }
 
      // Error printout when no acceptable decay channels found.
      if (studyCloser && !openChannel) { 
        os << " Error: no acceptable decay channel found for particle " 
           << id << "\n"; 
        hasPrinted = true;
        ++nErr;
      }      

      // Warning printout when branching ratios do not sum to unity.
      if (studyCloser && (!hasAnti || (!hasPosBR && !hasNegBR))
        && abs(bRatioSum + bRatioPos - 1.) > 1e-8) {
        os << " Warning: particle " << id  << fixed << setprecision(8)
           << " has branching ratio sum " << bRatioSum << "\n"; 
        hasPrinted = true;
        ++nErr;
      } else if (studyCloser && hasAnti 
        && (abs(bRatioSum + bRatioPos - 1.) > 1e-8 
        || abs(bRatioSum + bRatioNeg - 1.) > 1e-8)) {
        os << " Warning: particle " << id  << fixed << setprecision(8)
           << " has branching ratio sum " << bRatioSum + bRatioPos
           << " while its antiparticle has " << bRatioSum + bRatioNeg  
           << "\n"; 
        hasPrinted = true;
        ++nErr;  
      }  

    // End study of decay channels and loop over particles.
    }
    if (hasPrinted) os << "\n";
  }

  // Final output. Done.
  os << " Total number of errors and warnings is " << nErr << "\n";
  os << "\n --------  End PYTHIA Check of Particle Data Table  --------"
     << "------\n" << endl;

}

//*********

// Return the id of the sequentially next particle stored in table.
  
int ParticleDataTable::nextId(int idIn) {

  // Return 0 for negative or unknown codes. Return first for 0.
  if (idIn < 0 || (idIn > 0 && !isParticle(idIn))) return 0;
  if (idIn == 0) return pdt.begin()->first;
  
  // Find pointer to current particle and step up. Return 0 if impossible. 
  map<int, ParticleDataEntry>::const_iterator pdtIn = pdt.find(idIn);
  if (pdtIn == pdt.end()) return 0;
  return (++pdtIn)->first;

}

//*********

// Convert string to lowercase for case-insensitive comparisons.

string ParticleDataTable::toLower(const string& name) { 

  string temp(name);
  for (int i = 0; i < int(temp.length()); ++i) 
    temp[i] = std::tolower(temp[i]); 
  return temp; 

}

//*********

// Allow several alternative inputs for true/false.

bool ParticleDataTable::boolString(string tag) {

  string tagLow = toLower(tag);
  return ( tagLow == "true" || tagLow == "1" || tagLow == "on" 
  || tagLow == "yes" || tagLow == "ok" ); 

}  

//*********

// Extract XML value string following XML attribute.

string ParticleDataTable::attributeValue(string line, string attribute) {

  if (line.find(attribute) == string::npos) return "";
  int iBegAttri = line.find(attribute); 
  int iBegQuote = line.find("\"", iBegAttri + 1);
  int iEndQuote = line.find("\"", iBegQuote + 1);
  return line.substr(iBegQuote + 1, iEndQuote - iBegQuote - 1);

}

//*********

// Extract XML bool value following XML attribute.

bool ParticleDataTable::boolAttributeValue(string line, string attribute) {

  string valString = attributeValue(line, attribute);
  if (valString == "") return false;
  return boolString(valString);   

}

//*********

// Extract XML int value following XML attribute.

int ParticleDataTable::intAttributeValue(string line, string attribute) {
  string valString = attributeValue(line, attribute);
  if (valString == "") return 0; 
  istringstream valStream(valString);
  int intVal; 
  valStream >> intVal; 
  return intVal;     

}

//*********

// Extract XML double value following XML attribute.

double ParticleDataTable::doubleAttributeValue(string line, string attribute) {
  string valString = attributeValue(line, attribute);
  if (valString == "") return 0.; 
  istringstream valStream(valString);
  double doubleVal; 
  valStream >> doubleVal; 
  return doubleVal;     

}

//**************************************************************************

} // end namespace Pythia8
