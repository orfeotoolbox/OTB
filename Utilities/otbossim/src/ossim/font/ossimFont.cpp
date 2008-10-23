//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: 
// 
//********************************************************************
// $Id: ossimFont.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/font/ossimFont.h>
#include "math.h"
#include <iostream>
using namespace std;

#include <ossim/matrix/newmatio.h>

RTTI_DEF2(ossimFont, "ossimFont", ossimObject, ossimErrorStatusInterface);

void ossimFont::setRotation(double rotationInDegrees)
{
    theRotation    = rotationInDegrees;

    computeMatrix();
}

void ossimFont::setScale(double horizontal,
                         double vertical)
{
   theHorizontalScale = horizontal;
   theVerticalScale   = vertical;
   
   computeMatrix();   
}

void ossimFont::computeMatrix()
{
   NEWMAT::Matrix r(2,2);
   NEWMAT::Matrix s(2,2);

   double radRotation = theRotation*M_PI/180.0;
   
   r << cos(radRotation) << -sin(radRotation)
     << sin(radRotation) << cos(radRotation);
   
   s << theHorizontalScale << theVerticalShear
     << theHorizontalShear << theVerticalScale;

   theAffineTransform = s*r;

}

void ossimFont::getBoundingClipBox(ossimIrect& clippedBox)
{
   getBoundingBox(clippedBox);
   
   if(!theClippingBox.hasNans()&&
      (theClippingBox.width()>1)&&(theClippingBox.height()>1) )
   {
      clippedBox = clippedBox.clipToRect(theClippingBox);
   }
}

ossimIrect ossimFont::getBoundingClipBox()
{
   ossimIrect clipBox;
   getBoundingClipBox(clipBox);
   
   return clipBox;
}
