//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Contains class declaration for ossimGeneralRasterInfo
//*******************************************************************
//  $Id: ossimGeneralRasterInfo.h 9852 2006-11-01 20:26:55Z gpotts $

#ifndef ossimGeneralRasterInfo_HEADER
#define ossimGeneralRasterInfo_HEADER

#include <vector>

#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/imaging/ossimImageMetaData.h>

//***
// Forward class declarations.
//***
class ossimKeywordlist;
class ossimString;


//***************************************************************************
// CLASS:  ossimGeneralRasterInfo
//***************************************************************************
class OSSIMDLLEXPORT ossimGeneralRasterInfo : public ossimObject,
                                              public ossimErrorStatusInterface
{
public:

   enum ossimFillMode
   {
      NONE,
      ZEROES_TO_MIN,
      ZEROES_TO_NULL_EDGES_ONLY
   };

   enum
   {
      UNKNOWN = -1
   };
   
   ossimGeneralRasterInfo();

   ossimGeneralRasterInfo(const std::vector<ossimFilename>& imageFileList,
                          ossimScalarType      pixelType,
                          ossimInterleaveType  il_type,
                          ossim_uint32         numberOfBands,
                          ossim_uint32         lines,
                          ossim_uint32         samples,
                          ossim_uint32         headerSize,
                          ossimFillMode        nullsMode,
                          ossim_uint32         pixelsToChop);
   
   ossimGeneralRasterInfo(const ossimKeywordlist& kwl, const char* prefix=0);

   ~ossimGeneralRasterInfo ();

   /*!
    *  Zero based rectangle of the valid image.
    */
   ossimIrect imageRect()       const { return theImageRect;      }

   /*!
    *  Returns the rectangle of where the image is within the raster file.
    *  In most cases this will be the same as the rawImageRect. (zero based)
    */
   ossimIrect validImageRect()  const { return theValidImageRect; }

   /*!
    *  Zero based rectangle of the raw image.
    */
   ossimIrect rawImageRect()    const { return theRawImageRect;   }

   /*!
    * Returns the offset of this image's origin to a full-image's origin, in
    * the case where this image is a sub-image of a larger full-image.
    */
   ossimIpt subImageOffset()  const { return theSubImageOffset; }

   /*!
    *  Returns the size of header in bytes.
    */
   ossim_uint32 headerSize()      const { return theHeaderSize;     }

   /*!
    *  Returns the fill mode.  This is used to flip values on ingest.
    *  Valid Mode Enumerations:
    *  - 0  Do nothing to the pixels on ingest.
    *  - 1  Flip all zeroes to the minimum pixel value.
    *  - 2  Flip zeroes within the image line to minimum pixel value.
    *       This is used by edge walking code.
    */
   ossim_uint32 fillToNullsMode() const { return theSetNullsMode;   }

   /*!
    *  Sets theSetNullsMode.
    */
   void setFillToNullsMode(ossim_uint32 mode);

   void setMinPixelValue(ossim_uint32 band, double value)
      {
         theMetaData.setMinPix(band, value);
      }
  double getMinPixelValue(ossim_uint32 band)const
  {
    return theMetaData.getMinPix(band);
  }
   void setMaxPixelValue(ossim_uint32 band, double value)
      {
         theMetaData.setMaxPix(band, value);
      }
  double getMaxPixelValue(ossim_uint32 band)const
  {
    return theMetaData.getMaxPix(band);
  }
   void setNullPixelValue(ossim_uint32 band, double value)
      {
         theMetaData.setNullPix(band, value);
      }
  double getNullPixelValue(ossim_uint32 band)const
  {
    return theMetaData.getNullPix(band);
  }
   /*!
    *  Sets theNumberOfBands.
    */
   void setNumberOfBands(ossim_uint32 bands) { theMetaData.setNumberOfBands(bands); }

   /*!
    *  Returns the number of pixels from the edge of a line
    *  to set to the fill value.  This can be used to eliminate
    *  resampling error on the edge of an image and still keep
    *  the image demensions relative to the geometry file.
    */
   ossim_uint32 pixelsToChop()   const { return thePixelsToChop;   } 

   /*!
    *  Number of bands.  Defaulted to one.
    */
   ossim_uint32 numberOfBands()  const { return theMetaData.getNumberOfBands();  }

   /*!
    *  Enumerated in InterleaveTypeLUT. Should be either bip,
    *  bil or bsq.  Defaulted to bip.
    */
   ossimInterleaveType interleaveType() const { return theInterleaveType; }

   /*!
    * Returns the pixel type represented as a "PixelType" enumeration.
    * See "constants.h" file for PixelType enumerations.
    */
   ossimScalarType getScalarType() const { return theMetaData.getScalarType(); }
   
   /*!
    *  Returns the number of lines within "theValidImageRect".
    */
   ossim_uint64 validLines()        const;

   /*!
    *  Returns the number of lines within "theRawImageRect".
    */
   ossim_uint64 rawLines()          const;

   /*!
    *  Returns the number of bytes in a raw line.
    */
   ossim_uint64 bytesPerRawLine()   const;

   /*!
    *  Returns the number of bytes in a valid line.
    */
   ossim_uint64 bytesPerValidLine() const;

   /*!
    *  Returns the number of samples within "theValidImageRect".
    */
   ossim_uint64 validSamples()      const;

   /*!
    *  Returns the number of samples within "theRawImageRect".
    */
   ossim_uint64 rawSamples()        const;

   /*!
    *  Returns the offset in bytes to the first valid sample in the image.
    */
   ossim_uint64 offsetToFirstValidSample() const;
   
   std::vector<ossimFilename> getImageFileList() const { return theImageFileList; }

   void setImageFileList(const std::vector<ossimFilename>& list)
      { theImageFileList = list; }
   void setHeaderSize(ossim_uint32 headerSize) {theHeaderSize = headerSize;}
   void setInterleaveType(ossimInterleaveType il_type)
      {theInterleaveType = il_type;}
   virtual std::ostream& print(std::ostream& out) const;
   void setImageRect(const ossimIrect& imageRect){theImageRect = imageRect;}
   void setValidImageRect(const ossimIrect &imageRect)
      {theValidImageRect = imageRect;}
   void setRawImageRect(const ossimIrect &imageRect)
      {theRawImageRect=imageRect;}
   void setSubImageOffset(const ossimIpt& d) { theSubImageOffset = d; }
   
   ossim_uint32 bytesPerPixel(ossimScalarType type) const;
   ossim_uint32 bytesPerPixel() const;
   
   ossimByteOrder getImageDataByteOrder() const
      { return theImageDataByteOrder; }

   void setImageDataByteOrder(ossimByteOrder byteOrder);
   /*!
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

private:
  ossimImageMetaData           theMetaData;
   std::vector<ossimFilename>  theImageFileList;
   ossimInterleaveType         theInterleaveType;
   ossim_uint32                theNumberOfBands;

   /*!
    *  The zero based rectangle of the entire image not including any header.
    */
   ossimIrect theRawImageRect;

   /*!
    *  The rectangle representing where the valid part of the image lies
    *  within "theRawImageRect".
    */
   ossimIrect theValidImageRect;

   /*!
    *  The valid image rectangle normalized to be zero based.
    */
   ossimIrect theImageRect;

   /*!
    *  The offset from the full-image origin to this image's origin
    */
   ossimIpt theSubImageOffset;

   /*!
    *  Size of header in bytes.
    */
   ossim_uint32 theHeaderSize;

   /*!
    *  Mode of how to handle pixels on import.
    *   0 = Do nothing the pixels.
    *   1 = Set any zeroes to min value of radiometry.
    *   2 = Set zeroes on line edges only to nulls; any other zeroes are
    *       set to min value of radiometry.
    */
   ossimFillMode theSetNullsMode;

   /*!
    *  Amount of pixels to chop for each line edge.
    */
   ossim_uint32 thePixelsToChop;

   /*!
    *  The byte order the image data is stored in.
    */
  ossimByteOrder theImageDataByteOrder;
};


//***
// Inlines
//***
inline ossim_uint64 ossimGeneralRasterInfo::validLines() const
{
   return static_cast<ossim_uint64>(theValidImageRect.height());
}

inline ossim_uint64 ossimGeneralRasterInfo::rawLines() const
{
   return static_cast<ossim_uint64>(theRawImageRect.height());
}

inline ossim_uint64 ossimGeneralRasterInfo::bytesPerRawLine() const
{
   if (interleaveType() == OSSIM_BIP)
   {
      return rawSamples() * bytesPerPixel() * numberOfBands();
   }
   else
   {
      return rawSamples() * bytesPerPixel();
   }
}

inline ossim_uint64 ossimGeneralRasterInfo::bytesPerValidLine() const
{
   if (interleaveType() == OSSIM_BIP)
   {
      return validSamples() * bytesPerPixel() * numberOfBands();
   }
   else
   {
      return validSamples() * bytesPerPixel();
   }
}

inline ossim_uint64 ossimGeneralRasterInfo::validSamples() const
{
   return static_cast<ossim_uint64>(theValidImageRect.width());
}

inline ossim_uint64 ossimGeneralRasterInfo::rawSamples() const
{
   return static_cast<ossim_uint64>(theRawImageRect.width());
}

inline ossim_uint32 ossimGeneralRasterInfo::bytesPerPixel() const
{
   return bytesPerPixel((ossimScalarType)theMetaData.getScalarType());
}

inline ossim_uint64 ossimGeneralRasterInfo::offsetToFirstValidSample() const
{
   if (interleaveType() == OSSIM_BIP)
   {
      return headerSize() +
         ( bytesPerRawLine() * validImageRect().ul().y ) +
         ( validImageRect().ul().x * numberOfBands() * bytesPerPixel() );
   }
   else if (interleaveType() == OSSIM_BIL)
   {
      return headerSize() +
         ( bytesPerRawLine() * validImageRect().ul().y * numberOfBands() ) +
         ( validImageRect().ul().x * bytesPerPixel());
   }
   else // BSQ
   {
      return headerSize() +
         ( bytesPerRawLine() * validImageRect().ul().y ) +
         ( validImageRect().ul().x * bytesPerPixel() );
   }
}

#endif
