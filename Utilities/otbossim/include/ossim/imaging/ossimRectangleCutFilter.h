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
// $Id: ossimRectangleCutFilter.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimRectangleCutFilter_HEADER
#define ossimRectangleCutFilter_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>

class OSSIM_DLL ossimRectangleCutFilter : public ossimImageSourceFilter
{
public:
   enum ossimRectangleCutType
   {
      OSSIM_RECTANGLE_NULL_INSIDE  = 0,
      OSSIM_RECTANGLE_NULL_OUTSIDE = 1
   };

   ossimRectangleCutFilter(ossimObject* owner,
                     ossimImageSource* inputSource=NULL);
   ossimRectangleCutFilter(ossimImageSource* inputSource=NULL);

   void setRectangle(const ossimIrect& rect);

   const ossimIrect&     getRectangle()const;
   ossimRectangleCutType getCutType()const;
   void                  setCutType(ossimRectangleCutType cutType);
   
   ossimIrect getBoundingRect(ossim_uint32 resLevel=0)const;
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
   
protected:
   ossimIrect            theRectangle;
   ossimRectangleCutType theCutType;
   vector<ossimDpt>      theDecimationList;
TYPE_DATA
};

#endif /* #ifndef ossimRectangleCutFilter_HEADER */
