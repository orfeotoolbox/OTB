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
// $Id: ossimTrimFilter.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimTrimFilter_HEADER
#define ossimTrimFilter_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimPolyCutter.h>

class ossimTrimFilter : public ossimImageSourceFilter
{
public:
   ossimTrimFilter();

   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0)const;
   virtual void getValidImageVertices(vector<ossimIpt>& validVertices,
                                      ossimVertexOrdering ordering=OSSIM_CLOCKWISE_ORDER,
                                      ossim_uint32 resLevel=0)const;

   ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                       ossim_uint32 resLevel=0);

   virtual void initialize();
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=NULL)const;
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=NULL);
   

   virtual ossimRefPtr<ossimProperty> getProperty(
      const ossimString& name)const;

   /**
    * Set property.
    *
    * @param property Property to set if property->getName() matches a
    * property name of this object.
    */
   virtual void setProperty(ossimRefPtr<ossimProperty> property);

   /**
    * Adds this objects properties to the list.
    *
    * @param propertyNames Array to add to.
    *
    * @note This method does not clear propertyNames prior to adding it's
    * names.
    */
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

protected:
   virtual ~ossimTrimFilter();
   double theLeftPercent;
   double theRightPercent;
   double theTopPercent;
   double theBottomPercent;
   std::vector<ossimIpt>  theValidVertices;
   ossimRefPtr<ossimPolyCutter> theCutter;
   
TYPE_DATA
};

#endif /* #ifndef ossimTrimFilter_HEADER */
