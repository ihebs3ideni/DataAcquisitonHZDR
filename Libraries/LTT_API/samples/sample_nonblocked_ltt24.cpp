// *******************************************
// Copyright (C) 2013 by LTT GmbH
//
// FILE:      ltt2api_sample_nonblocked_ltt24.c
//
// DESCR:     Sample for LTT24
//
// AUTHOR:    STK <stefan.knorr@tasler.de>
//
// VERSION:   3.0.x
//
// *******************************************


#include <stdio.h>
#include <stdlib.h>

#include <lttosal.h>
#include <ltt2api.h>
#include "ltt2api_emulation.h"

// prototype
__LTT2API_IMP int32_t __APIDEC LTTSetDeviceEmulation( int32_t num );

int test_ltt( void )
{
    // --- INIT ---------------------------------------------------------------
    int32_t ch_idx, dev_cnt, dev_idx, status, trig, sequence_len, sequence_cnt, seq_idx, an_cnt;
    uint32_t sample_size;
    LTTDeviceInfo p_dev_info[ 4 ];
    int32_t p_chnl_id[ 4 * 32 ];
    int32_t p_seq_id[ 4 * 32 ];
    double p_seq_conv[ 4 * 32 ];

    // emulator: 1x LTT24-08

    const uint32_t new_order[4] = {1234,1235,1237,1236};
    auto res = LTTSetDeviceOrder(4, new_order);
    printf( "RES: %d\n", res );

    // initialize LTT2API
    LTTSetDeviceEmulation( 0x00021821+3 );
    dev_cnt = LTTInit( NULL, NULL, NULL, NULL );

    if( dev_cnt <= 0 ) {
        printf( "INIT: no devices found !!\n" );
        return -1;
    }
    printf("%d\n",dev_cnt );
//    LTTConnectDevices();
//    LTTCascadingMode(0x02);
    // check device type and get channel count
    an_cnt = 0;
    for( dev_idx = 0; dev_idx < dev_cnt; dev_idx++ ) {
        // get device info
        LTTGetDeviceInfoEx( dev_idx, p_dev_info + dev_idx );
        std::cout << "SN: " << p_dev_info[dev_idx].lSerialNumber << ";" ;
        if( p_dev_info[ dev_idx ].lDeviceType != 24 ) {
            printf( "INIT: at least one of the devices is not a LTT24 device !!Mixing different device types is still not possible!\n" );
            return -2;
        }

        if( dev_idx == 0 )
            sample_size = p_dev_info[ dev_idx ].sample_size;

        else if( sample_size != p_dev_info[ dev_idx ].sample_size ) {
            printf( "INIT: devices have different sample-sizes !!Currently not supported!\n" );
            return -2;
        }

        printf( "INIT: devices %d --> LTT%d [%dBit]\n", dev_idx, p_dev_info[ dev_idx ].lDeviceType, sample_size * 8 );

        an_cnt += p_dev_info[ dev_idx ].lChannels;
    }

    // get channel-id list
    for( ch_idx = 0; ch_idx < an_cnt; ch_idx++ )
        p_chnl_id[ ch_idx ] = LTTGetChannelID( ch_idx, LTT_CHNL_TYPE_ANALOG );

    printf( "INIT: done\n\n" );


    // --- CONFIG --------------------------------------------------------------
    LTTResetParameter( 0 );

    // ### CHANNELS ##################

    // switch every second channel on
    sequence_len = 0;
    for( ch_idx = 0; ch_idx < an_cnt; ch_idx++ ) {
        if( ( ch_idx & 0x0001 ) )
            continue;

        // configuration:
        //
        //   active:          ON
        //   input-coupling:  VOLT-DC SingleEnded,
        //   input-range:     +/- 5V
        //   sensor-supply:   OFF
        //   offset:          0 --> NONE
        //
        status = LTTAnalogChannel( p_chnl_id[ ch_idx ],  LTT_CHNL_NORMAL, LTT_CHNL_24_COUP_VOLT_SE_DC_BNC, 50000, 0, 0 );
        if( status != OSAL_OK ) {
            printf( "CONFIG: configuration of channel %d failed with error %d\n", p_chnl_id[ ch_idx ], status );
            return -3;
        }

        printf( "CONFIG: channel %d --> ON\n", p_chnl_id[ ch_idx ] );

        p_seq_id[ sequence_len ] = p_chnl_id[ ch_idx ];
        sequence_len++;
    }


    // ### TRIGGER ##################

    // trigger: OFF
    LTTTrigger( 0, 0, 0, 0, 0, 0, 0 );
    trig = 0;

    /*
    // trigger: ON
    //   slope:    positive
    //   channel:  0
    //   level:    2V [=40% of 5V FSR]
    //   sens:     20mV [=0,4% of 5V FSR]
    LTTTrigger( 1, LTT_TRIG_SLOPE_POS, p_chnl_id[ 0 ], LTT_CHNL_TYPE_ANALOG, LTT_TRIG_MODE_LEVEL | LTT_TRIG_TYPE_SINGLE, 40.0, 0.4 );
    trig = 1;

    // PRE-trigger: 1 kSample
    LTTTrigger_SetPreTrigger( 1, 0 );
    */


    // ### TIMEBASE/FILTER ##########

    // timebase: 500kHz [=2000ns]
    status = LTTSampleTime( 2000, LTT_SMPLTIME_FLAG_ABOVE );
    if( status != 2000 ) {
        printf( "CONFIG: setting of timebase failed\n" );
        return -3;
    }

    // Filter: OFF
    status = LTTFilter( LTT_FILTER_OFF, 0 );

    /*
    // Filter: Butterworth 6-pol, 100kHz [=10000ns]
    status = LTTFilter( LTT_FILTER_ON | LTT_FILT_TYPE_BUTTERWORTH | LTT_FILT_POL_SIX, 10000 );
    */


    // ### BUFFER-Type ##############
    LTTSTREAMTransfer( 0, 0 );

    // ### UPLOAD ###################
    status = LTTUploadParameters();
    if( status != 0 ) {
        printf( "CONFIG: parameter uplaod failed with error %d\n", status );
        return -3;
    }

    // post upload: get conversion factor: [ADC] --> [V]
    for( seq_idx = 0; seq_idx < sequence_len; seq_idx++ )
        p_seq_conv[ seq_idx ] = LTTGetChannelConverter( p_seq_id[ seq_idx ] ) * 0.001;

    printf( "CONFIG: done\n\n" );


    // --- MEASUREMENT ----------------------------------------------------------
    unsigned char *p_data;
    int32_t idx, xfer_seq_cnt, chunk_seq_cnt, xfer_error, xfer_prg_cnt;
    double volt_val;
    double p_seq_sum[ 4 * 32 ];

    // block-length as multiple of 1kS !!
    // 1.25MS --> 2.621s
    sequence_cnt  = 1280 * 1024;
    // chunks:
    // 4kS --> 8.192ms
    chunk_seq_cnt = 4 * 1024;

    p_data = (unsigned char *)malloc( dev_cnt * chunk_seq_cnt * sequence_len * sample_size );
    if( !p_data ) {
        printf( "MEAS: memory allocatin error!\n" );
        return -4;
    }

    for( seq_idx = 0; seq_idx < sequence_len; seq_idx++ )
        p_seq_sum[ seq_idx ] = 0.0;

    // start measurement in device and activate trigger (if configured)
    LTTStart();
    printf( "MEAS: measurement started\n" );

    // check trigger
    if( trig ) {
        while( 1 ) {
            if( LTTCheckTrigger( 50 ) ) {
                printf( "MEAS: trigger fullfilled\n" );
                break;
            }
        }
    }

    // get all data in chunks
    xfer_seq_cnt = 0;
    xfer_error   = 0;
    xfer_prg_cnt = 0;
    while( xfer_seq_cnt < sequence_cnt ) {
        LTTSyncCorrection(1);
        // -----------------------------------------------------------------------------------
        // initiate transfer NON-BLOCKED
        status = LTTStream2Buffer( p_data, chunk_seq_cnt, LTT_TRANSFER_NONEBLOCKED | LTT_TRANSFER_CONTINUOUS );
        if( status != 0 ) {
            printf( "MEAS: error initating transfer %d\n", status );
            break;
        }

        // -----------------------------------------------------------------------------------
        // wait till transfer finished
        while( 1 ) {
            // check for transfer
            status = LTTDataTransferStatus();
            if( status == chunk_seq_cnt )
                // chunk received
                break;

            else if( status >= 0 || status < -3 ) {
                printf( "MEAS: transfer error %d\n", status );
                xfer_error = 1;
                break;
            }

            else {
                // nothing to do

                //

            }

            // !!!! Do own stuff while waiting !!!!!!!

        }

        if( xfer_error )
            break;


        // -----------------------------------------------------------------------------------
        // handle received chunk

        // data are organized in groups af sequences of active channels.
        // example:
        //
        // channels 0, 1, 3, 7 are active.
        // the buffer will look like:
        //
        //   |           0           |           1           |           2           |...
        //   |[ 0 ] [ 1 ] [ 3 ] [ 7 ]|[ 0 ] [ 1 ] [ 3 ] [ 7 ]|[ 0 ] [ 1 ] [ 3 ] [ 7 ]|...

        // transfer OK --> analyze data
        if( sample_size == 2 ) {
            // 16-Bit samples
            int16_t *p_smpl;

            // go through sequence
            for( seq_idx = 0; seq_idx < sequence_len; seq_idx++ ) {

                // get first sample of channel
                p_smpl = ( (int16_t *)p_data ) + seq_idx;

                // go through all samples of channel
                for( idx = 0; idx < status; idx++ ) {
                    // convert sample to volt
                    volt_val = ( (double )(*p_smpl) * p_seq_conv[ seq_idx ] );

                    // for example: sum values
                    p_seq_sum[ seq_idx ] += volt_val;

                    // next sample of channel
                    p_smpl += sequence_len;
                }
            }
        }

        else if( sample_size == 4 ) {
            // 32-Bit samples
            int32_t *p_smpl;

            // go through sequence
            for( seq_idx = 0; seq_idx < sequence_len; seq_idx++ ) {

                // get first sample of channel
                p_smpl = ( (int32_t *)p_data ) + seq_idx;

                // go through all samples of channel
                for( idx = 0; idx < status; idx++ ) {
                    // convert sample to volt
                    volt_val = ( (double )(*p_smpl) * p_seq_conv[ seq_idx ] );

                    // for example: sum values
                    p_seq_sum[ seq_idx ] += volt_val;

                    // next sample of channel
                    p_smpl += sequence_len;
                }
            }
        }

        // adapt transfer counter
        xfer_seq_cnt += chunk_seq_cnt;

        printf( "+" );
        xfer_prg_cnt++;
        if( xfer_prg_cnt == 40 ) {
            printf( "\n" );
            xfer_prg_cnt = 0;
        }
    }

    if( !xfer_prg_cnt )
        printf( "\n" );

    for( seq_idx = 0; seq_idx < sequence_len; seq_idx++ )
        printf( "MEAS: channel %d --> %gV\n", p_seq_id[ seq_idx ], p_seq_sum[ seq_idx ] / (double )xfer_seq_cnt );

    printf( "MEAS: done\n\n" );

    LTTDeInit();

    return 0;
}
