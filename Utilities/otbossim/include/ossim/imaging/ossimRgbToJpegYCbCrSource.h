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
// $Id: ossimRgbToJpegYCbCrSource.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimRgbToJpegYCbCrSource_HEADER
#define ossimRgbToJpegYCbCrSource_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>

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
