//*******************************************************************
// Copyright (C) 2004 Intelligence Data Systems, Inc.  All rights reserved.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: David Burken
//
// Description: Common file for global functions.
//
//*************************************************************************
// $Id: ossimCommon.cpp,v 1.9 2006/01/10 21:04:02 gpotts Exp $

#include <base/common/ossimCommon.h>
#include <base/common/ossimConstants.h>
#include <base/common/ossimPreferences.h>
#include <base/common/ossimTrace.h>
#include <base/context/ossimNotifyContext.h>
#include <base/data_types/ossimIpt.h>
#include <sstream>

ossimMutex theCriticalSectionMutex;

static ossimTrace traceDebug("ossimCommon:debug");

ossimByteOrder ossimGetByteOrder()
{
   union
   {
      short s;
      char  c[sizeof(short)];
   } un;

   un.s = 0x0102;
   if (un.c[0] ==  2 && un.c[1] == 1)
   {
      return OSSIM_LITTLE_ENDIAN;
   }
   else
   {
      return OSSIM_BIG_ENDIAN;
   }
}

double ossimGetDefaultMin(ossimScalarType scalarType)
{
   switch(scalarType)
   {
      case OSSIM_UINT8:
      {
	return OSSIM_DEFAULT_MIN_PIX_UCHAR;
      }
      case OSSIM_SINT8:
      {
	return OSSIM_DEFAULT_MIN_PIX_SINT8;
      }
      case OSSIM_UINT16:
      {
         return OSSIM_DEFAULT_MIN_PIX_UINT16;
      }
      case OSSIM_SINT16:
      {
         return OSSIM_DEFAULT_MIN_PIX_SINT16;
      }
      case OSSIM_USHORT11:
      {
         return OSSIM_DEFAULT_MIN_PIX_UINT11;
      }
      case OSSIM_UINT32:
      {
         return OSSIM_DEFAULT_MIN_PIX_UINT32;
      }
      case OSSIM_SINT32:
      {
         return OSSIM_DEFAULT_MIN_PIX_SINT32;
      }
      case OSSIM_FLOAT32:
      {
         return OSSIM_DEFAULT_MIN_PIX_FLOAT;
      }
      case OSSIM_NORMALIZED_FLOAT:
      {
         return OSSIM_DEFAULT_MIN_PIX_NORM_FLOAT;
      }
      case OSSIM_FLOAT64:
      {
         return OSSIM_DEFAULT_MIN_PIX_DOUBLE;
      }
      case OSSIM_NORMALIZED_DOUBLE:
      {
         return OSSIM_DEFAULT_MIN_PIX_NORM_DOUBLE;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << __FILE__ << ":" << __LINE__
               << "\nUnhandled scalar type:  " << scalarType << endl;
         }
         break;
      }
   }
   
   return OSSIM_DBL_NAN;
}

double ossimGetDefaultMax(ossimScalarType scalarType)
{
   switch(scalarType)
   {
      case OSSIM_UINT8:
      {
         return OSSIM_DEFAULT_MAX_PIX_UCHAR;
      }
      case OSSIM_SINT8:
      {
         return OSSIM_DEFAULT_MAX_PIX_SINT8;
      }
      case OSSIM_UINT16:
      {
	return OSSIM_DEFAULT_MAX_PIX_UINT16;
      }
      case OSSIM_SINT16:
      {
         return OSSIM_DEFAULT_MAX_PIX_SINT16;
      }
      case OSSIM_USHORT11:
      {
         return OSSIM_DEFAULT_MAX_PIX_UINT11;
      }
      case OSSIM_UINT32:
      {
	return OSSIM_DEFAULT_MAX_PIX_UINT32;
      }
      case OSSIM_SINT32:
      {
         return OSSIM_DEFAULT_MAX_PIX_SINT32;
      }
      case OSSIM_FLOAT32:
      {
         return OSSIM_DEFAULT_MAX_PIX_FLOAT;
      }
      case OSSIM_NORMALIZED_FLOAT:
      {
         return OSSIM_DEFAULT_MAX_PIX_NORM_FLOAT;
      }
      case OSSIM_FLOAT64:
      {
         return OSSIM_DEFAULT_MAX_PIX_DOUBLE;
      }
      case OSSIM_NORMALIZED_DOUBLE:
      {
         return OSSIM_DEFAULT_MAX_PIX_NORM_DOUBLE;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << __FILE__ << ":" << __LINE__
               << "\nUnhandled scalar type:  " << scalarType << endl;
         }
         break;
      }
   }
   return OSSIM_DBL_NAN;
}

double ossimGetDefaultNull(ossimScalarType scalarType)
{
   switch(scalarType)
   {
      case OSSIM_UINT8:
      {
         return 0;
      }
      case OSSIM_SINT8:
      {
         return OSSIM_DEFAULT_MIN_PIX_SINT8-10;
      }
      case OSSIM_UINT16:
      {
         return 0;
      }
      case OSSIM_SINT16:
      {
         return OSSIM_SSHORT_NAN;
      }
      case OSSIM_USHORT11:
      {
         return 0;
      }
      case OSSIM_UINT32:
      {
         return 0;
      }
      case OSSIM_SINT32:
      {
         return OSSIM_INT_NAN;
      }
      case OSSIM_FLOAT32:
      {
         return OSSIM_FLT_NAN;
      }
      case OSSIM_NORMALIZED_FLOAT:
      {
         return 0.0;
      }
      case OSSIM_FLOAT64:
      {
         return OSSIM_DBL_NAN;
      }
      case OSSIM_NORMALIZED_DOUBLE:
      {
         return 0.0;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << __FILE__ << ":" << __LINE__
               << "\nUnhandled scalar type:  " << scalarType << endl;
         }
         break;
      }
    }

  return OSSIM_DBL_NAN;
}

ossim_uint32 ossimGetScalarSizeInBytes(ossimScalarType scalarType)
{
   switch(scalarType)
   {
      case OSSIM_UINT8:
      {
         return sizeof(ossim_uint8);
      }
      case OSSIM_SINT8:
      {
         return sizeof(ossim_sint8);
      }
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         return sizeof(ossim_uint16);
      }
      case OSSIM_SINT16:
      {
         return sizeof(ossim_sint16);
      }
      case OSSIM_UINT32:
      {
         return sizeof(ossim_uint32);
      }
      case OSSIM_SINT32:
      {
         return sizeof(ossim_sint32);
      }
      case OSSIM_FLOAT32:
      case OSSIM_NORMALIZED_FLOAT:
      {
         return sizeof(ossim_float32);
      }
      case OSSIM_FLOAT64:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         return sizeof(ossim_float64);
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << __FILE__ << ":" << __LINE__
               << "\nUnhandled scalar type:  " << scalarType << endl;
         }
         break;
      }
  }
  
  return 1;
}

void ossimGetDefaultTileSize(ossimIpt& tileSize)
{
   const char* tileSizeKw = ossimPreferences::instance()->
      findPreference("tile_size");

   if(tileSizeKw)
   {
      std::istringstream in(tileSizeKw);
      bool hasX = true;
      in >> tileSize.x >> tileSize.y;
      
      if(tileSize.x < 1)
      {
         tileSize.x = OSSIM_DEFAULT_TILE_WIDTH;
         hasX = false;
      }
      if(tileSize.y < 1)
      {
         if(!hasX)
         {
            tileSize.y = OSSIM_DEFAULT_TILE_HEIGHT;
         }
         else
         {
            tileSize.y = tileSize.x;
         }
      }
   }
   else
   {
      tileSize.x = OSSIM_DEFAULT_TILE_WIDTH;
      tileSize.y = OSSIM_DEFAULT_TILE_HEIGHT;
   }
}
