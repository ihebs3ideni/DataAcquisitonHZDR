// *******************************************
// Copyright (C) 2004-2010 by LTT GmbH
//
// FILE:      lttosal.h
//
// DESCR:
//
// AUTHOR:    Stefan Knorr
//
// VERSION:   2.2.x
//
// *******************************************


#ifndef __LTTOSAL_H__
#define __LTTOSAL_H__

#if defined __GNUC__
#  include <inttypes.h>
#else
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#endif

#if !defined(_WIN32)
#include <byteswap.h>
//#include <lttosal_byteswap.h>
#endif

// useful macros's
#define _CONV_INT64_T(x)     *( (int64_t *)( (void *)&(x) ) )
#define _HI32_INT64_T(x)     ( (int32_t          )( (x) >> 32 ) )
#define _LO32_INT64_T(x)     ( (uint32_t )( (x) & 0x00000000FFFFFFFFLL ) )

#define ABSVAL(x)            ( (x) < 0 ? -(x) : (x) )

#define ROTL64(x,n)          ( ( (x) << ( (n) & 0x3F ) ) | ( (x) >> ( 64 - ( (n) & 0x3F ) ) ) )
#define ROTR64(x,n)          ( ( (x) >> ( (n) & 0x3F ) ) | ( (x) << ( 64 - ( (n) & 0x3F ) ) ) )

#define ROTL32(x,n)          ( ( (x) << ( (n) & 0x1F ) ) | ( (x) >> ( 32 - ( (n) & 0x1F ) ) ) )
#define ROTR32(x,n)          ( ( (x) >> ( (n) & 0x1F ) ) | ( (x) << ( 32 - ( (n) & 0x1F ) ) ) )

#define ROTL16(x,n)          ( ( (x) << ( (n) & 0x0F ) ) | ( (x) >> ( 16 - ( (n) & 0x0F ) ) ) )
#define ROTR16(x,n)          ( ( (x) >> ( (n) & 0x0F ) ) | ( (x) << ( 16 - ( (n) & 0x0F ) ) ) )

#define ROTL8(x,n)           ( ( (x) << ( (n) & 0x07 ) ) | ( (x) >> (  8 - ( (n) & 0x07 ) ) ) )
#define ROTR8(x,n)           ( ( (x) >> ( (n) & 0x07 ) ) | ( (x) << (  8 - ( (n) & 0x07 ) ) ) )

#define SHIFTL(x,n)          ( (x) << (n) )
#define SHIFTR(x,n)          ( (x) >> (n) )


// useful base-types
typedef struct __OSALCOMPLEX_32 {
  int32_t re;
  int32_t im;
} OSALComplex_32;


typedef struct __OSALCOMPLEX_FL {
  float re;
  float im;
} OSALComplex_FL;


typedef struct __OSALLONG_64 {
  uint32_t low;
  int32_t high;
} OSALLong_64;



#ifdef SKNET_ARCH_PPC
#  define OSALNET_SWP_16(x)               ( ROTR16( (x), 8 ) )
#  define OSALNET_SWP_32(x)               ( ( ROTR32( (x), 8 ) & 0xFF00FF00 ) | ( ROTL32( (x), 8 ) & 0x00FF00FF ) )
#  define OSALNET_SWP_64(x)               ( ( ROTR64( (x),  8 ) & 0xFF000000FF000000LL ) | ( ROTL64( (x),  8 ) & 0x000000FF000000FFLL ) | ( ROTL64( (x), 24 ) & 0x00FF000000FF0000LL ) | ( ROTL64( (x), 24 ) & 0x0000FF000000FF00LL ) | )
#  define OSALNET_REVSWP_16(x)            (x)
#  define OSALNET_REVSWP_32(x)            (x)
#  define OSALNET_REVSWP_64(x)            (x)
#else
#  define OSALNET_SWP_16(x)               (x)
#  define OSALNET_SWP_32(x)               (x)
#  define OSALNET_SWP_64(x)               (x)
#  define OSALNET_REVSWP_16(x)            ( ROTR16( (x), 8 ) )
#  define OSALNET_REVSWP_32(x)            ( ( ROTR32( (x), 8 ) & 0xFF00FF00 ) | ( ROTL32( (x), 8 ) & 0x00FF00FF ) )
#  define OSALNET_REVSWP_64(x)            ( ( ROTR64( (x),  8 ) & 0xFF000000FF000000LL ) | ( ROTL64( (x),  8 ) & 0x000000FF000000FFLL ) | ( ROTL64( (x), 24 ) & 0x00FF000000FF0000LL ) | ( ROTL64( (x), 24 ) & 0x0000FF000000FF00LL ) | )
#endif


enum OSAL_WAITCODES {
  OSAL_NO_WAIT         =  0,
  OSAL_WAIT_INFINITE   = -1
};

// define error codes
enum OSAL_ERRCODES {
  OSAL_TRUE            =  1,
  OSAL_FALSE           =  0,

  OSAL_PENDING         =  1,
  OSAL_OK              =  0,
  OSAL_EIOCTL          = -1,
  OSAL_ENOTCONNECTED   = -2,
  OSAL_EHANDLE         = -3,
  OSAL_ECONNECTION     = -4,
  OSAL_ESCANDISK       = -5,
  OSAL_ESCANINTERFACE  = -6,
  OSAL_EINIT           = -7,
  OSAL_ERESCAN         = -8,
  OSAL_EBREAK          = -9,
  OSAL_EOBJECT         = -10,
  OSAL_EARG            = -11,
  OSAL_EALLOC          = -12,
  OSAL_EBUSY           = -13,
  OSAL_ETIMEOUT        = -14,
  OSAL_ERROR           = -15,
  OSAL_EAGAIN          = -16,

  OSAL_EIO             = -20,
  OSAL_EEXIST          = -21,
  OSAL_ESCAN_TYPE      = -22,
  OSAL_ENOT_IMPL       = -23,
  OSAL_ENOPERMISION    = -24,
  OSAL_ENODATA         = -25,
  OSAL_EFORMAT         = -26,
  OSAL_EQUEUE_FULL     = -27,
  OSAL_ESOCKET         = -28,
  OSAL_ENOMEM          = -29,
  OSAL_EACCESS         = -30,
  OSAL_EINTERUPTED     = -31,

  OSAL_ENODETYPE       = -40,
  OSAL_ENODECONFIG     = -41,

  OSAL_EFFTCONFIG      = -50,

  OSAL_ENODECNT        = -80,
  OSAL_EXFERON         = -81,
  OSAL_EDENIED         = -82,
  OSAL_EREQUEST        = -83,
  OSAL_EXFERCONFIG     = -84,
  OSAL_EXFERSTART      = -85,
  OSAL_EATTACH         = -86,
  OSAL_ECFGBUFLEN      = -87,
  OSAL_EOPMODE         = -88,
  OSAL_ECHECKSUM       = -89,
  OSAL_EPLLSYNC        = -90,
  OSAL_EDEVTYPE        = -91,

  OSAL_ELICENSE        = -128,
  OSAL_EHLMISSING      = -129,
  OSAL_EHLTYPE         = -130,
  OSAL_EMASTERDENIED   = -131,
  
  OSAL_EOTHER          = -255

};


#define OSAL_SW_MAYOR(x)            ( ( (uint32_t )(x) >> 28 ) & 0x0000000F )
#define OSAL_SW_MINOR(x)            ( ( (uint32_t )(x) >> 24 ) & 0x0000000F )
#define OSAL_SW_BUG(x)              ( ( (uint32_t )(x) >> 16 ) & 0x000000FF )
#define OSAL_SW_BUILD(x)            (   (uint32_t )(x)         & 0x0000FFFF )

#define OSAL_SW_CREATE(x,y,z,b)     ( ( ( (x) & 0x0000000F ) << 28 ) | ( ( (y) & 0x0000000F ) << 24 ) | ( ( (z) & 0x000000FF ) << 16 ) | ( (b) & 0x0000FFFF ) )

// system specific macros
#ifdef _WIN32
#  define OSAL_LOG_PREFIX           "C:"
#  define OSAL_PATH_SEPARATOR       "\\"
#else
#  define OSAL_LOG_PREFIX           "/tmp"
#  define OSAL_PATH_SEPARATOR       "/"

#  ifndef _XOPEN_SOURCE
#    define _XOPEN_SOURCE           600
#  endif
#endif


#endif


