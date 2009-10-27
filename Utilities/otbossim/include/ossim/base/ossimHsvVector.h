//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// Description:
//
//*************************************************************************
// $Id: ossimHsvVector.h 14789 2009-06-29 16:48:14Z dburken $
#ifndef ossimHsvVector_HEADER
#define ossimHsvVector_HEADER
#include <iostream>
using namespace std;
#include <ossim/base/ossimConstants.h>

class ossimRgbVector;

class OSSIMDLLEXPORT ossimHsvVector
{
public:
   friend ostream& operator << (ostream& out, const ossimHsvVector & data)
      {
         out << "<" << data.theBuf[0] << ", "
             << data.theBuf[1] << ", "
             << data.theBuf[2] << ">";

         return out;
      }
   // assumed normalized floats
   //
   //
   ossimHsvVector(float h=0, float s=0, float i=0)
      {
         theBuf[0] = h;
         theBuf[1] = s;
         theBuf[2] = i;
      }
   ossimHsvVector(const ossimRgbVector& rgb);

   const ossimHsvVector& operator =(const ossimRgbVector& rgb);
   
   float getH()const { return theBuf[0]; }
   float getS()const { return theBuf[1]; }
   float getV()const { return theBuf[2]; }
   
   unsigned char getVUnNormalized()const
      {
         return static_cast<unsigned char>(theBuf[2]*255);
      }
   void setH(float H) { theBuf[0] = H; }
   void setS(float S) { theBuf[1] = S; }
   void setV(float V) { theBuf[2] = V; }

   float clamp(float colorValue, float min=0, float max=255)const
      {
         colorValue = colorValue > max? max:colorValue;
         colorValue = colorValue < min? min:colorValue;
         
         return colorValue;
      }
   
   static const float OSSIM_HSV_UNDEFINED;
protected:
   /*!
    * buf[0] = hue     [0..1]
    * buf[1] = saturation [0..1]
    * buf[2] = value [0..1]
    */
   float theBuf[3];

};

#endif
