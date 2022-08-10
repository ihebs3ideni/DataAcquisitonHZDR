// ********************************************************
// Copyright (C) 2004-2013 by LTT GmbH
//
// FILE:      lttosal_export.h
//
// DESCR:
//
// AUTHOR:    Stefan Knorr
//
// VERSION:   3.0.x
//
// ********************************************************


#ifndef __LTTOSAL_EXPORT_H__
#define __LTTOSAL_EXPORT_H__


#ifdef _WIN32
#  define __OSALDEC  __stdcall

#  ifdef LTTOSAL_SHARED_LIB
#    ifdef LTTOSAL_BUILD_DLL
#      define __OSALIMP __declspec (dllexport)
#    else
#      define __OSALIMP __declspec (dllimport)
#    endif
#  else
#    define __OSALIMP 
#  endif

#else
#  define __OSALDEC  
#  define __OSALIMP 

#endif

#endif


