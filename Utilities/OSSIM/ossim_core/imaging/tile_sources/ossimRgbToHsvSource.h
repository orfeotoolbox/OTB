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
// $Id: ossimRgbToHsvSource.h,v 1.8 2005/02/11 15:07:34 dburken Exp $
#ifndef ossimRgbToHsvSource_HEADER
#define ossimJpegRgbToYCbCrSource_HEADER
#include <imaging/tile_sources/ossimImageSourceFilter.h>

class ossimRgbToHsvSource : public ossimImageSourceFilter
{
public:
   ossimRgbToHsvSource();
   ossimRgbToHsvSource(ossimImageSource* inputSource);
   virtual ~ossimRgbToHsvSource();
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);
   
   virtual ossimScalarType getOutputScalarType() const;
   virtual ossim_uint32    getNumberOfOutputBands()const;
   virtual double getNullPixelValue()const;
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   virtual void initialize();
       
protected:

   /**
    * Called on first getTile, will initialize all data needed.
    */
   void allocate();
   
   ossimRefPtr<ossimImageData> theBlankTile;
   ossimRefPtr<ossimImageData> theTile;

TYPE_DATA
};

#endif
