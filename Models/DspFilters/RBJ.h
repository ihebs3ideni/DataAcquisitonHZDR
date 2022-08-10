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

#ifndef DSPFILTERS_RBJ_H
#define DSPFILTERS_RBJ_H

#include "Common.h"
#include "Biquad.h"
#include "Design.h"
#include "Filter.h"

namespace Dsp {

/*
 * Filter realizations based on Robert Bristol-Johnson formulae:
 *
 * http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
 *
 */

namespace RBJ {

//
// Raw filters
//

struct LowPass : BiquadBase
{
  void setup (double sampleRate,
              double cutoffFrequency,
              double q);
};

struct HighPass : BiquadBase
{
  void setup (double sampleRate,
              double cutoffFrequency,
              double q);
};

struct BandPass1 : BiquadBase
{
  // (constant skirt gain, peak gain = Q)
  void setup (double sampleRate,
              double centerFrequency,
              double bandWidth);
};

struct BandPass2 : BiquadBase
{
  // (constant 0 dB peak gain)
  void setup (double sampleRate,
              double centerFrequency,
              double bandWidth);
};

struct BandStop : BiquadBase
{
  void setup (double sampleRate,
              double centerFrequency,
              double bandWidth);
};

struct LowShelf : BiquadBase
{
  void setup (double sampleRate,
              double cutoffFrequency,
              double gainDb,
              double shelfSlope);
};

struct HighShelf : BiquadBase
{
  void setup (double sampleRate,
              double cutoffFrequency,
              double gainDb,
              double shelfSlope);
};

struct BandShelf : BiquadBase
{
  void setup (double sampleRate,
              double centerFrequency,
              double gainDb,
              double bandWidth);
};

struct AllPass : BiquadBase
{
  void setup (double sampleRate,
              double phaseFrequency,
              double q);
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

  static const ParamInfo getParamInfo_1 ()
  {
    return ParamInfo::defaultCutoffFrequencyParam ();
  }

  static const ParamInfo getParamInfo_2 ()
  {
    return ParamInfo::defaultQParam ();
  }
};

template <class FilterClass>
struct TypeI : TypeIBase, FilterClass
{
  void setParams (const Params& params)
  {
    FilterClass::setup (params[0], params[1], params[2]);
  }
};

struct TypeIIBase : DesignBase
{
  enum
  {
    NumParams = 3
  };

  static int getNumParams ()
  {
    return 3;
  }

  static const ParamInfo getParamInfo_1 ()
  {
    return ParamInfo::defaultCenterFrequencyParam ();
  }

  static const ParamInfo getParamInfo_2 ()
  {
    return ParamInfo::defaultBandwidthParam ();
  }
};

template <class FilterClass>
struct TypeII : TypeIIBase, FilterClass
{
  void setParams (const Params& params)
  {
    FilterClass::setup (params[0], params[1], params[2]);
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

  static const ParamInfo getParamInfo_1 ()
  {
    return ParamInfo::defaultCutoffFrequencyParam ();
  }

  static const ParamInfo getParamInfo_2 ()
  {
    return ParamInfo::defaultGainParam ();
  }

  static const ParamInfo getParamInfo_3 ()
  {
    return ParamInfo::defaultSlopeParam ();
  }
};

template <class FilterClass>
struct TypeIII : TypeIIIBase, FilterClass
{
  void setParams (const Params& params)
  {
    FilterClass::setup (params[0], params[1], params[2], params[3]);
  }
};

struct TypeIVBase : DesignBase
{
  enum
  {
    NumParams = 4
  };

  static int getNumParams ()
  {
    return 4;
  }

  static const ParamInfo getParamInfo_1 ()
  {
    return ParamInfo::defaultCenterFrequencyParam ();
  }

  static const ParamInfo getParamInfo_2 ()
  {
    return ParamInfo::defaultGainParam ();
  }

  static const ParamInfo getParamInfo_3 ()
  {
    return ParamInfo::defaultBandwidthParam ();
  }
};

template <class FilterClass>
struct TypeIV : TypeIVBase , FilterClass
{
  void setParams (const Params& params)
  {
    FilterClass::setup (params[0], params[1], params[2], params[3]);
  }
};

//------------------------------------------------------------------------------

struct LowPass : TypeI <RBJ::LowPass>
{
  static Kind getKind () { return kindLowPass; }
  static const char* getName() { return "RBJ Low Pass"; }
};

struct HighPass : TypeI <RBJ::HighPass>
{
  static Kind getKind () { return kindHighPass; }
  static const char* getName() { return "RBJ High Pass"; }
};

struct BandPass1 : TypeII <RBJ::BandPass1>
{
  static Kind getKind () { return kindBandPass; }
  static const char* getName() { return "RBJ Band Pass 1"; }
};

struct BandPass2 : TypeII <RBJ::BandPass2>
{
  static Kind getKind () { return kindBandPass; }
  static const char* getName() { return "RBJ Band Pass 2"; }
};

struct BandStop : TypeII <RBJ::BandStop>
{
  static Kind getKind () { return kindBandStop; }
  static const char* getName() { return "RBJ Band Stop"; }
};

struct LowShelf : TypeIII <RBJ::LowShelf>
{
  static Kind getKind () { return kindLowShelf; }
  static const char* getName() { return "RBJ Low Shelf"; }
};

struct HighShelf : TypeIII <RBJ::HighShelf>
{
  static Kind getKind () { return kindHighShelf; }
  static const char* getName() { return "RBJ High Shelf"; }
};

struct BandShelf : TypeIV <RBJ::BandShelf>
{
  static Kind getKind () { return kindBandShelf; }
  static const char* getName() { return "RBJ Band Shelf"; }
};

struct AllPass : TypeI <RBJ::AllPass>
{
  static Kind getKind () { return kindOther; }
  static const char* getName() { return "RBJ All Pass"; }
};

}

}

}

namespace Dsp {

    namespace RBJ {

        void LowPass::setup (double sampleRate,
                             double cutoffFrequency,
                             double q)
        {
            double w0 = 2 * doublePi * cutoffFrequency / sampleRate;
            double cs = cos (w0);
            double sn = sin (w0);
            double AL = sn / (2 * q);
            double b0 = (1 - cs) / 2;
            double b1 =  1 - cs;
            double b2 = (1 - cs) / 2;
            double a0 =  1 + AL;
            double a1 = -2 * cs;
            double a2 =  1 - AL;
            setCoefficients (a0, a1, a2, b0, b1, b2);
        }

        void HighPass::setup (double sampleRate,
                              double cutoffFrequency,
                              double q)
        {
            double w0 = 2 * doublePi * cutoffFrequency / sampleRate;
            double cs = cos (w0);
            double sn = sin (w0);
            double AL = sn / ( 2 * q );
            double b0 =  (1 + cs) / 2;
            double b1 = -(1 + cs);
            double b2 =  (1 + cs) / 2;
            double a0 =  1 + AL;
            double a1 = -2 * cs;
            double a2 =  1 - AL;
            setCoefficients (a0, a1, a2, b0, b1, b2);
        }

        void BandPass1::setup (double sampleRate,
                               double centerFrequency,
                               double bandWidth)
        {
            double w0 = 2 * doublePi * centerFrequency / sampleRate;
            double cs = cos (w0);
            double sn = sin (w0);
            double AL = sn / ( 2 * bandWidth );
            double b0 = bandWidth * AL;// sn / 2;
            double b1 =  0;
            double b2 = -bandWidth * AL;//-sn / 2;
            double a0 =  1 + AL;
            double a1 = -2 * cs;
            double a2 =  1 - AL;
            setCoefficients (a0, a1, a2, b0, b1, b2);
        }

        void BandPass2::setup (double sampleRate,
                               double centerFrequency,
                               double bandWidth)
        {
            double w0 = 2 * doublePi * centerFrequency / sampleRate;
            double cs = cos (w0);
            double sn = sin (w0);
            double AL = sn / ( 2 * bandWidth );
            double b0 =  AL;
            double b1 =  0;
            double b2 = -AL;
            double a0 =  1 + AL;
            double a1 = -2 * cs;
            double a2 =  1 - AL;
            setCoefficients (a0, a1, a2, b0, b1, b2);
        }

        void BandStop::setup (double sampleRate,
                              double centerFrequency,
                              double bandWidth)
        {
            double w0 = 2 * doublePi * centerFrequency / sampleRate;
            double cs = cos (w0);
            double sn = sin (w0);
            double AL = sn / ( 2 * bandWidth );
            double b0 =  1;
            double b1 = -2 * cs;
            double b2 =  1;
            double a0 =  1 + AL;
            double a1 = -2 * cs;
            double a2 =  1 - AL;
            setCoefficients (a0, a1, a2, b0, b1, b2);
        }

        void LowShelf::setup (double sampleRate,
                              double cutoffFrequency,
                              double gainDb,
                              double shelfSlope)
        {
            double A  = pow (10, gainDb/40);
            double w0 = 2 * doublePi * cutoffFrequency / sampleRate;
            double cs = cos (w0);
            double sn = sin (w0);
            double AL = sn / 2 * ::std::sqrt ((A + 1/A) * (1/shelfSlope - 1) + 2);
            double sq = 2 * sqrt(A) * AL;
            double b0 =    A*( (A+1) - (A-1)*cs + sq );
            double b1 =  2*A*( (A-1) - (A+1)*cs );
            double b2 =    A*( (A+1) - (A-1)*cs - sq );
            double a0 =        (A+1) + (A-1)*cs + sq;
            double a1 =   -2*( (A-1) + (A+1)*cs );
            double a2 =        (A+1) + (A-1)*cs - sq;
            setCoefficients (a0, a1, a2, b0, b1, b2);
        }

        void HighShelf::setup (double sampleRate,
                               double cutoffFrequency,
                               double gainDb,
                               double shelfSlope)
        {
            double A  = pow (10, gainDb/40);
            double w0 = 2 * doublePi * cutoffFrequency / sampleRate;
            double cs = cos (w0);
            double sn = sin (w0);
            double AL = sn / 2 * ::std::sqrt ((A + 1/A) * (1/shelfSlope - 1) + 2);
            double sq = 2 * sqrt(A) * AL;
            double b0 =    A*( (A+1) + (A-1)*cs + sq );
            double b1 = -2*A*( (A-1) + (A+1)*cs );
            double b2 =    A*( (A+1) + (A-1)*cs - sq );
            double a0 =        (A+1) - (A-1)*cs + sq;
            double a1 =    2*( (A-1) - (A+1)*cs );
            double a2 =        (A+1) - (A-1)*cs - sq;
            setCoefficients (a0, a1, a2, b0, b1, b2);
        }

        void BandShelf::setup (double sampleRate,
                               double centerFrequency,
                               double gainDb,
                               double bandWidth)
        {
            double A  = pow (10, gainDb/40);
            double w0 = 2 * doublePi * centerFrequency / sampleRate;
            double cs = cos(w0);
            double sn = sin(w0);
            double AL = sn * sinh( doubleLn2/2 * bandWidth * w0/sn );
            assert (!Dsp::is_nan (AL));
            double b0 =  1 + AL * A;
            double b1 = -2 * cs;
            double b2 =  1 - AL * A;
            double a0 =  1 + AL / A;
            double a1 = -2 * cs;
            double a2 =  1 - AL / A;
            setCoefficients (a0, a1, a2, b0, b1, b2);
        }

        void AllPass::setup (double sampleRate,
                             double phaseFrequency,
                             double q)
        {
            double w0 = 2 * doublePi * phaseFrequency / sampleRate;
            double cs = cos (w0);
            double sn = sin (w0);
            double AL = sn / ( 2 * q );
            double b0 =  1 - AL;
            double b1 = -2 * cs;
            double b2 =  1 + AL;
            double a0 =  1 + AL;
            double a1 = -2 * cs;
            double a2 =  1 - AL;
            setCoefficients (a0, a1, a2, b0, b1, b2);
        }

    }

}

#endif
