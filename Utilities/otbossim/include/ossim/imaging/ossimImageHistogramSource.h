//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageHistogramSource.h 17194 2010-04-23 15:05:19Z dburken $
#ifndef ossimImageHistogramSource_HEADER
#define ossimImageHistogramSource_HEADER
#include <ossim/base/ossimHistogramSource.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/base/ossimProcessInterface.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimObjectEvents.h>
#include <ossim/base/ossimIrect.h>

/*!
 * This source expects as input an ossimImageSource.
 * it will slice up the requested region into tiles and compute
 * the histogram of the passed in rectangle.
 */
class OSSIMDLLEXPORT ossimImageHistogramSource : public ossimHistogramSource,
                                                 public ossimConnectableObjectListener,
                                                 public ossimProcessInterface
{
public:

   ossimImageHistogramSource(ossimObject* owner = 0);
   
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
   virtual ossimRefPtr<ossimMultiResLevelHistogram> getHistogram(const ossimIrect& rect);

   virtual ossimRefPtr<ossimMultiResLevelHistogram> getHistogram();
   virtual bool execute();
   
   virtual bool canConnectMyInputTo(ossim_int32 myInputIndex,
                                    const ossimConnectableObject* object)const;

   void setNumberOfBinsOverride(ossim_int32 numberOfBinsOverride);

   void setMinValueOverride(ossim_float32 minValueOverride);

   void setMaxValueOverride(ossim_float32 maxValueOverride);

   ossimHistogramMode getComputationMode()const;
   void setComputationMode(ossimHistogramMode mode);
	
   virtual void propertyEvent(ossimPropertyEvent& event);
   
   virtual void connectInputEvent(ossimConnectionEvent& event);
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
	
protected:
   virtual ~ossimImageHistogramSource();

   void getBinInformation(ossim_uint32& numberOfBins,
                          ossim_float64& minValue,
                          ossim_float64& maxValue)const;
   virtual void computeNormalModeHistogram();
   virtual void computeFastModeHistogram();
   
   /*!
    * Initialized to ossimNAN'S
    */
   ossimIrect theAreaOfInterest;
   bool       theHistogramRecomputeFlag;
   
   /*!
    * Will default to 1.  This means it will use up to max
    * number of res levels for the histogram.
    */
   ossim_uint32       theMaxNumberOfResLevels; 
   ossim_float64      theMinValueOverride;
   ossim_float64      theMaxValueOverride;
   ossim_int32        theNumberOfBinsOverride;
   ossimHistogramMode theComputationMode;
   ossim_uint32       theNumberOfTilesToUseInFastMode;
TYPE_DATA
};

#endif
