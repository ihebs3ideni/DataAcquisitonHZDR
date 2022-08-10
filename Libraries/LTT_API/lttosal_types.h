// ********************************************************
// Copyright (C) 2004-2013 by LTT GmbH
//
// FILE:      lttosal_types.h
//
// DESCR:
//
// AUTHOR:    Stefan Knorr
//
// VERSION:   3.0.x
//
// ********************************************************


#ifndef __LTTOSAL_TYPES_H__
#define __LTTOSAL_TYPES_H__

#include "lttosal.h"


#ifdef __cplusplus
extern "C" {
#endif

enum OSAL_DataTypeSizes {
  OSAL_DATATYPE_BITSIZE_16      = 0x0010,
  OSAL_DATATYPE_BITSIZE_24      = 0x0018,
  OSAL_DATATYPE_BITSIZE_32      = 0x0020,
  OSAL_DATATYPE_BITSIZE_48      = 0x0030,
  OSAL_DATATYPE_BITSIZE_64      = 0x0040,
  OSAL_DATATYPE_BITSIZE_80      = 0x0050,
  OSAL_DATATYPE_BITSIZE_MASK    = 0x00FF
};

enum OSAL_DataTypeFlags {
  OSAL_DATATYPE_FLAG_SGND_INT   = 0x0000,
  OSAL_DATATYPE_FLAG_USGND_INT  = 0x0100,
  OSAL_DATATYPE_FLAG_FLOAT      = 0x0200,
  OSAL_DATATYPE_FLAG_DIG        = 0x0400,
  OSAL_DATATYPE_FLAG_MASK       = 0x0F00,

  OSAL_DATATYPE_OPTFLAG_MINMAX  = 0x1000,
  OSAL_DATATYPE_OPTFLAG_MASK    = 0xF000
};

enum OSAL_DataTypes {
  OSAL_DATATYPE_INT_16          = OSAL_DATATYPE_BITSIZE_16 | OSAL_DATATYPE_FLAG_SGND_INT,
  OSAL_DATATYPE_INT_24          = OSAL_DATATYPE_BITSIZE_24 | OSAL_DATATYPE_FLAG_SGND_INT,
  OSAL_DATATYPE_INT_32          = OSAL_DATATYPE_BITSIZE_32 | OSAL_DATATYPE_FLAG_SGND_INT,
  OSAL_DATATYPE_INT_48          = OSAL_DATATYPE_BITSIZE_48 | OSAL_DATATYPE_FLAG_SGND_INT,
  OSAL_DATATYPE_INT_64          = OSAL_DATATYPE_BITSIZE_64 | OSAL_DATATYPE_FLAG_SGND_INT,
                               
  OSAL_DATATYPE_UINT_16         = OSAL_DATATYPE_BITSIZE_16 | OSAL_DATATYPE_FLAG_USGND_INT,
  OSAL_DATATYPE_UINT_24         = OSAL_DATATYPE_BITSIZE_24 | OSAL_DATATYPE_FLAG_USGND_INT,
  OSAL_DATATYPE_UINT_32         = OSAL_DATATYPE_BITSIZE_32 | OSAL_DATATYPE_FLAG_USGND_INT,
  OSAL_DATATYPE_UINT_48         = OSAL_DATATYPE_BITSIZE_48 | OSAL_DATATYPE_FLAG_USGND_INT,
  OSAL_DATATYPE_UINT_64         = OSAL_DATATYPE_BITSIZE_64 | OSAL_DATATYPE_FLAG_USGND_INT,
  
  OSAL_DATATYPE_FLOAT_32        = OSAL_DATATYPE_BITSIZE_32 | OSAL_DATATYPE_FLAG_FLOAT,
//  OSAL_DATATYPE_FLOAT_64        = OSAL_DATATYPE_BITSIZE_64 | OSAL_DATATYPE_FLAG_FLOAT,
//  OSAL_DATATYPE_FLOAT_80        = OSAL_DATATYPE_BITSIZE_80 | OSAL_DATATYPE_FLAG_FLOAT,

  OSAL_DATATYPE_MINMAX_INT_16   = OSAL_DATATYPE_INT_16 | OSAL_DATATYPE_OPTFLAG_MINMAX,
  OSAL_DATATYPE_MINMAX_INT_24   = OSAL_DATATYPE_INT_24 | OSAL_DATATYPE_OPTFLAG_MINMAX,
  OSAL_DATATYPE_MINMAX_INT_32   = OSAL_DATATYPE_INT_32 | OSAL_DATATYPE_OPTFLAG_MINMAX,
  OSAL_DATATYPE_MINMAX_INT_48   = OSAL_DATATYPE_INT_48 | OSAL_DATATYPE_OPTFLAG_MINMAX,
  OSAL_DATATYPE_MINMAX_INT_64   = OSAL_DATATYPE_INT_64 | OSAL_DATATYPE_OPTFLAG_MINMAX,

  OSAL_DATATYPE_MINMAX_FLOAT_32 = OSAL_DATATYPE_FLOAT_32 | OSAL_DATATYPE_OPTFLAG_MINMAX,

  OSAL_DATATYPE_DIG_16          = OSAL_DATATYPE_BITSIZE_16 | OSAL_DATATYPE_FLAG_DIG,
  OSAL_DATATYPE_DIG_24          = OSAL_DATATYPE_BITSIZE_24 | OSAL_DATATYPE_FLAG_DIG,
  OSAL_DATATYPE_DIG_32          = OSAL_DATATYPE_BITSIZE_32 | OSAL_DATATYPE_FLAG_DIG,
  OSAL_DATATYPE_DIG_48          = OSAL_DATATYPE_BITSIZE_48 | OSAL_DATATYPE_FLAG_DIG,
  OSAL_DATATYPE_DIG_64          = OSAL_DATATYPE_BITSIZE_64 | OSAL_DATATYPE_FLAG_DIG,

  OSAL_DATATYPE_MINMAX_DIG_16   = OSAL_DATATYPE_DIG_16 | OSAL_DATATYPE_OPTFLAG_MINMAX,
  OSAL_DATATYPE_MINMAX_DIG_24   = OSAL_DATATYPE_DIG_24 | OSAL_DATATYPE_OPTFLAG_MINMAX,
  OSAL_DATATYPE_MINMAX_DIG_32   = OSAL_DATATYPE_DIG_32 | OSAL_DATATYPE_OPTFLAG_MINMAX,
  OSAL_DATATYPE_MINMAX_DIG_48   = OSAL_DATATYPE_DIG_48 | OSAL_DATATYPE_OPTFLAG_MINMAX,
  OSAL_DATATYPE_MINMAX_DIG_64   = OSAL_DATATYPE_DIG_64 | OSAL_DATATYPE_OPTFLAG_MINMAX
};


#define OSAL_DATATYPE_FLAG(x)     ( (x) & OSAL_DATATYPE_FLAG_MASK )
#define OSAL_DATATYPE_OPTFLAG(x)  ( (x) & OSAL_DATATYPE_OPTFLAG_MASK )
//#define OSAL_DATATYPE_BITSIZE(x)  ( ( (x) & OSAL_DATATYPE_BITSIZE_MASK ) << ( ( (x) & OSAL_DATATYPE_OPTFLAG_MINMAX ) >> 12 ) )
//#define OSAL_DATATYPE_SIZE(x)     ( OSAL_DATATYPE_BITSIZE(x) >> 3 )
#define OSAL_DATATYPE_BITSIZE(x)  ( (x) & OSAL_DATATYPE_BITSIZE_MASK )
#define OSAL_DATATYPE_SIZE(x)     ( ( OSAL_DATATYPE_BITSIZE(x) >> 3 ) << ( ( (x) & OSAL_DATATYPE_OPTFLAG_MINMAX ) >> 12 ) )


// --- OSAL_DATATYPE_UINT_16 ------------------------------------------------------------------

// ********************************************************
// FUNCTION:  OSALTypeGetUInt16
//
// DESCR:     Gets value from pointer
//
// INPUT:     p_ptr:
//              pointer to buffer
//
// OUTPUT:    value
//
// ********************************************************
static inline uint16_t OSALTypeGetUInt16( const void *p_ptr ) { return (*( (uint16_t *)p_ptr )); }


// ********************************************************
// FUNCTION:  OSALTypeSetUInt16
//
// DESCR:     Writes value to pointer
//
// INPUT:     p_ptr:
//              pointer to buffer
//
//            val:
//              value
//
// OUTPUT:    None
//
// ********************************************************
static inline void OSALTypeSetUInt16( void *p_ptr, uint16_t val ) { (*( (uint16_t *)p_ptr )) = val; }



// --- OSAL_DATATYPE_UINT_24 ------------------------------------------------------------------

// ********************************************************
// FUNCTION:  OSALTypeGetUInt24
//
// DESCR:     Gets value from pointer
//
// INPUT:     p_ptr:
//              pointer to buffer
//
// OUTPUT:    value
//
// ********************************************************
static inline uint32_t OSALTypeGetUInt24( const void *p_ptr ) { return ( (*( (int32_t *)p_ptr )) & 0x00FFFFFFL ); }


// ********************************************************
// FUNCTION:  OSALTypeSetUInt24
//
// DESCR:     Writes value to pointer
//
// INPUT:     p_ptr:
//              pointer to buffer
//
//            val:
//              value
//
// OUTPUT:    None
//
// ********************************************************
static inline void OSALTypeSetUInt24( const void *p_ptr, uint32_t val ) { (*( (uint32_t *)p_ptr )) = ( val & 0x00FFFFFFL ) | ( (*( (uint32_t *)p_ptr )) & 0xFF000000L ); }



// --- OSAL_DATATYPE_UINT_32 ------------------------------------------------------------------

// ********************************************************
// FUNCTION:  OSALTypeGetUInt32
//
// DESCR:     Gets value from pointer
//
// INPUT:     p_ptr:
//              pointer to buffer
//
// OUTPUT:    value
//
// ********************************************************
static inline uint32_t OSALTypeGetUInt32( const void *p_ptr ) { return (*( (uint32_t *)p_ptr )); }


// ********************************************************
// FUNCTION:  OSALTypeSetUInt32
//
// DESCR:     Writes value to pointer
//
// INPUT:     p_ptr:
//              pointer to buffer
//
//            val:
//              value
//
// OUTPUT:    None
//
// ********************************************************
static inline void OSALTypeSetUInt32( const void *p_ptr, uint32_t val ) { (*( (uint32_t *)p_ptr )) = val; }



// --- OSAL_DATATYPE_INT_16 -------------------------------------------------------------------

// ********************************************************
// FUNCTION:  OSALTypeGetInt16
//
// DESCR:     Gets value from pointer
//
// INPUT:     p_ptr:
//              pointer to buffer
//
// OUTPUT:    value
//
// ********************************************************
static inline int16_t OSALTypeGetInt16( const void *p_ptr ) { return (*( (int16_t *)p_ptr )); }


// ********************************************************
// FUNCTION:  OSALTypeSetInt16
//
// DESCR:     Writes value to pointer
//
// INPUT:     p_ptr:
//              pointer to buffer
//
//            val:
//              value
//
// OUTPUT:    None
//
// ********************************************************
static inline void OSALTypeSetInt16( const void *p_ptr, int16_t val ) { (*( (int16_t *)p_ptr )) = val; }



// --- OSAL_DATATYPE_INT_24 -------------------------------------------------------------------

// ********************************************************
// FUNCTION:  OSALTypeGetInt24
//
// DESCR:     Gets value from pointer
//
// INPUT:     p_ptr:
//              pointer to buffer
//
// OUTPUT:    value
//
// ********************************************************
static inline int32_t OSALTypeGetInt24( const void *p_ptr ) { return ( (*( (int32_t *)p_ptr )) & 0x00FFFFFFL ) | ( ~( ( (*( (int32_t *)p_ptr )) & 0x00800000L ) - 1 ) ); }


// ********************************************************
// FUNCTION:  OSALTypeSetInt24
//
// DESCR:     Writes value to pointer
//
// INPUT:     p_ptr:
//              pointer to buffer
//
//            val:
//              value
//
// OUTPUT:    None
//
// ********************************************************
static inline void OSALTypeSetInt24( const void *p_ptr, int32_t val ) { (*( (int32_t *)p_ptr )) = ( val & 0x00FFFFFFL ) | ( (*( (int32_t *)p_ptr )) & 0xFF000000L ); }



// --- OSAL_DATATYPE_INT_32 -------------------------------------------------------------------

// ********************************************************
// FUNCTION:  OSALTypeGetInt32
//
// DESCR:     Gets value from pointer
//
// INPUT:     p_ptr:
//              pointer to buffer
//
// OUTPUT:    value
//
// ********************************************************
static inline int32_t OSALTypeGetInt32( const void *p_ptr ) { return (*( (int32_t *)p_ptr )); }


// ********************************************************
// FUNCTION:  OSALTypeSetInt32
//
// DESCR:     Writes value to pointer
//
// INPUT:     p_ptr:
//              pointer to buffer
//
//            val:
//              value
//
// OUTPUT:    None
//
// ********************************************************
static inline void OSALTypeSetInt32( const void *p_ptr, int32_t val ) { (*( (int32_t *)p_ptr )) = val; }



// --- OSAL_DATATYPE_FLOAT_32 ------------------------------------------------------------------

// ********************************************************
// FUNCTION:  OSALTypeGetFloat32
//
// DESCR:     Gets value from pointer
//
// INPUT:     p_ptr:
//              pointer to buffer
//
// OUTPUT:    value
//
// ********************************************************
static inline float OSALTypeGetFloat32( const void *p_ptr ) { return (*( (float *)p_ptr )); }


// ********************************************************
// FUNCTION:  OSALTypeSetFloat32
//
// DESCR:     Writes value to pointer
//
// INPUT:     p_ptr:
//              pointer to buffer
//
//            val:
//              value
//
// OUTPUT:    None
//
// ********************************************************
static inline void OSALTypeSetFloat32( const void *p_ptr, float val ) { (*( (float *)p_ptr )) = val; }


#ifdef __cplusplus
}
#endif


#endif

