//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts  (gpotts@imagelinks.com)
//
//*******************************************************************
// $Id: ossimAigDataFileHeader.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimAigDataFileHeader_HEADER
#define ossimAigDataFileHeader_HEADER
#include <iostream>
#include <ossim/base/ossimConstants.h>
#include <string.h>

class ossimAigDataFileHeader
{
public:
   ossimAigDataFileHeader()
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
   bool writeStream(std::ostream& out)const;
   
   ossim_uint8 theMagicNumber[8];
   ossim_uint8 theZeroFill1[16];
   /*!
    *  The size of this file in number of shorts.
    */
   ossim_int32 theFileSize;
   ossim_uint8 theZeroFill2[72];


   // ________________ These fields are replicated for every tile__________
   
   /*!
    * Size of this tiles data measured in
    * shorts. This matches the size in
    * the index file, and does not
    * include the tile size itself. The
    * next tile starts 2*n+2 bytes after
    * the start of this tile, where n is the
    * value of this field.
    */
      
   ossim_int16 theTileSize;

   //____________________These are from Frank's docs on the grid format
   /*!
    * Tile type code indicating the
    * organization of the following data
    * (integer coverages only).
    *
    * theTileType = 0x00 (constant block)
    *               All pixels take the value of the RMin. Data is ignored.
    *               It appears there is sometimes a bit of
    *               meaningless data (up to four bytes) in the block.
    *
    * theTileType = 0x01 (raw 1bit data)
    *
    * theTileType = 0x04 (raw 4bit data)
    *               One full tiles worth of data pixel values follows the
    *               RMin field, with 4 bits per pixel. The high
    *               order four bits of a byte comes before the
    *               low order four bits.
    * theTileType = 0x08 (raw byte data)
    *               One full tiles worth of data pixel values
    *               (one byte per pixel) follows the RMin field.
    * theTileType = 0x10 (raw 16bit data)
    *               One full tiles worth of data pixel values
    *               follows the RMin field, with 16 bits per pixel (MSB).
    * theTileType = 0xCF (16 bit literal runs/nodata runs)
    *               The data is organized in a series of runs.
    *               Each run starts with a marker which should be
    *               interpreted as:
    *                 Marker < 128: The marker is followed by Marker
    *                                pixels of literal data with two MSB bytes
    *                                per pixel. 
    *                 Marker > 127: The marker indicates that 256-Marker pixels
    *                               of no data pixels should be put into the
    *                               output stream. No data
    *                               (other than the next marker) follows
    *                               this marker. 
    * theTileType = 0xD7 (literal runs/nodata runs)
    *               The data is organized in a series of runs.
    *               Each run starts with a marker which should be
    *               interpreted as:
    *                 Marker < 128: The marker is followed by Marker pixels of
    *                               literal data with one byte per pixel. 
    *                 Marker > 127: The marker indicates that 256-Marker pixels
    *                               of no data pixels should be put into the
    *                               output stream. No data (other than the next
    *                               marker) follows this marker.
    * theTileType = 0xDF (RMin runs/nodata runs)
    *               The data is organized in a series of runs.
    *               Each run starts with a marker which should be
    *               interpreted as: 
    *                 Marker < 128: The marker is followed by Marker pixels of
    *                               literal data with one byte per pixel. 
    *                 Marker > 127: The marker indicates that 256-Marker pixels
    *                               of no data pixels should be put into the
    *                               output stream. No data
    *                               (other than the next marker) follows this
    *                               marker.
    *                 This is similar to 0xD7, except that the data size is zero bytes instead of 1,
    *                 so only RMin values are inserted into the output stream.
    * theTileType = 0xE0 (run length encoded 32bit)
    *               The data is organized in a series of runs.
    *               Each run starts with a marker which should be
    *               interpreted as a count. The four bytes following the count
    *               should be interpreted as an MSB
    *               Int32 value. They indicate that count pixels of value
    *               should be inserted into the output stream.
    * theTileType = 0xF0 (run length encoded 16bit)
    *               The data is organized in a series of runs. Each run
    *               starts with a marker which should be interpreted as a count.
    *               The two bytes following the count should be interpreted as an MSB
    *               Int16 value. They indicate that count pixels of value should be
    *               inserted into the output stream.
    * theTileType = 0xFC/0xF8 (run length encoded 8bit)
    *               The data is organized in a series of runs.
    *               Each run starts with a marker which should be
    *               interpreted as a count. The following byte is the value.
    *               They indicate that count pixels of value should be inserted
    *               into the output stream.  The intepretation is the same for
    *               0xFC, and 0xF8. I believe that 0xFC has a lower dynamic (2bit) range than 0xF8 (4 or 8 bit).
    * theTileType = 0xFF (unknown)
    *               I have yet to deduce how this is organized, though it seems to be used for data with one bit of
    *               dynamic range. and presumably uses some sort of run length encoding. But it's operation is not
    *               obvious despite many hours, productions of hundreds of test files, and attempts to develop a
    *               dictionary of binary strings for different run values.
   */
   
   ossim_uint8 theTileType;

   /*!
    * Number of bytes following to
    * form the minimum value for the
    * tile (integer coverages only). This describes
    * the size of the next field
    */
   ossim_uint8 theMinSize;

   ossim_int32 theMin;
};

#endif
