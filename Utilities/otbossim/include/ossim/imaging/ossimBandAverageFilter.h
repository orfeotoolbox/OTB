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
// $Id: ossimBandAverageFilter.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimBandAverageFilter_HEADER
#define ossimBandAverageFilter_HEADER
#include <vector>
#include <ossim/imaging/ossimImageSourceFilter.h>

/**
 * This filter outputs a single band that is the weighted average of all the
 * input bands retrieved from the getTile.  This filter will work with any
 * input scalar type.  Note the OSSIMDLLEXPORT is used for the different
 * compilers that require importing and exporting symbols for shared library
 * support.
 */
class OSSIMDLLEXPORT ossimBandAverageFilter : public ossimImageSourceFilter
{
public:
   ossimBandAverageFilter();
   ossimBandAverageFilter(ossimImageSource* input,
                          const std::vector<double>& weights);

   /**
    * The data object argument is deprecated and should not be used.
    * eventually we will remove it from the base classes.  This method will
    * return 1 if our source is enabled else it calls the base class
    * getNumberOfOutputBands.
    */
   virtual ossim_uint32 getNumberOfOutputBands()const;
   
   /**
    * Since this filter is going to operate in native pixel type we will use
    * a tmeplate method to do this.  It will check the scalar tpe of the input
    * and then cat it to set the template variabel to be used within the
    * template method averageInput.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);

   /**
    * Since we are merging all bands we will use the null pix of the
    * first input band as our null pix.  If we are disabled we just
    * return the input s null
    */
   virtual double getNullPixelValue(ossim_uint32 band=0)const;

   /**
    * We will use the min of all bands as our min.  If we aredisabled
    * we just return the inputs min
    */
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   
   /**
    * We will use the max of all bands as our max.  If we aredisabled
    * we just return the inputs max
    */
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   /**
    * simple access method to get a weight
    */
   double getWeight(ossim_uint32 band)const;

   /**
    * Sinple access method to set the weight of a band,
    * We make sure that it is a positive weight
    */
   void setWeight(ossim_uint32 band, double weight);

   /**
    * allocates its tile buffer
    */
   virtual void initialize();

   /**
    * loads its weights to a keywordlist
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   /**
    * Saves its weights to a keywordlist
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

protected:
   virtual ~ossimBandAverageFilter();
   
  ossimRefPtr<ossimImageData> theTile;
   std::vector<double>         theWeights;

   void checkWeights();
   
   template<class T> void averageInput(
      T dummyVariable, // used only for template type, value ignored
      ossimRefPtr<ossimImageData> inputDataObject);

// for Runtime Type Info (RTTI)
TYPE_DATA
};

#endif
