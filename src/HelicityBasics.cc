// HelicityBasics.cc is a part of the PYTHIA event generator.
// Copyright (C) 2011 Philip Ilten, Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Function definitions (not found in the header) for helper classes
// used in tau decays.

#include "HelicityBasics.h"

namespace Pythia8 {

//==========================================================================

// Wave4 class.
// Friend methods to it.

//--------------------------------------------------------------------------

// complex * Wave4.

Wave4 operator*(complex s, const Wave4& w) {return Wave4(
  s*w.values[0], s*w.values[1], s*w.values[2], s*w.values[3]);
}

//--------------------------------------------------------------------------

// Complex conjugate.

Wave4 conj(Wave4 w) {w(0) = conj(w(0)); w(1) = conj(w(1)); 
  w(2) = conj(w(2)); w(3) = conj(w(3)); return w;
}

//--------------------------------------------------------------------------

// Invariant squared mass for REAL Wave4 (to save time).

double m2(Wave4 w1, Wave4 w2) {
  return real(w1(0))*real(w2(0)) - real(w1(1))*real(w2(1)) 
       - real(w1(2))*real(w2(2)) - real(w1(3))*real(w2(3));
}

//--------------------------------------------------------------------------
    
// Print a Wave4 vector.

ostream& operator<< (ostream& output, Wave4 w) {
  output << left << setprecision(2);
  for (int i = 0; i < 4; i++) output << setw(20) << w.values[i];
  output << "\n";
  return output;
}

//==========================================================================

  /*
    Constructor for the GammaMatrix class. Gamma(1) through Gamma(3) give the
    corresponding gamma matrices using the Weyl basis as outlined in the HELAS
    paper. Gamma(4) gives the +--- metric, while Gamma(5) gives the gamma^5
    matrix.
   */
  GammaMatrix::GammaMatrix(int mu) {
    zero = 0;
    if (mu == 0) {
      values[0] = 1; values[1] = 1; values[2] = 1; values[3] = 1;
      index[0] = 2; index[1] = 3; index[2] = 0; index[3] = 1;
    }
    else if (mu == 1) {
      values[0] = -1; values[1] = -1; values[2] = 1; values[3] = 1;
      index[0] = 3; index[1] = 2; index[2] = 1; index[3] = 0;
    }
    else if (mu == 2) {
      values[0] = complex(0,-1); values[1] = complex(0,1);
      values[2] = complex(0,1);  values[3] = complex(0,-1);
      index[0] = 3; index[1] = 2; index[2] = 1; index[3] = 0;
    }
    else if (mu == 3) {
      values[0] = -1; values[1] = 1; values[2] = 1; values[3] = -1;
      index[0] = 2; index[1] = 3; index[2] = 0; index[3] = 1;
    }
    else if (mu == 4) {
      values[0] = 1; values[1] = -1; values[2] = -1; values[3] = -1;
      index[0] = 0; index[1] = 1; index[2] = 2; index[3] = 3;
    }
    else if (mu == 5) {
      values[0] = -1; values[1] = -1; values[2] = 1; values[3] = 1;
      index[0] = 0; index[1] = 1; index[2] = 2; index[3] = 3;
    }
  }

//--------------------------------------------------------------------------

// Wave4 * GammaMatrix

Wave4 operator*(Wave4 w, GammaMatrix g) {
  complex w0 = w(g.index[0]); complex w1 = w(g.index[1]); 
  complex w2 = w(g.index[2]); complex w3 = w(g.index[3]); 
  w(0) = w0*g.values[0]; w(1) = w1*g.values[1];
  w(2) = w2*g.values[2]; w(3) = w3*g.values[3]; 
  return w;
}

//--------------------------------------------------------------------------

// Scalar * GammaMatrix

GammaMatrix operator*(complex s, GammaMatrix g) {
  g.values[0] = s*g.values[0]; g.values[1] = s*g.values[1];
  g.values[2] = s*g.values[2]; g.values[3] = s*g.values[3]; 
  return g;
}

//--------------------------------------------------------------------------

// I*Scalar - Gamma5

GammaMatrix operator-(complex s, GammaMatrix g) {
  g.values[0] = s-g.values[0]; g.values[1] = s-g.values[1];
  g.values[2] = s-g.values[2]; g.values[3] = s-g.values[3]; 
  return g;
}

//--------------------------------------------------------------------------

// I*Scalar + Gamma5

GammaMatrix operator+(complex s, GammaMatrix g) {
  g.values[0] = s+g.values[0]; g.values[1] = s+g.values[1];
  g.values[2] = s+g.values[2]; g.values[3] = s+g.values[3]; 
  return g;
}

//--------------------------------------------------------------------------

// Print GammaMatrix

ostream& operator<< (ostream& output, GammaMatrix g) {
  output << left << setprecision(2);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) output << setw(20) << g(i,j);
    output << "\n";
  }
  return output;
}

//==========================================================================

  /*
    Weyl helicity wave functions for spin 1/2 fermions and spin 1 boson.

    This is the basis as given by the HELAS collaboration on page 122 of
    "HELas: HELicity Amplitude Subroutines for Feynman Diagram Evaluations" by
    H. Murayama, I. Watanabe, K. Hagiwara.

    The spinors become ill-defined for p -> -pz and the polarization vectors
    become ill-defined when pT -> 0. For these special cases limits are used
    and are triggered when the difference of the approached quantity falls
    below the variable TOLERANCE.
   */
  Wave4 HelicityParticle::wave(int h) {
    // Tolerance limit.
    double TOLERANCE = 0.000001;

    // Create wave vector to return.
    Wave4 w;

    // Fermion (spin 1/2) spinor.
    if (spinType() == 2) {
      // Calculate helicity indpendent normalization.
      double P = pAbs();
      double n = sqrtpos(2*P*(P+pz()));
      
      // Calculate eigenspinor basis.
      vector< vector<complex> > xi(2, vector<complex>(2));
      // Helicity -1
      xi[0][0] = fabs(P+pz()) < TOLERANCE ? -1 : complex(-px(),py())/n;
      xi[0][1] = fabs(P+pz()) < TOLERANCE ?  0 : (P+pz())/n;
      // Helicity +1
      xi[1][0] = fabs(P+pz()) < TOLERANCE ? 0 : (P+pz())/n;
      xi[1][1] = fabs(P+pz()) < TOLERANCE ? 1 : complex(px(),py())/n;
      
      // Calculate helicity dependent normalization.
      vector<double> omega(2);
      omega[0] = sqrtpos(e()-P);
      omega[1] = sqrtpos(e()+P);
      vector<double> hsign(2,1); hsign[0] = -1;
      
      // Create particle spinor.
      if (this->id() > 0) {
	w(0) = omega[!h]*xi[h][0];
	w(1) = omega[!h]*xi[h][1];
	w(2) = omega[h] *xi[h][0];
	w(3) = omega[h] *xi[h][1];
      }
      // Create anti-particle spinor.
      else {
	w(0) = hsign[!h]*omega[h] *xi[!h][0];
	w(1) = hsign[!h]*omega[h] *xi[!h][1];
	w(2) = hsign[h] *omega[!h]*xi[!h][0];
	w(3) = hsign[h] *omega[!h]*xi[!h][1];
      }
    }

    // Boson (spin 1) polarization vector.
    else if (spinType() == 3) {
      double P  = pAbs();
      double PT = pT();
      // Create helicity +1 or -1 polarization vector.
      if (h >= 0 && h <= 1) {
	double hsign = h ? -1 : 1;
	if (PT > TOLERANCE) {
	  w(0) = 0;
	  w(1) = complex(hsign*px()*pz()/(P*PT), -py()/PT);
	  w(2) = complex(hsign*py()*pz()/(P*PT),  px()/PT);
	  w(3) = complex(-hsign*PT/P, 0);
	}
	else {
	  w(0) = 0;
	  w(1) = hsign*pz();
	  w(2) = 0;
	  w(3) = 0;
	}
      }
      // Create helicity 0 polarization vector (ensure boson massive).
      else if (h == 2 && m() > TOLERANCE) {
	w(0) = P/m(); 
	w(1) = px()*e()/(m()*P); 
	w(2) = py()*e()/(m()*P); 
	w(3) = pz()*e()/(m()*P); 
      }
    }

    // Unknown wave function.
    else {
      w(0) = 0;
      w(1) = 0;
      w(2) = 0;
      w(3) = 0;
    }
    return w;
  }

//--------------------------------------------------------------------------

  // Bar of a wave function.
  Wave4 HelicityParticle::waveBar(int h) {
    if (spinType() == 2) {
      return conj(wave(h))*GammaMatrix(0);
    }
    else {
      return conj(wave(h));
    }
  }

//--------------------------------------------------------------------------

  // Normalize the rho or D matrices.
  void HelicityParticle::normalize(vector< vector<complex> >& matrix) {
    complex trace = 0;
    for (unsigned int i = 0; i < matrix.size(); i++)
      trace += matrix[i][i];
    for (unsigned int i = 0; i < matrix.size(); i++) {
      for (unsigned int j = 0; j < matrix.size(); j++) {
	if (trace != complex(0,0))
	  matrix[i][j] /= trace;
	else
	  matrix[i][j] = 1 / static_cast<double>(matrix.size());
      }
    }
  }

//==========================================================================

} // end namespace Pythia8