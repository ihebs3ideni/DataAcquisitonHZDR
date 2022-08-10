/**************************************************************
 *    LTT2API - Header
 *
 *  (c) Labortechnik Tasler GmbH
 *      Friedrich-Bergius-Ring 15
 *      97076 Wrzburg
 *      Germany
 *      http://www.tasler.de
 *
 *  V:  2.0.0
 *  A:  STK
 *
 **************************************************************/

#ifndef __LTT2API_EXPORT_H__
#define __LTT2API_EXPORT_H__


#ifdef _WIN32
#  define __APIDEC  __stdcall

#  ifdef LTT2API_SHARED_LIB
#    ifdef LTT2API_BUILD_DLL
#      define __LTT2API_IMP __declspec(dllexport)
#    else
#      define __LTT2API_IMP __declspec(dllimport)
#    endif
#  else
#    define __LTT2API_IMP 
#  endif

#else
#  define __APIDEC  
#  define __LTT2API_IMP 

#endif


#endif


