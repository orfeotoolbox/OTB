//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Oscar Kramer
//
//*******************************************************************
//  $Id: ossimDespeckleFilter.h 2644 2011-05-26 15:20:11Z oscar.kramer $
#ifndef ossimDespeckleFilter_HEADER
#define ossimDespeckleFilter_HEADER

#include <ossim/imaging/ossimImageSourceFilter.h>

class ossimDespeckleFilter : public ossimImageSourceFilter
{
public:
   ossimDespeckleFilter();
   ossimDespeckleFilter(ossimImageSource* inputSource, ossim_uint32 filter_radius=1);
   
   virtual void setRadius(ossim_uint32 radius) { theFilterRadius = (ossim_int32) radius; }
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=NULL)const;
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=NULL);

   ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect, ossim_uint32 resLevel=0);
   
   virtual void initialize();
   
   /* ------------------- PROPERTY INTERFACE -------------------- */
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   /* ------------------ PROPERTY INTERFACE END ------------------- */

protected:
   virtual ~ossimDespeckleFilter();
   template<class T> void despeckle(T dummyVariable, ossimRefPtr<ossimImageData> inputTile);

   void allocate();
   
   ossimRefPtr<ossimImageData> theTile;
   ossim_int32                theFilterRadius;
   
TYPE_DATA
};

#endif /* #ifndef ossimDespeckleFilter_HEADER */
