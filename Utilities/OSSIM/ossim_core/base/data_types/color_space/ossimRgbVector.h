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
// $Id: ossimRgbVector.h,v 1.10 2002/11/01 19:08:48 gpotts Exp $
#ifndef ossimRgbVector_HEADER
#define ossimRgbVector_HEADER
#include <iostream>
using namespace std;
#include "base/common/ossimConstants.h"

class OSSIMDLLEXPORT ossimJpegYCbCrVector;
class OSSIMDLLEXPORT ossimHsiVector;
class OSSIMDLLEXPORT ossimHsvVector;
class OSSIMDLLEXPORT ossimCmyVector;

class OSSIMDLLEXPORT ossimRgbVector
{
public:
   friend ostream& operator <<(ostream& out, const ossimRgbVector& data)
      {
         out << "<" << (long)data.theBuf[0] << ", "
             << (long)data.theBuf[1] << ", "
             << (long)data.theBuf[2] << ">";

         return out;
      }
   ossimRgbVector(unsigned char r=0, unsigned char g=0, unsigned char b=0)
      {
         theBuf[0] = r;
         theBuf[1] = g;
         theBuf[2] = b;
      }
   ossimRgbVector(unsigned char grey)
      {
         theBuf[0] = grey;
         theBuf[1] = grey;
         theBuf[2] = grey;
      }
   ossimRgbVector(unsigned char colorData[3])
      {
         theBuf[0] = colorData[0];
         theBuf[1] = colorData[1];
         theBuf[2] = colorData[2];
      }

   ossimRgbVector(const ossimJpegYCbCrVector& YCbCr);
   ossimRgbVector(const ossimHsiVector& hsi);
   ossimRgbVector(const ossimHsvVector& hsv);
   ossimRgbVector(const ossimCmyVector& cmy);

   ossimRgbVector operator -(const ossimRgbVector& rgb)const
      {
         return ossimRgbVector((unsigned char)clamp((long)theBuf[0] - (long)rgb.theBuf[0]),
                               (unsigned char)clamp((long)theBuf[1] - (long)rgb.theBuf[1]),
                               (unsigned char)clamp((long)theBuf[2] - (long)rgb.theBuf[2]));
      }

   ossimRgbVector operator +(const ossimRgbVector& rgb)const
      {
         return ossimRgbVector((unsigned char)clamp((long)theBuf[0] + (long)rgb.theBuf[0]),
                               (unsigned char)clamp((long)theBuf[1] + (long)rgb.theBuf[1]),
                               (unsigned char)clamp((long)theBuf[2] + (long)rgb.theBuf[2]));
      }
   ossimRgbVector operator *(double t)const
      {
         return ossimRgbVector((unsigned char)clamp((long)(theBuf[0]*t)),
                               (unsigned char)clamp((long)(theBuf[1]*t)),
                               (unsigned char)clamp((long)(theBuf[2]*t)));
      }
   const ossimRgbVector& operator =(const ossimJpegYCbCrVector& data);
   const ossimRgbVector& operator =(const ossimHsiVector& hsi);
   const ossimRgbVector& operator =(const ossimHsvVector& hsv);
   const ossimRgbVector& operator =(const ossimCmyVector& cmy);
   bool operator ==(const ossimRgbVector& rgb)
      {
         return ( (rgb.theBuf[0] == theBuf[0])&&
                  (rgb.theBuf[1] == theBuf[1])&&
                  (rgb.theBuf[2] == theBuf[2]));
      }
   bool operator !=(const ossimRgbVector& rgb)
      {
         return ( (rgb.theBuf[0] != theBuf[0])||
                  (rgb.theBuf[1] != theBuf[1])||
                  (rgb.theBuf[2] != theBuf[2]));
      }
   static long clamp(long colorValue, unsigned char min=0, unsigned char max=255)
      {
         colorValue = colorValue > (long)max? (long)max:colorValue;
         colorValue = colorValue < (long)min? (long)min:colorValue;
         
         return colorValue;
      }
   unsigned char getR() const { return theBuf[0]; }
   unsigned char getG()const  { return theBuf[1]; }
   unsigned char getB()const  { return theBuf[2]; }
   void setR(unsigned char  R) { theBuf[0] = R; }
   void setG(unsigned char G)  { theBuf[1] = G; }
   void setB(unsigned char B)  { theBuf[2] = B; }
   
protected:
   unsigned char theBuf[3];
};

#endif
