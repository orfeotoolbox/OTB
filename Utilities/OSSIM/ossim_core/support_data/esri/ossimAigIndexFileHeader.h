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
// $Id: ossimAigIndexFileHeader.h,v 1.4 2003/05/13 11:31:10 dburken Exp $
#ifndef ossimAigIndexFileHeader_HEADER
#define ossimAigIndexFileHeader_HEADER
#include <iostream>
#include "base/common/ossimConstants.h"
#include <string.h>

class ossimAigIndexFileHeader
{
public:
   friend std::ostream& operator <<(std::ostream& out, const ossimAigIndexFileHeader& data)
      {
         out << "theFileSize: " << data.theFileSize;
         
         return out;
      }
   ossimAigIndexFileHeader()
      {
         reset();
      }
   void reset()
      {
         theMagicNumber[0] = 0x00;
         theMagicNumber[1] = 0x00;
         theMagicNumber[2] = 0x27;
         theMagicNumber[3] = 0x0A;
         theMagicNumber[4] = 0xFF;
         theMagicNumber[5] = 0xFF;
         theMagicNumber[6] = 0xFC;
         theMagicNumber[7] = 0x14;
         memset((char*)theZeroFill1,
                0x00,
                16);
         memset((char*)theZeroFill2,
                0x00,
                72);
         theFileSize = 50;
      }
   bool writeStream(std::ostream& out);

   ossim_uint8 theMagicNumber[8];

   ossim_uint8 theZeroFill1[16];
   /*!
    *  The size of this file in number of shorts.
    */
   ossim_int32 theFileSize;

   ossim_uint8 theZeroFill2[72];


   /*!
    *  The next two fields that follow are replicated the number
    * of tiles in the file.  These two fields holds the offset of the tile
    * in the data file and the size of the tile in the data file( both are in
    * shorts and not bytes);
    */
// 100 + t*8 
//           4 
//                     MSB Int32 
//                              Offset to tile t in w001001.adf measured in two byte
//                              shorts. 
// 104 + t*8 
//           4 
//                     MSB Int32 
//                              Size of tile t in 2 byte shorts.   
};

#endif
