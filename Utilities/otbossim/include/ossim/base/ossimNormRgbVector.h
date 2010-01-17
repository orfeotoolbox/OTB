//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// License: LGPL
// Author: Garrett Potts 
// Description:
//
//*************************************************************************
// $Id: ossimNormRgbVector.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimNormRgbVector_HEADER
#define ossimNormRgbVector_HEADER
#include <iostream>
#include <ossim/base/ossimConstants.h>

class ossimJpegYCbCrVector;
class ossimHsiVector;
class ossimHsvVector;
class ossimCmyVector;
class ossimRgbVector;

class OSSIM_DLL ossimNormRgbVector
{
public:
   friend std::ostream& operator <<(std::ostream& out, const ossimNormRgbVector& data)
   {
      out << "<" << data.theBuf[0] << ", "
          << data.theBuf[1] << ", "
          << data.theBuf[2] << ">";

         return out;
      }
   ossimNormRgbVector(ossim_float64 r=0, ossim_float64 g=0, ossim_float64 b=0)
      {
         theBuf[0] = r;
         theBuf[1] = g;
         theBuf[2] = b;
      }
   ossimNormRgbVector(ossim_float64 grey)
      {
         theBuf[0] = grey;
         theBuf[1] = grey;
         theBuf[2] = grey;
      }
   ossimNormRgbVector(ossim_float64 colorData[3])
      {
         theBuf[0] = colorData[0];
         theBuf[1] = colorData[1];
         theBuf[2] = colorData[2];
      }
   ossimNormRgbVector(const ossimNormRgbVector& rgb)
   {
      theBuf[0] = rgb.theBuf[0];
      theBuf[1] = rgb.theBuf[1];
      theBuf[2] = rgb.theBuf[2];
   }
   ossimNormRgbVector(const ossimRgbVector& rgb);
   ossimNormRgbVector(const ossimJpegYCbCrVector& YCbCr);
   ossimNormRgbVector(const ossimHsiVector& hsi);
   ossimNormRgbVector(const ossimHsvVector& hsv);
   ossimNormRgbVector(const ossimCmyVector& cmy);
   ossimNormRgbVector operator -(const ossimNormRgbVector& rgb)const
   {
      return ossimNormRgbVector(clamp(theBuf[0] - rgb.theBuf[0]),
                                clamp(theBuf[1] - rgb.theBuf[1]),
                                clamp(theBuf[2] - rgb.theBuf[2]));
   }
   ossimNormRgbVector operator *(ossim_float64 t)const
   {
      return ossimNormRgbVector(clamp(theBuf[0]*t),
                                clamp(theBuf[1]*t),
                                clamp(theBuf[2]*t));
   }
   
   const ossimNormRgbVector& operator =(const ossimJpegYCbCrVector& data);
   const ossimNormRgbVector& operator =(const ossimHsiVector& hsi);
   const ossimNormRgbVector& operator =(const ossimHsvVector& hsv);
   const ossimNormRgbVector& operator =(const ossimCmyVector& cmy);
   bool operator ==(const ossimNormRgbVector& rgb)
      {
         return ( (rgb.theBuf[0] == theBuf[0])&&
                  (rgb.theBuf[1] == theBuf[1])&&
                  (rgb.theBuf[2] == theBuf[2]));
      }
   bool operator !=(const ossimNormRgbVector& rgb)
      {
         return ( (rgb.theBuf[0] != theBuf[0])||
                  (rgb.theBuf[1] != theBuf[1])||
                  (rgb.theBuf[2] != theBuf[2]));
      }
   static ossim_float64 clamp(ossim_float64 colorValue, ossim_float64 min=0.0, ossim_float64 max=1.0)
      {
         colorValue = colorValue > max? max:colorValue;
         colorValue = colorValue < min? min:colorValue;
         
         return colorValue;
      }
   ossim_float64 getR() const { return theBuf[0]; }
   ossim_float64 getG()const  { return theBuf[1]; }
   ossim_float64 getB()const  { return theBuf[2]; }
   void setR(ossim_float64 r) { theBuf[0] = r; }
   void setG(ossim_float64 g) { theBuf[1] = g; }
   void setB(ossim_float64 b) { theBuf[2] = b; }
   
protected:
   ossim_float64 theBuf[3];
   
};

#endif
