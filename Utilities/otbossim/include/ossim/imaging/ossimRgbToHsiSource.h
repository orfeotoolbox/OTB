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
// $Id: ossimRgbToHsiSource.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimRgbToHsiSource_HEADER
#define ossimRgbToHsiSource_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>

class ossimRgbToHsiSource : public ossimImageSourceFilter
{
public:
   ossimRgbToHsiSource();
   ossimRgbToHsiSource(ossimImageSource* inputSource);

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);
   
   virtual ossimScalarType getOutputScalarType() const;
   virtual ossim_uint32    getNumberOfOutputBands()const;
   virtual double          getNullPixelValue()const;
   virtual double          getMinPixelValue(ossim_uint32 band=0)const;
   virtual double          getMaxPixelValue(ossim_uint32 band=0)const;

   virtual void initialize();

protected:
   virtual ~ossimRgbToHsiSource();

   /**
    * Called on first getTile, will initialize all data needed.
    */
   void allocate();
   
   ossimRefPtr<ossimImageData> theBlankTile;
   ossimRefPtr<ossimImageData> theTile;

TYPE_DATA
};

#endif /* #ifndef ossimRgbToHsiSource_HEADER */
