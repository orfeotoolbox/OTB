//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimRgbToJpegYCbCrSource.h,v 1.6 2005/02/11 15:07:34 dburken Exp $
#ifndef ossimRgbToJpegYCbCrSource_HEADER
#define ossimRgbToJpegYCbCrSource_HEADER
#include <imaging/tile_sources/ossimImageSourceFilter.h>

class ossimRgbToJpegYCbCrSource : public ossimImageSourceFilter
{
public:
   
   ossimRgbToJpegYCbCrSource();
   ossimRgbToJpegYCbCrSource(ossimImageSource* inputSource);
   virtual ~ossimRgbToJpegYCbCrSource();
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);
       
protected:
   ossimRefPtr<ossimImageData> theBlankTile;

TYPE_DATA
};

#endif
