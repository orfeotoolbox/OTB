//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimImageViewAffineTransform.cpp,v 1.9 2006/01/05 12:03:52 gpotts Exp $
#include "ossimImageViewAffineTransform.h"
#include "base/data_types/ossimKeywordlist.h"
#include "base/data_types/ossimMatrix3x3.h"

RTTI_DEF1(ossimImageViewAffineTransform, "ossimImageViewAffineTransform", ossimImageViewTransform)

ossimImageViewAffineTransform::ossimImageViewAffineTransform(double rotateDegrees,
                                                             double scaleXValue,
                                                             double scaleYValue,
                                                             double translateXValue,
                                                             double translateYValue,
                                                             double translateOriginXValue,
                                                             double translateOriginYValue)
   :theTransform(3,3),
    theInverseTransform(3,3),
    theRotation(rotateDegrees),
    theScale(scaleXValue, scaleYValue),
    theTranslate(translateXValue, translateYValue),
    theTranslateOrigin(translateOriginXValue, translateOriginYValue)
{
   theTransform << 1 << 0 << 0
                << 0 << 1 << 0
                << 0 << 0 << 1;
   
   theInverseTransform << 1 << 0 << 0
                       << 0 << 1 << 0
                       << 0 << 0 << 1;

   
   rotate(rotateDegrees);
   scale(scaleXValue, scaleYValue);
   translate(translateXValue, translateYValue);
   translateOrigin(translateOriginXValue, translateOriginYValue);
}

ossimImageViewAffineTransform::~ossimImageViewAffineTransform()
{
}

void ossimImageViewAffineTransform::imageToView(const ossimDpt& imagePoint,
                                                ossimDpt&       viewPoint)const
{
   
//    viewPoint.x = theTransform[0][0]*(imagePoint.x - translateRotationOriginPoint.x) + theTransform[0][1]*(imagePoint.y - translateRotationOriginPoint.y) + theTransform[0][2];
//    viewPoint.y = theTransform[1][0]*(imagePoint.x - translateRotationOriginPoint.x) + theTransform[1][1]*(imagePoint.y - translateRotationOriginPoint.y) + theTransform[1][2];
    viewPoint.x = theTransform[0][0]*imagePoint.x + theTransform[0][1]*imagePoint.y + theTransform[0][2];
    viewPoint.y = theTransform[1][0]*imagePoint.x + theTransform[1][1]*imagePoint.y + theTransform[1][2];
}

void ossimImageViewAffineTransform::viewToImage(const ossimDpt& viewPoint,
                                                ossimDpt&       imagePoint)const
{
   
//    imagePoint.x = theInverseTransform[0][0]*viewPoint.x + theInverseTransform[0][1]*viewPoint.y + theInverseTransform[0][2] + translateRotationOriginPoint.x;
//    imagePoint.y = theInverseTransform[1][0]*viewPoint.x + theInverseTransform[1][1]*viewPoint.y + theInverseTransform[1][2] + translateRotationOriginPoint.y;
   imagePoint.x = theInverseTransform[0][0]*viewPoint.x + theInverseTransform[0][1]*viewPoint.y + theInverseTransform[0][2];
   imagePoint.y = theInverseTransform[1][0]*viewPoint.x + theInverseTransform[1][1]*viewPoint.y + theInverseTransform[1][2];
   
}

void ossimImageViewAffineTransform::setMatrix(NEWMAT::Matrix& matrix)
{
   theTransform        = matrix;
   theInverseTransform = theTransform.i();
}

const NEWMAT::Matrix& ossimImageViewAffineTransform::getMatrix()const
{
   return theTransform;
}



void ossimImageViewAffineTransform::scale(double x, double y)
{
//    NEWMAT::Matrix m(3, 3);

//    m << x << 0 << 0
//      << 0 << y << 0
//      << 0 << 0 << 1;

   
//   theTransform = (m*theTransform);

//   theInverseTransform = theTransform.i();

   theScale = ossimDpt(x,y);
   buildCompositeTransform();
}

void ossimImageViewAffineTransform::translate(double deltaX,
                                              double deltaY)
{
   theTranslate = ossimDpt(deltaX, deltaY);
   buildCompositeTransform();
//    NEWMAT::Matrix m(3,3);

//    m << 1 << 0 << deltaX
//      << 0 << 1 << deltaY
//      << 0 << 0 << 1;
   
   
//    theTransform = m*theTransform;
//    theInverseTransform = theTransform.i();
}

void ossimImageViewAffineTransform::translateX(double deltaX)
{
   translate(deltaX, theTranslate.y);
}

void ossimImageViewAffineTransform::translateY(double deltaY)
{
   translate(theTranslate.x, deltaY);
}

void ossimImageViewAffineTransform::scaleX(double x)
{
   scale(x, theScale.y);
}

void ossimImageViewAffineTransform::scaleY(double y)
{
   scale(theScale.x, y);
}

void ossimImageViewAffineTransform::translateOrigin(double originX, double originY)
{
   theTranslateOrigin.x = originX;
   theTranslateOrigin.y = originY;
   buildCompositeTransform();
}

void ossimImageViewAffineTransform::translateOriginX(double originX)
{
   translateOrigin(originX,theTranslateOrigin.y);
}

void ossimImageViewAffineTransform::translateOriginY(double originY)
{
   translateOrigin(theTranslateOrigin.x, originY);
}

void ossimImageViewAffineTransform::rotate(double degrees)
{
   theRotation = degrees;
   buildCompositeTransform();
//   theTransform = (theTransform * ossimMatrix3x3::createRotationZMatrix(degrees));
//    theTransform = ossimMatrix3x3::createRotationZMatrix(degrees) * theTransform;

//    theInverseTransform = theTransform.i();
}

void ossimImageViewAffineTransform::buildCompositeTransform()
{
   NEWMAT::Matrix scaleM(3, 3);
   NEWMAT::Matrix rotzM = ossimMatrix3x3::createRotationZMatrix(theRotation);
   NEWMAT::Matrix transM(3,3);
   NEWMAT::Matrix transOriginM(3,3);
   NEWMAT::Matrix transOriginNegatedM(3,3);
   
   transM << 1 << 0 << theTranslate.x
          << 0 << 1 << theTranslate.y
          << 0 << 0 << 1;
   
   transOriginM << 1 << 0 << theTranslateOrigin.x
                << 0 << 1 << theTranslateOrigin.y
                << 0 << 0 << 1;
   
   transOriginNegatedM << 1 << 0 << -theTranslateOrigin.x
                       << 0 << 1 << -theTranslateOrigin.y
                       << 0 << 0 << 1;
   
   scaleM << theScale.x << 0 << 0
          << 0 << theScale.y << 0
          << 0 << 0 << 1;

   theTransform        = transOriginM*transM*scaleM*rotzM*transOriginNegatedM;
   theInverseTransform = theTransform.i();
   
}


bool ossimImageViewAffineTransform::loadState(const ossimKeywordlist& kwl,
                                              const char* prefix)
{
   ossimString newPrefix = ossimString(prefix) + "transform.";
   
   const char* lookup = kwl.find(newPrefix.c_str(),
                                 "m11");
   if(lookup)
   {
      theTransform[0][0] = ossimString(lookup).toDouble();
   }

   lookup = kwl.find(newPrefix.c_str(),
                     "m12");
   if(lookup)
   {
      theTransform[0][1] = ossimString(lookup).toDouble();
   }

   lookup = kwl.find(newPrefix.c_str(),
                     "m13");
   if(lookup)
   {
      theTransform[0][2] = ossimString(lookup).toDouble();
   }

   lookup = kwl.find(newPrefix.c_str(),
                     "m21");
   if(lookup)
   {
      theTransform[1][0] = ossimString(lookup).toDouble();
   }

   lookup = kwl.find(newPrefix.c_str(),
                     "m22");
   if(lookup)
   {
      theTransform[1][1] = ossimString(lookup).toDouble();
   }

   lookup = kwl.find(newPrefix.c_str(),
                     "m23");
   if(lookup)
   {
      theTransform[1][2] = ossimString(lookup).toDouble();
   }

   lookup = kwl.find(newPrefix.c_str(),
                     "m31");
   if(lookup)
   {
      theTransform[2][0] = ossimString(lookup).toDouble();
   }

   lookup = kwl.find(newPrefix.c_str(),
                     "m32");
   if(lookup)
   {
      theTransform[2][1] = ossimString(lookup).toDouble();
   }

   lookup = kwl.find(newPrefix.c_str(),
                     "m33");
   if(lookup)
   {
      theTransform[2][2] = ossimString(lookup).toDouble();
   }

   lookup = kwl.find(newPrefix.c_str(),
                     "rotate");
   if(lookup)
   {
      double degrees = ossimString(lookup).toDouble();
      theTransform *=  ossimMatrix3x3::createRotationZMatrix(degrees);
   }
   theInverseTransform = theTransform.i();

   return true;
}

bool ossimImageViewAffineTransform::saveState(ossimKeywordlist& kwl,
                                              const char* prefix)const
{
   
   kwl.add(prefix,
           "transform.m11",
           theTransform[0][0],
           true);
   kwl.add(prefix,
           "transform.m12",
           theTransform[0][1],
           true);
   kwl.add(prefix,
           "transform.m13",
           theTransform[0][2],
           true);
   kwl.add(prefix,
           "transform.m21",
           theTransform[1][0],
           true);
   kwl.add(prefix,
           "transform.m22",
           theTransform[1][1],
           true);
   kwl.add(prefix,
           "transform.m23",
           theTransform[1][2],
           true);
   kwl.add(prefix,
           "transform.m31",
           theTransform[2][0],
           true);
   kwl.add(prefix,
           "transform.m32",
           theTransform[2][1],
           true);
   kwl.add(prefix,
           "transform.m33",
           theTransform[2][2],
           true);
   
   return true;
}

bool ossimImageViewAffineTransform::isValid()const
{
   return true;
}

bool ossimImageViewAffineTransform::setView(ossimObject* /* obj */,
                                            bool /* ownsProjection*/ )
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
