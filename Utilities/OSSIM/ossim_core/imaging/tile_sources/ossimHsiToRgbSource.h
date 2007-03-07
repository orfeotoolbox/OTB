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
// $Id: ossimHsiToRgbSource.h,v 1.9 2005/02/11 15:07:33 dburken Exp $
#ifndef ossimHsiToRgbSource_HEADER
#define ossimHsiToRgbSource_HEADER

#include <imaging/tile_sources/ossimImageSourceFilter.h>

class ossimHsiToRgbSource : public ossimImageSourceFilter
{
public:
   ossimHsiToRgbSource();
   ossimHsiToRgbSource(ossimImageSource* inputSource);
   virtual ~ossimHsiToRgbSource();
   virtual ossimRefPtr<ossimImageData> getTile(const  ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);

   virtual ossimScalarType getOutputScalarType() const;

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

   void initializeBuffers(ossimRefPtr<ossimImageData>& data);
   
TYPE_DATA
};

#endif /* #ifndef ossimHsiToRgbSource_HEADER */

