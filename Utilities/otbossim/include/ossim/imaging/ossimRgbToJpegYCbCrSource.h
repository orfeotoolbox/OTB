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
// $Id: ossimRgbToJpegYCbCrSource.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimRgbToJpegYCbCrSource_HEADER
#define ossimRgbToJpegYCbCrSource_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>

class ossimRgbToJpegYCbCrSource : public ossimImageSourceFilter
{
public:
   
   ossimRgbToJpegYCbCrSource();
   ossimRgbToJpegYCbCrSource(ossimImageSource* inputSource);
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);
       
protected:
   virtual ~ossimRgbToJpegYCbCrSource();
   ossimRefPtr<ossimImageData> theBlankTile;

TYPE_DATA
};

#endif
