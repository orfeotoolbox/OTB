//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAnnotationPolyObject.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimAnnotationPolyObject_HEADER
#define ossimAnnotationPolyObject_HEADER

#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimPolygon.h>

class OSSIMDLLEXPORT ossimAnnotationPolyObject : public ossimAnnotationObject
{
public:
   ossimAnnotationPolyObject(bool enableFill = false,
                             ossim_uint8 r=255,
                             ossim_uint8 g=255,
                             ossim_uint8 b=255,
                             ossim_uint8 thickness=1);
   
   ossimAnnotationPolyObject(const vector<ossimDpt>& imagePts,
                             bool enableFill = false,
                             ossim_uint8 r=255,
                             ossim_uint8 g=255,
                             ossim_uint8 b=255,
                             ossim_uint8 thickness=1);
   
   ossimAnnotationPolyObject(const ossimAnnotationPolyObject& rhs);

   ossimObject* dup()const;


   virtual void applyScale(double x, double y);

   virtual void draw(ossimRgbImage& anImage)const;

   virtual bool intersects(const ossimDrect& rect)const;
   
   virtual ossimAnnotationObject* getNewClippedObject(
      const ossimDrect& rect)const;

   virtual std::ostream& print(std::ostream& out)const;

   virtual void getBoundingRect(ossimDrect& rect)const;

   virtual void addPoint(const ossimDpt& pt);

   virtual void setPolygon(const vector<ossimDpt>& imagePoints);

   virtual void setPolygon(const ossimPolygon& polygon);

   virtual void setPolygon(const ossimIrect& rect);

   virtual void setPolygon(const ossimDrect& rect);

   virtual void computeBoundingRect();

   virtual bool isPointWithin(const ossimDpt& imagePoint)const;

   const ossimPolygon& getPolygon()const;

   ossimPolygon& getPolygon();

   virtual void setFillFlag(bool flag);

protected:
   virtual ~ossimAnnotationPolyObject();
  ossimPolygon thePolygon;
   ossimDrect   theBoundingRect;
   bool         theFillEnabled;

TYPE_DATA
};

#endif
