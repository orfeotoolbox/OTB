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
//  $Id: ossimEdgeFilter.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimEdgeFilter_HEADER
#define ossimEdgeFilter_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>

/**
 * class ossimEdgeFilter
 *
 * This has default implementation for the certain filter types.  For the
 * different filter type please see method setFilterType.
 *
 *
 */
class ossimEdgeFilter : public ossimImageSourceFilter
{
public:
   ossimEdgeFilter(ossimObject* owner=NULL);
   ossimEdgeFilter(ossimImageSource* inputSource);
   ossimEdgeFilter(ossimObject* owner,
                    ossimImageSource* inputSource);
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);
   virtual void initialize();
   virtual void getFilterTypeNames(std::vector<ossimString>& filterNames)const;
   virtual ossimString getFilterType()const;
   /**
    * The filter type can be one of the following strings.  It is
    * case insensitive for the compares.
    *
    *  Sobel
    *  Laplacian
    *  Prewitt
    *  
    */
   virtual void setFilterType(const ossimString& filterType);

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   virtual bool saveState(ossimKeywordlist& kwl,const char* prefix=0)const;
   virtual bool loadState(const ossimKeywordlist& kwl,const char* prefix=0);
   
protected:
   ossimRefPtr<ossimImageData> theTile;
   ossimString                 theFilterType;
   
   void adjustRequestRect(ossimIrect& requestRect)const;
   
   template <class T>
   void runFilter(T dummyVariable,
                  ossimRefPtr<ossimImageData> inputData);
   template <class T>
   void runSobelFilter(T dummyVariable,
                       ossimRefPtr<ossimImageData> inputData);
   template <class T>
   void runPrewittFilter(T dummyVariable,
                         ossimRefPtr<ossimImageData> inputData);
   template <class T>
   void runLaplacianFilter(T dummyVariable,
                           ossimRefPtr<ossimImageData> inputData);
   template <class T>
   void runRobertsFilter(T dummyVariable,
                           ossimRefPtr<ossimImageData> inputData);
   template <class T>
   void runLocalMax8Filter(T dummyVariable,
                          ossimRefPtr<ossimImageData> inputData);
   
TYPE_DATA
};


#endif
