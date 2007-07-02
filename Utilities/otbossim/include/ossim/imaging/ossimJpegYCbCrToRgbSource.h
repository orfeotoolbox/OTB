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
// $Id: ossimJpegYCbCrToRgbSource.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimJpegYCbCrToRgbSource_HEADER
#define ossimJpegYCbCrToRgbSource_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>

class ossimJpegYCbCrToRgbSource : public ossimImageSourceFilter
{
public:
   ossimJpegYCbCrToRgbSource();
   ossimJpegYCbCrToRgbSource(ossimImageSource* inputSource);
   virtual ~ossimJpegYCbCrToRgbSource();
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& origin,
                                               ossim_uint32 resLevel=0);

   virtual void initialize();
       
protected:

   virtual void allocate();
   
   ossimRefPtr<ossimImageData> theBlankTile;

TYPE_DATA
};

#endif
