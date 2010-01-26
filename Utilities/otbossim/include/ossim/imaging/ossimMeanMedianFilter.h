//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimMeanMedianFilter.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimMeanMedianFilter_HEADER
#define ossimMeanMedianFilter_HEADER

#include <ossim/imaging/ossimImageSourceFilter.h>

/*!
 * class ossimMeanMedianFilter
 *
 * Allows you to change between a median or mean filter.  You can
 * also specify a window size which the median or mean is computed and
 * the center pixel is replaced.
 *
 */
class OSSIM_DLL ossimMeanMedianFilter : public ossimImageSourceFilter
{
public:

   enum ossimMeanMedianFilterType
   {
      /** Applies filter to any non-null center pixel. */
      OSSIM_MEDIAN                  = 0,

      /** Applies filter to all pixels including null center pixels. */
      OSSIM_MEDIAN_FILL_NULLS       = 1,

      /** Applies filter to only null center pixels. */
      OSSIM_MEDIAN_NULL_CENTER_ONLY = 2,

      /** Applies filter to any non-null center pixel. */
      OSSIM_MEAN                    = 3,

      /* Applies filter to all pixels including null center pixels. */
      OSSIM_MEAN_FILL_NULLS         = 4,

      /** Applies filter to only null center pixels. */
      OSSIM_MEAN_NULL_CENTER_ONLY   = 5
   };

   ossimMeanMedianFilter(ossimObject* owner=NULL);

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);
   virtual void initialize();

   void setWindowSize(ossim_uint32 windowSize);
   ossim_uint32 getWindowSize()const;

   /**
    * @param flag Set "theAutoGrowRectFlag".  This only affects filter types
    * that set nulls.  Will have a growing affect on the edges.
    */
   void setAutoGrowRectFlag(bool flag);
   bool getAutoGrowRectFlag() const;

   void setFilterType(ossimMeanMedianFilterType type);
   void setFilterType(const ossimString& type);
   ossimString getFilterTypeString() const;
   void getFilterTypeList(std::vector<ossimString>& list) const;
   
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   
   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /**
    * @return Returns the bounding rectangle which is the input bounding
    * rectangle with any expansion (from autogrow) added in.
    */
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0)const;

protected:
   virtual ~ossimMeanMedianFilter();
   
   ossimRefPtr<ossimImageData> theTile;
   ossimMeanMedianFilterType   theFilterType;
   ossim_uint32                theWindowSize;

   /** Used by applyMean and applyMedian for "fill null" modes. */
   bool theEnableFillNullFlag;

   /**
    * If true rectangle is expanded by half filter in each direction if the
    * theFilterType fills nulls.
    */
   bool theAutoGrowRectFlag;

   void applyFilter(ossimRefPtr<ossimImageData>& input);

   template <class T>
      void applyMean(T dummyVariable,
                     ossimRefPtr<ossimImageData>& inputData);
   template <class T>
      void applyMeanNullCenterOnly(T dummyVariable,
                                   ossimRefPtr<ossimImageData>& inputData);

   template <class T>
      void applyMedian(T dummyVariable,
                       ossimRefPtr<ossimImageData>& inputData);
   template <class T>
      void applyMedianNullCenterOnly(T dummyVariable,
                                     ossimRefPtr<ossimImageData>& inputData);
TYPE_DATA
};

#endif
