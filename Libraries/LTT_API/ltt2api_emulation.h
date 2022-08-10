// *******************************************
// Copyright (C) 2004-2012 by LTT GmbH
//
// FILE:      ltt2api_emulation.h
//
// DESCR:     LTT2API - Header
//
// AUTHOR:    STK <stefan.knorr@tasler.de>
//
// VERSION:   3.0.x
//
// *******************************************

#ifndef __LTT2API_EMULATION_API_H__
#define __LTT2API_EMULATION_API_H__

#include "lttosal.h"
#include "ltt2api_export.h"

#ifdef __cplusplus
extern "C" {
#endif


enum LTT2APAI_DATAGEN_SIGTYPE {
  LTTDG_SIG_DC              = 0,
  LTTDG_SIG_RECTANGLE       = 1,
  LTTDG_SIG_SINUS           = 2,
  LTTDG_SIG_TRIANGLE        = 3,
  LTTDG_SIG_TRIANGLE_SWEEP  = 8,
  LTTDG_SIG_RECTANGLE_SWEEP = 9,
  LTTDG_SIG_SINUS_SWEEP     = 10,
  LTTDG_SIG_PEAK            = 16,
  LTTDG_SIG_XFER_TEST       = 64,
  LTTDG_SIG_XFER_CONST      = 65
};


/*****************************************

  FUNCTION: LTTSetDeviceEmulation

  DESCR.:   Enables/Disables device
              emulation.
              Default: OFF

  INPUT:    Amount of LTT-184-16 devices
              to emulate
              
              0 - Emulation OFF
             >0 - Emulate n devices

  OUTPUT:        0 - allways

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTSetDeviceEmulation( int32_t num );


/*****************************************

  FUNCTION: LTTDeviceEmulationActive

  DESCR.:   Checks statis of device 
              emulation

  INPUT:    

  OUTPUT:        0 - no emulation
             >0 - emulation active

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTDeviceEmulationActive();


/*****************************************

  FUNCTION: LTTSetDeviceEmulationSignal

  DESCR.:

  INPUT:    type:
                  Signal type

                  0 - DC signal
                  1 - rectangle signal
                  2 - sinus signal
                  3 - triangle signal
                  8 - triangle-sweep signal
                  9 - rectangle-sweep signal
                 10 - sinus-sweep signal
                 10 - peak signal
                 64 - test pattern

              p_emupara:
                  paramter vector for signal

                  [0] - timebase-start 0
                  [1] - amplitude 1
                  [2] - offset 0
                  [3] - timebase-end 1s
                  [4] - sweep-time 0

  OUTPUT:        0 - allways

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTSetDeviceEmulationSignal( int32_t type,
                                                            int32_t *p_emupara );


#ifdef __cplusplus
}
#endif

#endif
 
