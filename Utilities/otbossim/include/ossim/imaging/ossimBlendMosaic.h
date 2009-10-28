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
// $Id: ossimBlendMosaic.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimBlendMosaic_HEADER
#define ossimBlendMosaic_HEADER
#include <ossim/imaging/ossimImageMosaic.h>

class OSSIMDLLEXPORT ossimBlendMosaic : public ossimImageMosaic
{
public:
   ossimBlendMosaic();
   ossimBlendMosaic(ossimConnectableObject::ConnectableObjectList& inputSources);
   virtual void initialize();

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& origin,
                                               ossim_uint32 resLevel=0);
   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=NULL)const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=NULL);

   void setWeight(ossim_uint32 index, double weight);
   void setWeights(const std::vector<double>& weights);
   
   double getWeight(ossim_uint32 index) const;
   const std::vector<double>& getWeights()const;


   void setNumberOfWeights(ossim_uint32 numberOfWeights);
   void setAllWeightsTo(double value);
   
   /**
    * Will find the min and max values and then
    * normalize all weights to be from 0 to 1
    */
   void normalizeWeights();
   void findMinMax(double& minValue, double& maxValue)const;
   
protected:
   virtual ~ossimBlendMosaic();
   vector<double>              theWeights;
   ossimRefPtr<ossimImageData> theNormResult;
   /**
    * If our inputs have output of different scalar
    * types then we must normalize so we can blend
    * different scalar data together.
    */
   

   template <class T>
      ossimRefPtr<ossimImageData> combine(T,
                                          const ossimIrect& tileRect,
                                          ossim_uint32);
   template <class T>
      ossimRefPtr<ossimImageData> combineNorm(T,
                                              const ossimIrect& tileRect,
                                              ossim_uint32);
TYPE_DATA
};

#endif /* #ifndef ossimBlendMosaic_HEADER */
