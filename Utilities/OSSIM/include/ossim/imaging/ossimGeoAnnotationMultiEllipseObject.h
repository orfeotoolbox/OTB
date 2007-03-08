//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationMultiEllipseObject.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimGeoAnnotationMultiEllipseObject_HEADER
#define ossimGeoAnnotationMultiEllipseObject_HEADER
#include <ossim/imaging/ossimGeoAnnotationObject.h>
#include <vector>
class ossimAnnotationMultiEllipseObject;
class OSSIM_DLL ossimGeoAnnotationMultiEllipseObject : public ossimGeoAnnotationObject
{
public:
   ossimGeoAnnotationMultiEllipseObject();
   ossimGeoAnnotationMultiEllipseObject(const std::vector<ossimGpt>& centerPoints,
                                        const ossimDpt& widthHeight,
                                        bool enableFill = false,
                                        unsigned char r = 255,
                                        unsigned char g = 255,
                                        unsigned char b = 255,
                                        long thickness = 1);
   ossimGeoAnnotationMultiEllipseObject(const ossimDpt& widthHeight,
                                        bool enableFill = false,
                                        unsigned char r = 255,
                                        unsigned char g = 255,
                                        unsigned char b = 255,
                                        long thickness = 1);
   ossimGeoAnnotationMultiEllipseObject(const ossimGeoAnnotationMultiEllipseObject& rhs);
   
   virtual ossimObject* dup()const;

   virtual ossimAnnotationObject* getNewClippedObject(const ossimDrect& rect)const;

   virtual void transform(ossimProjection* projection);

   virtual void applyScale(double x, double y);

   virtual void draw(ossimRgbImage& anImage)const;
   virtual bool intersects(const ossimDrect& rect)const;
   
   virtual void addPoint(const ossimGpt& point);

   virtual void setColor(unsigned char r,
                         unsigned char g,
                         unsigned char b);
   virtual void setThickness(ossim_uint8 thickness);
   
   virtual void setPoint(int i, const ossimGpt& point);

   virtual void setWidthHeight(const ossimDpt& widthHeight);
   virtual void resize(ossim_uint32 newSize);

   virtual void setFillFlag(bool fillFlag);
   virtual std::ostream& print(std::ostream& out)const;
   virtual void getBoundingRect(ossimDrect& rect)const;
   virtual void computeBoundingRect();
   
protected:
   ossimAnnotationMultiEllipseObject* theProjectedObject;
   
   std::vector<ossimGpt> thePointList;
   ossimDpt              theWidthHeight;
   bool                  theFillFlag;
};

#endif /* #ifndef ossimGeoAnnotationMultiEllipseObject_HEADER */
