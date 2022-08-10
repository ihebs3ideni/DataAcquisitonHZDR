// *******************************************
// Copyright (C) 2004-2012 by LTT GmbH
//
// FILE:      ltt2api.h
//
// DESCR:     LTT2API - Header
//
// AUTHOR:    STK <stefan.knorr@tasler.de>
//
// VERSION:   3.0.x
//
// *******************************************


#ifndef __LTT2API_H__
#define __LTT2API_H__

#include "lttosal.h"
#include "lttosal_time.h"

#include "ltt2api_export.h"

#ifdef __cplusplus
extern "C" {
#endif


// samplerate:
enum LTT_TB_ExternalClock {
  LTT_EXTCLOCK_16BIT                = 0x00000000,
  LTT_EXTCLOCK_15BIT                = 0x00000001,
  LTT_EXTCLOCK_14BIT                = 0x00000002,
  LTT_EXTCLOCK_12BIT                = 0x00000003,

  LTT_24_EXTCLK_MAX__57KHZ          = 0x00000000,
  LTT_24_EXTCLK_MAX_107KHZ          = 0x00000001,
  LTT_24_EXTCLK_MAX_188KHZ          = 0x00000002,
  LTT_24_EXTCLK_MAX_277KHZ          = 0x00000003

};

enum LTT_TB_Flags {
  LTT_SMPLTIME_FLAG_ABOVE           = 0x0000000,
  LTT_SMPLTIME_FLAG_BELOW           = 0x0000001,
  LTT_SMPLTIME_FLAG_CALIB           = 0x0001000,

  LTT_TIMEBASE_MASK                 = 0x0FFFFFFF,
  LTT_TIMEBASE_SLAVEMODE            = 0x10000000,
  LTT_TIMEBASE_MODE_MASK            = 0xF0000000
};


// transfer:
enum LTT_TransferMode {
  LTT_TRANSFER_BLOCKED              = 0x00000000,
  LTT_TRANSFER_NONEBLOCKED          = 0x00000001,
  LTT_TRANSFER_CONTINUOUS           = 0x00000004,
  LTT_TRANSFER_HIGHPRECISION        = 0x00000008
};


// trigger:
enum LTT_TriggerMode {
  LTT_TRIG_MODE_LEVEL               = 0x00000000,
  LTT_TRIG_MODE_COMPARISION         = 0x00000001,
  LTT_TRIG_MODE_REGION              = 0x00000002,
  LTT_TRIG_MODE_LEVEL_DELTA         = 0x00000004,

  LTT_TRIG_DIGMODE_SINGLE_BIT       = 0x00000000
};

enum LTT_TriggerState {
  LTT_TRIG_STATE_OFF                = 0x00000000,
  LTT_TRIG_STATE_ON                = 0x00000001
};

enum LTT_TriggerType {
  LTT_TRIG_TYPE_SINGLE              = 0x00000000,
  LTT_TRIG_TYPE_TRIGTIMER           = 0x00000001,
  LTT_TRIG_TYPE_MISSING_EVENT       = 0x00000002,
  LTT_TRIG_TYPE_ABA_MODE            = 0x00000010
};

enum LTT_TriggerSlope {
  LTT_TRIG_SLOPE_POS                = 0x00000000,
  LTT_TRIG_SLOPE_NEG                = 0x00000001,
  LTT_TRIG_COMP_BIGGER              = LTT_TRIG_SLOPE_POS,
  LTT_TRIG_COMP_SMALLER             = LTT_TRIG_SLOPE_NEG,
  LTT_TRIG_REGION_ENTER             = LTT_TRIG_SLOPE_POS,
  LTT_TRIG_REGION_EXIT              = LTT_TRIG_SLOPE_NEG
};

// channels:
enum LTT_ChannelType {
  LTT_CHNL_TYPE_ANALOG              = 0x00000000,
  LTT_CHNL_TYPE_DIGITAL             = 0x00000001,
  LTT_CHNL_TYPE_RPM                 = 0x00000002
};

enum LTT_ChannelIDBit {
  LTT_CHNL_IDBIT_ANALOG             = 0x00000000,
  LTT_CHNL_IDBIT_DIGITAL            = 0x00001000,
  LTT_CHNL_IDBIT_RPM                = 0x00002000
};

enum LTT_ChannelCoupling {
  // LTTAnalogChannel()
  LTT_CHNL_COUP_GND                  = 0x00000000,

  // -- LTT-184 -------------------------------------
  LTT_CHNL_184_COUP_SE_DC_PLUS       = 0x00001110,
  LTT_CHNL_184_COUP_SE_DC_MINUS      = 0x00002110,
  LTT_CHNL_184_COUP_DE_DC            = 0x00003110,
  LTT_CHNL_184_COUP_SE_AC_PLUS       = 0x00001210,
  LTT_CHNL_184_COUP_SE_AC_MINUS      = 0x00002210,
  LTT_CHNL_184_COUP_DE_AC            = 0x00003210,
  LTT_CHNL_184_COUP_SE_DC_PLUS_1M    = 0x00051110,
  LTT_CHNL_184_COUP_SE_DC_MINUS_1M   = 0x00052110,
  LTT_CHNL_184_COUP_DE_DC_1M         = 0x00053110,

  // -- LTT-180 -------------------------------------
  LTT_CHNL_180_COUP_VOLT_DC          = 0x00001110,
  LTT_CHNL_180_COUP_VOLT_AC          = 0x00001210,
  LTT_CHNL_180_COUP_ICP_AC           = 0x00001220,
  LTT_CHNL_180_COUP_DMS_FULL         = 0x00003150,
  LTT_CHNL_180_COUP_DMS_HALF         = 0x00001160,
  LTT_CHNL_180_COUP_DMS_ZERO         = 0x00003140,
  LTT_CHNL_180_COUP_DMS_FULL_CAL     = 0x00103150,
  LTT_CHNL_180_COUP_DMS_HALF_CAL     = 0x00101160,
  LTT_CHNL_180_COUP_DMS_CURRENT      = 0x00003280,
  LTT_CHNL_180_COUP_DMS_QTR          = 0x00001170,
  LTT_CHNL_180_COUP_DMS_QTR_CAL      = 0x00101170,
  LTT_CHNL_180_COUP_DMS_FULL_INV     = 0x01003150,
  LTT_CHNL_180_COUP_DMS_HALF_INV     = 0x01001160,
  LTT_CHNL_180_COUP_DMS_QTR_INV      = 0x01001170,

  // -- LTT24 ---------------------------------------
  // VOLT
  LTT_CHNL_24_COUP_VOLT_SE_DC        = 0x00001110,
  LTT_CHNL_24_COUP_VOLT_SE_DC_BNC    = 0x00011110,
  LTT_CHNL_24_COUP_VOLT_DE_DC        = 0x00003110,
  LTT_CHNL_24_COUP_VOLT_DE_DC_BNC    = 0x00013110,
  LTT_CHNL_24_COUP_VOLT_SE_AC        = 0x00001210,
  LTT_CHNL_24_COUP_VOLT_SE_AC_BNC    = 0x00011210,
  LTT_CHNL_24_COUP_VOLT_DE_AC        = 0x00003210,
  LTT_CHNL_24_COUP_VOLT_DE_AC_BNC    = 0x00013210,

  // IEPE
  LTT_CHNL_24_COUP_IEPE_SE_DC        = 0x00001120,
  LTT_CHNL_24_COUP_IEPE_SE_DC_BNC    = 0x00011120,
  LTT_CHNL_24_COUP_IEPE_SE_DC_3WIRE  = 0x00061120,
  LTT_CHNL_24_COUP_IEPE_DE_DC_4WIRE  = 0x00073120,
  LTT_CHNL_24_COUP_IEPE_SE_AC        = 0x00001220,
  LTT_CHNL_24_COUP_IEPE_SE_AC_BNC    = 0x00011220,
  LTT_CHNL_24_COUP_IEPE_SE_AC_3WIRE  = 0x00061220,
  LTT_CHNL_24_COUP_IEPE_DE_AC_4WIRE  = 0x00073220,

  // BRDG-FULL
  LTT_CHNL_24_COUP_BRDG_FULL_DE_DC            = 0x00003150,
  LTT_CHNL_24_COUP_BRDG_FULL_DE_DC_CAL        = 0x00103150,
  LTT_CHNL_24_COUP_BRDG_FULL_DE_DC_SENSE      = 0x00203150,
  LTT_CHNL_24_COUP_BRDG_FULL_DE_DC_SENSE_CAL  = 0x00303150,
  LTT_CHNL_24_COUP_BRDG_FULL_DE_AC            = 0x00003250,
  LTT_CHNL_24_COUP_BRDG_FULL_DE_AC_CAL        = 0x00103250,
  LTT_CHNL_24_COUP_BRDG_FULL_DE_AC_SENSE      = 0x00203250,
  LTT_CHNL_24_COUP_BRDG_FULL_DE_AC_SENSE_CAL  = 0x00303250,

  // BRDG-HALF
  LTT_CHNL_24_COUP_BRDG_HALF_SE_DC            = 0x00001150,
  LTT_CHNL_24_COUP_BRDG_HALF_SE_DC_CAL        = 0x00101150,
  LTT_CHNL_24_COUP_BRDG_HALF_SE_DC_SENSE      = 0x00201150,
  LTT_CHNL_24_COUP_BRDG_HALF_SE_DC_SENSE_CAL  = 0x00301150,
  LTT_CHNL_24_COUP_BRDG_HALF_SE_AC            = 0x00001250,
  LTT_CHNL_24_COUP_BRDG_HALF_SE_AC_CAL        = 0x00101250,
  LTT_CHNL_24_COUP_BRDG_HALF_SE_AC_SENSE      = 0x00201250,
  LTT_CHNL_24_COUP_BRDG_HALF_SE_AC_SENSE_CAL  = 0x00301250,

  // BRDG-QUARTER
  LTT_CHNL_24_COUP_BRDG_QRTR_SE_DC_R350            = 0x00021170,
  LTT_CHNL_24_COUP_BRDG_QRTR_SE_DC_R350_CAL        = 0x00121170,
  LTT_CHNL_24_COUP_BRDG_QRTR_SE_DC_R350_SENSE      = 0x00221170,
  LTT_CHNL_24_COUP_BRDG_QRTR_SE_DC_R350_SENSE_CAL  = 0x00321170,
  LTT_CHNL_24_COUP_BRDG_QRTR_SE_DC_R120            = 0x00031170,
  LTT_CHNL_24_COUP_BRDG_QRTR_SE_DC_R120_CAL        = 0x00131170,
  LTT_CHNL_24_COUP_BRDG_QRTR_SE_DC_R120_SENSE      = 0x00231170,
  LTT_CHNL_24_COUP_BRDG_QRTR_SE_DC_R120_SENSE_CAL  = 0x00331170,
  LTT_CHNL_24_COUP_BRDG_QRTR_SE_AC_R350            = 0x00021270,
  LTT_CHNL_24_COUP_BRDG_QRTR_SE_AC_R350_CAL        = 0x00121270,
  LTT_CHNL_24_COUP_BRDG_QRTR_SE_AC_R350_SENSE      = 0x00221270,
  LTT_CHNL_24_COUP_BRDG_QRTR_SE_AC_R350_SENSE_CAL  = 0x00321270,
  LTT_CHNL_24_COUP_BRDG_QRTR_SE_AC_R120            = 0x00031270,
  LTT_CHNL_24_COUP_BRDG_QRTR_SE_AC_R120_CAL        = 0x00131270,
  LTT_CHNL_24_COUP_BRDG_QRTR_SE_AC_R120_SENSE      = 0x00231270,
  LTT_CHNL_24_COUP_BRDG_QRTR_SE_AC_R120_SENSE_CAL  = 0x00331270,

  // CURRENT
  LTT_CHNL_24_COUP_CURRENT_SE_DC                   = 0x00001190,
  LTT_CHNL_24_COUP_CURRENT_SE_DC_BNC               = 0x00011190,
  LTT_CHNL_24_COUP_CURRENT_SE_AC                   = 0x00001290,
  LTT_CHNL_24_COUP_CURRENT_SE_AC_BNC               = 0x00011290,

  // LVDT
  LTT_CHNL_24_COUP_LVDT_DE_DC_4WIRE                = 0x000731A0,
  LTT_CHNL_24_COUP_LVDT_DE_DC_4WIRE_PHASE          = 0x001731A0,
  LTT_CHNL_24_COUP_LVDT_DE_AC_4WIRE                = 0x000732A0,
  LTT_CHNL_24_COUP_LVDT_DE_AC_4WIRE_PHASE          = 0x001732A0,

  // CHARGE
  LTT_CHNL_24_COUP_CHARGE_SE                       = 0x00001030,
  LTT_CHNL_24_COUP_CHARGE_SE_BNC                   = 0x00011030,
  LTT_CHNL_24_COUP_CHARGE_DE                       = 0x00003030,
  LTT_CHNL_24_COUP_CHARGE_DE_BNC                   = 0x00013030,

  // DIGLINE
  LTT_CHNL_24_COUP_DIGLINE_OFF                     = 0x00000000,
  LTT_CHNL_24_COUP_DIGLINE_DC                      = 0x00000001,
  LTT_CHNL_24_COUP_DIGLINE_AC                      = 0x00000002
};


// for compatibility with LTT2API V2 and earlier
// the couplings for the legacy functions:
enum LTT_ChannelCoupling_V2 {
  // LTTAnalogChannel_V2()
  LTT_CHNL_AMPTYPE_GND              = 0x00000000,
  LTT_CHNL_AMPTYPE_SE_DC_PLUS       = 0x00000001,
  LTT_CHNL_AMPTYPE_SE_DC_MINUS      = 0x00000002,
  LTT_CHNL_AMPTYPE_DE_DC            = 0x00000003,
  LTT_CHNL_AMPTYPE_SE_AC_PLUS       = 0x00000004,
  LTT_CHNL_AMPTYPE_SE_AC_MINUS      = 0x00000005,
  LTT_CHNL_AMPTYPE_DE_AC            = 0x00000006,
  LTT_CHNL_AMPTYPE_SE_DC_PLUS_1M    = 0x00000007,
  LTT_CHNL_AMPTYPE_SE_DC_MINUS_1M   = 0x00000008,
  LTT_CHNL_AMPTYPE_DE_DC_1M         = 0x00000009
};

enum LTT_ChannelDMSCoupling_V2 {
  // LTTAnalogChannelDMS_V2()
  LTT_CHNL_AMPTYPE_DMS_GND          = 0x00000010,
  LTT_CHNL_AMPTYPE_DMS_VDC          = 0x00000011,
  LTT_CHNL_AMPTYPE_DMS_VAC          = 0x00000012,
  LTT_CHNL_AMPTYPE_DMS_ICP          = 0x00000013,
  LTT_CHNL_AMPTYPE_DMS_FULL         = 0x00000014,
  LTT_CHNL_AMPTYPE_DMS_HALF         = 0x00000015,
  LTT_CHNL_AMPTYPE_DMS_ZERO         = 0x00000016,
  LTT_CHNL_AMPTYPE_DMS_FULL_CAL     = 0x00000017,
  LTT_CHNL_AMPTYPE_DMS_HALF_CAL     = 0x00000018,
  LTT_CHNL_AMPTYPE_DMS_CURRENT      = 0x00000019,
  LTT_CHNL_AMPTYPE_DMS_QTR          = 0x0000001A,
  LTT_CHNL_AMPTYPE_DMS_QTR_CAL      = 0x0000001B,
  LTT_CHNL_AMPTYPE_DMS_FULL_INV     = 0x00000114,
  LTT_CHNL_AMPTYPE_DMS_HALF_INV     = 0x00000115,
  LTT_CHNL_AMPTYPE_DMS_QTR_INV      = 0x0000011A
};


enum LTT_ChannelDigLineFilterTypes {
  LTT_CHNL_24_FILT_DIGLINE_9MHZ                = 9,
  LTT_CHNL_24_FILT_DIGLINE_16MHZ               = 16,
  LTT_CHNL_24_FILT_DIGLINE_35MHZ               = 35,
  LTT_CHNL_24_FILT_DIGLINE_190MHZ              = 190
};


enum LTT_ChannelDACOutputMode {
  // DACOUT-LINE
  LTT_CHNL_24_DACOUT_OFF                           = 0x00000000,

  LTT_CHNL_24_DACOUT_ADC                           = 0x00000011,
  LTT_CHNL_24_DACOUT_CIRCULAR_BUFF                 = 0x00000012,
  LTT_CHNL_24_DACOUT_STREAM_INTHD                  = 0x00000014,

  LTT_CHNL_24_DACOUT_PULSE_DELTA_PHI_2_DOT         = 0x00000021,
  LTT_CHNL_24_DACOUT_PULSE_DELTA_PHI_DOT           = 0x00000022,
  LTT_CHNL_24_DACOUT_PULSE_DELTA_PHI               = 0x00000023,
  LTT_CHNL_24_DACOUT_PULSE_PHI_2_DOT               = 0x00000024,
  LTT_CHNL_24_DACOUT_PULSE_PHI_DOT                 = 0x00000025,
  LTT_CHNL_24_DACOUT_PULSE_PHI                     = 0x00000026,
  LTT_CHNL_24_DACOUT_PULSE_PULSE_COUNT             = 0x00000027

};

enum LTT_ChannelDACOutputSignal {
  LTT_CHNL_24_DACOUT_SIG_DC                   = 0x00000000,
  LTT_CHNL_24_DACOUT_SIG_RECTANGLE            = 0x00000001,
  LTT_CHNL_24_DACOUT_SIG_SINE                 = 0x00000002,
  LTT_CHNL_24_DACOUT_SIG_TRIANGLE             = 0x00000003,
  LTT_CHNL_24_DACOUT_SIG_BUFFER               = 0x00000004,
  LTT_CHNL_24_DACOUT_SIG_SAWTOOTH             = 0x00000005
  //LTT_CHNL_24_DACOUT_SIG_SINE_SWEEP           = 0x00000012
};

enum LTT_ChannelPulseRecogModes {
  LTT_CHNL_24_PULSE_MODE_OFF                  = 0x00000000,
  LTT_CHNL_24_PULSE_MODE_ON                   = 0x00000001,
  LTT_CHNL_24_PULSE_MODE_MASK                 = 0x0000000F,

  LTT_CHNL_24_PULSE_LINE_A_NEG_EDGE           = 0x00020000,
  LTT_CHNL_24_PULSE_LINE_A_ALLOW_MISS_PLS     = 0x00040000,
  LTT_CHNL_24_PULSE_LINE_A_USE_CORR_TABLE     = 0x00080000,
  LTT_CHNL_24_PULSE_LINE_A_MODE_MASK          = 0x000F0000,

  LTT_CHNL_24_PULSE_LINE_B_USED               = 0x00100000,
  LTT_CHNL_24_PULSE_LINE_B_INVERT             = 0x00200000,
  LTT_CHNL_24_PULSE_LINE_B_MODE_MASK          = 0x00F00000,

  LTT_CHNL_24_PULSE_LINE_Z_USED               = 0x01000000,
  LTT_CHNL_24_PULSE_LINE_Z_USE_ONCE           = 0x02000000,
  LTT_CHNL_24_PULSE_LINE_Z_AS_MATHSTART       = 0x04000000,
  LTT_CHNL_24_PULSE_LINE_Z_ACTIVE_HIGH        = 0x08000000,
  LTT_CHNL_24_PULSE_LINE_Z_CH0_USED           = 0x10000000,
  LTT_CHNL_24_PULSE_LINE_Z_MISS_PLS_USED      = 0x20000000,
  LTT_CHNL_24_PULSE_LINE_Z_MISS_PLS_CNT_USED  = 0x40000000,
  LTT_CHNL_24_PULSE_LINE_Z_MODE_MASK          = 0xFF000000,

  LTT_CHNL_24_PULSE_LINE_A_CORR_MAX           = 0x00000C00
};

enum LTT_ChannelImpedance {
  LTT_CHNL_IMPEDANCE_POS            = 0x00000000,
  LTT_CHNL_IMPEDANCE_NEG            = 0x00000001,
  LTT_CHNL_IMPEDANCE_DIFF           = 0x00000002,
  LTT_CHNL_IMPEDANCE_INFINTE        = 1000000000
};

enum LTT_ChannelState {
  LTT_CHNL_OFF                      = 0x00000000,
  LTT_CHNL_NORMAL                   = 0x00000001,
  LTT_CHNL_RAWDATA                  = 0x00000002
};


enum LTT_ChannelSampleType {
  LTT_CHNL_SMPL_SGND_INT_16         = 0x00000010,
  LTT_CHNL_SMPL_SGND_INT_24         = 0x00000018,
  LTT_CHNL_SMPL_SGND_INT_32         = 0x00000020,
  LTT_CHNL_SMPL_SGND_INT_48         = 0x00000030,
  LTT_CHNL_SMPL_SGND_INT_64         = 0x00000040
};

#define LTT_CHNL_SMPL_BITSIZE(x)    ( (x) )
#define LTT_CHNL_SMPL_SIZE(x)       ( LTT_CHNL_SMPL_BITSIZE(x) >> 3 )

enum LTT_FilterMode {
  LTT_FILTER_OFF                    = 0x00000000,
  LTT_FILTER_ON                     = 0x00000001
};

enum LTT_FilterType {
  LTT_FILT_TYPE_BUTTERWORTH         = 0x00000000,
  LTT_FILT_TYPE_BESSEL              = 0x00000002,
  LTT_FILT_TYPE_CHEBYCHEV           = 0x00000004,
  LTT_FILT_TYPE_USERDEFINED         = 0x00000006,
  LTT_FILT_TYPE_MASK                = 0x0000000E
};

enum LTT_FilterPol {
  LTT_FILT_POL_TWO                  = 0x00000000,
  LTT_FILT_POL_FOUR                 = 0x00000010,
  LTT_FILT_POL_SIX                  = 0x00000020,
  LTT_FILT_POL_EIGHT                = 0x00000030,
  LTT_FILT_POL_TEN                  = 0x00000040,
  LTT_FILT_POL_TWELVE               = 0x00000050,
  LTT_FILT_POL_FOURTEEN             = 0x00000060,
  LTT_FILT_POL_SIXTEEN              = 0x00000070,
  LTT_FILT_POL_MASK                 = 0x000000F0
};


enum LTT_DeviceOptions {
  LTT_OPT_ODD_PRETRIGGER            = 0x00000040,
  LTT_OPT_FRONT_SYNC                = 0x00000200,
  LTT_OPT_180_BANDWIDTH_100K        = 0x00000800,
  LTT_OPT_TIMEBASE_2500KHZ          = 0x00002000,
  LTT_OPT_DMSTYPE_DMS2              = 0x00008000,

  LTT_OPT_24_CHNLHW_V2              = 0x00000001,
  LTT_OPT_24_INTERNAL_HD            = 0x00000002,
  LTT_OPT_24_FW_PULSERECOG          = 0x00000004,
  LTT_OPT_24_DIGLINE_CHNL           = 0x00000008,

  LTT_OPT_24_CHNL_200V              = 0x00000010,
  LTT_OPT_24_CHNL_DIGLINE           = 0x00000020,
  LTT_OPT_24_CHNL_16BIT_DAC         = 0x00000040,
  LTT_OPT_24_CHNL_DIGLINE_EXT_35V   = 0x00000080,
  LTT_OPT_24_CHNL_DIGLINE_35V       = 0x00000100

};

#define LTT_ORDER_FRONT_FLAGS(x)    ( ( (x)       ) & 0x0000FFFF )
#define LTT_ORDER_BACK_FLAGS(x)     ( ( (x) >> 16 ) & 0x0000FFFF )

enum LTT_DeviceCalibStatus {
  LTT_CALIB_UNCALIBRATED            = 0x00000000,
  LTT_CALIB_CALIBRATED              = 0x00000001,
  LTT_CALIB_CERTIFIED               = 0x00000010,
  LTT_CALIB_STATUSMASK              = 0x000000FF
};

enum LTT_CascadingMode {
  LTT_CASCADE_MODE_NONE            = 0x00000000,
  LTT_CASCADE_MODE_START           = 0x00000001,
  LTT_CASCADE_MODE_START_CLOCK     = 0x00000002,
  LTT_CASCADE_MODE_MASK            = 0x0000000F,

  LTT_CASCADE_FLAG_SYNCED          = 0x00000010
};


typedef struct _LTTDEVICECHANNELINFO {
  uint32_t serial;
  uint32_t firmware;
  uint32_t device;
  uint32_t channel;
  uint64_t option;
  // calibration info
  float calib_temperature;
  uint32_t calib_status;
  OSALLocalTime calib_time;
  char p_calib_certified_name[128];
  // reserved
  uint32_t p_reserved[8];
} LTTDeviceChannelInfo;


typedef struct _LTTDEVICEINFO {
  uint32_t lDeviceType;
  uint32_t lSerialNumber;
  uint32_t lFirmware;
  uint32_t lFirmwareBuild;
  uint32_t lChannels;
  uint32_t lOption;
  uint32_t lMOhm;
  // calibration info
  float calib_temperature;
  uint32_t calib_status;
  OSALLocalTime calib_time;
  char p_calib_certified_name[256];
  // additional device info
  uint32_t frame_serial;
  uint32_t order_flags;
  uint32_t memory;
  float range_corr;
  uint32_t sample_size;
  // internal HD
  int64_t hd_capacity;
  uint32_t hd_int_bitsize;
  // additional device-info (2)
  uint32_t frame_type;
  uint32_t frame_channels;
  uint32_t frame_status;
  uint32_t p_chnl_option[ 32 ];
  // reserved
  uint32_t p_reserved[8];
} LTTDeviceInfo;


typedef struct __TTRIGGERPARAM {
  uint32_t active;
  uint32_t type;
  uint32_t slope;
  uint32_t channel;
  uint32_t channel_type;
  float p_para[2];
  uint32_t scan_field;
  uint32_t pre_trigger;
  uint32_t trig_timer;
} LTTTriggerParam;


typedef struct __TDATAPARAM {
  unsigned char *p_data;
  uint32_t buffer_len;
} LTTDataParam;


typedef struct __LTTCHANNELPARAM {
  uint32_t chnl_id;
  uint32_t active;
  uint32_t range;
  uint32_t coupling;
  uint32_t supply;
  int32_t dmsoffset;
  // new stuff
  uint32_t sample_type;
  /*
  char p_name[32];
  char p_signature[32];
  char p_unit[32];
  float usr_scale;
  float usr_offset;
  // channel impedance
  uint32_t imp_pos;
  uint32_t imp_neg;
  uint32_t imp_diff;
  */
} LTTChannelParam;


typedef struct __LTTFILTERPARAM {
  int32_t active;
  int32_t cutoff_timebase;
  int32_t p_reserved[8];
} LTTFilterParam;


typedef struct __LTTPARAM {
  int32_t xfer_infinite;
  int64_t l_datalength;
  uint32_t seqlength;
  uint32_t timebase;
  uint32_t analog_cnt;
  uint32_t digital_cnt;
  uint32_t rpm_cnt;
  LTTDataParam data;
  LTTTriggerParam trigger;
  LTTFilterParam filter;
  LTTChannelParam *p_analog;
  LTTChannelParam *p_digital;
  LTTChannelParam *p_rpm;
} LTTParam;


typedef struct __LTTDMSOFFSET {
  int16_t offset_dac;
  int16_t offset;
} LTTDMSOffset;


typedef struct __LTTRECORDDEVINFO {
  uint32_t dev_idx;
  uint32_t dev_serial;
  uint32_t recorded;
  uint64_t xfer_hd_seq_cnt;
  uint32_t hd_int_bitsize;
  uint32_t hd_use_button;
  uint32_t hd_timebase;
  OSALLocalTime upload_time;
  uint32_t md5sum_valid;
  unsigned char p_md5sum[ 16 ];
  uint32_t rec_dev_cnt;
  uint32_t p_rec_devorder[ 4 ];
  uint32_t hd_dev_seq_len;
  uint32_t hd_dev_seq_size;
  uint32_t hd_ext_info_size;
  // sector stuff
  uint32_t sec_offset;
  uint32_t sec_cnt;
} LTTRecordDevInfo;


typedef struct __LTTRECORDDEVINFOLIST {
  uint32_t sec_limit;
  uint32_t info_cnt;
  uint32_t info_ver;
  LTTRecordDevInfo p_info[ 128 ];
} LTTRecordDevInfoList;


typedef struct __LTTRECORDINFO {
  uint32_t min_info_cnt;
  uint32_t dev_match;
  uint32_t p_info_match[ 128 ];
  uint32_t dev_cnt;
  LTTRecordDevInfoList p_devinfo[ 4 ];
} LTTRecordInfo;


/***************************************
 ***************************************
 *  STDCALL
 ***************************************
 ***************************************/

//========================================
//  LTT2API-MAIN
//========================================

/*****************************************

  FUNCTION: LTTInit

  DESCR.:   Initialize internal resource
            and searches for LTT-devices

  INPUT:    p_analog_cnt:
              give back amount of
              available analog
              channels

            p_digital_cnt:
              give back amount of
              available digiatl
              channels

            p_rpm_cnt:
              give back amount of
              available RPM
              channels

            p_digio_cnt:
              give back amount of
              available DIG-IO
              channels

  OUTPUT:   number of devices found

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTInit( int32_t *p_analog_cnt,
                                        int32_t *p_digital_cnt,
                                        int32_t *p_rpm_cnt,
                                        int32_t *p_digio_cnt );


/*****************************************

  FUNCTION: LTTDeInit

  DESCR.:   Shut down internal resources

  INPUT:    None

  OUTPUT:   None

*****************************************/
__LTT2API_IMP void __APIDEC LTTDeInit( void );



/*****************************************

  FUNCTION: LTTResetParamter

  DESCR.:   Reset parameter settings for
            LTT devices

  INPUT:

  OUTPUT:   

*****************************************/
__LTT2API_IMP void __APIDEC LTTResetParameter( int32_t mode );


/*****************************************

  FUNCTION: LTTConnectDevices

  DESCR.:   Open device handles

  INPUT:    None

  OUTPUT:   None

*****************************************/
__LTT2API_IMP void __APIDEC LTTConnectDevices( void );


/*****************************************

  FUNCTION: LTTReleaseDevices

  DESCR.:   Close device handles

  INPUT:    None

  OUTPUT:   None

*****************************************/
__LTT2API_IMP void __APIDEC LTTReleaseDevices( void );


/*****************************************

  FUNCTION: LTTCascadingMode

  DESCR.:   Specifies how multiple LTT devices
            will work together. Independently or
            in cascading mode.

  INPUT:    mode:
              specifies whether multiple
              LTT devices are running
              cascaded or independent
              but still with the same
              timebase

                0x00 - independent mode
                0x01 - meas. start
                       synchronised
                0x02 - meas. start and clock
                       synchronised

              Flags:
                0x10 - device stack is externally
                       synchronised

  OUTPUT:   None

*****************************************/
__LTT2API_IMP void __APIDEC LTTCascadingMode( int32_t mode );


/*****************************************

  FUNCTION: LTTMasterCorrectionMode

  DESCR.:   Corrects Master-Slave sample
            divergence.

  INPUT:    mode:
              switch correction on/off

                0 - OFF
                1 - ON

  OUTPUT:   None

*****************************************/
__LTT2API_IMP void __APIDEC LTTMasterCorrectionMode( int32_t mode );


/*****************************************

  FUNCTION: LTTSyncCorrection

  DESCR.:   Omits sequencences at measurement
            start

  INPUT:    corr:
              omit 'corr' sequences

  OUTPUT:   None

*****************************************/
__LTT2API_IMP void __APIDEC LTTSyncCorrection( int32_t corr );


/*****************************************

  FUNCTION: LTTSetTransferMode

  DESCR.:   Changes OSAL transfer subsystem.
              Must be called before LTTInit()

  INPUT:    type:
              transfer subsystem

                1 - new SCSI-PD
                2 - old SCSI-SPT
                3 - NETROA
                4 - LTTROA system service [WIN32 only]

            p_local:
              pointer to IP-address string
              of local network adapter

            p_netmask:
              pointer to IP-subnetmask string

            p_remote:
              pointer to IP-address string
              of remote LTTROAD-system,
              to which LTT2API should
              connect

  OUTPUT:   None

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTSetTransferMode( int32_t type,
                                                   const char *p_local,
                                                   const char *p_netmask,
                                                   const char *p_remote );


/*****************************************

  FUNCTION: LTTSetDeviceOrder

  DESCR.:   Set a device order.
            If device-order doesn't match
            the devices found, it will be 
            ignored

            CAUTION:
            Must be called before LTTInit()

  INPUT:    dev_cnt:
              device-count
              [0..3]

            p_devorder:
              pointer to list with serial
              numbers

  OUTPUT:   0 - Everything OK
           -1 - invalid pointer
           -2 - device-count out-of-range

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTSetDeviceOrder(  uint32_t dev_cnt, 
                                                   const uint32_t *p_devorder );



//== NET-ROA =============================

/*****************************************

  FUNCTION: LTTNetRoa_Scan

  DESCR.:   Scans network for available
            NER-ROA clients with LTT-devices

  INPUT:    p_local:
              pointer to IP-address string
              of local network adapter

            p_netmask:
              pointer to IP-subnetmask string

  OUTPUT:   None

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTNetRoa_Scan( const char *p_local,
                                               const char *p_netmask );


/*****************************************

  FUNCTION: LTTNetRoa_GetScanAddress

  DESCR.:

  INPUT:    scan_idx:
              index in scanlist

            p_addr:
              pointer to string where
              IP-address  will be stored.

              Application must provide at
              least 32 Bytes

  OUTPUT:   None

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTNetRoa_GetScanAddress( uint32_t scan_idx,
                                                         char *p_addr );



//== UPLOAD ==============================

/*****************************************

  FUNCTION: LTTUploadParameters

  DESCR.:   Checks, if current paramter
              settings are correct and
              if yes, uploads them to
              LTT-devices.

  INPUT:    None

  OUTPUT:   The returnede error code can
            be a sum of single error:

              0 - Everything OK
             -1 - no devices present
             -2 - Wrong sample rate by current
                  channel setting
             -4 - No channels active
             -8 - transfer active
            -16 - Wrong channel combination!!
                  When backside channels used,
                  at least one front channel must
                  be active also!
            -32 - Trigger channel not active
            -64 - PRE-Trigger settings wrong
           -256 - PRE-Trigger settings wrong
           -512 - No Buffer settings specified!!
          -1024 - master is not active (internal error)
          -2048 - filter cut-off frequency out fo range

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTUploadParameters( void );


/*****************************************

  FUNCTION: LTTGetParameters

  DESCR.:   Gets current valid setup

  INPUT:    p_para:
              Pointer to data-structure, where
              current setup-parameters will be
              stored.

  OUTPUT:   0 - Everything OK
           -1 - invalid pointer

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTGetParameters( LTTParam *p_para );


//== LTT24 STORE DEFAULT SETUP =========
/*****************************************

  FUNCTION: LTT24StoreParameters

  DESCR.:

  INPUT:    None

  OUTPUT:   The returnede error code can
            be a sum of single error:

              0 - Everything OK
             -1 - no devices present
             -2 - Wrong sample rate by current
                  channel setting
             -4 - No channels active
             -8 - transfer active
            -16 - Wrong channel combination!!
                  When backside channels used,
                  at least one front channel must
                  be active also!
            -32 - Trigger channel !active
            -64 - PRE-Trigger settings wrong
           -256 - PRE-Trigger settings wrong
           -512 - No Buffer settings specified!!
          -1024 - master is not active (internal error)
          -2048 - filter cut-off frequency out fo range

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTT24StoreParameters();


//== STAND-ALONE =========================
/*****************************************

  FUNCTION: LTTHDGetSectorCount

  DESCR.:

  INPUT:    None

  OUTPUT:   None

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTHDGetSectorCount( uint32_t *p_dev_sec_cnt, uint32_t info_size );


/*****************************************

  FUNCTION: LTTHDRecordReplay

  DESCR.:   Switches API to REPLAY-mode
            and transfers on internal HD's
            recorded measurements

  INPUT:

  OUTPUT:   None

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTHDRecordReplay( uint32_t rec_idx, char *p_info, uint32_t info_max_size );


/*****************************************

  FUNCTION: LTTHDRecordReplaySetPos

  DESCR.:   Sets position for data-replay

  INPUT:    seq_pos:
              sequence position

  OUTPUT:   None

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTHDRecordReplaySetPos( uint64_t seq_pos );


/*****************************************

  FUNCTION: LTTHDRecordLeaveReplay

  DESCR.:   Switches API to ONLIONE-mode.

  INPUT:

  OUTPUT:   None

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTHDRecordLeaveReplay();


//== MEASUREMENT =========================

/*****************************************

  FUNCTION: LTTStart

  DESCR.:   Starts LTT-device and activates
            Trigger (if set)

  INPUT:    None

  OUTPUT:   0 - everything's OK
           -1 - no devices present or
                no active channels

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTStart( void );

/*****************************************

  FUNCTION: LTTStop

  DESCR.:   Stops the data aquisition
            at LTT device.

  INPUT:    None

  OUTPUT:   None

*****************************************/
__LTT2API_IMP void __APIDEC LTTStop( void );


/*****************************************

  FUNCTION: LTTCheckTrigger

  DESCR.:   Checks if the trigger has been
            fulfilled.

            In the case the trigger is still
            active, the call waits the specified
            amount of time before the call
            returns.

  INPUT:    MilliSecond:
              amount of time to wait
              after negative trigger
              result

  OUTPUT:   0 - Trigger active
            1 - Trigger successful

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTCheckTrigger( uint32_t MilliSecond );



//== CALIBRATION =========================

/*****************************************

  FUNCTION: LTTGetCalibTemperature

  DESCR.:   gives back internal temperature
            of specified device at last
            calibration

  INPUT:    dev_idx:
              Device index

  OUTPUT:   internal devices temperature
            in degrees C

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTGetCalibTemperature( uint32_t dev_idx );



//== TEMPERATUR ==========================

/*****************************************

  FUNCTION: LTTGetTemperature

  DESCR.:   measures temperature in specified
            device

  INPUT:    dev_idx:
              Device index

  OUTPUT:   current internal devices
            temperature

*****************************************/
__LTT2API_IMP float __APIDEC  LTTGetTemperature( uint32_t dev_idx );



//=== DATA-INFO ==========================

/*****************************************

  FUNCTION: LTTGetDLLInfo

  DESCR.:   gives back identification
            string of DLL

  INPUT:    p_buffer:
              pointer to char-field;
              minimum size: 64 Bytes!

  OUTPUT:   0 allways

*****************************************/
__LTT2API_IMP int32_t __APIDEC   LTTGetDLLInfo( char *p_buffer );


/********************************************
  FUNCTION: LTTGetVersion

  DESCR.:   gives back LTT2API version

  INPUT:    None

  OUTPUT:   coded version

 ********************************************/
__LTT2API_IMP int32_t __APIDEC LTTGetVersion( void );


/*****************************************

  FUNCTION: LTTGetDeviceInfo

  DESCR.:   gives back identification
            string of specified device

  INPUT:    dev_idx:
              Device index

            p_buffer:
              pointer to char-field;
              minimum size: 64 Bytes!

  OUTPUT:   >0 - number of analog channels of
                 the device
            -1 - device not found

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTGetDeviceInfo( uint32_t dev_idx,
                                                 char *p_buffer );


/*****************************************

  FUNCTION: LTTGetDeviceInfoEx

  DESCR.:   gives back identification
            string of specified device

  INPUT:    dev_idx:
              Device index

            p_devinfo:
              pointer to LTTDeviceInfo
              structure

  OUTPUT:   0 - everything's OK
           -1 - device not found

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTGetDeviceInfoEx( uint32_t dev_idx,
                                                   LTTDeviceInfo *p_devinfo );


/*****************************************

  FUNCTION: LTTGetDevices

  DESCR.:   gives back number of LTT
            devices found

  INPUT:    None

  OUTPUT:   number of LTT devices found

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTGetDevices( void );


/*****************************************

  FUNCTION: LTTChannel2Device

  DESCR.:   Returns device index of channel

  INPUT:    ch_idx:
              channel index

            ch_type:
              type of channel

  OUTPUT:  device index

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTChannel2Device( uint32_t ch_idx,
                                                  uint32_t ch_type );


/*****************************************

  FUNCTION: LTTChannelAtDevice

  DESCR.:   Returns channel index at device
            of channel

  INPUT:    ch_idx:
              channel index

            ch_type:
              type of channel

  OUTPUT:  device channel index

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTChannelAtDevice( uint32_t ch_idx,
                                                   uint32_t ch_type );


/*****************************************

  FUNCTION: LTTDevice2Channel

  DESCR.:   Returns channel index of device
            index and device channel index.

  INPUT:    dev_idx:
              device index

            dev_ch_idx:
              device channel index

            dev_ch_type:
              channel type

  OUTPUT: >=0 - channel index
           -1 - invalid channel type
           -2 - invalid device index
           -3 - invalid device channel index

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTDevice2Channel( uint32_t dev_idx,
                                                  uint32_t dev_ch_idx,
                                                  uint32_t dev_ch_type );


//========================================
//  LTT2API-TRIGGER
//========================================

/*****************************************

  FUNCTION: LTTTrigger

  DESCR.:   Enables/Disables trigger channel and
            sets trigger parameters

  INPUT:    Trig:
              0 - Trigger OFF
              1 - Trigger ON

            Slope:
              Level, Comparison + Delta:
                0 - positive slope
                1 - negative slope
              Comparison:
                0 - bigger then threshold
                1 - smaller then threshold
              Region:
                0 - enter region
                1 - exit region

            Channel:
              Trigger channel index
              starting with 0

            ChType:
              0 - analog channel
              1 - digital channel

            TrigType:
              Analog:
                0 - Level
                1 - Comparison
                2 - Region
                4 - Comparison + Delta
              Digital:
                0 - Single Bit mode

            f1:
              Level, Comparison, Comparison + Delta:
                Threshhold  [% FSR]
              Region:
                Lower Threshhold  [% FSR]
              Digital:
                Trigger-Bit

            f2:
              Level:
                Sensitivity [% FSR]
              Region:
                Upper Threshhold [% FSR]
              Comparison + Delta:
                min. difference to previous sample [% FSR]
              Comparison, Digital:
                not used

  OUTPUT:   0 - OK
           -1 - invalid channel index
           -2 - level trigger:
                can't have negative hysterisis.
           -3 - delta trigger:
                invalid range for x2 ( 0 < x2 < 6 ).
           -4 - level+delta trigger:
                invalid range for x2 ( 0 < x2 ).
           -5 - invalid digital trigger bit(s)
                FW  6xx ( 0 or 7 < Channel < 16)
                FW >700 ( 0 or 9 < Channel < 16)
           -6 - invalid trigger channel type
           -7 - invalid trigger type

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTTrigger( uint32_t Trig,
                                           uint32_t Slope,
                                           uint32_t Channel,
                                           uint32_t ChType,
                                           uint32_t TrigType,
                                           float f1,
                                           float f2 );


/*****************************************

  DESCR.:   Set PRE-Trigger part of signal.

            The pretrigger length is limited by
            60 Megasamples (120*1024*1024 byte).
            So with 6 active channels the maximum
            pretrigger per channel is 10 Megasamples.

  INPUT:    pre_triglen:
              Amount of pre-tigger
              samples [kS]

            pre_sampletime:
              Sample time in [ns] before
              trigger has happened.

              CAUTION:
              Not all times are allowed.

              0 - use no different sampletime

  OUTPUT:   0 - OK
           -1 - No trigger active
           -2 - Wrong PRE-trigger length

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTTrigger_SetPreTrigger( uint32_t pre_triglen,
                                                         uint32_t pre_sampletime );


/*****************************************

  FUNCTION: LTTTrigger_GetPreTriggerLimit

  DESCR.:   Get maximum allowed 
            PRE-Trigger length.

  INPUT:    
  
  OUTPUT:   maximum PRE-trigger 
            length [KS/ch]

*****************************************/
__LTT2API_IMP uint32_t __APIDEC LTTTrigger_GetPreTriggerLimit();


/*****************************************

  FUNCTION: LTTTrigger_SetReactivationTime

  DESCR.:   Sets trigger reactivation time. after timer
            has expired trigger will be reactivated.
            This can result in multiple trigger
            events in data stream with no direct
            time correlation.

  INPUT:    react_time:
              Time after which the trigger
              will be reactivated
              automatically [ns]
              allowed values:
              0         - No trigger timer
              100ns..1s - allowed range

  OUTPUT:   0 - OK
           -1 - no trigger active;
           -2 - reactivation time too big;

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTTrigger_SetReactivationTime( uint32_t react_time );


/*****************************************

  FUNCTION: LTTTrigger_SetABAMode

  DESCR.:   Sets trigger in ABA mode

  INPUT:    aba_btime:
              Time after which the device
              changes back to PRE-Trigger
              sampletime;
              the upper limit depends on
              the firmware version;
              allowed values:
              0            - No trigger timer
              100ns..limit - allowed range;
                             limit >= 250ms

            aba_sampletime:
              Sample time in [ns] before
              trigger has happened and to
              which is switched back after
              timer has expired.

              CAUTION:
              Not all times are allowed.

  OUTPUT:   0 - OK
           -1 - no trigger active;
           -2 - time for B-Mode too big;
           -3 - feature not supported by firmware

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTTrigger_SetABAMode( uint32_t aba_btime,
                                                      uint32_t aba_sampletime );


/*****************************************

  FUNCTION: LTTTrigger_SetEventTimoutMode

  DESCR.:   Sets trigger in Missing-Event mode

  INPUT:    ok_timeout:
              The periodic trigger-event set by
              LTTTrigger() resets internally a timer.
              If the timer exceeds the 'ok_timeout',
              the measurement is started
              0            - No Missing-Event mode
              100ns..limit - allowed range;
                             limit >= 250ms

  OUTPUT:   0 - OK
           -1 - no trigger active;
           -2 - time too big;
           -3 - feature not supported by firmware

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTTrigger_SetEventTimoutMode( uint32_t ok_timeout );


/*****************************************

  FUNCTION: LTTTriggerScan

  DESCR.:   Sets OR-Scantrigger on analog
              channels. is limited to a device.
              inter-device scantrigger is not
              possible !!

  INPUT:    scan_channels:
              Bit-Field, which channels should be scaned
              for active trigger.
              CAUTION:
              Cannot be used in conjunction with ABA-Mode
              and Reactivation-Timer.

  OUTPUT:   0 - OK

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTTriggerScan( uint32_t scan_channels );


//========================================
//  LTT2API-CHANNELS
//========================================

/*****************************************

  FUNCTION: LTTAnalogChannel

  DESCR.:   Turns on/off analog channels
            and apply the channel settings.

  INPUT:    ch_idx:
              LTT channel index starting
              with 0

            active:
              0 - channel OFF
              1 - channel ON
              2 - channel RAW

            coup:
              channel coupling (see enum LTT_ChannelCoupling above)

            range:
              channel range specified in [mV].
              Depends on the coupling.

            supply:
              channel power supply specified in
              Allowed settings on VOLT [uV]:
                [ 0, 1000000, 2000000, 5000000, 10000000, 15000000 ]
              Allowed settings on Current [uA]:
                [    0,  500, 1000, 1500, 2000, 2500,
                  3000, 3500, 4000, 4500, 5000, 5500,
                  6000, 6500, 7000, 7500, 8000, 8500,
                  9000, 9500, 10000 ]

            offset:
              integer value which is calculated by AutoZero
              0 normaly

  OUTPUT:   0  if successful
           -1  channel doesn't exist
           -2  invalid dms_coup
           -3  dms_coup doesn't match LTT-Hardware
           -4  channel dms_range doesn't match dms_coup settings
          -12  device doesn't support DMS

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTAnalogChannel( uint32_t ch_idx,
                                                 uint32_t active,
                                                 uint32_t coup,
                                                 uint32_t range,
                                                 uint32_t supply,
                                                 int32_t offset );


/*****************************************

  FUNCTION: LTTDigitalChannel

  DESCR.:   Turns on/off digital channels

  INPUT:    ch_idx:
              channel ID, which to be modified.

            active:
              0 - channel OFF
              1 - channel ON

  OUTPUT:    0  if successful
            -1  if channel doesn't exist.

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTDigitalChannel( uint32_t ch_idx,
                                                  uint32_t active );


/*****************************************

  FUNCTION: LTTRPMChannel

  DESCR.:   Turns on/off RPM channels

            CAUTION:
            Only the RPM-channel at the master
            device per device stack is supported.

  INPUT:    ch_idx:
              channel ID, which to be modified.
              Must be 0 !!

            active:
              0 - channel OFF
              1 - channel ON

  OUTPUT:    0  if successful
            -1  if channel doesn't exist.

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTRPMChannel( uint32_t ch_idx,
                                              uint32_t active );



/*****************************************

  FUNCTION: LTTOutputImpedance

  DESCR.:   Sets output impedance of signal source
            for specified input coupling.

  INPUT:    ch_idx
              Analog channel index

            conn_type:
              type of input coupling:
                0 - SE+ coupling
                1 - SE- coupling
                2 - DE  coupling

            impedance:
              impedance value in [Ohm];
              values > 1000000000 --> infinite

  OUTPUT:   0  if successful
           -1  if channel doesn't exist
           -2  invalid connection type

*****************************************/
__LTT2API_IMP  int32_t __APIDEC LTTOutputImpedance( uint32_t ch_idx,
                                                    uint32_t conn_type,
                                                    uint32_t impedance );


/*****************************************

  FUNCTION: LTT24PulseRecognitionChannel

  DESCR.:

  INPUT:    ch_id
                Analog channel index

            mode:
                <to be dexcribed> [0x0000...0x3FFF]

            pls_cnt:
                <to be dexcribed> [0x0000...0x3FFF]

            pls_ok_cnt:
                <to be dexcribed> [0x0000...0x3FFF]

            pls_rot:
                <to be dexcribed> [0x0000...0x3FFF]

            pls_mean:
                <to be dexcribed> [0x0000...0x3FFF]

            p_corr:
                pointer to correction buffer
                correction factor is relativ error
                in the range [0.0...2.0].
                no correction: [1.0]
                max. 3072 values!


  OUTPUT:   OSAL_OK:       if successful
            OSAL_EARG:     invalid channel or
                           invalid pulse-cnt per rotation or
                           invalid correction value
            OSAL_EDEVTYPE: device doesn't support pulse
            OSAL_EOBJECT:  invalid pointer

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTT24PulseRecognitionChannel( uint32_t ch_id,
                                                             uint32_t mode,
                                                             uint32_t pls_cnt,
                                                             uint32_t pls_ok_cnt,
                                                             uint32_t pls_rot,
                                                             uint32_t pls_mean,
                                                             const float *p_corr );


/*****************************************

  FUNCTION: LTT24PlsRecogChannelAdvanced

  DESCR.:

  INPUT:    ch_idx
                Analog channel index

            ...

  OUTPUT:      0  if successful
              -1   if channel doesn't exist
              -2   invalid dac value

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTT24PlsRecogChannelAdvanced( uint32_t ch_id,
                                                             uint32_t gn_dphi,
                                                             uint32_t gn_phid,
                                                             uint32_t gn_phi2d,
                                                             uint32_t gn_dphid,
                                                             uint32_t gn_dphi2d );


/*****************************************

  FUNCTION: LTT24DACOutputChannel

  DESCR.:

  INPUT:    ch_id
                Analog channel index

            line:
                DAC-line at channel [0,1]

            mode:
                DAC operation mode:
                  [LTT_CHNL_24_DACOUT_OFF,

                   LTT_CHNL_24_DACOUT_ADC,

                   LTT_CHNL_24_DACOUT_PULSE_DELTA_PHI_2_DOT,
                   LTT_CHNL_24_DACOUT_PULSE_DELTA_PHI_DOT,
                   LTT_CHNL_24_DACOUT_PULSE_DELTA_PHI,
                   LTT_CHNL_24_DACOUT_PULSE_PHI_2_DOT,
                   LTT_CHNL_24_DACOUT_PULSE_PHI_DOT,
                   LTT_CHNL_24_DACOUT_PULSE_PHI,
                   LTT_CHNL_24_DACOUT_PULSE_PULSE_COUNT]

            gain:
                DAC gain [0...15]


  OUTPUT:      0  if successful
              -1   if channel doesn't exist
              -2   invalid dac value

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTT24DACOutputChannel( uint32_t ch_id,
                                                      uint32_t line,
                                                      uint32_t mode,
                                                      uint32_t gain );


/*****************************************

  FUNCTION: LTT24DACOutputSignalChannel

  DESCR.:

  INPUT:    ch_id
                Analog channel index

            line:
                DAC-line at channel [0,1]

            mode:
                signal type:
                  []

            dac_tb:
                dac output rate [ns]
                  [ 500, 1000, 2000, 4000, 8000 ]

            sig_tb:
                signal timebase [ns]

            amp:
                signal amplitude [relative]

            amp:
                signal offset [relative]

            phi:
                signal phase offset [radiant]

            p_buf:
                pointer to user-defined buffer.
                Currently max. 1536 samples


  OUTPUT:      0  if successful
              -1   if channel doesn't exist
              -2   invalid dac value

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTT24DACOutputSignalChannel( uint32_t ch_id,
                                                            uint32_t line,
                                                            uint32_t mode,
                                                            uint32_t dac_tb,
                                                            uint32_t sig_tb,
                                                            float amp,
                                                            float off,
                                                            float phi0,
                                                            const int16_t *p_buf);


/*****************************************

  FUNCTION: LTT24DigitalLineChannel

  DESCR.:

  INPUT:    ch_idx
                Analog channel index

            ...


  OUTPUT:      0  if successful
              -1   if channel doesn't exist
              -2   invalid dac value

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTT24DigitalLineChannel( uint32_t ch_id,
                                                        uint32_t line,
                                                        uint32_t coup,
                                                        uint32_t filt,
                                                        uint32_t mid,
                                                        int32_t level );


/*****************************************

  FUNCTION: LTTAnalogAutoZero

  DESCR.:   Automatic ReZero of Signal-Level
            for selected active channels
            in current range.
            [LTT-180/182 only]

  INPUT:    chnl_cnt:
              length of channel-list

            p_chnl_list:
              pointer to channel-list for rezero'ing

  OUTPUT:   0 - always

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTAnalogAutoZero( uint32_t chnl_cnt,
                                                  uint32_t *p_chnl_list );


/*****************************************

  FUNCTION: LTTAnalogChargeClear

  DESCR.:   Charge-Clear for selected
            channels.

  INPUT:    chnl_cnt:
                length of channel-list

            p_chnl_list:
                pointer to channel-list for rezero'ing

  OUTPUT:   0 - always

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTAnalogChargeClear( uint32_t chnl_cnt,
                                                     uint32_t *p_chnl_list );


/*****************************************

  FUNCTION: LTTAnalogIOCompensation

  DESCR.:   Internal Offset Compensation
            of GND-Level for selected active 
            channels.
            [LTT-184/186 only]

  INPUT:    chnl_cnt:
              length of channel-list

            p_chnl_list:
              pointer to channel-list for rezero'ing

  OUTPUT:   0 - always

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTAnalogIOCompensation( uint32_t chnl_cnt,
                                                        uint32_t *p_chnl_list );


/*****************************************

  FUNCTION: LTTSetInternReference

  DESCR.:   Sets all the channels for a given device
            to the internal reference.

  INPUT:    dev_idx:
              Selects the LTT device.

            ref_idx:
              index of reference value table
              0 <=>  GND
              1 <=>  857mV
              2 <=>  3.00V
              3 <=>  Vtemp
              4 <=>  Vtemp ^u 3.00V
              5 <=>  3.00V ^u 3.00V
              6 <=>  0 - 857mV

  OUTPUT:   0  OK
           -1  invalid device index
           -2  invalid reference index

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTSetInternReference( uint32_t dev_idx,
                                                      uint32_t ref_idx );


/*****************************************

  FUNCTION: LTTGetChannelConverter

  DESCR.:   gives back conversion factor
            for requested channel to
            calculate the voltage value
            from a data sample:

              [V] = [16Bit] * F

  INPUT:    ch_idx:
              Analog channel index

  OUTPUT:   conversion factor

*****************************************/
__LTT2API_IMP double __APIDEC LTTGetChannelConverter( uint32_t ch_idx );


/*****************************************

  FUNCTION: LTTGetChannelRange

  DESCR.:   gives back range for
            requested channel

  INPUT:    ch_idx:
              Analog channel index

  OUTPUT:   maximum range; +/- value in [mV]

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTGetChannelRange( uint32_t ch_idx );


/*****************************************

  FUNCTION: LTTGetDMSChannelOffset

  DESCR.:   gives back offset of channel

  INPUT:    ch_idx:
                Analog channel index

  OUTPUT:   coded channel offset

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTGetDMSChannelOffset( uint32_t ch_idx );


/*****************************************

  FUNCTION: LTTGetChannelCouplingListLength

  DESCR.:   gives back length of list of
            supported channel input
            couplings

  INPUT:    dev_idx:
              device index

  OUTPUT:   length of list

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTGetChannelCouplingListLength( uint32_t dev_idx );


/*****************************************

  FUNCTION: LTTGetChannelCouplingList

  DESCR.:   fills list with supported
            channel input couplings

  INPUT:    p_couplist:
              Pointer to list vector

            dev_idx:
              device index

            coup:
              input coupling

  OUTPUT:   0 - successful
           -1 - invalid pointer

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTGetChannelCouplingList(  uint32_t *p_couplist,
                                                           uint32_t dev_idx );


/*****************************************

  FUNCTION: LTTGetChannelRangeListLength

  DESCR.:   gives back length of list of
            supported channel-ranges

  INPUT:    dev_idx:
              device index

            coup:
              input coupling

  OUTPUT:   length of list

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTGetChannelRangeListLength( uint32_t dev_idx,
                                                             uint32_t coup );


/*****************************************

  FUNCTION: LTTGetChannelRangeList

  DESCR.:   fills list with supported
            channel-ranges

  INPUT:    p_rnglist:
              Pointer to list vector

            dev_idx:
              device index

            coup:
              input coupling

  OUTPUT:   0 - successful
           -1 - invalid pointer

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTGetChannelRangeList( uint32_t *p_rnglist,
                                                       uint32_t dev_idx,
                                                       uint32_t coup );


/*****************************************

  FUNCTION: LTTGetChannelID

  DESCR.:   Get channel-id to indexed channel

  INPUT:    ch_idx:
              channel index;
              [0..ch_cnt] of requested
              channel type

            ch_type:
              LTT_CHNL_TYPE_ANALOG
              LTT_CHNL_TYPE_DIGITAL
              LTT_CHNL_TYPE_RPM

  OUTPUT: >=0 - channel-id
           -1 - invalid channel index
           -2 - invalid channel type

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTGetChannelID( uint32_t ch_idx, 
                                                uint32_t ch_type );


// LEGACY: LTT2API V2.x and earlier
/*****************************************

  FUNCTION: LTTAnalogChannel_V2

  DESCR.:   Turns on/off analog channels on
            standard hardware LTT-184/186 and
            apply the channel settings

  INPUT:    ch_idx:
              LTT channel index starting
              with 0

            active:
              0 - channel OFF
              1 - channel ON
              2 - channel RAW

            range:
              channel range specified in [mV].
              Allowed settings:
                [  1000,   2000,   2500,   5000,
                  10000,  20000,  25000,  50000,
                 100000, 200000, 250000, 500000]

            coup:
              channel coupling (see enum LTT_ChannelCoupling_V2 above)

            ioc_offset:
              integer value which is calculated by IOC
              (Internal Offset Compensation)
              0 normaly


  OUTPUT:   0  if successful
           -1  if channel doesn't exist
           -2  invalid coup
           -3  coup doesn't match LTT-Hardware
           -4  channel range doesn't match coup settings

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTAnalogChannel_V2( uint32_t ch_idx,
                                                    uint32_t active,
                                                    uint32_t range,
                                                    uint32_t coup,
                                                    int32_t ioc_offset );


/*****************************************

  FUNCTION: LTTAnalogDMSChannel_V2

  DESCR.:   Turns on/off analog channels
            on DMS-hardware LTT-180/182 and
            apply the channel settings.

  INPUT:    ch_idx:
              LTT channel index starting
              with 0

            active:
              0 - channel OFF
              1 - channel ON
              2 - channel RAW

            range:
              channel range specified in [mV].
              Allowed settings VOLT/GND:
                [ 125, 250, 500, 1000, 2000, 5000, 10000 ]
              Allowed settings DMS:
                [ 1, 2, 5, 10, 20, 50, 100, 200, 500 ]

            coup:
              channel coupling (see enum LTT_ChannelDMSCoupling_V2 above)

            supply:
              channel supply settings
              Allowed settings on DMS/VOLT in [mV]:
                [ 0, 1000, 2000, 5000, 10000, 15000 ]
              Allowed settings on DMS Current/ICP in [uA]:
                [    0,  500, 1000, 1500, 2000, 2500,
                  3000, 3500, 4000, 4500, 5000, 5500,
                  6000, 6500, 7000, 7500, 8000, 8500,
                  9000, 9500, 10000 ]

            offset:
              integer value which is calculated by AutoZero
              0 normaly

  OUTPUT:   0  if successful
           -1  channel doesn't exist
           -2  invalid dms_coup
           -3  dms_coup doesn't match LTT-Hardware
           -4  channel dms_range doesn't match dms_coup settings
          -12  device doesn't support DMS

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTAnalogDMSChannel_V2( uint32_t ch_idx,
                                                       uint32_t active,
                                                       uint32_t range,
                                                       uint32_t coup,
                                                       uint32_t supply,
                                                       int32_t offset );


//=======================================
//  LTT2API-TIMEBASE
//========================================

/*****************************************

  FUNCTION: LTTSampleTime

  DESCR.:   Sets the sampling time.

  INPUT:    sample_time:
              Sample time in nano-seconds;

              Caution:
              Not all times are allowed.

              Supported sample times depend on the particular
              hardware type.
              You can get a full list of supported sample-times
              from the API.

            NEW:
                To use devices in slave-mode,
                the flag 0x1000000 is added to
                timebase !!

            time_flag:
              If sample time is between two allowed
              sample times, this parameter indicates,
              which of both is choosen for legal
              sample time

              0 - upper sample time
              1 - lower sample time

  OUTPUT:   The actual sampling time used in [ns].

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTSampleTime( uint32_t sample_time,
                                              int32_t time_flag );


/*****************************************

  FUNCTION: LTTFilter

  DESCR.:   Sets the digital filter.
            A common filter for all channels
            is supported.

  INPUT:    filter_mode:
              A combination of certain flags, which
              specifies the requested digital filter.

              Filter mode:
                  [LTT_FILTER_OFF,
                   LTT_FILTER_ON]

              Filter types:
                  [LTT_FILT_TYPE_BUTTERWORTH,
                   LTT_FILT_TYPE_BESSEL,
                   LTT_FILT_TYPE_CHEBYCHEV]

              Filter pol:
                  [LTT_FILT_POL_TWO,
                   LTT_FILT_POL_FOUR,
                   LTT_FILT_POL_SIX,
                   LTT_FILT_POL_EIGHT,
                   LTT_FILT_POL_TEN,
                   LTT_FILT_POL_TWELVE,
                   LTT_FILT_POL_FOURTEEN,
                   LTT_FILT_POL_SIXTEEN]

              filter_cutoff:
                   Cut-Off frequency of the filter [ns]
                   (-3dB point)

  OUTPUT:
               0 - Everything's OK
              -1 - wrong filter type
              -2 - wrong filter pol

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTFilter( uint32_t filter_mode,
                                          uint32_t filter_cutoff );

/*****************************************

  FUNCTION: LTTGetResolution

  DESCR.:   gives back resolution at current
              sample rate

  INPUT:    None

  OUTPUT:   resolution in bits

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTGetResolution( void );


/*****************************************

  FUNCTION: LTTGetSampleTimeListLength

  DESCR.:   gives back length of list of
              supported sample-times

  INPUT:    None

  OUTPUT:   length of list

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTGetSampleTimeListLength( void );


/*****************************************

  FUNCTION: LTTGetSampleTimeList

  DESCR.:   fills list with supported
            sample-times

  INPUT:    p_list:
              Pointer to list vector

  OUTPUT:   0 - successful
           -1 - invalid pointer

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTGetSampleTimeList( int32_t *p_list );



//========================================
//  LTT2API-DATA
//========================================

/*****************************************

  FUNCTION: LTTGetRAMBufferSettings

  DESCR.:   Gives back needed buffer size
            in bytes.

  INPUT:    seq_cnt
              Amount of sequences

            seq_len:
              Amount of active channels

  OUTPUT:   The size of the buffer in bytes.

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTGetRAMBufferSettings( uint32_t seq_cnt,
                                                        uint32_t seq_len );

/*****************************************

  FUNCTION: LTTRAMBufferSettings

  DESCR.:   Setting up the RAM buffer system
            for data transfer

  INPUT:    p_buffer:
              Pointer to RAM-Buffer.

            seq_cnt:
              Amount of sequences;
              must be multiple of 1024

            seq_len:
              Amount of active channels

  OUTPUT:   0 success
           -1 STREAM already set up !!
              RAM and STREAM are exclusive buffer
              settings.
           -2 length per channel is invalid

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTRAMBufferSettings( unsigned char *p_buffer,
                                                     uint32_t seq_cnt,
                                                     uint32_t seq_len );

/*****************************************

  FUNCTION: LTTSTREAMTransfer

  DESCR.:   Activates new stream transfer.

  INPUT:    mode:
              currently not used.

            seq_cnt:
              Number of sequences

  OUTPUT:   0 success
           -1 ERROR: RAM already set up !!
                     RAM and STREAM are exclusive buffer
                     settings.

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTSTREAMTransfer( int32_t mode,
                                                  uint32_t seq_cnt );

/*****************************************

  FUNCTION: LTTBufferReset

  DESCR.:   Resets the essential parameters
            for the buffers.

  INPUT:    None

  OUTPUT:   None

*****************************************/
__LTT2API_IMP void __APIDEC LTTBufferReset( void );


//== STAND-ALONE =========================

/*****************************************

  FUNCTION: LTTHDRecordLength

  DESCR.:   Sets the HD record length.

  INPUT:    dwSamplesPerChannel:
              Number of samples per channel

  OUTPUT:   None

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTHDRecordLength( uint64_t dwHDSamplesPerChannel );


/*****************************************

  FUNCTION: LTTHDRecordButton

  DESCR.:   Sets the HD button before to
            activate measurement.

  INPUT:    use_button:
              0 - no button (default)
              1 - use button

  OUTPUT:   None

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTHDRecordButton( uint32_t use_button );


/*****************************************

  FUNCTION: LTTHDGetRecordInfo

  DESCR.:   Sets the HD record length.

  INPUT:    p_recinfo:
              pointer to Record-Info

  OUTPUT:   None

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTHDGetRecordInfo( LTTRecordInfo *p_recinfo );


/*****************************************

  FUNCTION: LTTHDGetRecordParameter

  DESCR.:   Gets the HD record-parameter

  INPUT:    rec_idx:
              record-info index
            
            p_para:
              pointer to LTTParam

  OUTPUT:   None

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTHDGetRecordParameter( uint32_t rec_idx, 
                                                        LTTParam *p_para,
                                                        char *p_info,
                                                        uint32_t info_max_size );



//== DATA-TRANSFER =======================

/*****************************************

  FUNCTION: LTTTransfer2Buffers

  DESCR.:   Transfers the data from the devices
            to the assigned buffers.
            This call initiates the real transfer.
            The transfer can work in BLOCKED or
            NONE_BLOCKED mode.

            BLOCKED:
            The call returns, when the transfer is
            done. This is not possible with the
            STREAM buffer system.

            NONE_BLOCKED:
            The transfer is initiated and the call
            returns immediately. The Transfer must be
            monitored with LTTDataTransferStatus.

  INPUT:    BufferType:
              Currently ignored!

            BlockMode:
              0 - Blocked mode;
                  Call returns, when data are in buffer.
              1 - NON-Blocked mode;
                  Call initiate transfer and
                  returns immediately.
                  End of transfer can be obtained
                  with LTTDataTransferStatus()

              Some flags can be applied to the mode:

              4 - Continuous transfer;
                  For subsequent calls of LTTTransfer2Buffers()
                  a restart of the LTT-systems is not done.
                  This is necessary for infinite-type of data
                  acquisition
              8 - High precision mode;
                  Not possible with Continuous-Mode;
                  Reduces noise feed-back of scsi-transfer.

  OUTPUT:   Blocked mode:
            >=0 - amount of transfered samples
                  CAUTION:
                  The amount of transferred samples must be
                  checked against the requested data length.
                  If the amount of transferred samples is
                  less than the requested amount, an
                  irrecoverable error has occurred during the
                  transfer, which led to a break of the transfer.
                  In the data buffer the correctly transferred
                  data are stored up to the point where the error
                  has occurred!!

            NON-Blocked mode:
              0 - transfer invoked

            Both:
             -1 - no record
             -2 - no buffer type set
             -3 - transfer is active, so can't invoke
                  another transfer !!
             -4 - file transfer already done.
                  reconfigure file buffer settings!

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTTransfer2Buffers( uint32_t buffer_type,
                                                    uint32_t xfer_mode );


/*****************************************

  FUNCTION: LTTStream2Buffer

  DESCR.:   Transfers the data from the devices
            to the submitted buffer.
            This call initiates the real transfer.
            The transfer can work in BLOCKED or
            NONE_BLOCKED mode.

            BLOCKED:
            The call returns, when the transfer is
            done. This is not possible with the
            STREAM buffer system.

            NONE_BLOCKED:
            The transfer is initiated and the call
            returns immediately. The Transfer must be
            monitored with LTTDataTransferStatus.

  INPUT:    p_buffer:
              Pointer to RAM-Buffer.

            seq_cnt:
              amount of sequences to store;
              must be multiple of 1024

            xfer_mode:
              0 - Blocked mode;
                  Call returns, when data are in buffer.
              1 - NON-Blocked mode;
                  Call initiate transfer and
                  returns imediately.
                  End of transfer can be obtained
                  with 'LTTDataTransferStatus'

  OUTPUT:   Blocked mode:
            >=0 - amount of transfered samples
                  CAUTION:
                  The amount of transferred samples must be
                  checked against the requested data length.
                  If the amount of transferred samples is
                  less than the requested amount, an
                  irrecoverable error has occurred during the
                  transfer, which led to a break of the transfer.
                  In the data buffer the correctly transferred
                  data are stored up to the point where the error
                  has occurred!!

            NON-Blocked mode:
              0 - transfer invoked

            Both:
             -1 - no record
             -2 - STREAM transfer not active
             -3 - transfer is active, so can't invoke
                  another transfer !!
             -4 - sequence count is not valid

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTStream2Buffer( unsigned char *p_buffer,
                                                 uint32_t seq_cnt,
                                                 uint32_t xfer_mode );


typedef void (*LTTStream2BufferDoneFP) ( unsigned char *, int32_t, void * );
/*****************************************

  FUNCTION: LTTStream2BufferCB

  DESCR.:   Transfers the data from the devices
            to the submitted buffer.
            This call initiates the real transfer.
            If the transfer has finished, the callback
            function id called.

  INPUT:    p_buffer:
                Pointer to RAM-Buffer.

            seq_cnt:
              amount of sequences to store;
              must be multiple of 1024

            xfer_flags:
              - continous transfer
              - high-prec transfer

            fp_finished:
              callback function pointer
              
            p_fp_data:
              data pointer, which is paased aint32_t to the
              callback function

  OUTPUT:   0 - transfer invoked
           -1 - no record
           -2 - STREAM transfer not active
           -3 - transfer is active, so can't invoke
                another transfer !!
           -4 - sequence count is not valid

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTStream2BufferCB( unsigned char *p_buffer,
                                                   uint32_t seq_cnt,
                                                   uint32_t xfer_flags,
                                                   LTTStream2BufferDoneFP fp_finished,
                                                   void *p_fp_data );



/*****************************************

  FUNCTION: LTTDataTransferStatus

  DESCR.:   Monitors the status of the transfer.

            This function is necessary for the
            NONE_BLOCKED Mode of transfer.

  INPUT:    None

  OUTPUT:   0 - nothing TODO
           -1 - waiting for trigger
           -2 - new block available
           -3 - no transfer active
         -255 - error happened

           >0 - transfer finished, gives back
                amount of transmited samples

                CAUTION:
                The amount of transferred samples must be
                checked against the requested data length.
                If the amount of transferred samples is
                less than the requested amount, an
                irrecoverable error has occurred during the
                transfer, which led to a break of the transfer.
                In the data buffer the correctly transferred
                data are stored up to the point where the error
                has occurred!!

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTDataTransferStatus( void );


/*****************************************

  FUNCTION: LTTDataTransferWait

  DESCR.:   Waits until the transfer has
            finished.

            This function is necessary for the
            NONE_BLOCKED Mode of transfer.

  INPUT:    None

  OUTPUT:  -3 - no transfer active
         -255 - error happened

           >0 - transfer finished, gives back
                amount of transmited samples

                CAUTION:
                The amount of transferred samples must be
                checked against the requested data length.
                If the amount of transferred samples is
                less than the requested amount, an
                irrecoverable error has occurred during the
                transfer, which led to a break of the transfer.
                In the data buffer the correctly transferred
                data are stored up to the point where the error
                has occurred!!

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTDataTransferWait( void );


/***************************************************

  FUNCTION: LTTDataTransferBreak

  DESCR.:   Breaks a running transfer.

  INPUT:    None

  OUTPUT:    0 - transfer breaked
            -1 - error happened

 ***************************************************/
__LTT2API_IMP int32_t __APIDEC LTTDataTransferBreak( void );


/*****************************************

  FUNCTION: LTTData_GetCounter

  DESCR.:   Gives back the amount of
            transferred samples.

            This call can be used in
            NONE_BLOCKED Mode of transfer
            to monitor the progress of the
            transfer.

  INPUT:    None

  OUTPUT:   amount of transfered samples

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTData_GetCounter();


/*****************************************

  FUNCTION: LTTDataTransferCounter

  DESCR.:   Returns the current transfer
            byte counter.

  INPUT:

  OUTPUT:   <byte-count>

*****************************************/
__LTT2API_IMP uint64_t __APIDEC LTTDataTransferCounter();


//== DATA-INFO ===========================

/*****************************************

  FUNCTION: LTTGetChannelIDSequence

  DESCR.:   Gives back channel ID in
            channel sequence

  INPUT:    p_seqlist:
              pointer to list

  OUTPUT:   None

*****************************************/
__LTT2API_IMP void __APIDEC LTTGetChannelIDSequence( uint32_t *p_seqlist );


/*****************************************

  FUNCTION: LTTGetChannelSequenceLength

  DESCR.:   Gives back length of channel
            sequence

  INPUT:    None

  OUTPUT:   Length of channel sequence

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTGetChannelSequenceLength( void );



//========================================
//  LTT2API-DIGIO
//========================================

/*****************************************

  FUNCTION: LTTDigIO_SetLineDirection

  DESCR.:

  INPUT:    ch_idx:
              DIGIO channel index

            input-mask:
              mask describes which lines
              are configured of input

  OUTPUT:   None

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTDigIO_SetLineDirection( int32_t ch_idx, uint32_t mask );


/*****************************************

  FUNCTION: LTTDigIO_GetInput

  DESCR.:

  INPUT:    ch_idx:
              DIGIO channel index

            p_data:
              pointer where input-state
              should be stored

  OUTPUT:   None

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTDigIO_GetInput( int32_t ch_idx, uint32_t *p_data );


/*****************************************

  FUNCTION: LTTDigIO_SetOutput

  DESCR.:

  INPUT:    ch_idx:
              DIGIO channel index

            data:
              new output-state of
              this channel

  OUTPUT:   None

*****************************************/
__LTT2API_IMP int32_t __APIDEC LTTDigIO_SetOutput( int32_t ch_idx, uint32_t data );



#ifdef __cplusplus
}
#endif


#endif


