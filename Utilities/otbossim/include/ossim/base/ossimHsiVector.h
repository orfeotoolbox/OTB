//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// Description:
//
//*************************************************************************
// $Id: ossimHsiVector.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimHsiVector_HEADER
#define ossimHsiVector_HEADER
#include <iostream>
#include <ossim/base/ossimConstants.h>

class ossimRgbVector;
class ossimNormRgbVector;

class OSSIMDLLEXPORT ossimHsiVector
{
public:
   friend std::ostream& operator << (std::ostream& out, const ossimHsiVector & data)
   {
      out << "<" << data.theBuf[0] << ", "
          << data.theBuf[1] << ", "
          << data.theBuf[2] << ">";
      
      return out;
   }
   // assumed normalized ossim_float64s
   //
   //
   ossimHsiVector(ossim_float64 h=0, ossim_float64 s=0, ossim_float64 i=0)
   {
      theBuf[0] = h;
      theBuf[1] = s;
      theBuf[2] = i;
   }
   ossimHsiVector(const ossimRgbVector& rgb);
   ossimHsiVector(const ossimNormRgbVector& rgb);

   ossimHsiVector& operator =(const ossimRgbVector& rgb);
   ossimHsiVector& operator =(const ossimNormRgbVector& rgb);

   void setFromRgb(ossim_float64 r, ossim_float64 g, ossim_float64 b);
   
   ossim_float64 getH()const { return theBuf[0]; }
   ossim_float64 getS()const { return theBuf[1]; }
   ossim_float64 getI()const { return theBuf[2]; }
   void setH(ossim_float64 H) { theBuf[0] = H; }
   void setS(ossim_float64 S) { theBuf[1] = S; }
   void setI(ossim_float64 I) { theBuf[2] = I; }

   ossim_float64 clamp(ossim_float64 colorValue, ossim_float64 min=0, ossim_float64 max=1)const
      {
         colorValue = colorValue > max? max:colorValue;
         colorValue = colorValue < min? min:colorValue;
         
         return colorValue;
      }
   
protected:
   /*!
    * buf[0] = hue
    * buf[1] = saturation
    * buf[2] = intensity
    */
   ossim_float64 theBuf[3];
};

#endif
