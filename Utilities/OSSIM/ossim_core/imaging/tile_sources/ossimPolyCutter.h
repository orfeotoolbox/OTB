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
// $Id: ossimPolyCutter.h,v 1.15 2005/04/05 18:48:29 gpotts Exp $
#ifndef ossimPolyCutter_HEADER
#define ossimPolyCutter_HEADER
#include <imaging/tile_sources/ossimImageSourceFilter.h>
#include <base/data_types/ossimPolygon.h>
#include <vector>

//class ossimPolyArea2d;
class ossimImageData;

class OSSIMDLLEXPORT ossimPolyCutter : public ossimImageSourceFilter
{
public:
   enum ossimPolyCutterCutType
   {
      OSSIM_POLY_NULL_INSIDE  = 0,
      OSSIM_POLY_NULL_OUTSIDE = 1
   };
   
   ossimPolyCutter();
    ossimPolyCutter(ossimImageSource* inputSource,
                    const ossimPolygon& polygon);
   virtual ~ossimPolyCutter();

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);
   
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0)const;

   virtual void initialize();
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual void setPolygon(const vector<ossimDpt>& polygon,
                           ossim_uint32 i = 0);
   
   virtual void setPolygon(const vector<ossimIpt>& polygon,
                           ossim_uint32 i = 0);
   
   virtual void addPolygon(const vector<ossimIpt>& polygon);
   virtual void addPolygon(const vector<ossimDpt>& polygon);
   virtual void addPolygon(const ossimPolygon& polygon);
   
   virtual void setNumberOfPolygons(ossim_uint32 count);
   
   virtual std::vector<ossimPolygon>& getPolygonList();

   virtual const std::vector<ossimPolygon>& getPolygonList()const;
   
   void setCutType(ossimPolyCutterCutType cutType);
   
   ossimPolyCutterCutType getCutType()const;
protected:

   /**
    * Called on first getTile, will initialize all data needed.
    */
   void allocate();

   ossimRefPtr<ossimImageData> theTile;

   /*!
    * Will hold a pre-computed bounding rect of the
    * polygon data.
    */
   ossimIrect theBoundingRect;

   std::vector<ossimPolygon> thePolygonList;

   /*!
    * theDefault fill will be outside.
    */
   ossimPolyCutterCutType theCutType;

   void computeBoundingRect();

TYPE_DATA  
};
#endif /* #ifndef ossimPolyCutter_HEADER */
