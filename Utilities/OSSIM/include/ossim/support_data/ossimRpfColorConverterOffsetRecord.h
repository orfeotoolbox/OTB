#ifndef ossimRpfColorConverterOffsetRecord_HEADER
#define ossimRpfColorConverterOffsetRecord_HEADER
#include <iostream>
using namespace std;
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>


class ossimRpfColorConverterOffsetRecord
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfColorConverterOffsetRecord& data);
   ossimRpfColorConverterOffsetRecord();
   
   ossimErrorCode parseStream(istream& in,
                              ossimByteOrder byteOrder);

   void clearFields();
   /*!
    * Is a two byte field as is described in Mil-STD-2411-1.
    * The id can be one of:
    *
    * 1       CCT/1--->1
    * 2       CCT/1--->2
    * 3       CCT/1--->3
    * 4       CCT/1--->4
    * 5       CCT/2--->2
    * 6       CCT/2--->3
    * 7       CCT/2--->4
    * 8       CCT/3--->3
    *
    * where CCT stands for Color Converter Table
    */
   ossim_uint16 theColorConverterTableId;

   /*!
    * Is a 4 byte field.
    */
   ossim_uint32  theNumberOfColorConverterRecords;

   /*!
    * Is a 4 byte field indicating the offset from the start of
    * the color converter subsection to the color converter table.
    */
   ossim_uint32  theColorConverterTableOffset;

   /*!
    * Is a 4 byte field indicating the offset from the colormap
    * subsection to the source color/grayscale offset table.
    */
   ossim_uint32  theSourceColorGrayscaleOffsetTableOffset;

   /*!
    * Is a 4 byte field indicating the offset from the colormap
    * subsection to the color/graysscale offset table.
    */
   ossim_uint32  theTargetColorGrayscaleOffsetTableOffset;
};

#endif
