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

#ifndef DSPFILTERS_BUTTERWORTH_H
#define DSPFILTERS_BUTTERWORTH_H

#include "Common.h"
#include "Cascade.h"
#include "Design.h"
#include "Filter.h"
#include "PoleFilter.h"

namespace Dsp {

/*
 * Filters with Butterworth response characteristics
 *
 */

namespace Butterworth {

// Half-band analog prototypes (s-plane)

class AnalogLowPass : public LayoutBase
{
public:
  AnalogLowPass ();

  void design (const int numPoles);

private:
  int m_numPoles;
};

//------------------------------------------------------------------------------

class AnalogLowShelf : public LayoutBase
{
public:
  AnalogLowShelf ();

  void design (int numPoles, double gainDb);

private:
  int m_numPoles;
  double m_gainDb;
};

//------------------------------------------------------------------------------

// Factored implementations to reduce template instantiations

struct LowPassBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency);
};

struct HighPassBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency);
};

struct BandPassBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double centerFrequency,
              double widthFrequency);
};

struct BandStopBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double centerFrequency,
              double widthFrequency);
};

struct LowShelfBase : PoleFilterBase <AnalogLowShelf>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency,
              double gainDb);
};

struct HighShelfBase : PoleFilterBase <AnalogLowShelf>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency,
              double gainDb);
};

struct BandShelfBase : PoleFilterBase <AnalogLowShelf>
{
  void setup (int order,
              double sampleRate,
              double centerFrequency,
              double widthFrequency,
              double gainDb);
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

struct TypeIIIBase : DesignBase
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
    return ParamInfo::defaultGainParam ();
  }
};

template <class FilterClass>
struct TypeIII : TypeIIIBase, FilterClass
{
  void setParams (const Params& params)
  {
    FilterClass::setup (int(params[1]),
                        params[0],
                        params[2],
                        params[3]);
  }
};

struct TypeIVBase : DesignBase
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
    return ParamInfo::defaultGainParam ();
  }
};

template <class FilterClass>
struct TypeIV : TypeIVBase, FilterClass
{
  void setParams (const Params& params)
  {
    FilterClass::setup (int(params[1]), params[0], params[2], params[3], params[4]);
  }
};

// Factored kind and name

struct LowPassDescription
{
  static Kind getKind () { return kindLowPass; }
  static const char* getName() { return "Butterworth Low Pass"; }
};

struct HighPassDescription
{
  static Kind getKind () { return kindHighPass; }
  static const char* getName() { return "Butterworth High Pass"; }
};

struct BandPassDescription
{
  static Kind getKind () { return kindHighPass; }
  static const char* getName() { return "Butterworth Band Pass"; }
};

struct BandStopDescription
{
  static Kind getKind () { return kindHighPass; }
  static const char* getName() { return "Butterworth Band Stop"; }
};

struct LowShelfDescription
{
  static Kind getKind () { return kindLowShelf; }
  static const char* getName() { return "Butterworth Low Shelf"; }
};

struct HighShelfDescription
{
  static Kind getKind () { return kindHighShelf; }
  static const char* getName() { return "Butterworth High Shelf"; }
};

struct BandShelfDescription
{
  static Kind getKind () { return kindBandShelf; }
  static const char* getName() { return "Butterworth Band Shelf"; }
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
struct LowPass : OrderBase <MaxOrder, TypeI, Butterworth::LowPass>,
                 LowPassDescription
{
};

template <int MaxOrder>
struct HighPass : OrderBase <MaxOrder, TypeI, Butterworth::HighPass>,
                  HighPassDescription
{
};

template <int MaxOrder>
struct BandPass : OrderBase <MaxOrder, TypeII, Butterworth::BandPass>,
                  BandPassDescription
{
};

template <int MaxOrder>
struct BandStop : OrderBase <MaxOrder, TypeII, Butterworth::BandStop>,
                  BandStopDescription
{
};

template <int MaxOrder>
struct LowShelf : OrderBase <MaxOrder, TypeIII, Butterworth::LowShelf>,
                  LowShelfDescription
{
};

template <int MaxOrder>
struct HighShelf : OrderBase <MaxOrder, TypeIII, Butterworth::HighShelf>,
                   HighShelfDescription
{
};

template <int MaxOrder>
struct BandShelf : OrderBase <MaxOrder, TypeIV, Butterworth::BandShelf>,
                   BandShelfDescription
{
};

}

}

}

namespace Dsp {

    namespace Butterworth {

        AnalogLowPass::AnalogLowPass ()
                : m_numPoles (-1)
        {
            setNormal (0, 1);
        }

        void AnalogLowPass::design (int numPoles)
        {
            if (m_numPoles != numPoles)
            {
                m_numPoles = numPoles;

                reset ();

                const double n2 = 2 * numPoles;
                const int pairs = numPoles / 2;
                for (int i = 0; i < pairs; ++i)
                {
                    complex_t c = std::polar (1., doublePi_2 + (2 * i + 1) * doublePi / n2);
                    addPoleZeroConjugatePairs (c, infinity());
                }

                if (numPoles & 1)
                    add (-1, infinity());
            }
        }

//------------------------------------------------------------------------------

        AnalogLowShelf::AnalogLowShelf ()
                : m_numPoles (-1)
        {
            setNormal (doublePi, 1);
        }

        void AnalogLowShelf::design (int numPoles, double gainDb)
        {
            if (m_numPoles != numPoles ||
                m_gainDb != gainDb)
            {
                m_numPoles = numPoles;
                m_gainDb = gainDb;

                reset ();

                const double n2 = numPoles * 2;
                const double g = pow (pow (10., gainDb/20), 1. / n2);
                const double gp = -1. / g;
                const double gz = -g;

                const int pairs = numPoles / 2;
                for (int i = 1; i <= pairs; ++i)
                {
                    const double theta = doublePi * (0.5 - (2 * i - 1) / n2);
                    addPoleZeroConjugatePairs (std::polar (gp, theta), std::polar (gz, theta));
                }

                if (numPoles & 1)
                    add (gp, gz);
            }
        }

//------------------------------------------------------------------------------

        void LowPassBase::setup (int order,
                                 double sampleRate,
                                 double cutoffFrequency)
        {
            m_analogProto.design (order);

            LowPassTransform (cutoffFrequency / sampleRate,
                              m_digitalProto,
                              m_analogProto);

            Cascade::setLayout (m_digitalProto);
        }

        void HighPassBase::setup (int order,
                                  double sampleRate,
                                  double cutoffFrequency)
        {
            m_analogProto.design (order);

            HighPassTransform (cutoffFrequency / sampleRate,
                               m_digitalProto,
                               m_analogProto);

            Cascade::setLayout (m_digitalProto);
        }

        void BandPassBase::setup (int order,
                                  double sampleRate,
                                  double centerFrequency,
                                  double widthFrequency)
        {
            m_analogProto.design (order);

            BandPassTransform (centerFrequency / sampleRate,
                               widthFrequency / sampleRate,
                               m_digitalProto,
                               m_analogProto);

            Cascade::setLayout (m_digitalProto);
        }

        void BandStopBase::setup (int order,
                                  double sampleRate,
                                  double centerFrequency,
                                  double widthFrequency)
        {
            m_analogProto.design (order);

            BandStopTransform (centerFrequency / sampleRate,
                               widthFrequency / sampleRate,
                               m_digitalProto,
                               m_analogProto);

            Cascade::setLayout (m_digitalProto);
        }

        void LowShelfBase::setup (int order,
                                  double sampleRate,
                                  double cutoffFrequency,
                                  double gainDb)
        {
            m_analogProto.design (order, gainDb);

            LowPassTransform (cutoffFrequency / sampleRate,
                              m_digitalProto,
                              m_analogProto);

            Cascade::setLayout (m_digitalProto);
        }

        void HighShelfBase::setup (int order,
                                   double sampleRate,
                                   double cutoffFrequency,
                                   double gainDb)
        {
            m_analogProto.design (order, gainDb);

            HighPassTransform (cutoffFrequency / sampleRate,
                               m_digitalProto,
                               m_analogProto);

            Cascade::setLayout (m_digitalProto);
        }

        void BandShelfBase::setup (int order,
                                   double sampleRate,
                                   double centerFrequency,
                                   double widthFrequency,
                                   double gainDb)
        {
            m_analogProto.design (order, gainDb);

            BandPassTransform (centerFrequency / sampleRate,
                               widthFrequency / sampleRate,
                               m_digitalProto,
                               m_analogProto);

            // HACK!
            m_digitalProto.setNormal (((centerFrequency/sampleRate) < 0.25) ? doublePi : 0, 1);

            Cascade::setLayout (m_digitalProto);
        }

    }

}

#endif

