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

#ifndef DSPFILTERS_CHEBYSHEVI_H
#define DSPFILTERS_CHEBYSHEVI_H

#include "Common.h"
#include "Cascade.h"
#include "Design.h"
#include "Filter.h"
#include "PoleFilter.h"

namespace Dsp {

/*
 * Filters with Chebyshev response characteristics
 *
 */

namespace ChebyshevI {

// Half-band analog prototypes (s-plane)

class AnalogLowPass : public LayoutBase
{
public:
  AnalogLowPass ();

  void design (const int numPoles,
               double rippleDb);

private:
  int m_numPoles;
  double m_rippleDb;
};

//------------------------------------------------------------------------------

class AnalogLowShelf : public LayoutBase
{
public:
  AnalogLowShelf ();

  void design (int numPoles,
               double gainDb,
               double rippleDb);

private:
  int m_numPoles;
  double m_rippleDb;
  double m_gainDb;
};

//------------------------------------------------------------------------------

// Factored implementations to reduce template instantiations

struct LowPassBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency,
              double rippleDb);
};

struct HighPassBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency,
              double rippleDb);
};

struct BandPassBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double centerFrequency,
              double widthFrequency,
              double rippleDb);
};

struct BandStopBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double centerFrequency,
              double widthFrequency,
              double rippleDb);
};

struct LowShelfBase : PoleFilterBase <AnalogLowShelf>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency,
              double gainDb,
              double rippleDb);
};

struct HighShelfBase : PoleFilterBase <AnalogLowShelf>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency,
              double gainDb,
              double rippleDb);
};

struct BandShelfBase : PoleFilterBase <AnalogLowShelf>
{
  void setup (int order,
              double sampleRate,
              double centerFrequency,
              double widthFrequency,
              double gainDb,
              double rippleDb);
};

//------------------------------------------------------------------------------

//
// Raw filters
//

template <int MaxOrder>
struct LowPass : PoleFilter <LowPassBase, MaxOrder>
{
};

template <int MaxOrder>
struct HighPass : PoleFilter <HighPassBase, MaxOrder>
{
};

template <int MaxOrder>
struct BandPass : PoleFilter <BandPassBase, MaxOrder, MaxOrder*2>
{
};

template <int MaxOrder>
struct BandStop : PoleFilter <BandStopBase, MaxOrder, MaxOrder*2>
{
};

template <int MaxOrder>
struct LowShelf : PoleFilter <LowShelfBase, MaxOrder>
{
};

template <int MaxOrder>
struct HighShelf : PoleFilter <HighShelfBase, MaxOrder>
{
};

template <int MaxOrder>
struct BandShelf : PoleFilter <BandShelfBase, MaxOrder, MaxOrder*2>
{
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
    NumParams = 4
  };

  static int getNumParams ()
  {
    return 4;
  }

  static const ParamInfo getParamInfo_2 ()
  {
    return ParamInfo::defaultCutoffFrequencyParam ();
  }

  static const ParamInfo getParamInfo_3 ()
  {
    return ParamInfo::defaultRippleDbParam ();
  }
};

template <class FilterClass>
struct TypeI : TypeIBase, FilterClass
{
  void setParams (const Params& params)
  {
    FilterClass::setup (int(params[1]), params[0], params[2], params[3]);
  }
};

struct TypeIIBase : DesignBase
{
  enum
  {
    NumParams = 5
  };

  static int getNumParams ()
  {
    return 5;
  }

  static const ParamInfo getParamInfo_2 ()
  {
    return ParamInfo::defaultCenterFrequencyParam ();
  }

  static const ParamInfo getParamInfo_3 ()
  {
    return ParamInfo::defaultBandwidthHzParam ();
  }

  static const ParamInfo getParamInfo_4 ()
  {
    return ParamInfo::defaultRippleDbParam ();
  }
};

template <class FilterClass>
struct TypeII : TypeIIBase, FilterClass
{
  void setParams (const Params& params)
  {
    FilterClass::setup (int(params[1]), params[0], params[2], params[3], params[4]);
  }
};

struct TypeIIIBase : DesignBase
{
  enum
  {
    NumParams = 5
  };

  static int getNumParams ()
  {
    return 5;
  }

  static const ParamInfo getParamInfo_2 ()
  {
    return ParamInfo::defaultCutoffFrequencyParam ();
  }

  static const ParamInfo getParamInfo_3 ()
  {
    return ParamInfo::defaultGainParam ();
  }

  static const ParamInfo getParamInfo_4 ()
  {
    return ParamInfo::defaultRippleDbParam ();
  }
};

template <class FilterClass>
struct TypeIII : TypeIIIBase, FilterClass
{
  void setParams (const Params& params)
  {
    FilterClass::setup (int(params[1]), params[0], params[2], params[3], params[4]);
  }
};

struct TypeIVBase : DesignBase
{
  enum
  {
    NumParams = 6
  };

  static int getNumParams ()
  {
    return 6;
  }

  static const ParamInfo getParamInfo_2 ()
  {
    return ParamInfo::defaultCenterFrequencyParam ();
  }

  static const ParamInfo getParamInfo_3 ()
  {
    return ParamInfo::defaultBandwidthHzParam ();
  }

  static const ParamInfo getParamInfo_4 ()
  {
    return ParamInfo::defaultGainParam ();
  }

  static const ParamInfo getParamInfo_5 ()
  {
    return ParamInfo::defaultRippleDbParam ();
  }
};

template <class FilterClass>
struct TypeIV : TypeIVBase, FilterClass
{
  void setParams (const Params& params)
  {
    FilterClass::setup (int(params[1]), params[0], params[2], params[3], params[4], params[5]);
  }
};

// Factored kind and name

struct LowPassDescription
{
  static Kind getKind () { return kindLowPass; }
  static const char* getName() { return "Chebyshev I Low Pass"; }
};

struct HighPassDescription
{
  static Kind getKind () { return kindHighPass; }
  static const char* getName() { return "Chebyshev I High Pass"; }
};

struct BandPassDescription
{
  static Kind getKind () { return kindHighPass; }
  static const char* getName() { return "Chebyshev I Band Pass"; }
};

struct BandStopDescription
{
  static Kind getKind () { return kindHighPass; }
  static const char* getName() { return "Chebyshev I Band Stop"; }
};

struct LowShelfDescription
{
  static Kind getKind () { return kindLowShelf; }
  static const char* getName() { return "Chebyshev I Low Shelf"; }
};

struct HighShelfDescription
{
  static Kind getKind () { return kindHighShelf; }
  static const char* getName() { return "Chebyshev I High Shelf"; }
};

struct BandShelfDescription
{
  static Kind getKind () { return kindBandShelf; }
  static const char* getName() { return "Chebyshev I Band Shelf"; }
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
struct LowPass : OrderBase <MaxOrder, TypeI, ChebyshevI::LowPass>,
                 LowPassDescription
{
};

template <int MaxOrder>
struct HighPass : OrderBase <MaxOrder, TypeI, ChebyshevI::HighPass>,
                  HighPassDescription
{
};

template <int MaxOrder>
struct BandPass : OrderBase <MaxOrder, TypeII, ChebyshevI::BandPass>,
                  BandPassDescription
{
};

template <int MaxOrder>
struct BandStop : OrderBase <MaxOrder, TypeII, ChebyshevI::BandStop>,
                  BandStopDescription
{
};

template <int MaxOrder>
struct LowShelf : OrderBase <MaxOrder, TypeIII, ChebyshevI::LowShelf>,
                  LowShelfDescription
{
};

template <int MaxOrder>
struct HighShelf : OrderBase <MaxOrder, TypeIII, ChebyshevI::HighShelf>,
                   HighShelfDescription
{
};

template <int MaxOrder>
struct BandShelf : OrderBase <MaxOrder, TypeIV, ChebyshevI::BandShelf>,
                   BandShelfDescription
{
};

}

}

}

namespace Dsp {

    namespace ChebyshevI {

        AnalogLowPass::AnalogLowPass ()
                : m_numPoles (-1)
        {
        }

        void AnalogLowPass::design (int numPoles,
                                    double rippleDb)
        {
            if (m_numPoles != numPoles ||
                m_rippleDb != rippleDb)
            {
                m_numPoles = numPoles;
                m_rippleDb = rippleDb;

                reset ();

                const double eps = std::sqrt (1. / std::exp (-rippleDb * 0.1 * doubleLn10) - 1);
                const double v0 = asinh (1 / eps) / numPoles;
                const double sinh_v0 = -sinh (v0);
                const double cosh_v0 = cosh (v0);

                const double n2 = 2 * numPoles;
                const int pairs = numPoles / 2;
                for (int i = 0; i < pairs; ++i)
                {
                    const int k = 2 * i + 1 - numPoles;
                    double a = sinh_v0 * cos (k * doublePi / n2);
                    double b = cosh_v0 * sin (k * doublePi / n2);

                    //addPoleZero (complex_t (a, b), infinity());
                    //addPoleZero (complex_t (a, -b), infinity());
                    addPoleZeroConjugatePairs (complex_t (a, b), infinity());
                }

                if (numPoles & 1)
                {
                    add (complex_t (sinh_v0, 0), infinity());
                    setNormal (0, 1);
                }
                else
                {
                    setNormal (0, pow (10, -rippleDb/20.));
                }
            }
        }

//------------------------------------------------------------------------------

//
// Chebyshev Type I low pass shelf prototype
// From "High-Order Digital Parametric Equalizer Design"
// Sophocles J. Orfanidis
// http://www.ece.rutgers.edu/~orfanidi/ece521/hpeq.pdf
//

        AnalogLowShelf::AnalogLowShelf ()
        {
            setNormal (doublePi, 1);
        }

        void AnalogLowShelf::design (int numPoles,
                                     double gainDb,
                                     double rippleDb)
        {
            if (m_numPoles != numPoles ||
                m_rippleDb != rippleDb ||
                m_gainDb != gainDb)
            {
                m_numPoles = numPoles;
                m_rippleDb = rippleDb;
                m_gainDb = gainDb;

                reset ();

                gainDb = -gainDb;

                if (rippleDb >= fabs(gainDb))
                    rippleDb = fabs (gainDb);
                if (gainDb<0)
                    rippleDb = -rippleDb;

                const double G  = std::pow (10., gainDb / 20.0 );
                const double Gb = std::pow (10., (gainDb - rippleDb) / 20.0);
                const double G0 = 1;
                const double g0 = pow (G0 , 1. / numPoles);

                double eps;
                if (Gb != G0 )
                    eps = sqrt((G*G-Gb*Gb)/(Gb*Gb-G0*G0));
                else
                    eps = G-1; // This is surely wrong

                const double b = pow (G/eps+Gb*sqrt(1+1/(eps*eps)), 1./numPoles);
                const double u = log (b / g0);
                const double v = log (pow (1. / eps+sqrt(1+1/(eps*eps)),1./numPoles));

                const double sinh_u = sinh (u);
                const double sinh_v = sinh (v);
                const double cosh_u = cosh (u);
                const double cosh_v = cosh (v);
                const double n2 = 2 * numPoles;
                const int pairs = numPoles / 2;
                for (int i = 1; i <= pairs; ++i)
                {
                    const double a = doublePi * (2 * i - 1) / n2;
                    const double sn = sin (a);
                    const double cs = cos (a);
                    addPoleZeroConjugatePairs (complex_t (-sn * sinh_u, cs * cosh_u),
                                               complex_t (-sn * sinh_v, cs * cosh_v));
                }

                if (numPoles & 1)
                    add (-sinh_u, -sinh_v);
            }
        }

//------------------------------------------------------------------------------

        void LowPassBase::setup (int order,
                                 double sampleRate,
                                 double cutoffFrequency,
                                 double rippleDb)
        {
            m_analogProto.design (order, rippleDb);

            LowPassTransform (cutoffFrequency / sampleRate,
                              m_digitalProto,
                              m_analogProto);

            Cascade::setLayout (m_digitalProto);
        }

        void HighPassBase::setup (int order,
                                  double sampleRate,
                                  double cutoffFrequency,
                                  double rippleDb)
        {
            m_analogProto.design (order, rippleDb);

            HighPassTransform (cutoffFrequency / sampleRate,
                               m_digitalProto,
                               m_analogProto);

            Cascade::setLayout (m_digitalProto);
        }

        void BandPassBase::setup (int order,
                                  double sampleRate,
                                  double centerFrequency,
                                  double widthFrequency,
                                  double rippleDb)
        {
            m_analogProto.design (order, rippleDb);

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
                                  double rippleDb)
        {
            m_analogProto.design (order, rippleDb);

            BandStopTransform (centerFrequency / sampleRate,
                               widthFrequency / sampleRate,
                               m_digitalProto,
                               m_analogProto);

            Cascade::setLayout (m_digitalProto);
        }

        void LowShelfBase::setup (int order,
                                  double sampleRate,
                                  double cutoffFrequency,
                                  double gainDb,
                                  double rippleDb)
        {
            m_analogProto.design (order, gainDb, rippleDb);

            LowPassTransform (cutoffFrequency / sampleRate,
                              m_digitalProto,
                              m_analogProto);

            Cascade::setLayout (m_digitalProto);
        }

        void HighShelfBase::setup (int order,
                                   double sampleRate,
                                   double cutoffFrequency,
                                   double gainDb,
                                   double rippleDb)
        {
            m_analogProto.design (order, gainDb, rippleDb);

            HighPassTransform (cutoffFrequency / sampleRate,
                               m_digitalProto,
                               m_analogProto);

            Cascade::setLayout (m_digitalProto);
        }

        void BandShelfBase::setup (int order,
                                   double sampleRate,
                                   double centerFrequency,
                                   double widthFrequency,
                                   double gainDb,
                                   double rippleDb)
        {
            m_analogProto.design (order, gainDb, rippleDb);

            BandPassTransform (centerFrequency / sampleRate,
                               widthFrequency / sampleRate,
                               m_digitalProto,
                               m_analogProto);

            m_digitalProto.setNormal (((centerFrequency/sampleRate) < 0.25) ? doublePi : 0, 1);

            Cascade::setLayout (m_digitalProto);
        }

    }

}


#endif

