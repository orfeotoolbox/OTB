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
// Author: Garrett Potts (gpotts@remotesensing.org)
// Description:
//
//*************************************************************************
// $Id: ossimHsiVector.cpp,v 1.5 2005/09/08 13:35:22 gpotts Exp $
#include "ossimHsiVector.h"
#include "ossimRgbVector.h"
#include "ossimNormRgbVector.h"
#include <math.h>
#include "base/common/ossimCommon.h"

ossimHsiVector::ossimHsiVector(const ossimRgbVector& rgb)
{
   setFromRgb(rgb.getR()/255.0, rgb.getG()/255.0, rgb.getB()/255.0);
}

ossimHsiVector::ossimHsiVector(const ossimNormRgbVector& rgb)
{
   setFromRgb(rgb.getR(), rgb.getG(), rgb.getB());
}

ossimHsiVector& ossimHsiVector::operator =(const ossimRgbVector& rgb)
{
   setFromRgb(rgb.getR()/255.0, rgb.getG()/255.0, rgb.getB()/255.0);
   
   return *this;
   
}

ossimHsiVector& ossimHsiVector::operator =(const ossimNormRgbVector& rgb)
{
   setFromRgb(rgb.getR(), rgb.getG(), rgb.getB());
   
   return *this;
   
}

void ossimHsiVector::setFromRgb(ossim_float64 r, ossim_float64 g, ossim_float64 b)
{
   ossim_float64 sum = r + g + b;
   
   theBuf[2] =  sum/3;

   if(theBuf[2] > FLT_EPSILON)
   {
      double deltaI1I2 = r - g;
      double root = deltaI1I2*deltaI1I2 +
                    ((r-b)*(g-b));

      // compute Saturation from RGB
      theBuf[1] = 1 - (3.0/sum)*(MIN(MIN(r,g),b));
      
      // compte Hue from Rgb.
      if(root >= FLT_EPSILON)
      {
         theBuf[0] = acos((.5*((r-g)+(r-b)))/
                          sqrt(root))*DEG_PER_RAD;

         if(b > g)
         {
            theBuf[0] = 360 - theBuf[0];
         }
      }
      else
      {
         theBuf[0] = b;
      }
   }
   else
   {
      theBuf[0] = 0;
      theBuf[1] = 0;
      theBuf[2] = 0;
   }

   setH(theBuf[0]);
   setS(theBuf[1]);
   setI(theBuf[2]);
}
