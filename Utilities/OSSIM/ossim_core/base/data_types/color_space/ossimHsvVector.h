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
// $Id: ossimHsvVector.h,v 1.5 2002/11/25 18:59:55 gpotts Exp $
#ifndef ossimHsvVector_HEADER
#define ossimHsvVector_HEADER
#include <iostream>
using namespace std;
#include "base/common/ossimConstants.h"
class OSSIMDLLEXPORT ossimRgbVector;

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
