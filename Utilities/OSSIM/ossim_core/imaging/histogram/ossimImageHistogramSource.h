//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageHistogramSource.h,v 1.16 2005/11/27 17:07:12 dburken Exp $
#ifndef ossimImageHistogramSource_HEADER
#define ossimImageHistogramSource_HEADER
#include <base/common/ossimHistogramSource.h>
#include <imaging/ossimImageSourceInterface.h>
#include <base/common/ossimProcessInterface.h>
#include <base/common/events/ossimConnectableObjectListener.h>
#include <base/common/events/ossimObjectEvents.h>
#include <base/data_types/ossimIrect.h>

/*!
 * This source expects as input an ossimImageSourceInterface.
 * it will slice up the requested region into tiles and compute
 * the histogram of the passed in rectangle.
 */
class OSSIMDLLEXPORT ossimImageHistogramSource : public ossimHistogramSource,
                                                 public ossimConnectableObjectListener,
                                                 public ossimProcessInterface
{
public:
   ossimImageHistogramSource(ossimObject* owner = NULL);
   ossimImageHistogramSource(ossimObject* owner,
                             ossim_uint32 numberOfInputs,
                             ossim_uint32 numberOfOutputs,
                             ossim_uint32 inputListFixedFlag,
                             ossim_uint32 outputListFixedFlag);
   virtual ~ossimImageHistogramSource();

   virtual ossimObject* getObject();
   virtual const ossimObject* getObject()const;

   void setAreaOfInterest(const ossimIrect& rect);

   ossimIrect getAreaOfInterest()const;

   void getAreaOfInterest(ossimIrect& rect)const;

   virtual ossim_uint32 getMaxNumberOfRLevels()const;

   virtual void setMaxNumberOfRLevels(ossim_uint32 number);

   /*!
    * The first argument is the region of interest that you wish to
    * use for the histogram.  If the region is different than
    * what it has already computed before then the histogram is considered
    * dirty and will be re-computed.  The returned object is a
    * multi-reslevel histogram
    */
   virtual ossimMultiResLevelHistogram* getHistogram(const ossimIrect& rect);

   virtual ossimMultiResLevelHistogram* getHistogram();
   virtual bool execute();
   
   virtual bool canConnectMyInputTo(ossim_int32 myInputIndex,
                                    const ossimConnectableObject* object)const;

   void setNumberOfBinsOverride(ossim_int32 numberOfBinsOverride);

   void setMinValueOverride(ossim_float32 minValueOverride);

   void setMaxValueOverride(ossim_float32 maxValueOverride);

   virtual void propertyEvent(ossimPropertyEvent& event);
   
   virtual void connectInputEvent(ossimConnectionEvent& event);
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
protected:
   virtual void computeHistogram();
   
   /*!
    * Initialized to ossimNAN'S
    */
   ossimIrect theAreaOfInterest;
   bool       theHistogramRecomputeFlag;

   /*!
    * Will default to 1.  This means it will use up to max
    * number of res levels for the histogram.
    */
   ossim_uint32     theMaxNumberOfResLevels;

   ossim_float64    theMinValueOverride;
   ossim_float64    theMaxValueOverride;
   ossim_int32      theNumberOfBinsOverride;

TYPE_DATA
};

#endif
