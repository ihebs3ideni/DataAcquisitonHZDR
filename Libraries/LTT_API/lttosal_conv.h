// ********************************************************
// Copyright (C) 2004-2013 by LTT GmbH
//
// FILE:      lttosal_conv.h
//
// DESCR:
//
// AUTHOR:    Stefan Knorr
//
// VERSION:   3.0.x
//
// ********************************************************

#ifndef __LTTOSAL_CONV_H__
#define __LTTOSAL_CONV_H__

#include "lttosal.h"
#include "lttosal_export.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************

  FUNCTION: OSALConvInt64ToStr

  DESCR.:   converts signed 64-Bit value to text

  INPUT:    value:
                signed 64-Bit value

            p_str:
                pointer to string

            base:
                base [2..16]

  OUTPUT:   p_str - pointer to string

*****************************************/
__OSALIMP char * __OSALDEC OSALConvInt64ToStr( int64_t value,
                                               char *p_str,
                                               int32_t base );


/*****************************************

  FUNCTION: OSALConvUInt64ToStr

  DESCR.:   converts unsigned 64-Bit value to text

  INPUT:    value:
                unsigned 64-Bit value

            p_str:
                pointer to string

            base:
                base [2..16]

  OUTPUT:   p_str - pointer to string

*****************************************/
__OSALIMP char * __OSALDEC OSALConvUInt64ToStr( uint64_t value,
                                                char *p_str,
                                                int32_t base );



#ifdef __cplusplus
}
#endif


#endif


