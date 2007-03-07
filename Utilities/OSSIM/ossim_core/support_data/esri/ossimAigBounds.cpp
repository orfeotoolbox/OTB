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
// $Id: ossimAigBounds.cpp,v 1.2 2002/08/12 16:12:59 gpotts Exp $
#include "support_data/esri/ossimAigBounds.h"
#include "base/misc/ossimEndian.h"

bool ossimAigBounds::writeStream(ostream& out)
{
   ossimEndian endian;
   double tempDouble;
   
   if(endian.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
   {
      tempDouble = ll_x;
      endian.swap(tempDouble);
      out.write((char*)(&tempDouble), 8);

      tempDouble = ll_y;
      endian.swap(tempDouble);
      out.write((char*)(&tempDouble), 8);
      
      tempDouble = ur_x;
      endian.swap(tempDouble);
      out.write((char*)(&tempDouble), 8);
      
      tempDouble = ur_y;
      endian.swap(tempDouble);
      out.write((char*)(&tempDouble), 8);
   }
   else
   {
      out.write((char*)(&ll_x), 8);
      out.write((char*)(&ll_y), 8);
      out.write((char*)(&ur_x), 8);
      out.write((char*)(&ur_y), 8);
   }

   return out.good();
}
