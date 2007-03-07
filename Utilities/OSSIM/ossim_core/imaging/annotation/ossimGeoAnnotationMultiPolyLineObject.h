//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationMultiPolyLineObject.h,v 1.5 2005/12/08 13:42:58 dburken Exp $
#ifndef ossimGeoAnnotationMultiPolyLineObject_HEADER
#define ossimGeoAnnotationMultiPolyLineObject_HEADER

#include <imaging/annotation/ossimGeoAnnotationObject.h>
#include <imaging/annotation/ossimAnnotationMultiPolyObject.h>
#include <imaging/annotation/ossimAnnotationMultiPolyLineObject.h>
#include <base/data_types/ossimIpt.h>
#include <base/data_types/ossimPolyLine.h>

class ossimDatum;

class OSSIMDLLEXPORT ossimGeoAnnotationMultiPolyLineObject : public ossimGeoAnnotationObject
{
public:
   ossimGeoAnnotationMultiPolyLineObject();
   ossimGeoAnnotationMultiPolyLineObject(const vector<ossimPolyLine>& multiPolyLine,
					 ossim_uint8 r=255,
					 ossim_uint8 g=255,
					 ossim_uint8 b=255,
					 ossim_uint8 thickness=1);
  
   ossimGeoAnnotationMultiPolyLineObject(const ossimGeoAnnotationMultiPolyLineObject& rhs);
   virtual ossimObject* dup()const;

   virtual ~ossimGeoAnnotationMultiPolyLineObject();
   virtual void transform(ossimProjection* projection);
   virtual void applyScale(double x, double y);
   virtual void draw(ossimRgbImage& anImage)const;
   virtual bool intersects(const ossimDrect& rect)const;
   
   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual std::ostream& print(std::ostream& out)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void addPoint(ossim_uint32 polygonIndex,
                         const ossimGpt& pt);
   
   virtual void setMultiPolyLine(const vector<ossimPolyLine>& multiPoly);

   virtual void computeBoundingRect();
   
   virtual bool isPointWithin(const ossimDpt& imagePoint)const;
   
   virtual void setColor(ossim_uint8 r,
                         ossim_uint8 g,
                         ossim_uint8 b);
   
   virtual void setThickness(long thickness);
   
   void setDatum(const ossimDatum* datum);

   const ossimDatum* getDatum()const;

   const std::vector<ossimPolyLine>& getMultiPolyLine()const;
   
   std::vector<ossimPolyLine>& getMultiPolyLine();

   /**
    * Saves the current state of this object.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
protected:
  vector<ossimPolyLine>               theMultiPolyLine;
  ossimDrect                          theBoundingRect;
  const ossimDatum*                   theDatum;
  ossimAnnotationMultiPolyLineObject* theProjectedPolyLineObject;
  
  void allocateProjectedPolyLine();

TYPE_DATA
};

#endif
