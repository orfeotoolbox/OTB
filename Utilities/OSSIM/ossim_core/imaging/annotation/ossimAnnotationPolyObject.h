//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAnnotationPolyObject.h,v 1.16 2005/12/09 17:16:05 dburken Exp $
#ifndef ossimAnnotationPolyObject_HEADER
#define ossimAnnotationPolyObject_HEADER

#include <imaging/annotation/ossimAnnotationObject.h>
#include <base/data_types/ossimIpt.h>
#include <base/data_types/ossimPolygon.h>

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

   virtual ~ossimAnnotationPolyObject();

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
   ossimPolygon thePolygon;
   ossimDrect   theBoundingRect;
   bool         theFillEnabled;

TYPE_DATA
};

#endif
