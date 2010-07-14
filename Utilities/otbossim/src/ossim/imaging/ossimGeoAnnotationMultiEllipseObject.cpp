//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationMultiEllipseObject.cpp 17195 2010-04-23 17:32:18Z dburken $

#include <ossim/imaging/ossimGeoAnnotationMultiEllipseObject.h>
#include <ossim/imaging/ossimAnnotationMultiEllipseObject.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimImageProjectionModel.h>
#include <ossim/base/ossimException.h>

ossimGeoAnnotationMultiEllipseObject::ossimGeoAnnotationMultiEllipseObject()
   :ossimGeoAnnotationObject(),
    theProjectedObject(new ossimAnnotationMultiEllipseObject()),
    theWidthHeight(1,1),
    theFillFlag(false)
{
   theProjectedObject->setFillFlag(false);
}

ossimGeoAnnotationMultiEllipseObject::ossimGeoAnnotationMultiEllipseObject(const std::vector<ossimGpt>& pointList,
                                                                           const ossimDpt& widthHeight,
                                                                           bool enableFill,
                                                                           unsigned char r,
                                                                           unsigned char g,
                                                                           unsigned char b,
                                                                           long thickness)
   :ossimGeoAnnotationObject(r, g, b, thickness),
    theProjectedObject(new ossimAnnotationMultiEllipseObject(widthHeight,enableFill, r, g, b, thickness )),
    thePointList(pointList),
    theWidthHeight(widthHeight),
    theFillFlag(enableFill)
{
   
}

ossimGeoAnnotationMultiEllipseObject::ossimGeoAnnotationMultiEllipseObject(const ossimDpt& widthHeight,
                                                                           bool enableFill,
                                                                           unsigned char r,
                                                                           unsigned char g,
                                                                           unsigned char b,
                                                                           long thickness)
   :ossimGeoAnnotationObject(r, g, b,thickness),
    theProjectedObject(new ossimAnnotationMultiEllipseObject(widthHeight,enableFill, r, g, b, thickness )),
    theWidthHeight(widthHeight),
    theFillFlag(enableFill)
{
}

ossimGeoAnnotationMultiEllipseObject::ossimGeoAnnotationMultiEllipseObject(const ossimGeoAnnotationMultiEllipseObject& rhs)
   :ossimGeoAnnotationObject(rhs),
    theProjectedObject((ossimAnnotationMultiEllipseObject*)rhs.theProjectedObject->dup()),
    thePointList(rhs.thePointList),
    theWidthHeight(rhs.theWidthHeight),
    theFillFlag(rhs.theFillFlag)
{
}
   
void ossimGeoAnnotationMultiEllipseObject::transform(
   ossimImageGeometry* projection)
{
   const std::vector<ossimGpt>::size_type BOUNDS = thePointList.size();
   theProjectedObject->resize((ossim_uint32)BOUNDS);
   for(std::vector<ossimGpt>::size_type i = 0; i < BOUNDS; ++i)
   {
      projection->worldToLocal(thePointList[(int)i], (*theProjectedObject)[(int)i]);
      
   }
   computeBoundingRect();
}

void ossimGeoAnnotationMultiEllipseObject::setFillFlag(bool fillFlag)
{
   theProjectedObject->setFillFlag(fillFlag);
   theFillFlag = fillFlag;
}

void ossimGeoAnnotationMultiEllipseObject::setColor(unsigned char r,
                                                    unsigned char g,
                                                    unsigned char b)
{
   ossimAnnotationObject::setColor(r,g,b);
   if(theProjectedObject)
   {
      theProjectedObject->setColor(r,g,b);
   }
}

void ossimGeoAnnotationMultiEllipseObject::setThickness(ossim_uint8 thickness)
{
   ossimAnnotationObject::setThickness(thickness);
   if(theProjectedObject)
   {
      theProjectedObject->setThickness(thickness);
   }
}


void ossimGeoAnnotationMultiEllipseObject::applyScale(double /* x */,
                                                      double /* y */)
{
   ossimNotify(ossimNotifyLevel_NOTICE)
      << "ossimGeoAnnotationMultiEllipseObject::applyScale NOT IMPLEMENTED"
      << endl;
}

void ossimGeoAnnotationMultiEllipseObject::draw(ossimRgbImage& anImage)const
{
   theProjectedObject->draw(anImage); 
}

bool ossimGeoAnnotationMultiEllipseObject::intersects(const ossimDrect& rect)const
{
   return theProjectedObject->intersects(rect);
}

void ossimGeoAnnotationMultiEllipseObject::setWidthHeight(const ossimDpt& widthHeight)
{
   theWidthHeight = widthHeight;
   theProjectedObject->setWidthHeight(widthHeight);
}

std::ostream& ossimGeoAnnotationMultiEllipseObject::print(std::ostream& out)const
{
   theProjectedObject->print(out);
   return out;
}

void ossimGeoAnnotationMultiEllipseObject::getBoundingRect(ossimDrect& rect)const
{
   theProjectedObject->getBoundingRect(rect);
}

void ossimGeoAnnotationMultiEllipseObject::computeBoundingRect()
{
   theProjectedObject->computeBoundingRect();
}

ossimObject* ossimGeoAnnotationMultiEllipseObject::dup()const
{
   return new ossimGeoAnnotationMultiEllipseObject(*this);
}

ossimAnnotationObject* ossimGeoAnnotationMultiEllipseObject::getNewClippedObject(
   const ossimDrect& /* rect */)const
{
   ossimNotify(ossimNotifyLevel_NOTICE)
      << "ossimGeoAnnotationMultiEllipseObject::getNewClippedObject "
      << "NOT IMPLEMENTED" << endl;
   return (ossimAnnotationObject*)dup();
}

void ossimGeoAnnotationMultiEllipseObject::addPoint(const ossimGpt& point)
{
   thePointList.push_back(point);
}

void ossimGeoAnnotationMultiEllipseObject::setPoint(int i,
                                                    const ossimGpt& point)
{
   thePointList[i] = point;
}

void ossimGeoAnnotationMultiEllipseObject::resize(ossim_uint32 newSize)
{
   if(newSize)
   {
      thePointList.resize(newSize);
   }
}
