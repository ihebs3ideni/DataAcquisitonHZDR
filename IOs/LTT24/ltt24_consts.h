//
// Created by iheb on 9/14/2020.
//

#ifndef HZDR_DEMO_SERVER_LTT_CONSTS_H
#define HZDR_DEMO_SERVER_LTT_CONSTS_H
#include "../LTT_API/ltt2api.h"
#include <map>
#include <string>

namespace ltt_enums{
    //used to map the txt config to the enum config in ltt2api.h

    const std::map<std::string, unsigned> TB_ExternalClock =
    {
        {"EXTCLOCK_16BIT" , 0x00000000},
        {"EXTCLOCK_15BIT" , 0x00000001},
        {"EXTCLOCK_14BIT" , 0x00000002},
        {"EXTCLOCK_12BIT" , 0x00000003},
        {"EXTCLK_MAX__57KHZ" , 0x00000000},
        {"EXTCLK_MAX_107KHZ" , 0x00000001},
        {"EXTCLK_MAX_188KHZ" , 0x00000002},
        {"EXTCLK_MAX_277KHZ" , 0x00000003}
    };

    const std::map<std::string, unsigned> TB_Flags =
    {
      {"SMPLTIME_FLAG_ABOVE" , 0x0000000},
      {"SMPLTIME_FLAG_BELOW" , 0x0000001},
      {"SMPLTIME_FLAG_CALIB" , 0x0001000},

      {"TIMEBASE_MASK" , 0x0FFFFFFF},
      {"TIMEBASE_SLAVEMODE" , 0x10000000},
      {"TIMEBASE_MODE_MASK"  , 0xF0000000}
    };

    const std::map<std::string, unsigned> TransferMode =
    {
      {"BLOCKED" , 0x00000000},
      {"NONEBLOCKED" , 0x00000001},
      {"CONTINUOUS" , 0x00000004},
      {"HIGHPRECISION" , 0x00000008}
    };


    const std::map<std::string, unsigned> TriggerMode =
    {
        {"LEVEL" , 0x00000000},
        {"COMPARISION" , 0x00000001},
        {"REGION"  , 0x00000002},
        {"LEVEL_DELTA" , 0x00000004},
        {"DIGMODE_SINGLE_BIT" , 0x00000000}
    };


    const std::map<std::string, unsigned> TriggerState =
    {
        {"OFF" , 0x00000000},
        {"ON"  , 0x00000001}
    };


    const std::map<std::string, unsigned> TriggerType =
    {
        {"SINGLE" , 0x00000000},
        {"TRIGTIMER" , 0x00000001},
        {"MISSING_EVENT" , 0x00000002},
        {"ABA_MODE" , 0x00000010}
    };


    const std::map<std::string, unsigned > TriggerSlope =
    {
        {"POS"  , 0x00000000},
        {"NEG"  , 0x00000001},
        {"COMP_BIGGER" , 0x00000000},
        {"COMP_SMALLER"  , 0x00000001},
        {"REGION_ENTER"  , 0x00000000},
        {"REGION_EXIT"  , 0x00000001}
    };



    // channels,
    const std::map<std::string, unsigned > ChannelType =
    {
       { "ANALOG" , 0x00000000},
        {"DIGITAL" , 0x00000001},
        {"RPM" , 0x00000002}
    };


    const std::map<std::string, unsigned> ChannelIDBit =
    {
        {"ANALOG" , 0x00000000},
        {"DIGITAL" , 0x00001000},
        {"RPM" , 0x00002000}
    };


    const std::map<std::string, unsigned> ChannelCoupling =
    {
        {"COUP_GND"                 , 0x00000000},

        {"184_COUP_SE_DC_PLUS"       , 0x00001110},
        {"184_COUP_SE_DC_MINUS"      , 0x00002110},
        {"184_COUP_DE_DC"            , 0x00003110},
        {"184_COUP_SE_AC_PLUS"       , 0x00001210},
        {"184_COUP_SE_AC_MINUS"      , 0x00002210},
        {"184_COUP_DE_AC"            , 0x00003210},
        {"184_COUP_SE_DC_PLUS_1M"    , 0x00051110},
        {"184_COUP_SE_DC_MINUS_1M"   , 0x00052110},
        {"184_COUP_DE_DC_1M"         , 0x00053110},


        {"180_COUP_VOLT_DC"          , 0x00001110},
        {"180_COUP_VOLT_AC"          , 0x00001210},
        {"180_COUP_ICP_AC"           , 0x00001220},
        {"180_COUP_DMS_FULL"         , 0x00003150},
        {"180_COUP_DMS_HALF"         , 0x00001160},
        {"180_COUP_DMS_ZERO"         , 0x00003140},
        {"180_COUP_DMS_FULL_CAL"     , 0x00103150},
        {"180_COUP_DMS_HALF_CAL"     , 0x00101160},
        {"180_COUP_DMS_CURRENT"      , 0x00003280},
        {"180_COUP_DMS_QTR"          , 0x00001170},
        {"180_COUP_DMS_QTR_CAL"      , 0x00101170},
        {"180_COUP_DMS_FULL_INV"     , 0x01003150},
        {"180_COUP_DMS_HALF_INV"     , 0x01001160},
        {"180_COUP_DMS_QTR_INV"      , 0x01001170},


        {"24_COUP_VOLT_SE_DC"        , 0x00001110},
        {"24_COUP_VOLT_SE_DC_BNC"    , 0x00011110},
        {"24_COUP_VOLT_DE_DC"        , 0x00003110},
        {"24_COUP_VOLT_DE_DC_BNC"    , 0x00013110},
        {"24_COUP_VOLT_SE_AC"        , 0x00001210},
        {"24_COUP_VOLT_SE_AC_BNC"    , 0x00011210},
        {"24_COUP_VOLT_DE_AC"        , 0x00003210},
        {"24_COUP_VOLT_DE_AC_BNC"    , 0x00013210},


        {"24_COUP_IEPE_SE_DC"        , 0x00001120},
        {"24_COUP_IEPE_SE_DC_BNC"    , 0x00011120},
        {"24_COUP_IEPE_SE_DC_3WIRE"  , 0x00061120},
        {"24_COUP_IEPE_DE_DC_4WIRE"  , 0x00073120},
        {"24_COUP_IEPE_SE_AC"        , 0x00001220},
        {"24_COUP_IEPE_SE_AC_BNC"    , 0x00011220},
        {"24_COUP_IEPE_SE_AC_3WIRE"  , 0x00061220},
        {"24_COUP_IEPE_DE_AC_4WIRE"  , 0x00073220},


        {"24_COUP_BRDG_FULL_DE_DC"            , 0x00003150},
        {"24_COUP_BRDG_FULL_DE_DC_CAL"        , 0x00103150},
        {"24_COUP_BRDG_FULL_DE_DC_SENSE"      , 0x00203150},
        {"24_COUP_BRDG_FULL_DE_DC_SENSE_CAL"  , 0x00303150},
        {"24_COUP_BRDG_FULL_DE_AC"            , 0x00003250},
        {"24_COUP_BRDG_FULL_DE_AC_CAL"        , 0x00103250},
        {"24_COUP_BRDG_FULL_DE_AC_SENSE"      , 0x00203250},
        {"24_COUP_BRDG_FULL_DE_AC_SENSE_CAL"  , 0x00303250},


        {"24_COUP_BRDG_HALF_SE_DC"            , 0x00001150},
        {"24_COUP_BRDG_HALF_SE_DC_CAL"        , 0x00101150},
        {"24_COUP_BRDG_HALF_SE_DC_SENSE"      , 0x00201150},
        {"24_COUP_BRDG_HALF_SE_DC_SENSE_CAL"  , 0x00301150},
        {"24_COUP_BRDG_HALF_SE_AC"            , 0x00001250},
        {"24_COUP_BRDG_HALF_SE_AC_CAL"        , 0x00101250},
        {"24_COUP_BRDG_HALF_SE_AC_SENSE"      , 0x00201250},
        {"24_COUP_BRDG_HALF_SE_AC_SENSE_CAL"  , 0x00301250},


        {"24_COUP_BRDG_QRTR_SE_DC_R350"        , 0x00021170},
        {"24_COUP_BRDG_QRTR_SE_DC_R350_CAL"    , 0x00121170},
        {"24_COUP_BRDG_QRTR_SE_DC_R350_SENSE"  , 0x00221170},
        {"24_COUP_BRDG_QRTR_SE_DC_R350_SENSE_CAL" , 0x00321170},
        {"24_COUP_BRDG_QRTR_SE_DC_R120"            , 0x00031170},
        {"24_COUP_BRDG_QRTR_SE_DC_R120_CAL"        , 0x00131170},
        {"24_COUP_BRDG_QRTR_SE_DC_R120_SENSE"      , 0x00231170},
        {"24_COUP_BRDG_QRTR_SE_DC_R120_SENSE_CAL"  , 0x00331170},
        {"24_COUP_BRDG_QRTR_SE_AC_R350"            , 0x00021270},
        {"24_COUP_BRDG_QRTR_SE_AC_R350_CAL"        , 0x00121270},
        {"24_COUP_BRDG_QRTR_SE_AC_R350_SENSE"      , 0x00221270},
        {"24_COUP_BRDG_QRTR_SE_AC_R350_SENSE_CAL"  , 0x00321270},
        {"24_COUP_BRDG_QRTR_SE_AC_R120"            , 0x00031270},
        {"24_COUP_BRDG_QRTR_SE_AC_R120_CAL"        , 0x00131270},
        {"24_COUP_BRDG_QRTR_SE_AC_R120_SENSE"      , 0x00231270},
        {"24_COUP_BRDG_QRTR_SE_AC_R120_SENSE_CAL"  , 0x00331270},


        {"24_COUP_CURRENT_SE_DC"                   , 0x00001190},
        {"24_COUP_CURRENT_SE_DC_BNC"               , 0x00011190},
        {"24_COUP_CURRENT_SE_AC"                   , 0x00001290},
        {"24_COUP_CURRENT_SE_AC_BNC"               , 0x00011290},


        {"24_COUP_LVDT_DE_DC_4WIRE"                , 0x000731A0},
        {"24_COUP_LVDT_DE_DC_4WIRE_PHASE"          , 0x001731A0},
        {"24_COUP_LVDT_DE_AC_4WIRE"                , 0x000732A0},
        {"24_COUP_LVDT_DE_AC_4WIRE_PHASE"          , 0x001732A0},


        {"24_COUP_CHARGE_SE"                       , 0x00001030},
        {"24_COUP_CHARGE_SE_BNC"                   , 0x00011030},
        {"24_COUP_CHARGE_DE"                       , 0x00003030},
        {"24_COUP_CHARGE_DE_BNC"                   , 0x00013030},


        {"24_COUP_DIGLINE_OFF"                     , 0x00000000},
        {"24_COUP_DIGLINE_DC"                      , 0x00000001},
        {"24_COUP_DIGLINE_AC"                      , 0x00000002}
    };




//    // for compatibility with LTT2API V2 and earlier
//    // the couplings for the legacy functions,
    const std::map<std::string, int>  ChannelCoupling_V2 =
    {
        {"GND"              , 0x00000000},
        {"SE_DC_PLUS"       , 0x00000001},
        {"SE_DC_MINUS"      , 0x00000002},
        {"DE_DC"            , 0x00000003},
        {"SE_AC_PLUS"       , 0x00000004},
        {"SE_AC_MINUS"      , 0x00000005},
        {"DE_AC"            , 0x00000006},
        {"SE_DC_PLUS_1M"    , 0x00000007},
        {"SE_DC_MINUS_1M"   , 0x00000008},
        {"DE_DC_1M"         , 0x00000009}
    };



    const std::map<std::string, int> ChannelDMSCoupling_V2 =
    {
        {"DMS_GND"          , 0x00000010},
        {"DMS_VDC"          , 0x00000011},
        {"DMS_VAC"          , 0x00000012},
        {"DMS_ICP"          , 0x00000013},
        {"DMS_FULL"         , 0x00000014},
        {"DMS_HALF"         , 0x00000015},
        {"DMS_ZERO"         , 0x00000016},
        {"DMS_FULL_CAL"     , 0x00000017},
        {"DMS_HALF_CAL"     , 0x00000018},
        {"DMS_CURRENT"      , 0x00000019},
        {"DMS_QTR"          , 0x0000001A},
        {"DMS_QTR_CAL"      , 0x0000001B},
        {"DMS_FULL_INV"     , 0x00000114},
        {"DMS_HALF_INV"     , 0x00000115},
        {"DMS_QTR_INV"      , 0x0000011A}
    };


    const std::map<std::string, int> ChannelDigLineFilterTypes=
     {
        {"9MHZ"                , 9},
        {"16MHZ"               , 16},
        {"35MHZ"               , 35},
        {"190MHZ"              , 190}
    };


    const std::map<std::string, int> ChannelDACOutputMode =
     {
        {"OFF"                           , 0x00000000},

        {"ADC"                           , 0x00000011},
        {"CIRCULAR_BUFF"                 , 0x00000012},
        {"LSTREAM_INTHD"                 , 0x00000014},

        {"PULSE_DELTA_PHI_2_DOT"         , 0x00000021},
        {"PULSE_DELTA_PHI_DOT"           , 0x00000022},
        {"PULSE_DELTA_PHI"               , 0x00000023},
        {"PULSE_PHI_2_DOT"               , 0x00000024},
        {"PULSE_PHI_DOT"                 , 0x00000025},
        {"PULSE_PHI"                     , 0x00000026},
        {"PULSE_PULSE_COUNT"             , 0x00000027}
    };


    const std::map<std::string, int> ChannelDACOutputSignal =
     {
        {"DC"                   , 0x00000000},
        {"RECTANGLE"            , 0x00000001},
        {"SINE"                 , 0x00000002},
        {"TRIANGLE"             , 0x00000003},
        {"BUFFER"               , 0x00000004},
        {"SAWTOOTH"             , 0x00000005},
        {"LTT_CHNL_24_DACOUT_SIG_SINE_SWEEP"   , 0x00000012}

     };

    const std::map<std::string, int> ChannelPulseRecogModes =
    {
        {"PULSE_MODE_OFF"                  , 0x00000000},
        {"PULSE_MODE_ON"                   , 0x00000001},
        {"PULSE_MODE_MASK"                 , 0x0000000F},

        {"PULSE_LINE_A_NEG_EDGE"           , 0x00020000},
        {"PULSE_LINE_A_ALLOW_MISS_PLS"     , 0x00040000},
        {"PULSE_LINE_A_USE_CORR_TABLE"     , 0x00080000},
        {"PULSE_LINE_A_MODE_MASK"          , 0x000F0000},

        {"PULSE_LINE_B_USED"               , 0x00100000},
        {"PULSE_LINE_B_INVERT"             , 0x00200000},
        {"PULSE_LINE_B_MODE_MASK"          , 0x00F00000},

        {"PULSE_LINE_Z_USED"               , 0x01000000},
        {"PULSE_LINE_Z_USE_ONCE"           , 0x02000000},
        {"PULSE_LINE_Z_AS_MATHSTART"       , 0x04000000},
        {"PULSE_LINE_Z_ACTIVE_HIGH"        , 0x08000000},
        {"PULSE_LINE_Z_CH0_USED"           , 0x10000000},
        {"PULSE_LINE_Z_MISS_PLS_USED"      , 0x20000000},
        {"PULSE_LINE_Z_MISS_PLS_CNT_USED"  , 0x40000000},
        {"PULSE_LINE_Z_MODE_MASK"          , 0xFF000000},

        {"PULSE_LINE_A_CORR_MAX"           , 0x00000C00}
    };



    const std::map<std::string, int> ChannelImpedance =
     {
        {"POS"            , 0x00000000},
        {"NEG"            , 0x00000001},
        {"DIFF"           , 0x00000002},
        {"INFINTE"        , 1000000000}
     };


    const std::map<std::string, int> ChannelState =
     {
        {"OFF"             , 0x00000000},
        {"NORMAL"          , 0x00000001},
        {"RAWDATA"         , 0x00000002}
    };



    const std::map<std::string, int> ChannelSampleType =
     {
        {"INT_16"         , 0x00000010},
        {"INT_24"         , 0x00000018},
        {"INT_32"         , 0x00000020},
        {"INT_48"         , 0x00000030},
        {"INT_64"         , 0x00000040}
    };


    const std::map<std::string, int> FilterMode =
    {
        {"OFF"                    , 0x00000000},
        {"ON"                     , 0x00000001}
    };


    const std::map<std::string, int> FilterType =
    {
        {"BUTTERWORTH"         , 0x00000000},
        {"BESSEL"              , 0x00000002},
        {"CHEBYCHEV"           , 0x00000004},
        {"USERDEFINED"         , 0x00000006},
        {"MASK"                , 0x0000000E}
    };


    const std::map<std::string, int> FilterPol =
     {
        {"TWO"          , 0x00000000},
        {"FOUR"         , 0x00000010},
        {"SIX"          , 0x00000020},
        {"EIGHT"        , 0x00000030},
        {"TEN"          , 0x00000040},
        {"TWELVE"       , 0x00000050},
        {"FOURTEEN"     , 0x00000060},
        {"SIXTEEN"      , 0x00000070},
        {"MASK"         , 0x000000F0}
    };


    const std::map<std::string, int> DeviceOptions =
    {
        {"ODD_PRETRIGGER"            , 0x00000040},
        {"FRONT_SYNC"                , 0x00000200},
        {"180_BANDWIDTH_100K"        , 0x00000800},
        {"TIMEBASE_2500KHZ"          , 0x00002000},
        {"DMSTYPE_DMS2"              , 0x00008000},

        {"24_CHNLHW_V2"              , 0x00000001},
        {"24_INTERNAL_HD"            , 0x00000002},
        {"24_FW_PULSERECOG"          , 0x00000004},
        {"24_DIGLINE_CHNL"           , 0x00000008},

        {"24_CHNL_200V"              , 0x00000010},
        {"24_CHNL_DIGLINE"           , 0x00000020},
        {"24_CHNL_16BIT_DAC"         , 0x00000040},
        {"24_CHNL_DIGLINE_EXT_35V"   , 0x00000080},
        {"24_CHNL_DIGLINE_35V"       , 0x00000100}

    };


    const std::map<std::string, int> DeviceCalibStatus =
    {
        {"UNCALIBRATED"            , 0x00000000},
        {"CALIBRATED"              , 0x00000001},
        {"CERTIFIED"               , 0x00000010},
        {"STATUSMASK"              , 0x000000FF}
    };


    const std::map<std::string, int> CascadingMode =
    {
        {"NONE"            , 0x00000000},
        {"START"           , 0x00000001},
        {"START_CLOCK"     , 0x00000002},
        {"MASK"            , 0x0000000F},

        {"FLAG_SYNCED"     , 0x00000010}
    };


    const std::map<std::string, int> Emulation_Signal_Types =
    {
            {"DC"               , 0},
            {"RECTANGLE"        , 1},
            {"SINUS"            , 2},
            {"TRIANGLE"         , 3},
            {"TRIANGLE_SWEEP"   , 8},
            {"RECTANGLE_SWEEP"  , 9},
            {"SINUS_SWEEP"      , 10},
            {"PEAK"             , 16},
            {"TRANSFER_TEST"    , 64},
            {"TRANSFER_CONST"   , 65}

    };


}


#endif //HZDR_DEMO_SERVER_LTT_CONSTS_H
