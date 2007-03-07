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
// $Id: ossimJpegYCbCrVector.h,v 1.3 2002/11/25 18:59:55 gpotts Exp $
#ifndef ossimJpegYCbCrVector_HEADER
#define ossimJpegYCbCrVector_HEADER
#include <iostream>
using namespace std;
#include "base/common/ossimConstants.h"

class OSSIMDLLEXPORT ossimRgbVector;

class OSSIMDLLEXPORT ossimJpegYCbCrVector
{
public:
   friend ostream& operator << (ostream& out, const ossimJpegYCbCrVector& data)
      {
         out << "<" << (long)data.theBuf[0] << ", "
             << (long)data.theBuf[1] << ", "
             << (long)data.theBuf[2] << ">";

         return out;
      }
   
   ossimJpegYCbCrVector(unsigned char Y=0,
                        unsigned char Cb=0,
                        unsigned char Cr=0)
      {
         theBuf[0] = Y;
         theBuf[1] = Cb;
         theBuf[2] = Cr;
      }
   
   ossimJpegYCbCrVector(const ossimRgbVector&);
   
   long clamp(long colorValue, unsigned char min=0, unsigned char max=255)const
      {
         colorValue = colorValue > max? max:colorValue;
         colorValue = colorValue < min? min:colorValue;

         return colorValue;
      }

   
   ossimJpegYCbCrVector& operator =(const ossimRgbVector&);
   
   unsigned char getY() const { return theBuf[0]; }
   unsigned char getCb()const { return theBuf[1]; }
   unsigned char getCr()const { return theBuf[2]; }
   void setY(unsigned char  Y)  { theBuf[0] = Y; }
   void setCb(unsigned char Cb) { theBuf[1] = Cb; }
   void setCr(unsigned char Cr) { theBuf[2] = Cr; }
   
protected:
   /*!
    * JpegYCbCr is an 8-bit YCbCr color model.  We
    * will let:
    * buf[0] = Y
    * buf[1] = Cb
    * buf[2] = Cr
    */
   unsigned char theBuf[3];
};

#endif
