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
// $Id: ossimJpegYCbCrToRgbSource.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimJpegYCbCrToRgbSource_HEADER
#define ossimJpegYCbCrToRgbSource_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>

class ossimJpegYCbCrToRgbSource : public ossimImageSourceFilter
{
public:
   ossimJpegYCbCrToRgbSource();
   ossimJpegYCbCrToRgbSource(ossimImageSource* inputSource);
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& origin,
                                               ossim_uint32 resLevel=0);

   virtual void initialize();
       
protected:
   virtual ~ossimJpegYCbCrToRgbSource();

   virtual void allocate();
   
   ossimRefPtr<ossimImageData> theBlankTile;

TYPE_DATA
};

#endif
