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
// Author:  Garrett Potts  (gpotts@imagelinks.com)
//
//*******************************************************************
// $Id: ossimAigStatistics.cpp,v 1.1 2002/08/12 15:26:04 gpotts Exp $
#include "base/misc/ossimEndian.h"
#include "support_data/esri/ossimAigStatistics.h"

bool ossimAigStatistics::writeStream(ostream& out)
{
   ossimEndian endian;
   double tempDouble;
   
   if(endian.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
   {
      tempDouble = theMin;
      endian.swap(tempDouble);
      out.write((char*)(&tempDouble), 8);

      tempDouble = theMax;
      endian.swap(tempDouble);
      out.write((char*)(&tempDouble), 8);
      
      tempDouble = theMean;
      endian.swap(tempDouble);
      out.write((char*)(&tempDouble), 8);
      
      tempDouble = theStandardDev;
      endian.swap(tempDouble);
      out.write((char*)(&tempDouble), 8);
   }
   else
   {
      out.write((char*)(&theMin), 8);
      out.write((char*)(&theMax), 8);
      out.write((char*)(&theMean), 8);
      out.write((char*)(&theStandardDev), 8);
   }

   return true;
}
