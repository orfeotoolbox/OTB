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
// $Id: ossimHsvVector.cpp,v 1.3 2002/01/14 16:28:04 gpotts Exp $
#include <algorithm>
//using namespace std;

#include "ossimHsvVector.h"
#include "ossimRgbVector.h"
#include <math.h>
#include "base/common/ossimCommon.h"

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
