//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimImageViewAffineTransform.cpp 19802 2011-07-11 11:08:07Z gpotts $

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
   
    viewPoint.x = m_transform[0][0]*imagePoint.x + m_transform[0][1]*imagePoint.y + m_transform[0][2];
    viewPoint.y = m_transform[1][0]*imagePoint.x + m_transform[1][1]*imagePoint.y + m_transform[1][2];
}

void ossimImageViewAffineTransform::viewToImage(const ossimDpt& viewPoint,
                                                ossimDpt&       imagePoint)const
{
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
}

bool ossimImageViewAffineTransform::isValid()const
{
   return true;
}

bool ossimImageViewAffineTransform::setView(ossimObject* obj)
{
   ossimImageViewAffineTransform* view = dynamic_cast<ossimImageViewAffineTransform*> (obj);
   if(view)
   {  
      m_transform = view->m_transform;
      m_inverseTransform = view->m_inverseTransform;
      m_rotation = view->m_rotation;
      m_scale = view->m_scale;
      m_translate = view->m_translate;
      m_pivot = view->m_pivot;
   }
   return (view!=0);
}

ossimObject* ossimImageViewAffineTransform::getView()
{
   return this;
}

const ossimObject* ossimImageViewAffineTransform::getView()const
{
   return this;
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

bool ossimImageViewAffineTransform::isEqualTo(const ossimObject& obj, ossimCompareType compareType)const
{
   bool result = ossimImageViewTransform::isEqualTo(obj,compareType);
   
   if(result)
   {
      result = false;
      const ossimImageViewAffineTransform* rhs = dynamic_cast<const ossimImageViewAffineTransform*> (&obj);
      if(rhs)
      {
         result = (ossim::almostEqual(m_rotation, rhs->m_rotation)&&
                   (m_scale.isEqualTo(rhs->m_scale))&&
                   (m_translate.isEqualTo(rhs->m_translate))&&
                   (m_pivot.isEqualTo(rhs->m_pivot))
                   );  
      }
   }
   
   return result;
}
