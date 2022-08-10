// ********************************************************
// Copyright (C) 2004-2013 by LTT GmbH
//
// FILE:      lttosal_time.h
//
// DESCR:
//
// AUTHOR:    Stefan Knorr
//
// VERSION:   3.0.x
//
// ********************************************************


#ifndef __LTTOSAL_TIME_H__
#define __LTTOSAL_TIME_H__

#include "lttosal.h"
#include "lttosal_export.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct _OSALLOCALTIME {
  int16_t year;
  int16_t month;
  int16_t day;
  int16_t hour;
  int16_t minute;
  int16_t second;
} OSALLocalTime;


typedef struct _OSALTIMECOUNT {
  int32_t sec;
  int32_t usec;
  int64_t _usec;
} OSALTimeCount;

#ifdef _WIN32
#define OSALTIMECOUNT_LIMIT                   ( 0x0000000100000000LL * 1000LL )
#else
#define OSALTIMECOUNT_LIMIT                   ( 0x4000000000000000LL )
#endif

#define OSALTimeConvertDiffTime2Second(x)     ( ( ( (x)->day * 24 + (x)->hour ) * 60 + (x)->minute ) * 60 + (x)->second )

/*****************************************

  FUNCTION: OSALTimeGetLocalTime

  DESCR.:   Get local-time

  INPUT:    p_localtime:
                pointer to local-time object

  OUTPUT:   OSAL_OK        - if successful
            OSAL_EOBJECT   - invalid pointer

*****************************************/
__OSALIMP int32_t __OSALDEC OSALTimeGetLocalTime( OSALLocalTime *p_localtime );


/*****************************************

  FUNCTION: OSALTimeDiffLocalTime

  DESCR.:   Get local-time difference
            diff = two - one

  INPUT:    p_diff:
                pointer to local-diff-time object

            p_one:
                pointer to first local-time object

            p_two:
                pointer to second local-time object

  OUTPUT:   OSAL_OK        - if successful
            OSAL_EOBJECT   - invalid pointer

*****************************************/
__OSALIMP int32_t __OSALDEC OSALTimeDiffLocalTime( OSALLocalTime *p_diff,
                                                   const OSALLocalTime *p_one,
                                                   const OSALLocalTime *p_two );

    
/*****************************************

  FUNCTION: OSALTimeConvertDiffTime

  DESCR.:   Convert time [s] --> [local-diff-time]

  INPUT:    p_localtime:
                pointer to local-time object

            diff_time:
                diff-time [s]

  OUTPUT:   OSAL_OK        - if successful
            OSAL_EOBJECT   - invalid pointer

*****************************************/
__OSALIMP int32_t __OSALDEC OSALTimeConvertDiffTime( OSALLocalTime *p_localtime,
                                                     uint32_t diff_time );


/*****************************************

  FUNCTION: OSALTimeConvertTime

  DESCR.:   Convert time [s] --> [local-time]
            since 1970

  INPUT:    p_localtime:
                pointer to local-time object

            time:
                time [s]

  OUTPUT:   OSAL_OK        - if successful
            OSAL_EOBJECT   - invalid pointer

*****************************************/
__OSALIMP int32_t __OSALDEC OSALTimeConvertTime( OSALLocalTime *p_localtime,
                                                 uint32_t time );
                                              
                                              
/*****************************************

  FUNCTION: OSALTimeConvertLocalTime

  DESCR.:   Convert [local-diff-time] --> time [s]

  INPUT:    p_time:
                time [s]

            p_localtime:
                pointer to local-time object

  OUTPUT:   OSAL_OK        - if successful
            OSAL_EOBJECT   - invalid pointer

*****************************************/
__OSALIMP int32_t __OSALDEC OSALTimeConvertLocalTime( uint32_t *p_time,
                                                      const OSALLocalTime *p_localtime );


/*****************************************

  FUNCTION: OSALTimeGetCount

  DESCR.:   Get time-count

  INPUT:    p_count:
                pointer to time-count

  OUTPUT:   OSAL_OK        - if successful
            OSAL_EOBJECT   - invalid pointer

*****************************************/
__OSALIMP int32_t __OSALDEC OSALTimeGetCount( OSALTimeCount *p_count );
  
      
/*****************************************

  FUNCTION: OSALTimeCountAdd

  DESCR.:   add to time-count

  INPUT:    p_result:
                pointer to time-count

            p_add:
                pointer to add time-count

  OUTPUT:   OSAL_OK        - if successful
            OSAL_EOBJECT   - invalid pointer

*****************************************/
__OSALIMP int32_t __OSALDEC OSALTimeCountAdd( OSALTimeCount *p_result,
                                              OSALTimeCount *p_add );


/*****************************************

  FUNCTION: OSALTimeCountDiff

  DESCR.:   get time-count difference

  INPUT:    p_result:
                pointer to time-count

            p_end:
                pointer to end time-count

            p_start:
                pointer to start time-count

  OUTPUT:   OSAL_OK        - if successful
            OSAL_EOBJECT   - invalid pointer

*****************************************/
__OSALIMP int32_t __OSALDEC OSALTimeCountDiff( OSALTimeCount *p_result,
                                               OSALTimeCount *p_end,
                                               OSALTimeCount *p_start );


/*****************************************

  FUNCTION: OSALTimeGetDiffCount

  DESCR.:   get time-count difference to
            current time-count. Current
            time-count will be stored in
            p_start.

  INPUT:    p_result:
                pointer to time-count

            p_start:
                pointer to start time-count

  OUTPUT:   OSAL_OK        - if successful
            OSAL_EOBJECT   - invalid pointer

*****************************************/
__OSALIMP int32_t __OSALDEC OSALTimeGetDiffCount( OSALTimeCount *p_result,
                                                  OSALTimeCount *p_start );


#ifdef __cplusplus
}
#endif

#endif

