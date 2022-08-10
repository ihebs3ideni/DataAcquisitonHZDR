/*******************************************************************************

"A Collection of Useful C++ Classes for Digital Signal Processing"
 By Vinnie Falco

Official project location:
https://github.com/vinniefalco/DSPFilters

See Documentation.cpp for contact information, notes, and bibliography.

--------------------------------------------------------------------------------

License: MIT License (http://www.opensource.org/licenses/mit-license.php)
Copyright (c) 2009 by Vinnie Falco

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*******************************************************************************/

#ifndef DSPFILTERS_LEGENDRE_H
#define DSPFILTERS_LEGENDRE_H

#include "Common.h"
#include "Cascade.h"
#include "Design.h"
#include "Filter.h"
#include "PoleFilter.h"
#include "RootFinder.h"

namespace Dsp {

/*
 * Filters with Legendre / "Optimum-L" response characteristics
 *
 */

namespace Legendre {

// Numerical computation of Legendre "Optimum-L" polynomials

class PolynomialFinderBase
{
public:
  void solve (int n);

  double* coef()
  {
    return m_w;
  }

private:
  void legendre (double* p, int n);

protected:
  int m_maxN;
  double* m_w;
  double* m_a;
  double* m_p;
  double* m_s;
  double* m_v;
  double* m_aa;
  double* m_bb;
};

template <int maxN>
class PolynomialFinder : public PolynomialFinderBase
{
public:
  PolynomialFinder ()
  {
    m_maxN = maxN;
    m_w = m_ws;
    m_a = m_as;
    m_p = m_ps;
    m_s = m_ss;
    m_v = m_vs;
    m_aa = m_aas;
    m_bb = m_bbs;
  }

  void solve (int n)
  {
    assert (n <= maxN);
    PolynomialFinderBase::solve (n);
  }

private:
  double m_ws  [2 * maxN + 1];
  double m_as  [    maxN + 1];
  double m_ps  [2 * maxN + 1];
  double m_ss  [2 * maxN + 1];
  double m_vs  [2 * maxN + 4];
  double m_aas [    maxN + 1];
  double m_bbs [    maxN + 1];
};

//------------------------------------------------------------------------------

// A Workspace is necessary to construct the polynomial and find its roots

struct WorkspaceBase
{
  WorkspaceBase (PolynomialFinderBase* polyBase,
                 RootFinderBase* rootsBase)
                 : poly (*polyBase)
                 , roots (*rootsBase)
  {
  }

  PolynomialFinderBase& poly;
  RootFinderBase& roots;

private:
  WorkspaceBase (WorkspaceBase&);
  WorkspaceBase& operator= (WorkspaceBase&);
};

template <int MaxOrder>
struct Workspace : WorkspaceBase
{
  Workspace ()
    : WorkspaceBase (&m_poly, &m_roots)
  {
  }

private:
  PolynomialFinder <MaxOrder> m_poly;
  RootFinder <MaxOrder * 2> m_roots;
};

//------------------------------------------------------------------------------

// Half-band analog prototypes (s-plane)

class AnalogLowPass : public LayoutBase
{
public:
  AnalogLowPass ();

  void design (const int numPoles, WorkspaceBase* w);

private:
  int m_numPoles;
};

//------------------------------------------------------------------------------

// Factored implementations to reduce template instantiations

struct LowPassBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency,
              WorkspaceBase* w);
};

struct HighPassBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency,
              WorkspaceBase* w);
};

struct BandPassBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double centerFrequency,
              double widthFrequency,
              WorkspaceBase* w);
};

struct BandStopBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double centerFrequency,
              double widthFrequency,
              WorkspaceBase* w);
};

//------------------------------------------------------------------------------

//
// Raw filters
//

template <int MaxOrder>
struct LowPass : PoleFilter <LowPassBase, MaxOrder>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency)
  {
    Workspace <MaxOrder> w;
    LowPassBase::setup (order,
                        sampleRate,
                        cutoffFrequency,
                        &w);
  }
};

template <int MaxOrder>
struct HighPass : PoleFilter <HighPassBase, MaxOrder>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency)
  {
    Workspace <MaxOrder> w;
    HighPassBase::setup (order,
                         sampleRate,
                         cutoffFrequency,
                         &w);
  }
};

template <int MaxOrder>
struct BandPass : PoleFilter <BandPassBase, MaxOrder, MaxOrder*2>
{
  void setup (int order,
              double sampleRate,
              double centerFrequency,
              double widthFrequency)
  {
    Workspace <MaxOrder> w;
    BandPassBase::setup (order,
                         sampleRate,
                         centerFrequency,
                         widthFrequency,
                         &w);
  }
};

template <int MaxOrder>
struct BandStop : PoleFilter <BandStopBase, MaxOrder, MaxOrder*2>
{
  void setup (int order,
              double sampleRate,
              double centerFrequency,
              double widthFrequency)
  {
    Workspace <MaxOrder> w;
    BandStopBase::setup (order,
                         sampleRate,
                         centerFrequency,
                         widthFrequency,
                         &w);
  }
};

//------------------------------------------------------------------------------

//
// Gui-friendly Design layer
//

namespace Design {

struct TypeIBase : DesignBase
{
  enum
  {
    NumParams = 3
  };

  static int getNumParams ()
  {
    return 3;
  }

  static const ParamInfo getParamInfo_2 ()
  {
    return ParamInfo::defaultCutoffFrequencyParam ();
  }
};

template <class FilterClass>
struct TypeI : TypeIBase, FilterClass
{
  void setParams (const Params& params)
  {
    FilterClass::setup (int(params[1]), params[0], params[2]);
  }
};

struct TypeIIBase : DesignBase
{
  enum
  {
    NumParams = 4
  };

  static int getNumParams ()
  {
    return 4;
  }

  static const ParamInfo getParamInfo_2 ()
  {
    return ParamInfo::defaultCenterFrequencyParam ();
  }

  static const ParamInfo getParamInfo_3 ()
  {
    return ParamInfo::defaultBandwidthHzParam ();
  }
};

template <class FilterClass>
struct TypeII : TypeIIBase, FilterClass
{
  void setParams (const Params& params)
  {
    FilterClass::setup (int(params[1]), params[0], params[2], params[3]);
  }
};

// Factored kind and name

struct LowPassDescription
{
  static Kind getKind () { return kindLowPass; }
  static const char* getName() { return "Legendre Low Pass"; }
};

struct HighPassDescription
{
  static Kind getKind () { return kindHighPass; }
  static const char* getName() { return "Legendre High Pass"; }
};

struct BandPassDescription
{
  static Kind getKind () { return kindHighPass; }
  static const char* getName() { return "Legendre Band Pass"; }
};

struct BandStopDescription
{
  static Kind getKind () { return kindHighPass; }
  static const char* getName() { return "Legendre Band Stop"; }
};

// This glues on the Order parameter
template <int MaxOrder,
          template <class> class TypeClass,
          template <int> class FilterClass>
struct OrderBase : TypeClass <FilterClass <MaxOrder> >
{
  const ParamInfo getParamInfo_1 () const
  {
    return ParamInfo (idOrder, "Order", "Order",
                       1, MaxOrder, 2,
                       &ParamInfo::Int_toControlValue,
                       &ParamInfo::Int_toNativeValue,
                       &ParamInfo::Int_toString);

  }
};

//------------------------------------------------------------------------------

//
// Design filters
//

template <int MaxOrder>
struct LowPass : OrderBase <MaxOrder, TypeI, Legendre::LowPass>,
                 LowPassDescription
{
};

template <int MaxOrder>
struct HighPass : OrderBase <MaxOrder, TypeI, Legendre::HighPass>,
                  HighPassDescription
{
};

template <int MaxOrder>
struct BandPass : OrderBase <MaxOrder, TypeII, Legendre::BandPass>,
                  BandPassDescription
{
};

template <int MaxOrder>
struct BandStop : OrderBase <MaxOrder, TypeII, Legendre::BandStop>,
                  BandStopDescription
{
};

}

}

}

namespace Dsp {

    namespace Legendre {

        static inline double m_sqrt2 ()
        {
            return 1.41421356237309504880;
        }

//  Optimum 'L' Filter algorithm.
//  (C) 2004, C. Bond.
//
//  Based on discussion in Kuo, "Network Analysis and Synthesis",
//  pp. 379-383. Original method due to A.Papoulis."On Monotonic
//  Response Filters", Proc. IRE, 47, Feb. 1959.
//
//  Rewritten by Vinnie Falco to change the way temporary
//  storage is allocated
//

//
//  This routine calculates the coefficients of the Legendre polynomial
//  of the 1st kind. It uses a recursion relation. The first few polynomials
//  are hard coded and the rest are found by recursion.
//
//  (n+1)Pn+1 = (2n+1)xPn - nPn-1 	Recursion relation.
//
        void PolynomialFinderBase::legendre (double *p, int n)
        {
            int i,j;

            if (n == 0) {
                p[0] = 1.0;
                return;
            }
            if (n == 1) {
                p[0] = 0.0;
                p[1] = 1.0;
                return;
            }
            p[0] = -0.5;
            p[1] = 0.0;
            p[2] = 1.5;

            if (n == 2) return;

            for (i=0;i<=n;i++) {
                m_aa[i] = m_bb[i] = 0.0;
            }
            m_bb[1] = 1.0;

            for (i=3;i<=n;i++) {
                for (j=0;j<=i;j++) {
                    m_aa[j] = m_bb[j];
                    m_bb[j] = p[j];
                    p[j] = 0.0;
                }
                for (j=i-2;j>=0;j-=2) {
                    p[j] -= (i-1)*m_aa[j]/i;
                }
                for (j=i-1;j>=0;j-=2) {
                    p[j+1] += (2*i-1)*m_bb[j]/i;
                }
            }
        }

//
//
//  In the following routine n = 2k + 1 for odd 'n' and n = 2k + 2 for
//  even 'n'.
//
//
//      n   k
//      -----
//      1   0
//      2   0
//      3   1
//      4   1
//      5   2
//      6   2
//

        void PolynomialFinderBase::solve (int n)
        {
            assert (n <= m_maxN);

            double c0,c1;
            int i,j,k;

            k = (n-1)/2;
            //
            //  form vector of 'a' constants
            //
            if (n & 1) {                // odd
                for (i=0;i<=k;i++) {
                    m_a[i] = (2.0*i+1.0)/(m_sqrt2()*(k+1.0));
                }
            }                           // even
            else {
                for (i=0;i<k+1;i++) {
                    m_a[i] = 0.0;
                }
                if (k & 1) {
                    for (i=1;i<=k;i+=2) {
                        m_a[i] = (2*i+1)/sqrt(double((k+1)*(k+2)));
                    }
                }
                else {
                    for (i=0;i<=k;i+=2) {
                        m_a[i] = (2*i+1)/sqrt(double((k+1)*(k+2)));
                    }
                }
            }
            for (i=0;i<=n;i++){
                m_s[i] = 0.0;
                m_w[i] = 0.0;
            }
            //
            // form s[] = sum of a[i]*P[i]
            //
            m_s[0] = m_a[0];
            m_s[1] = m_a[1];
            for (i=2;i<=k;i++) {
                legendre(m_p,i);
                for (j=0;j<=i;j++) {
                    m_s[j] += m_a[i]*m_p[j];
                }
            }
            //
            //  form v[] = square of s[]
            //
            for (i=0;i<=2*k+2;i++) {
                m_v[i] = 0.0;
            }
            for (i=0;i<=k;i++) {
                for (j=0;j<=k;j++) {
                    m_v[i+j] += m_s[i]*m_s[j];
                }
            }
            //
            //  modify integrand for even 'n'
            //
            m_v[2*k+1] = 0.0;
            if ((n & 1) == 0) {
                for (i=n;i>=0;i--) {
                    m_v[i+1] += m_v[i];
                }
            }
            //
            //  form integral of v[]
            //
            for (i=n+1;i>=0;i--) {
                m_v[i+1] = m_v[i]/(double)(i+1.0);
            }
            m_v[0] = 0.0;
            //
            // clear s[] for use in computing definite integral
            //
            for (i=0;i<(n+2);i++){
                m_s[i] = 0.0;
            }
            m_s[0] = -1.0;
            m_s[1] = 2.0;
            //
            //  calculate definite integral
            //
            for (i=1;i<=n;i++) {
                if (i > 1) {
                    c0 = -m_s[0];
                    for (j=1;j<i+1;j++) {
                        c1 = -m_s[j] + 2.0*m_s[j-1];
                        m_s[j-1] = c0;
                        c0 = c1;
                    }
                    c1 = 2.0*m_s[i];
                    m_s[i] = c0;
                    m_s[i+1] = c1;
                }
                for (j=i;j>0;j--) {
                    m_w[j] += (m_v[i]*m_s[j]);
                }
            }
            if ((n & 1) == 0) m_w[1] = 0.0;
        }

//------------------------------------------------------------------------------

        AnalogLowPass::AnalogLowPass ()
                : m_numPoles (-1)
        {
            setNormal (0, 1);
        }

        void AnalogLowPass::design (int numPoles,
                                    WorkspaceBase* w)
        {
            if (m_numPoles != numPoles)
            {
                m_numPoles = numPoles;

                reset ();

                PolynomialFinderBase& poly (w->poly);
                RootFinderBase& poles (w->roots);

                poly.solve (numPoles);
                int degree = numPoles * 2;

                poles.coef()[0] = 1 + poly.coef()[0];
                poles.coef()[1] = 0;
                for (int i = 1; i <= degree; ++i)
                {
                    poles.coef()[2*i] = poly.coef()[i] * ((i & 1) ? -1 : 1);
                    poles.coef()[2*i+1] = 0;
                }
                poles.solve (degree);

                int j = 0;
                for (int i = 0; i < degree; ++i)
                    if (poles.root()[i].real() <= 0)
                        poles.root()[j++] = poles.root()[i];
                // sort descending imag() and cut degree in half
                poles.sort (degree/2);

                const int pairs = numPoles / 2;
                for (int i = 0; i < pairs; ++i)
                {
                    complex_t c = poles.root()[i];
                    addPoleZeroConjugatePairs (c, infinity());
                }

                if (numPoles & 1)
                    add (poles.root()[pairs].real(), infinity());
            }
        }

//------------------------------------------------------------------------------

        void LowPassBase::setup (int order,
                                 double sampleRate,
                                 double cutoffFrequency,
                                 WorkspaceBase* w)
        {
            m_analogProto.design (order, w);

            LowPassTransform (cutoffFrequency / sampleRate,
                              m_digitalProto,
                              m_analogProto);

            Cascade::setLayout (m_digitalProto);
        }

        void HighPassBase::setup (int order,
                                  double sampleRate,
                                  double cutoffFrequency,
                                  WorkspaceBase* w)
        {
            m_analogProto.design (order, w);

            HighPassTransform (cutoffFrequency / sampleRate,
                               m_digitalProto,
                               m_analogProto);

            Cascade::setLayout (m_digitalProto);
        }

        void BandPassBase::setup (int order,
                                  double sampleRate,
                                  double centerFrequency,
                                  double widthFrequency,
                                  WorkspaceBase* w)
        {
            m_analogProto.design (order, w);

            BandPassTransform (centerFrequency / sampleRate,
                               widthFrequency / sampleRate,
                               m_digitalProto,
                               m_analogProto);

            Cascade::setLayout (m_digitalProto);
        }

        void BandStopBase::setup (int order,
                                  double sampleRate,
                                  double centerFrequency,
                                  double widthFrequency,
                                  WorkspaceBase* w)
        {
            m_analogProto.design (order, w);

            BandStopTransform (centerFrequency / sampleRate,
                               widthFrequency / sampleRate,
                               m_digitalProto,
                               m_analogProto);

            Cascade::setLayout (m_digitalProto);
        }

    }

}

#endif

