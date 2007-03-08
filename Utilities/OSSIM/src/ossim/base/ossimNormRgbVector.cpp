//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// License: LGPL
// Author: Garrett Potts 
// Description:
//
//*************************************************************************
// $Id: ossimNormRgbVector.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/base/ossimNormRgbVector.h>
#include <ossim/base/ossimRgbVector.h>
#include <ossim/base/ossimJpegYCbCrVector.h>
#include <ossim/base/ossimHsiVector.h>
#include <ossim/base/ossimHsvVector.h>
#include <ossim/base/ossimCmyVector.h>
#include <ossim/base/ossimCommon.h>
#include <math.h>

ossimNormRgbVector::ossimNormRgbVector(const ossimRgbVector& rgb)
{
   theBuf[0] = rgb.getR()/255.0;
   theBuf[1] = rgb.getG()/255.0;
   theBuf[2] = rgb.getB()/255.0;
}

ossimNormRgbVector::ossimNormRgbVector(const ossimJpegYCbCrVector& YCbCr)
{
   (*this) = YCbCr;
   
}

ossimNormRgbVector::ossimNormRgbVector(const ossimHsiVector& hsi)
{
   (*this)=hsi;
}

ossimNormRgbVector::ossimNormRgbVector(const ossimHsvVector& hsv)
{
   (*this) = hsv;
}

ossimNormRgbVector::ossimNormRgbVector(const ossimCmyVector& cmy)
{
   theBuf[0] = 255 - cmy.getC();
   theBuf[1] = 255 - cmy.getM();
   theBuf[2] = 255 - cmy.getY();
}

const ossimNormRgbVector& ossimNormRgbVector::operator =(const ossimHsvVector& hsv)
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
      theBuf[0] = clamp(v);
      theBuf[1] = clamp(v);
      theBuf[2] = clamp(v);
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
         theBuf[0] = clamp(v);
         theBuf[1] = clamp(n);
         theBuf[2] = clamp(m);
         break;
      }
      case 1:
      {
         theBuf[0] = clamp(n);
         theBuf[1] = clamp(v);
         theBuf[2] = clamp(m);
         break;
      }
      case 2:
      {
         theBuf[0] = clamp(m);
         theBuf[1] = clamp(v);
         theBuf[2] = clamp(n);
         break;
      }
      case 3: 
      {
         theBuf[0] = clamp(m);
         theBuf[1] = clamp(n);
         theBuf[2] = clamp(v);
         break;
      }
      case 4:
      {
         theBuf[0] = clamp(n);
         theBuf[1] = clamp(m);
         theBuf[2] = clamp(v);
         break;
      }
      case 5: 
      {
         theBuf[0] = clamp(v);
         theBuf[1] = clamp(m);
         theBuf[2] = clamp(n);
         break;
      }
      }
   }
   return *this;
}

const ossimNormRgbVector& ossimNormRgbVector::operator =(const ossimJpegYCbCrVector& YCbCr)
{
   theBuf[0] = clamp((YCbCr.getY() +
                      1.402 * (YCbCr.getCr()-128.0))/255.0);
   theBuf[1] = clamp((YCbCr.getY() -
                      0.34414 *(YCbCr.getCb()-128.0) -
                      0.71414*(YCbCr.getCr()-128.0))/255.0);
   theBuf[2] = clamp((YCbCr.getY() +
                      1.772 * ( YCbCr.getCb()-128.0))/255.0);

   return *this;
}

const ossimNormRgbVector& ossimNormRgbVector::operator =(const ossimHsiVector& hsi)
{
   ossim_float32 h = hsi.getH();
   ossim_float32 s = hsi.getS();
   ossim_float32 i = hsi.getI();

   ossim_float32 r=0;
   ossim_float32 g=0;
   ossim_float32 b=0;
   
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
   
   theBuf[0] = clamp(r);
   theBuf[1] = clamp(g);
   theBuf[2] = clamp(b);
   
   return *this;
}

const ossimNormRgbVector& ossimNormRgbVector::operator =(const ossimCmyVector& cmy)
{
   theBuf[0] = (255 - cmy.getC())/255.0;
   theBuf[1] = (255 - cmy.getM())/255.0;
   theBuf[2] = (255 - cmy.getY())/255.0;

   return *this;
}
