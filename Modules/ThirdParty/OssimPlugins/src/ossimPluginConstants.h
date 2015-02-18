//----------------------------------------------------------------------------
// Copyright (c) 2005, David Burken, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Constants file for ossim plugins.
//
// $Id: ossimPluginConstants.h 7530 2005-05-23 17:21:24Z dburken $
//----------------------------------------------------------------------------
#ifndef ossimPluginConstants_HEADER
#define ossimPluginConstants_HEADER

/**
 * DLL IMPORT/EXORT SECTION
 */
#if defined(OSSIM_STATIC)
#  define OSSIMPLUGINSEXPORT
#  define OSSIMPLUGINSIMPORT
#  define OSSIMPLUGINSDLLEXPORT
#  define OSSIM_PLUGINS_DLL
#elif defined(__MINGW32__) || defined(__CYGWIN__) || defined(_MSC_VER) || defined(__VISUALC__) || defined(__BORLANDC__) || defined(__WATCOMC__)
#  define OSSIMPLUGINSEXPORT __declspec(dllexport)
#  define OSSIMPLUGINSIMPORT __declspec(dllimport)
#  ifdef OSSIMPLUGINSMAKINGDLL
#    define OSSIMPLUGINSDLLEXPORT OSSIMPLUGINSEXPORT
#    define OSSIM_PLUGINS_DLL       OSSIMPLUGINSEXPORT
#  else
#    define OSSIMPLUGINSDLLEXPORT OSSIMPLUGINSIMPORT
#    define OSSIM_PLUGINS_DLL      OSSIMPLUGINSIMPORT
#  endif
#else /* not #if defined(_MSC_VER) */
#  define OSSIMPLUGINSEXPORT
#  define OSSIMPLUGINSIMPORT
#  define OSSIMPLUGINSDLLEXPORT
#  define OSSIM_PLUGINS_DLL
#endif /* #if defined(_MSC_VER) */

#endif /* #ifndef ossimPluginConstants_HEADER */
