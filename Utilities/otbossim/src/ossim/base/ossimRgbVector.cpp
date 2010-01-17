//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@remotesensing.org)
// Description:
//
//*************************************************************************
// $Id: ossimRgbVector.cpp 11347 2007-07-23 13:01:59Z gpotts $
#include <ossim/base/ossimRgbVector.h>
#include <ossim/base/ossimJpegYCbCrVector.h>
#include <ossim/base/ossimHsiVector.h>
#include <ossim/base/ossimHsvVector.h>
#include <ossim/base/ossimCmyVector.h>
#include <ossim/base/ossimCommon.h>
#include <math.h>

ossimRgbVector::ossimRgbVector(const ossimJpegYCbCrVector& YCbCr)
{
   theBuf[0] = static_cast<unsigned char>(clamp(ossim::round<int>(YCbCr.getY() +
                                                      1.402 * (YCbCr.getCr()-128.0))));
   theBuf[1] = static_cast<unsigned char>(clamp(ossim::round<int>(YCbCr.getY() -
                                                      0.34414 *(YCbCr.getCb()-128.0) -
                                                      0.71414*(YCbCr.getCr()-128.0))));
   theBuf[2] = static_cast<unsigned char>(clamp(ossim::round<int>(YCbCr.getY() +
                                                      1.772 * ( YCbCr.getCb()-128.0))));
   
}

ossimRgbVector::ossimRgbVector(const ossimHsiVector& hsi)
{
   float h = hsi.getH();
   float s = hsi.getS();
   float i = hsi.getI();

   float r=0;
   float g=0;
   float b=0;
   
   if(h <= 120.0)
   {
      b = i*(1-s);

      r = i*(1 + s*cos(RAD_PER_DEG*h)/cos((60-h)*RAD_PER_DEG));
      g = 3*i - (r+b);
   }
    else if(h <= 240.0)
    {
       h-=120;

       r = i*(1-s);
       g = i*(1 + s*cos(RAD_PER_DEG*h)/cos((60-h)*RAD_PER_DEG));
       b = 3*i - (r+g);
    }
    else if(h <= 360.0)
    {
       h-=240;

       g = i*(1-s);
       b = i*(1 + s*cos(RAD_PER_DEG*h)/cos((60-h)*RAD_PER_DEG));
       r = 3*i - (g+b);      
    }
   
   theBuf[0] = clamp((long)(r*255));
   theBuf[1] = clamp((long)(g*255));
   theBuf[2] = clamp((long)(b*255));
}

ossimRgbVector::ossimRgbVector(const ossimHsvVector& hsv)
{
   // H is given on [0, 6] or UNDEFINED. S and V are given on [0, 1]. 
      // RGB are each returned on [0, 1]. 
   float h = hsv.getH(), // unnormalize it
         s = hsv.getS(),
         v = hsv.getV();
   float m, n, f; 
   int i; 
   if(h == ossimHsvVector::OSSIM_HSV_UNDEFINED)
   {
      theBuf[0] = clamp(ossim::round<int>(v*255));
      theBuf[1] = clamp(ossim::round<int>(v*255));
      theBuf[2] = clamp(ossim::round<int>(v*255));
   }
   else
   {
      h*=6.0; // unnormalize h
      i = (int)floor(h); 
      f = h - i; 
      if(!(i & 1)) f = 1 - f; // if i is even 
      m = v * (1 - s); 
      n = v * (1 - s * f); 
      switch (i)
      { 
      case 6: 
      case 0:
      {
         theBuf[0] = clamp(ossim::round<int>(v*255));
         theBuf[1] = clamp(ossim::round<int>(n*255));
         theBuf[2] = clamp(ossim::round<int>(m*255));
         break;
      }
      case 1:
      {
         theBuf[0] = clamp(ossim::round<int>(n*255));
         theBuf[1] = clamp(ossim::round<int>(v*255));
         theBuf[2] = clamp(ossim::round<int>(m*255));
         break;
      }
      case 2:
      {
         theBuf[0] = clamp(ossim::round<int>(m*255));
         theBuf[1] = clamp(ossim::round<int>(v*255));
         theBuf[2] = clamp(ossim::round<int>(n*255));
         break;
      }
      case 3: 
      {
         theBuf[0] = clamp(ossim::round<int>(m*255));
         theBuf[1] = clamp(ossim::round<int>(n*255));
         theBuf[2] = clamp(ossim::round<int>(v*255));
         break;
      }
      case 4:
      {
         theBuf[0] = clamp(ossim::round<int>(n*255));
         theBuf[1] = clamp(ossim::round<int>(m*255));
         theBuf[2] = clamp(ossim::round<int>(v*255));
         break;
      }
      case 5: 
      {
         theBuf[0] = clamp(ossim::round<int>(v*255));
         theBuf[1] = clamp(ossim::round<int>(m*255));
         theBuf[2] = clamp(ossim::round<int>(n*255));
         break;
      }
      }
   }
}

ossimRgbVector::ossimRgbVector(const ossimCmyVector& cmy)
{
   theBuf[0] = 255 - cmy.getC();
   theBuf[1] = 255 - cmy.getM();
   theBuf[2] = 255 - cmy.getY();
}

const ossimRgbVector& ossimRgbVector::operator =(const ossimHsvVector& hsv)
{
   // H is given on [0, 6] or UNDEFINED. S and V are given on [0, 1]. 
      // RGB are each returned on [0, 1]. 
   float h = hsv.getH(), // unnormalize it
         s = hsv.getS(),
         v = hsv.getV();
   float m, n, f; 
   int i; 
   if(h == ossimHsvVector::OSSIM_HSV_UNDEFINED)
   {
      theBuf[0] = clamp(ossim::round<int>(v*255));
      theBuf[1] = clamp(ossim::round<int>(v*255));
      theBuf[2] = clamp(ossim::round<int>(v*255));
   }
   else
   {
      h*=6.0; // unnormalize h
      i = (int)floor(h); 
      f = h - i; 
      if(!(i & 1)) f = 1 - f; // if i is even 
      m = v * (1 - s); 
      n = v * (1 - s * f); 
      switch (i)
      { 
      case 6: 
      case 0:
      {
         theBuf[0] = clamp(ossim::round<int>(v*255));
         theBuf[1] = clamp(ossim::round<int>(n*255));
         theBuf[2] = clamp(ossim::round<int>(m*255));
         break;
      }
      case 1:
      {
         theBuf[0] = clamp(ossim::round<int>(n*255));
         theBuf[1] = clamp(ossim::round<int>(v*255));
         theBuf[2] = clamp(ossim::round<int>(m*255));
         break;
      }
      case 2:
      {
         theBuf[0] = clamp(ossim::round<int>(m*255));
         theBuf[1] = clamp(ossim::round<int>(v*255));
         theBuf[2] = clamp(ossim::round<int>(n*255));
         break;
      }
      case 3: 
      {
         theBuf[0] = clamp(ossim::round<int>(m*255));
         theBuf[1] = clamp(ossim::round<int>(n*255));
         theBuf[2] = clamp(ossim::round<int>(v*255));
         break;
      }
      case 4:
      {
         theBuf[0] = clamp(ossim::round<int>(n*255));
         theBuf[1] = clamp(ossim::round<int>(m*255));
         theBuf[2] = clamp(ossim::round<int>(v*255));
         break;
      }
      case 5: 
      {
         theBuf[0] = clamp(ossim::round<int>(v*255));
         theBuf[1] = clamp(ossim::round<int>(m*255));
         theBuf[2] = clamp(ossim::round<int>(n*255));
         break;
      }
      }
   }
   return *this;
}

const ossimRgbVector& ossimRgbVector::operator =(const ossimJpegYCbCrVector& YCbCr)
{
   theBuf[0] = static_cast<unsigned char>(clamp(ossim::round<int>(YCbCr.getY() +
                                                      1.402 * (YCbCr.getCr()-128.0))));
   theBuf[1] = static_cast<unsigned char>(clamp(ossim::round<int>(YCbCr.getY() -
                                                      0.34414 *(YCbCr.getCb()-128.0) -
                                                      0.71414*(YCbCr.getCr()-128.0))));
   theBuf[2] = static_cast<unsigned char>(clamp(ossim::round<int>(YCbCr.getY() +
                                                      1.772 * ( YCbCr.getCb()-128.0))));

   return *this;
}

const ossimRgbVector& ossimRgbVector::operator =(const ossimHsiVector& hsi)
{
   float h = hsi.getH();
   float s = hsi.getS();
   float i = hsi.getI();

   float r=0;
   float g=0;
   float b=0;
   
   if(h <= 120.0)
   {
      b = i*(1-s);

      r = i*(1 + s*cos(RAD_PER_DEG*h)/cos((60-h)*RAD_PER_DEG));
      g = 3*i - (r+b);
   }
    else if(h <= 240.0)
    {
       h-=120;

       r = i*(1-s);
       g = i*(1 + s*cos(RAD_PER_DEG*h)/cos((60-h)*RAD_PER_DEG));
       b = 3*i - (r+g);
    }
    else if(h <= 360.0)
    {
       h-=240;

       g = i*(1-s);
       b = i*(1 + s*cos(RAD_PER_DEG*h)/cos((60-h)*RAD_PER_DEG));
       r = 3*i - (g+b);      
    }
   
   theBuf[0] = clamp((long)(r*255));
   theBuf[1] = clamp((long)(g*255));
   theBuf[2] = clamp((long)(b*255));
   
   return *this;
}

const ossimRgbVector& ossimRgbVector::operator =(const ossimCmyVector& cmy)
{
   theBuf[0] = 255 - cmy.getC();
   theBuf[1] = 255 - cmy.getM();
   theBuf[2] = 255 - cmy.getY();

   return *this;
}
