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

#ifndef DSPFILTERS_PARAMS_H
#define DSPFILTERS_PARAMS_H

#include "Common.h"
#include "Types.h"
#include <iomanip>

namespace Dsp {

/*
 * System for abstracting parameterizable filter specifications.
 *
 * This provides a "GUI-friendly" layer to the filters. Note that
 * it is not necessary to use this layer, it is possible to instantiate
 * the filters and their associated processing state directly,
 * and bypass the overhead for this API if it is not needed.
 *
 */

// Unique IDs to help identify parameters
enum ParamID
{
  idSampleRate,
  idFrequency,
  idQ,
  idBandwidth,
  idBandwidthHz,
  idGain,
  idSlope,
  idOrder,
  idRippleDb,
  idStopDb,
  idRolloff,

  idPoleRho,
  idPoleTheta,
  idZeroRho,
  idZeroTheta,

  idPoleReal,
  idZeroReal
};

enum
{
  maxParameters = 8
};

struct Params
{
  void clear ()
  {
    for (int i = 0; i < maxParameters; ++i)
      value[i] = 0;
  }

  double& operator[] (int index)
  {
    return value[index];
  }

  const double& operator[] (int index) const
  {
    return value[index];
  }

  double value[maxParameters];
};

//
// Provides meta-information about a filter parameter
// to achieve run-time introspection.
//
class ParamInfo
{
public:
  typedef double (ParamInfo::*toControlValue_t) (double) const;
  typedef double (ParamInfo::*toNativeValue_t) (double) const;
  typedef std::string (ParamInfo::*toString_t) (double) const;

  // dont use this one
  ParamInfo (); // throws std::logic_error

  ParamInfo (ParamID id,
              const char* szLabel,
              const char* szName,
              double arg1,
              double arg2,
              double defaultNativeValue,
              toControlValue_t toControlValue_proc,
              toNativeValue_t toNativeValue_proc,
              toString_t toString_proc)
                  : m_id (id)
    , m_szLabel (szLabel)
    , m_szName (szName)
    , m_arg1 (arg1)
    , m_arg2 (arg2)
    , m_defaultNativeValue (defaultNativeValue)
    , m_toControlValue (toControlValue_proc)
    , m_toNativeValue (toNativeValue_proc)
    , m_toString (toString_proc)
  {
  }

  // Used to identify well-known parameters (like cutoff frequency)
  ParamID getId () const
  {
    return m_id;
  }

  // Returns a short label suitable for placement on a control
  const char* getLabel () const
  {
    return m_szLabel;
  }

  // Returns the full name
  const char* getName () const
  {
    return m_szName;
  }

  double getDefaultValue () const
  {
    return m_defaultNativeValue;
  }

  //
  // Control value is always in the range [0..1]
  //
  double toControlValue (double nativeValue) const
  {
    return (this->*m_toControlValue) (nativeValue);
  }

  //
  // Native value is in filter-specific units. For example,
  // cutoff frequency would probably be in Hertz.
  //
  double toNativeValue (double controlValue) const
  {
    return (this->*m_toNativeValue) (controlValue);
  }

  std::string toString (double nativeValue) const
  {
    return (this->*m_toString) (nativeValue);
  }

  double clamp (double nativeValue) const;

  //
  // These routines are used as function pointers when
  // constructing the various ParamInfo used by filters
  //

  double Int_toControlValue (double nativeValue) const;
  double Int_toNativeValue (double controlValue) const;

  double Real_toControlValue (double nativeValue) const;
  double Real_toNativeValue (double controlValue) const;

  double Log_toControlValue (double nativeValue) const;
  double Log_toNativeValue (double controlValue) const;

  double Pow2_toControlValue (double nativeValue) const;
  double Pow2_toNativeValue (double controlValue) const;

  std::string Int_toString (double nativeValue) const;
  std::string Hz_toString (double nativeValue) const;
  std::string Real_toString (double nativeValue) const;
  std::string Db_toString (double nativeValue) const;

  //
  // Creates the specified ParamInfo
  //

  static ParamInfo defaultSampleRateParam ();
  static ParamInfo defaultCutoffFrequencyParam ();
  static ParamInfo defaultCenterFrequencyParam ();
  static ParamInfo defaultQParam ();
  static ParamInfo defaultBandwidthParam ();
  static ParamInfo defaultBandwidthHzParam ();
  static ParamInfo defaultGainParam ();
  static ParamInfo defaultSlopeParam ();
  static ParamInfo defaultRippleDbParam ();
  static ParamInfo defaultStopDbParam ();
  static ParamInfo defaultRolloffParam ();
  static ParamInfo defaultPoleRhoParam ();
  static ParamInfo defaultPoleThetaParam ();
  static ParamInfo defaultZeroRhoParam ();
  static ParamInfo defaultZeroThetaParam ();
  static ParamInfo defaultPoleRealParam ();
  static ParamInfo defaultZeroRealParam ();

private:
  ParamID m_id;
  const char* m_szLabel;
  const char* m_szName;
  double m_arg1;
  double m_arg2;
  double m_defaultNativeValue;
  toControlValue_t m_toControlValue;
  toNativeValue_t m_toNativeValue;
  toString_t m_toString;
};

}

namespace Dsp {

    ParamInfo::ParamInfo ()
    {
        throw std::logic_error ("invalid usage of ParamInfo");
    }

    double ParamInfo::clamp (double nativeValue) const
    {
        const double minValue = toNativeValue (0);
        const double maxValue = toNativeValue (1);
        if (nativeValue < minValue)
            nativeValue = minValue;
        else if (nativeValue > maxValue)
            nativeValue = maxValue;
        return nativeValue;
    }

//------------------------------------------------------------------------------

    double ParamInfo::Int_toControlValue (double nativeValue) const
    {
        return (nativeValue - m_arg1) / (m_arg2 - m_arg1);
    }

    double ParamInfo::Int_toNativeValue (double controlValue) const
    {
        return std::floor (m_arg1 + controlValue * (m_arg2 - m_arg1) + 0.5);
    }

//------------------------------------------------------------------------------

    double ParamInfo::Real_toControlValue (double nativeValue) const
    {
        return (nativeValue - m_arg1) / (m_arg2 - m_arg1);
    }

    double ParamInfo::Real_toNativeValue (double controlValue) const
    {
        return m_arg1 + controlValue * (m_arg2 - m_arg1);
    }

//------------------------------------------------------------------------------

    double ParamInfo::Log_toControlValue (double nativeValue) const
    {
        const double base = 1.5;
        double l0 = log (m_arg1) / log (base);
        double l1 = log (m_arg2) / log (base);
        return (log (nativeValue) / log(base) - l0) / (l1 - l0);
    }

    double ParamInfo::Log_toNativeValue (double controlValue) const
    {
        const double base = 1.5;
        double l0 = log (m_arg1) / log (base);
        double l1 = log (m_arg2) / log (base);
        return pow (base, l0 + controlValue * (l1 - l0));
    }

//------------------------------------------------------------------------------

    double ParamInfo::Pow2_toControlValue (double nativeValue) const
    {
        return ((log (nativeValue) / log (2.)) - m_arg1) / (m_arg2 - m_arg1);
    }

    double ParamInfo::Pow2_toNativeValue (double controlValue) const
    {
        return pow (2., (controlValue * (m_arg2 - m_arg1)) + m_arg1);
    }

//------------------------------------------------------------------------------

    std::string ParamInfo::Int_toString (double nativeValue) const
    {
        std::ostringstream os;
        os << int (nativeValue);
        return os.str();
    }

    std::string ParamInfo::Hz_toString (double nativeValue) const
    {
        std::ostringstream os;
        os << int (nativeValue) << " Hz";
        return os.str();
    }

    std::string ParamInfo::Real_toString (double nativeValue) const
    {
        std::ostringstream os;
        os << std::fixed << std::setprecision(3) << nativeValue;
        return os.str();
    }

    std::string ParamInfo::Db_toString (double nativeValue) const
    {
        const double af = fabs (nativeValue);
        int prec;
        if (af < 1)
            prec = 3;
        else if (af < 10)
            prec = 2;
        else
            prec = 1;
        std::ostringstream os;
        os << std::fixed << std::setprecision (prec) << nativeValue << " dB";
        return os.str();
    }

//------------------------------------------------------------------------------

    ParamInfo ParamInfo::defaultSampleRateParam ()
    {
        return ParamInfo (idSampleRate, "Fs", "Sample Rate",
                          11025, 192000, 44100,
                          &ParamInfo::Real_toControlValue,
                          &ParamInfo::Real_toNativeValue,
                          &ParamInfo::Hz_toString);
    }

    ParamInfo ParamInfo::defaultCutoffFrequencyParam ()
    {
        return ParamInfo (idFrequency, "Fc", "Cutoff Frequency",
                          10, 22040, 2000,
                          &ParamInfo::Log_toControlValue,
                          &ParamInfo::Log_toNativeValue,
                          &ParamInfo::Hz_toString);
    }

    ParamInfo ParamInfo::defaultCenterFrequencyParam ()
    {
        return ParamInfo (idFrequency, "Fc", "Center Frequency",
                          10, 22040, 2000,
                          &ParamInfo::Log_toControlValue,
                          &ParamInfo::Log_toNativeValue,
                          &ParamInfo::Hz_toString);
    }

    ParamInfo ParamInfo::defaultQParam ()
    {
        return ParamInfo (idQ, "Q", "Resonance",
                          -4, 4, 1,
                          &ParamInfo::Pow2_toControlValue,
                          &ParamInfo::Pow2_toNativeValue,
                          &ParamInfo::Real_toString);
    }

    ParamInfo ParamInfo::defaultBandwidthParam ()
    {
        return ParamInfo (idBandwidth, "BW", "Bandwidth (Octaves)",
                          -4, 4, 1,
                          &ParamInfo::Pow2_toControlValue,
                          &ParamInfo::Pow2_toNativeValue,
                          &ParamInfo::Real_toString);
    }

    ParamInfo ParamInfo::defaultBandwidthHzParam ()
    {
        return ParamInfo (idBandwidthHz, "BW", "Bandwidth (Hz)",
                          10, 22040, 1720,
                          &ParamInfo::Log_toControlValue,
                          &ParamInfo::Log_toNativeValue,
                          &ParamInfo::Hz_toString);
    }

    ParamInfo ParamInfo::defaultGainParam ()
    {
        return ParamInfo (idGain, "Gain", "Gain",
                          -24, 24, -6,
                          &ParamInfo::Real_toControlValue,
                          &ParamInfo::Real_toNativeValue,
                          &ParamInfo::Db_toString);
    }

    ParamInfo ParamInfo::defaultSlopeParam ()
    {
        return ParamInfo (idSlope, "Slope", "Slope",
                          -2, 2, 1,
                          &ParamInfo::Pow2_toControlValue,
                          &ParamInfo::Pow2_toNativeValue,
                          &ParamInfo::Real_toString);
    }

    ParamInfo ParamInfo::defaultRippleDbParam ()
    {
        return ParamInfo (idRippleDb, "Ripple", "Ripple dB",
                          0.001, 12, 0.01,
                          &ParamInfo::Real_toControlValue,
                          &ParamInfo::Real_toNativeValue,
                          &ParamInfo::Db_toString);
    }

    ParamInfo ParamInfo::defaultStopDbParam ()
    {
        return ParamInfo (idStopDb, "Stop", "Stopband dB",
                          3, 60, 48,
                          &ParamInfo::Real_toControlValue,
                          &ParamInfo::Real_toNativeValue,
                          &ParamInfo::Db_toString);
    }

    ParamInfo ParamInfo::defaultRolloffParam ()
    {
        return ParamInfo (idRolloff, "W", "Transition Width",
                          -16, 4, 0,
                          &ParamInfo::Real_toControlValue,
                          &ParamInfo::Real_toNativeValue,
                          &ParamInfo::Real_toString);
    }

    ParamInfo ParamInfo::defaultPoleRhoParam ()
    {
        return ParamInfo (idPoleRho, "Pd", "Pole Distance",
                          0, 1, 0.5,
                          &ParamInfo::Real_toControlValue,
                          &ParamInfo::Real_toNativeValue,
                          &ParamInfo::Real_toString);
    }

    ParamInfo ParamInfo::defaultPoleThetaParam ()
    {
        return ParamInfo (idPoleTheta, "Pa", "Pole Angle",
                          0, doublePi, doublePi/2,
                          &ParamInfo::Real_toControlValue,
                          &ParamInfo::Real_toNativeValue,
                          &ParamInfo::Real_toString);
    }

    ParamInfo ParamInfo::defaultZeroRhoParam ()
    {
        return ParamInfo (idZeroRho, "Pd", "Zero Distance",
                          0, 1, 0.5,
                          &ParamInfo::Real_toControlValue,
                          &ParamInfo::Real_toNativeValue,
                          &ParamInfo::Real_toString);
    }

    ParamInfo ParamInfo::defaultZeroThetaParam ()
    {
        return ParamInfo (idZeroTheta, "Pa", "Zero Angle",
                          0, doublePi, doublePi/2,
                          &ParamInfo::Real_toControlValue,
                          &ParamInfo::Real_toNativeValue,
                          &ParamInfo::Real_toString);
    }

    ParamInfo ParamInfo::defaultPoleRealParam ()
    {
        return ParamInfo (idPoleReal, "A1", "Pole Real",
                          -1, 1, 0.25,
                          &ParamInfo::Real_toControlValue,
                          &ParamInfo::Real_toNativeValue,
                          &ParamInfo::Real_toString);
    }

    ParamInfo ParamInfo::defaultZeroRealParam ()
    {
        return ParamInfo (idZeroReal, "B1", "Zero Real",
                          -1, 1, -0.25,
                          &ParamInfo::Real_toControlValue,
                          &ParamInfo::Real_toNativeValue,
                          &ParamInfo::Real_toString);
    }
}

#endif
