//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: 
// 
//********************************************************************
// $Id: ossimFont.cpp,v 1.9 2002/08/23 14:05:22 gpotts Exp $
#include "ossimFont.h"
#include "math.h"
#include <iostream>
using namespace std;

#include "matrix/newmatio.h"

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
