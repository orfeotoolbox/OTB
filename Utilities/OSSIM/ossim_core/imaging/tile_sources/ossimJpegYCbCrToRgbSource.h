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
// $Id: ossimJpegYCbCrToRgbSource.h,v 1.5 2005/02/11 15:07:33 dburken Exp $
#ifndef ossimJpegYCbCrToRgbSource_HEADER
#define ossimJpegYCbCrToRgbSource_HEADER
#include <imaging/tile_sources/ossimImageSourceFilter.h>

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
