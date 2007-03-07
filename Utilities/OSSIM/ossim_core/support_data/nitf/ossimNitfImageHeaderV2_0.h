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
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfImageHeaderV2_0.h,v 1.15 2005/10/04 17:26:26 gpotts Exp $
#ifndef ossimNitfImageHeaderV2_0_HEADER
#define ossimNitfImageHeaderV2_0_HEADER
#include "ossimNitfImageHeader.h"
#include "ossimNitfImageBandV2_0.h"
#include "ossimNitfCompressionHeader.h"
#include <iterator>

class OSSIMDLLEXPORT ossimNitfImageHeaderV2_0 : public ossimNitfImageHeader
{
public:
   ossimNitfImageHeaderV2_0();
   virtual ~ossimNitfImageHeaderV2_0();

   virtual void parseStream(std::istream &in);
   virtual void writeStream(std::ostream &out);
   
   virtual std::ostream& print(std::ostream &out)const;
   
   virtual ossimDrect getImageRect()const;
   virtual ossimDrect getBlockImageRect()const;
   virtual bool isCompressed()const;
   virtual bool isEncrypted()const;
   virtual ossimString getCompressionCode()const;

   virtual long getDisplayLevel()const;
   virtual long getAttachmentLevel()const;
   
   virtual long getNumberOfBands()const;
   virtual long getNumberOfRows()const;
   virtual long getNumberOfCols()const;
   virtual long getNumberOfBlocksPerRow()const;
   virtual long getNumberOfBlocksPerCol()const;
   virtual long getNumberOfPixelsPerBlockHoriz()const;
   virtual long getNumberOfPixelsPerBlockVert()const;
   virtual ossimString getImageId()const;
   virtual long        getBitsPerPixelPerBand()const;
   virtual long        getActualBitsPerPixelPerBand()const;
   virtual ossimString getIMode()const;
   virtual ossim_uint64 getDataLocation()const;
   virtual ossimString getSecurityClassification()const;
   virtual ossimString getAcquisitionDateMonthDayYear(ossim_uint8 separationChar='-')const;
   virtual ossimString getCategory()const;
   virtual ossimString getImageSource()const;
   virtual ossimString getRepresentation()const;
   virtual ossimString getCoordinateSystem()const;
   virtual ossimString getGeographicLocation()const;
   virtual ossimString getPixelValueType()const;
   virtual bool hasBlockMaskRecords()const;
   virtual bool hasPadPixelMaskRecords()const;
   virtual bool hasTransparentCode()const;
   virtual ossim_uint32 getTransparentCode()const;
   virtual ossim_uint32 getBlockMaskRecordOffset(ossim_uint32 blockNumber,
                                                 ossim_uint32 bandNumber)const;
   virtual ossim_uint32 getPadPixelMaskRecordOffset(ossim_uint32 blockNumber,
                                                    ossim_uint32 bandNumber)const;
   
   virtual const ossimRefPtr<ossimNitfCompressionHeader> getCompressionHeader()const;
   virtual const ossimRefPtr<ossimNitfImageBand> getBandInformation(ossim_uint32 idx)const;

TYPE_DATA
private:
   //
   
   void clearFields();
   /*!
    * Is a required 2 byte value and is IM.
    */
   char theType[3];

   /*!
    * Is a required 10 Alphanumeric value.
    */
   char theImageId[11];

   /*!
    * Contains the time of acquisition of the image.
    * this is a 14 byte field and has the format:
    *
    * DDHHMMSSZMONYY
    *
    * DD   is the day of the month
    * HH   is the Hour
    * MM   is the minutes
    * SS   is the seconds
    * Z    is fixed (Zulu time)
    * MON  is the first 3 chars of the month
    * YY   is the year
    */
   char theDateTime[15];

   /*!
    * is a 17 byte field.  Has the form
    *
    * BBBBBBBBBBFFFFFCC
    *
    * where:
    *
    * B     is the encyclopedia id
    * F     is the functional category code
    * C     is the country code.
    */
   char theTargetId[18];

   /*!
    * Is 80 bytes and contains the title
    */
   char theTitle[81];

   /*!
    * Is a required 1 byte field and has value:
    *
    * T    = Top secret
    * S    = Secrect
    * C    = confidential
    * R    = Restricted
    * U    = Unclassified
    */
   char theSecurityClassification[2];

   /*!
    * is a 40 byte field
    */
   char theCodewords[41];

   /*!
    * is a 40 byte field
    */
   char theControlAndHandling[41];

   /*!
    * Is a 40 byte field.
    */
   char theReleasingInstructions[41];

   /*!
    * Is a 20 byte field
    */
   char theClassificationAuthority[21];

   /*!
    *
    */
   char theSecurityControlNumber[21];

   /*!
    * Is a 6 byte field with form:
    *
    *  YYMMDD.  Year month day
    *
    * 999999 indicates that he Originating
    *        agencies determination is required
    *        (OADR)
    * 999998 downgrade event will specify at what
    *        point and time the declassification
    *        or downgrading is to take place.
    */
   char theSecurityDowngrade[7];

   /*!
    * This is a conditional field and is present
    * if the Security downgrade is 999998.
    */
   char theDowngradingEvent[41];

   /*!
    * Is arequired 1 byte field.
    *
    * 0  = no encryption
    * 1  = encryption
    */
   char theEncryption[2];

   /*!
    * Is a 42 byte optional field.
    */
   char theImageSource[43];

   /*!
    * Required 8 byte number 0-99999999
    */
   char theSignificantRows[9];

   /*!
    * Required 8 byte number 0-99999999
    */
   char theSignificantCols[9];

   /*!
    * Required 3 byte value.
    * Values:
    *
    * INT   integer 16 bits
    * SI    singend 16 bit integer
    * R     32 bit float
    * C     for complex (32 bit float real part, 32 bit float imag part)
    * B     for bitmapped image represented as single bits 1 or 0
    * U     for user defined.
    */
   char thePixelValueType[4];

   /*!
    * This is required 8 byte field.
    *
    * Values:
    *
    * MONO    indicates monochrome
    * RGB     indicates true color red green blue
    * RGB/LUT indicates RGB lookup table.
    * MULTI   indicates multiband imgery
    * YCbCr601 indicates compressed in CCIR 601
    *          color space using JPEG (field IC=C3)
    */
   char theRepresentation[9];

   /*!
    * This is a required 8 byte field.  Contains info
    * about the specific category of the image.
    *
    * VIS    indicates visible imagery
    * MAP    indicates maps
    * SAR    indicates Synthetic Aperture Radar
    * IR     indicates infrared.
    * MS     indicates multispectral
    * FP     indicates fingerprints
    * MRI    indicates Magnetic Resonance imagery
    * XRAY   indicates x-rays
    * CAT    indicates cat scans
    *
    * default value is VIS
    */
   char theCategory[9];

   /*!
    * Is optional and indicates the number of
    * bits per band 01-64
    */
   char theBitsPerPixelPerBand[3];

   /*!
    * Is 1 byte optional field.  Can have
    * values L (left) or R (right).  Any other value
    * is to interpreted as right justified.
    *
    * This indicates how the significant bits are justified
    * in the field
    */
   char theJustification[2];

   /*!
    * Is a 1 byte required field and can have
    * values U, G, C, or N
    *
    * U indicates UTM
    * G indicates Geodetic (Geographic)
    * C indicates Geocentric
    * N indicates None
    */
   char theCoordinateSystem[2];

   /*!
    * This is a conditional field.  If theCoordinateSystem
    * is N then it does not exist.  If it's any other
    * value then it's given in image coordinate order
    *
    * (0,0) (0,MaxCol) (MaxRow, MaxCol) (MaxRow, 0)
    *
    * if its geodetic or geocentric it will have a lat lon coordinate
    * with each coordinate as:
    * Lat = ddmmssX
    *       where d is degrees and m is minutes
    *       and s is seconds and X is either N (North) or S (South).
    * Lon = dddmmssX
    *       where d is degrees and m is minutes
    *       and s is seconds and X is either N (North) or S (South).
    * if its a U the the coordinates are expressed in a UTM grid coord
    * which is also known as (Military Grid Reference System).
    */
   char theGeographicLocation[61];

   /*!
    * this is a required 1 byte field and indicates the number of comments.
    */
   char theNumberOfComments[2];

   /*!
    * Dynamic buffer for the comments.  Each comment is 80 bytes
    */
   char *theImageComments;

   /*!
    * This is a required 2 byte field and indicates the compression
    * type of the image.
    *
    * C1  indicates bi-level
    * C2  indicates ARIDPCM
    * C3  inidates JPEG
    * C4  inidcates Vector Quantization
    * NC  indicates not compressed
    * M0  indicates compressed images
    * M3  indicates compressed images
    * M4  indicates compressed images
    * NM  indicates uncompressed
    *
    * The M's and NM are blocked images that contain
    * block mask and/or transparent pixel mask.  There
    * will be an Image Data Mask Subheader at the beginning
    * of the image data area.
    *
    * Compression C1 and C2 will not exist if the Number of bands
    * is > 1 or NBLOCKS > 1
    *
    */
   char theCompression[3];

   /*!
    * This is a conditional field.  if theCompression is
    * not equal to NC then this field exists.
    */
   char theCompressionRateCode[5];

   /*!
    * Is required 1 byte field  and will have value 1-9
    *
    * If theRepresentation is MONO this should be 1
    * if theRepresentation is RGB  this should be 3
    * if theRepresentation is RGB/LUT this should be 1
    * if theRepresentation YCbCr601 this should be 3
    * if theRepresentation is MULTI this should be 2-9
    */
   char theNumberOfBands[2];

   std::vector<ossimRefPtr<ossimNitfImageBandV2_0> > theImageBands;

   /*!
    * Required 1 byte field.  Has value 0 or 4
    */
   char theImageSyncCode[2];

   /*!
    * Required 1 byte field. Has value B, P, or S
    * For uncompressed images:
    * B = Interleaved by block
    * P = Interleaved by pixel
    * S = Band sequential
    */
   char theImageMode[2];

   /*!
    * Required 4 byte value.  ranges from
    * 1-9999
    */
    char theNumberOfBlocksPerRow[5];

   /*!
    * Required 4 byte value. Ranges from
    * 1-9999
    */
   char theNumberOfBlocksPerCol[5];

   /*!
    * Reuqired 4 byte field
    */
   char theNumberOfPixelsPerBlockHoriz[5];

   /*!
    * Reuqired 4 byte field
    */
   char theNumberOfPixelsPerBlockVert[5];

   /*!
    * Required 2 byte field.
    */
   char theNumberOfBitsPerPixelPerBand[3];

   /*!
    * Required 3 byte field.
    */
   char theDisplayLevel[4];

   /*!
    * Required 3 byte field
    */
   char theAttachmentLevel[4];

   /*!
    * Required 10 byte field. Has the form
    * rrrrrccccc  where r is row and c is column
    */
   char theImageLocation[11];

   /*!
    * Required 4 byte field
    */
   char theImageMagnification[5];

   /*!
    * Required 5 byte field.
    */
   char theUserDefinedImageDataLength[6];

   /*!
    * conditional 3 byte field. if theUserDefinedImageDataLength
    * is not 0 
    */
   char theUserDefinedOverflow[4];

   /*!
    * Is a required 5 byte field. 0-99999
    */
   char theExtendedSubheaderDataLen[6];

   /*!
    * Is a conditional 3 byte field.  This depends on
    * theExtendedSubheaderDataLen not being 0.
    */
   char theExtendedSubheaderOverflow[4];

   /*******************************************************************************
    *                    THIS IS THE MASK TABLE IF PRESENT
    *
    */
   
   /**
    * FIELD NAME: IMDATOFF
    * 
    * Blocked Image Data Offset.
    * 
    * 4 byte Binary integer: 0x00000000 to 0xFFFFFFFF
    * 
    * This field is included if the IC value equals NM, M0, M3, or M4.
    * It identifies the offset from the beginning of the Image Data Mask Subheader
    * to the first byte of the blocked image data. This offset, when used in combination
    * with the offsets provided in the BMR fields, can provide random access to any recorded
    * image block in any image band.
    * 
    */
   ossim_uint32 theBlockedImageDataOffset;

   /**
    * FIELD NAME: BMRLNTH
    *
    * Block Mask Record Length.
    *
    * 2 byte Unsigned binary integer
    * 0x0000=No Block mask record; 0x0004=Block mask
    * records (4 bytes each) are present  
    * 
    * This field is included if the IC value equals NM, M0, M3, or M4.
    * It identifies the length of each Block Mask Record in bytes. The total
    * length of the Block Mask Records is equal to BMRLNTH x NBPR x NBPC x NBANDS if the IMODE is S and
    * BMRLNTH x NBPR x NBPC otherwise. If all of the image blocks are recorded,
    * this value is set to 0, and the conditional BMR fields are not recorded/transmitted.
    * If this field is present, but coded as 0, then a transparent pixel mask is included. 
    *
    */
   ossim_uint16 theBlockMaskRecordLength;

   /**
    * FIELD: TMRLNTH
    *
    * Pad Pixel Mask Record Length.
    *
    * 2 byte Unsigned binary integer a value of
    * 0x0000=No Pad pixel mask records and 
    * 0x0004=Pad pixel mask records (4 bytes each) are present
    * 
    * This field is included if the IC value equals NM, M0, M3, or M4.
    * It identifies the length of each Transparent Pixel Mask Record in bytes.
    * The total length of the Transparent Pixel Mask Records is equal to
    * TMRLNTH x NBPR x NBPC x NBANDS when IMODE is S and TMRLNTH x NBPR x NBPC otherwise.
    * If none of the image blocks contain transparent pixels, this value is set to 0,
    * and the conditional TMR fields are not recorded/transmitted. For IC value of M3,
    * the value is set to 0. If this field is present, but coded as 0, then a Block Mask is included.
    */
   ossim_uint16 thePadPixelMaskRecordLength;
   
   /**
    *  FIELD: TPXCDLNTH
    *
    *  Transparent Output Pixel Code Length.
    *
    *  2 byte Unsigned binary integer;
    *  0x0000=No pad pixels; 0x0001 to 0x0010=pad pixel code length in bits
    *  
    *  This field is included if the IC value equals NM, M0, M3, or M4.
    *  It identifies the length in bits of the Transparent Output Pixel Code.
    *  If coded as 0, then no transparent pixels are present,
    *  and the TPXCD field is not recorded. For IC value of M3, the value is set to 0.
    */
   ossim_uint16 theTransparentOutputPixelCodeLength;


   /**
    * FIELD: TPXCD
    *
    * Pad Output Pixel Code.
    *
    * This field is included if the IC value equals NM, M0, or M4, and
    * TPXCDLNTH is not 0. It contains the output pixel code
    * that represents a transparent pixel in the image. This
    * value is unique within the image, and allows the user to
    * identify transparent pixels. The transparent pixel output code
    * length is determined by TPXCDLNTH, but the value is stored in two bytes.
    * If the number of bits used by TPXCD is less than the number of
    * bits available for storage, the value shall be justified in
    * accordance with the PJUST field in the image subheader.
    */
   ossim_uint16 thePadOutputPixelCode;
   std::vector<ossim_uint32> theBlockMaskRecords;
   std::vector<ossim_uint32> thePadPixelMaskRecords;

   ossimRefPtr<ossimNitfCompressionHeader> theCompressionHeader;
   ossim_uint64 theDataLocation;
   
};

#endif
