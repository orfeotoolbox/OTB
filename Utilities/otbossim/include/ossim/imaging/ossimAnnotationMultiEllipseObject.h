//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAnnotationMultiEllipseObject.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimAnnotationMultiEllipseObject_HEADER
#define ossimAnnotationMultiEllipseObject_HEADER
#include <ossim/imaging/ossimAnnotationObject.h>
#include <vector>

class OSSIMDLLEXPORT ossimAnnotationMultiEllipseObject : public ossimAnnotationObject
{
public:
   ossimAnnotationMultiEllipseObject(const std::vector<ossimDpt>& pointList,
                                     const ossimDpt& widthHeight = ossimDpt(1,1),
                                     bool enableFill = false,
                                     unsigned char r = 255,
                                     unsigned char g = 255,
                                     unsigned char b = 255,
                                     long thickness = 1);

   ossimAnnotationMultiEllipseObject(const ossimDpt& widthHeight = ossimDpt(1,1),
                                     bool enableFill = false,
                                     unsigned char r = 255,
                                     unsigned char g = 255,
                                     unsigned char b = 255,
                                     long thickness = 1);

   ossimAnnotationMultiEllipseObject(const ossimAnnotationMultiEllipseObject& rhs);

   ossimObject* dup()const;


   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;
   virtual void applyScale(double x, double y);
   virtual void draw(ossimRgbImage& anImage)const;
   virtual bool intersects(const ossimDrect& rect)const;
   virtual std::ostream& print(std::ostream& out)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void computeBoundingRect();
  /*!
   */
   virtual bool isPointWithin(const ossimDpt& imagePoint)const;
   
   void setFillFlag(bool flag);

   void resize(ossim_uint32 size);

   virtual void setWidthHeight(const ossimDpt& widthHeight);

   ossimDpt& operator[](int i);
   
   const ossimDpt& operator[](int i)const;
   
protected:
   virtual ~ossimAnnotationMultiEllipseObject();
   std::vector<ossimDpt>  thePointList;
   ossimDpt               theWidthHeight;
   bool                   theFillFlag;
   ossimDrect             theBoundingRect;
};

#endif
