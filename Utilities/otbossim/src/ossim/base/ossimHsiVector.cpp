//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@remotesensing.org)
// Description:
//
//*************************************************************************
// $Id: ossimHsiVector.cpp 11955 2007-10-31 16:10:22Z gpotts $
#include <ossim/base/ossimHsiVector.h>
#include <ossim/base/ossimRgbVector.h>
#include <ossim/base/ossimNormRgbVector.h>
#include <math.h>
#include <ossim/base/ossimCommon.h>

// nonstandard versions that use operator>, so they behave differently
// than std:::min/max and ossim::min/max.  kept here for now for that
// reason.
#ifndef MAX
#  define MAX(x,y) ((x)>(y)?(x):(y))
#  define MIN(x,y) ((x)>(y)?(y):(x))
#endif

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
