//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@remotesensing.org)
// Description:
//
//*************************************************************************
// $Id: ossimHsvVector.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <algorithm>
//using namespace std;

#include <ossim/base/ossimHsvVector.h>
#include <ossim/base/ossimRgbVector.h>
#include <math.h>
#include <ossim/base/ossimCommon.h>

const float ossimHsvVector::OSSIM_HSV_UNDEFINED = -1;
ossimHsvVector::ossimHsvVector(const ossimRgbVector& rgb)
{
   float r,g,b;
     
   r = rgb.getR()/255.0;
   g = rgb.getG()/255.0;
   b = rgb.getB()/255.0;

   // this code was taken from http://www.acm.org/jgt/papers/SmithLyons96/hsv_rgb.html
   // the input RGB is required to be on [0, 1].
   // Their S and V are returned on [0, 1] and H is but we will try
   // to make it return on 0..255 for S and V and see what that looks
   // like and keep the H on 0..6.
   // returned on [0, 6]. Exception: H is returned UNDEFINED if S==0.
   
   float x, v, f, i;
   
   x = min(min(r, g), b);
   v = max(max(r, g), b); 
   if(v==x)
   {
      theBuf[0] = OSSIM_HSV_UNDEFINED, // we really need to mark as undefined 
      theBuf[1] = 0;
      theBuf[2] = v;
   }
   else
   {
      f = (r == x) ? g - b : ((g == x) ? b - r : r - g); 
      i = (r == x) ? 3 : ((g == x) ? 5 : 1);
      
      theBuf[0] = clamp(i - f /(v - x), 0, 6);
      theBuf[0]/=6.0;
      theBuf[1] = clamp((v - x)/v, 0, 1);
      theBuf[2] = clamp(v, 0, 1);
   }
}

const ossimHsvVector& ossimHsvVector::operator =(const ossimRgbVector& rgb)
{
   float r,g,b;
     
   r = rgb.getR()/255.0;
   g = rgb.getG()/255.0;
   b = rgb.getB()/255.0;

   // this code was taken from http://www.acm.org/jgt/papers/SmithLyons96/hsv_rgb.html
   // the input RGB is required to be on [0, 1].
   // Their S and V are returned on [0, 1] and H is but we will try
   // to make it return on 0..255 for S and V and see what that looks
   // like and keep the H on 0..6.
   // returned on [0, 6]. Exception: H is returned UNDEFINED if S==0.
   
   float x, v, f, i;
   
   x = min(min(r, g), b);
   v = max(max(r, g), b); 
   if(v==x)
   {
      theBuf[0] = OSSIM_HSV_UNDEFINED, // we really need to mark as undefined 
      theBuf[1] = 0;
      theBuf[2] = v;
   }
   else
   {
      f = (r == x) ? g - b : ((g == x) ? b - r : r - g); 
      i = (r == x) ? 3 : ((g == x) ? 5 : 1);
      
      theBuf[0] = clamp(i - f /(v - x), 0, 6);
      theBuf[0]/=6.0;
      theBuf[1] = clamp((v - x)/v, 0, 1);
      theBuf[2] = clamp(v, 0, 1);
   }

   return *this;
}
