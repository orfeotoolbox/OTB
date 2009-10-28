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
// $Id: ossimBandMergeSource.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimBandMergeSource_HEADER
#define ossimBandMergeSource_HEADER
#include <ossim/imaging/ossimImageCombiner.h>

class ossimImageSourceConnection;
class ossimImageSource;

class OSSIM_DLL ossimBandMergeSource : public ossimImageCombiner
{
public:
   ossimBandMergeSource();
   ossimBandMergeSource(ossimConnectableObject::ConnectableObjectList& inputSources);
   
   /**
    * This will merge all the bands from the input source list
    * into one contiguous ossimImageData object.  This is very
    * useful when you want to split off an intesity channel in
    * HSI and then run some algorithms on it and then merge it back.
    * You can also try to do fusions by converting to an YCbCr and
    * replace the Y or intensity channel with maybe a high-pass
    * convolution on the pan.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);

   virtual void initialize();
   
   /**
    * Returns the number of bands in a tile returned from this TileSource.
    * Note: we are supporting sources that can have multiple data objects.
    * If you want to know the scalar type of an object you can pass in the 
    */
   virtual ossim_uint32 getNumberOfOutputBands() const;

   virtual double getNullPixelValue(ossim_uint32 band=0)const;
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;
   
protected:
   virtual ~ossimBandMergeSource();
   /** Performs allocation of "theTile". */
   void allocate();
   
   ossim_uint32                theNumberOfOutputBands;
   ossimRefPtr<ossimImageData> theTile;
   
   ossim_uint32 computeNumberOfInputBands()const;

TYPE_DATA
};
#endif /* #ifndef ossimBandMergeSource_HEADER */
