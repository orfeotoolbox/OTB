//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfSymbolHeaderV2_0.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimNitfSymbolHeaderV2_0_HEADER
#define ossimNitfSymbolHeaderV2_0_HEADER
#include <ossim/support_data/ossimNitfSymbolHeader.h>
#include <ossim/support_data/ossimNitfImageLutV2_0.h>

class ossimNitfSymbolHeaderV2_0 : public ossimNitfSymbolHeader
{
public:
   ossimNitfSymbolHeaderV2_0();
   virtual ~ossimNitfSymbolHeaderV2_0(){}
   
   virtual void parseStream(std::istream &in);
   virtual std::ostream& print(std::ostream &out)const;
   virtual ossim_int32 getDisplayLevel()const;
   virtual ossimDrect getImageRect()const;

TYPE_DATA
private:
   void clearFields();
   
   /*!
    * Is a required 2 byte field.  The expected value
    * is SY.
    */
   char theFilePartType[3];

   /*!
    * is a 10 byte required alphanumeric field.
    */
   char theSymbolId[11];

   /*!
    * is an optional 20 byte alphanumeric field
    */
   char theSymbolName[21];

   /*!
    * Security classification is a required 1 byte field.
    * Expected values can
    * either be:
    *
    * T  Top secret
    * S  Secret
    * C  Confidential
    * R  Restricted
    * U  UnClassified
    */
   char theSymbolSecurityClass[2];

   /*!
    * 40 byte optional field
    */
   char theSymbolCodewords[41];

   /*!
    * 40 byte optional field.
    */
   char theSymbolControlAndHandling[41];

   /*!
    * 40 byte optional field.
    */
   char theSymbolReleasingInstructions[41];

   /*!
    * optional 20 byte field.
    */
   char theSymbolClassificationAuthority[21];

   /*!
    * optional 20 byte field.
    */
   char theSymbolSecurityControlNum[21];

   /*!
    * optional 6 byte value.
    */
   char theSymbolSecurityDowngrade[7];

   /*!
    * Conditional field.  This field exists if
    * theSymbolSecurityDowngrade has the value of
    * 999998.  If it exists it will be a 40 byte
    * field.
    */
   char theSymbolDowngradingEvent[41];

   /*!
    * This is a required 1 byte field and can have
    * a value of:
    *
    * 0    Not encrypted
    * 1    Encrytped
    *
    */
   char theSymbolEncryption[2];

   /*!
    * This is a required one byte field and can have the value
    * of either:
    *
    * B    Bit-mapped
    * C    for CGM
    * O    for object
    *
    *
    * if it's O then theSymbolNumber will hold a number to the symbol
    * object to draw.  See field theSymbolNumber for object
    * numbers and object types.
    */
   char theSymbolType[2];

   /*!
    * This is a required 4 byte field. Ranges from 0-9999.
    *
    * if theSymbolType is B or O this field will contain
    * the number of rows (lines) in the symbol image. This
    * field shall contain 0 if theSymbolType is C
    */
   char theNumberLinesPerSymbol[5];

   /*!
    * This is a required 4 byte field. Ranges from 0-9999
    *
    * if theSymbolType is B or O this field will contain
    * the number of pixels in each row. This
    * field shall contain 0 if theSymbolType is C
    */
   char theSymbolNumberPixelsPerLine[5];

   /*!
    * This is a required 4 byte field.  Ranges from 0-9999
    *
    * if theSymbolType is O this field will contain
    * the line width for the object symbol in pixels.
    *
    * if this field equals theNumberLinesPerSymbol then the
    * symbol should be drawn solid and filled in.
    */
   char theSymbolLineWidth[5];

   /*!
    * This is a required 1 byte field.  Ranges from 0-8
    */
   char theSymbolNumberBitsPerPixel[2];

   /*!
    * This is a required 3 byte field. Ranges from 1-999
    */
   char theSymbolDisplayLevel[4];

   /*!
    * This is a required 3 byte field. Ranges from 0-998
    */
   char theSymbolAttachmentLevel[4];

   /*!
    * This is a required 10 byte field.  This indicates the location
    * in image space.
    *
    *  rrrrrccccc five characters for r followed by five for the column
    */
   char theSymbolLocation[11];

   /*!
    * This is an optional 10 byte field.  Has format
    * rrrrrccccc  where r is row c is column.
    */
   char theSecondSymbolLocation[11];

   /*!
    * This is a required 1 byte field.
    */
   char theSymbolColor[2];

   /*!
    * This is an optional 6 byte field.
    *
    * 
    */
   char theSymbolNumber[7];

   /*!
    * This is a required 3 byte field. Ranges from 0-359
    */
   char theSymbolRotation[4];

   /*!
    * This is a required 3 byte field.  Ranges from 0-256
    */
   char theSymbolNumberOfLutEntries[4];

   /*!
    * This will hold the lookup table values for
    * the pixel data.  This is a conditional field and will
    * exist if theSymbolNumberOfLutEntries is not 0.
    */
   ossimNitfImageLutV2_0 theSymbolLutData;

   /*!
    * This is is a required 5 byte field.
    * Will range from 0-08833.
    */
   char theSymbolExtendedSubheaderDataLength[6];

   /*!
    * This is a conditional 3 byte field that ranges from
    * 0-999.  This field will depend on theSymbolExtendedSubheaderDataLength
    * not being 0.
    */
   char theSymbolSubheaderOverflow[4];
};
#endif
