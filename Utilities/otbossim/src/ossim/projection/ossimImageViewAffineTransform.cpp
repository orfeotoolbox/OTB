//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimImageViewAffineTransform.cpp 18999 2011-03-02 15:54:26Z gpotts $

#include <ossim/projection/ossimImageViewAffineTransform.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimMatrix3x3.h>
#include <ossim/base/ossimString.h>

RTTI_DEF1(ossimImageViewAffineTransform, "ossimImageViewAffineTransform", ossimImageViewTransform)

ossimImageViewAffineTransform::ossimImageViewAffineTransform(double rotateDegrees,
                                                             double scaleXValue,
                                                             double scaleYValue,
                                                             double translateXValue,
                                                             double translateYValue,
                                                             double pivotXValue,
                                                             double pivotYValue)
   :m_transform(3,3),
    m_inverseTransform(3,3),
    m_rotation(rotateDegrees),
    m_scale(scaleXValue, scaleYValue),
    m_translate(translateXValue, translateYValue),
    m_pivot(pivotXValue, pivotYValue)
{
   m_transform << 1 << 0 << 0
                << 0 << 1 << 0
                << 0 << 0 << 1;
   
   m_inverseTransform << 1 << 0 << 0
                       << 0 << 1 << 0
                       << 0 << 0 << 1;

   
   rotate(rotateDegrees);
   scale(scaleXValue, scaleYValue);
   translate(translateXValue, translateYValue);
   pivot(m_pivot.x, m_pivot.y);
}

ossimImageViewAffineTransform::~ossimImageViewAffineTransform()
{
}

void ossimImageViewAffineTransform::imageToView(const ossimDpt& imagePoint,
                                                ossimDpt&       viewPoint)const
{
   
//    viewPoint.x = m_transform[0][0]*(imagePoint.x - translateRotationOriginPoint.x) + m_transform[0][1]*(imagePoint.y - translateRotationOriginPoint.y) + m_transform[0][2];
//    viewPoint.y = m_transform[1][0]*(imagePoint.x - translateRotationOriginPoint.x) + m_transform[1][1]*(imagePoint.y - translateRotationOriginPoint.y) + m_transform[1][2];
    viewPoint.x = m_transform[0][0]*imagePoint.x + m_transform[0][1]*imagePoint.y + m_transform[0][2];
    viewPoint.y = m_transform[1][0]*imagePoint.x + m_transform[1][1]*imagePoint.y + m_transform[1][2];
}

void ossimImageViewAffineTransform::viewToImage(const ossimDpt& viewPoint,
                                                ossimDpt&       imagePoint)const
{
   
//    imagePoint.x = m_inverseTransform[0][0]*viewPoint.x + m_inverseTransform[0][1]*viewPoint.y + m_inverseTransform[0][2] + translateRotationOriginPoint.x;
//    imagePoint.y = m_inverseTransform[1][0]*viewPoint.x + m_inverseTransform[1][1]*viewPoint.y + m_inverseTransform[1][2] + translateRotationOriginPoint.y;
   imagePoint.x = m_inverseTransform[0][0]*viewPoint.x + m_inverseTransform[0][1]*viewPoint.y + m_inverseTransform[0][2];
   imagePoint.y = m_inverseTransform[1][0]*viewPoint.x + m_inverseTransform[1][1]*viewPoint.y + m_inverseTransform[1][2];
   
}

void ossimImageViewAffineTransform::setMatrix(NEWMAT::Matrix& matrix)
{
   m_transform        = matrix;
   m_inverseTransform = m_transform.i();
}

const NEWMAT::Matrix& ossimImageViewAffineTransform::getMatrix()const
{
   return m_transform;
}



void ossimImageViewAffineTransform::scale(double x, double y)
{
//    NEWMAT::Matrix m(3, 3);

//    m << x << 0 << 0
//      << 0 << y << 0
//      << 0 << 0 << 1;

   
//   m_transform = (m*m_transform);

//   m_inverseTransform = m_transform.i();

   m_scale = ossimDpt(x,y);
   buildCompositeTransform();
}

void ossimImageViewAffineTransform::translate(double deltaX,
                                              double deltaY)
{
   m_translate = ossimDpt(deltaX, deltaY);
   buildCompositeTransform();
//    NEWMAT::Matrix m(3,3);

//    m << 1 << 0 << deltaX
//      << 0 << 1 << deltaY
//      << 0 << 0 << 1;
   
   
//    m_transform = m*m_transform;
//    m_inverseTransform = m_transform.i();
}

void ossimImageViewAffineTransform::translateX(double deltaX)
{
   translate(deltaX, m_translate.y);
}

void ossimImageViewAffineTransform::translateY(double deltaY)
{
   translate(m_translate.x, deltaY);
}

void ossimImageViewAffineTransform::scaleX(double x)
{
   scale(x, m_scale.y);
}

void ossimImageViewAffineTransform::scaleY(double y)
{
   scale(m_scale.x, y);
}

void ossimImageViewAffineTransform::pivot(double originX, double originY)
{
   m_pivot.x = originX;
   m_pivot.y = originY;
   buildCompositeTransform();
}

void ossimImageViewAffineTransform::pivotX(double originX)
{
   pivot(originX,m_pivot.y);
}

void ossimImageViewAffineTransform::pivotY(double originY)
{
   pivot(m_pivot.x, originY);
}

void ossimImageViewAffineTransform::rotate(double degrees)
{
   m_rotation = degrees;
   buildCompositeTransform();
//   m_transform = (m_transform * ossimMatrix3x3::createRotationZMatrix(degrees));
//    m_transform = ossimMatrix3x3::createRotationZMatrix(degrees) * m_transform;

//    m_inverseTransform = m_transform.i();
}

void ossimImageViewAffineTransform::buildCompositeTransform()
{
   NEWMAT::Matrix scaleM(3, 3);
   NEWMAT::Matrix rotzM = ossimMatrix3x3::createRotationZMatrix(m_rotation);
   NEWMAT::Matrix transM(3,3);
   NEWMAT::Matrix transOriginM(3,3);
   NEWMAT::Matrix transOriginNegatedM(3,3);
   
   transM << 1 << 0 << m_translate.x
          << 0 << 1 << m_translate.y
          << 0 << 0 << 1;
   
   transOriginM << 1 << 0 << m_pivot.x
                << 0 << 1 << m_pivot.y
                << 0 << 0 << 1;
   
   transOriginNegatedM << 1 << 0 << -m_pivot.x
                       << 0 << 1 << -m_pivot.y
                       << 0 << 0 << 1;
   
   scaleM << m_scale.x << 0 << 0
          << 0 << m_scale.y << 0
          << 0 << 0 << 1;

//   m_transform        = transOriginM*transM*scaleM*rotzM*transOriginNegatedM;
   // pivot should just be around the rotation
   m_transform        = transM*scaleM*transOriginM*rotzM*transOriginNegatedM;
   m_inverseTransform = m_transform.i();
   
}


bool ossimImageViewAffineTransform::loadState(const ossimKeywordlist& kwl,
                                              const char* prefix)
{
   ossimString scaleString(kwl.find(prefix,"scale"));
   ossimString pivotString(kwl.find(prefix,"pivot"));
   ossimString translateString(kwl.find(prefix,"translate"));
   ossimString rotateString(kwl.find(prefix,"rotate"));
   
   if(!scaleString.empty())
   {
      m_scale.toPoint(scaleString);
   }
   if(!pivotString.empty())
   {
      m_pivot.toPoint(pivotString);
   }
   if(!translateString.empty())
   {
      m_translate.toPoint(translateString);
   }
   if(!rotateString.empty())
   {
      m_rotation = rotateString.toDouble();
   }
   buildCompositeTransform();
   ossimImageViewTransform::loadState(kwl, prefix);
#if 0
   ossimString newPrefix = ossimString(prefix) + "transform.";
   
   const char* lookup = kwl.find(newPrefix.c_str(),
                                 "m11");
   if(lookup)
   {
      m_transform[0][0] = ossimString(lookup).toDouble();
   }

   lookup = kwl.find(newPrefix.c_str(),
                     "m12");
   if(lookup)
   {
      m_transform[0][1] = ossimString(lookup).toDouble();
   }

   lookup = kwl.find(newPrefix.c_str(),
                     "m13");
   if(lookup)
   {
      m_transform[0][2] = ossimString(lookup).toDouble();
   }

   lookup = kwl.find(newPrefix.c_str(),
                     "m21");
   if(lookup)
   {
      m_transform[1][0] = ossimString(lookup).toDouble();
   }

   lookup = kwl.find(newPrefix.c_str(),
                     "m22");
   if(lookup)
   {
      m_transform[1][1] = ossimString(lookup).toDouble();
   }

   lookup = kwl.find(newPrefix.c_str(),
                     "m23");
   if(lookup)
   {
      m_transform[1][2] = ossimString(lookup).toDouble();
   }

   lookup = kwl.find(newPrefix.c_str(),
                     "m31");
   if(lookup)
   {
      m_transform[2][0] = ossimString(lookup).toDouble();
   }

   lookup = kwl.find(newPrefix.c_str(),
                     "m32");
   if(lookup)
   {
      m_transform[2][1] = ossimString(lookup).toDouble();
   }

   lookup = kwl.find(newPrefix.c_str(),
                     "m33");
   if(lookup)
   {
      m_transform[2][2] = ossimString(lookup).toDouble();
   }

   lookup = kwl.find(newPrefix.c_str(),
                     "rotate");
   if(lookup)
   {
      double degrees = ossimString(lookup).toDouble();
      m_transform *=  ossimMatrix3x3::createRotationZMatrix(degrees);
   }
   m_inverseTransform = m_transform.i();
#endif
   return true;
}

bool ossimImageViewAffineTransform::saveState(ossimKeywordlist& kwl,
                                              const char* prefix)const
{
   kwl.add(prefix,
           "rotation",
           m_rotation,
           true);
   kwl.add(prefix,
           "pivot",
           m_pivot.toString(),
           true);
   kwl.add(prefix,
           "scale",
           m_scale.toString(),
           true);
   kwl.add(prefix,
           "translate",
           m_translate.toString(),
           true);
           
   return ossimImageViewTransform::saveState(kwl,prefix);
   /*
   kwl.add(prefix,
           "transform.m11",
           m_transform[0][0],
           true);
   kwl.add(prefix,
           "transform.m12",
           m_transform[0][1],
           true);
   kwl.add(prefix,
           "transform.m13",
           m_transform[0][2],
           true);
   kwl.add(prefix,
           "transform.m21",
           m_transform[1][0],
           true);
   kwl.add(prefix,
           "transform.m22",
           m_transform[1][1],
           true);
   kwl.add(prefix,
           "transform.m23",
           m_transform[1][2],
           true);
   kwl.add(prefix,
           "transform.m31",
           m_transform[2][0],
           true);
   kwl.add(prefix,
           "transform.m32",
           m_transform[2][1],
           true);
   kwl.add(prefix,
           "transform.m33",
           m_transform[2][2],
           true);
   */
}

bool ossimImageViewAffineTransform::isValid()const
{
   return true;
}

bool ossimImageViewAffineTransform::setView(ossimObject* /* obj */)
{
   return false;
}

ossimObject* ossimImageViewAffineTransform::getView()
{
   return NULL;
}

const ossimObject* ossimImageViewAffineTransform::getView()const
{
   return NULL;
}

ossimDpt ossimImageViewAffineTransform::getInputMetersPerPixel()const
{
   return ossimDpt(1,1);
}

ossimDpt ossimImageViewAffineTransform::getOutputMetersPerPixel()const
{
   ossimDpt result;
   
   result.makeNan();
   
   return result;
}
