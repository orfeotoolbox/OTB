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
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description:
//
//*************************************************************************
// $Id: ossimCmyVector.h,v 1.3 2003/05/13 11:31:02 dburken Exp $
#include <iostream>
#include "base/common/ossimConstants.h"

class OSSIMDLLEXPORT ossimJpegYCbCrVector;
class OSSIMDLLEXPORT ossimHsiVector;
class OSSIMDLLEXPORT ossimHsvVector;
class OSSIMDLLEXPORT ossimRgbVector;

class OSSIMDLLEXPORT ossimCmyVector
{
public:
   friend std::ostream& operator <<(std::ostream& out, const ossimCmyVector& cmy)
      {
         out << "<" << cmy.theC << ", " << cmy.theM <<", " << cmy.theY << ">" << std::endl;
         
         return out;
      }
   ossimCmyVector(unsigned char c,
                  unsigned char m,
                  unsigned char y)
      :
         theC(c),
         theM(m),
         theY(y)
      {}
   ossimCmyVector(const ossimRgbVector& rgb);

   ossim_uint8 getC()const
      {
         return theC;
      }
   ossim_uint8 getM()const
      {
         return theC;
      }
   ossim_uint8 getY()const
      {
         return theC;
      }
   const ossimCmyVector& operator = (const ossimRgbVector& rgb);
protected:
   ossim_uint8 theC;
   ossim_uint8 theM;
   ossim_uint8 theY;
};
