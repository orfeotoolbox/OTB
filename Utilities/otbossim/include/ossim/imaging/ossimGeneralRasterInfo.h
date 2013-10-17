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
//  $Id: ossimGeneralRasterInfo.h 22197 2013-03-12 02:00:55Z dburken $

#ifndef ossimGeneralRasterInfo_HEADER
#define ossimGeneralRasterInfo_HEADER 1

#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/imaging/ossimImageMetaData.h>
#include <vector>

//---
// Forward class declarations.
//---
class ossimEnviHeader;
class ossimKeywordlist;

/**
 * @class ossimGeneralRasterInfo
 *
 */
class OSSIMDLLEXPORT ossimGeneralRasterInfo : public ossimObject
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

   /* @brief default constructor */
   ossimGeneralRasterInfo();
   
   ossimGeneralRasterInfo(const std::vector<ossimFilename>& imageFileList,
                          ossimScalarType      pixelType,
                          ossimInterleaveType  il_type,
                          ossim_int32          numberOfBands,
                          ossim_int32          lines,
                          ossim_int32          samples,
                          ossim_int32          headerSize,
                          ossimFillMode        nullsMode,
                          ossim_int32          pixelsToChop);
   
   ossimGeneralRasterInfo(const ossimKeywordlist& kwl, const char* prefix=0);

   /* @brief copy constructor */
   ossimGeneralRasterInfo( const ossimGeneralRasterInfo& obj );

   /** @brief Assignment operator=. */
   const ossimGeneralRasterInfo& operator=( const ossimGeneralRasterInfo& rhs );
   
   virtual ~ossimGeneralRasterInfo ();

   /**
    *  Zero based rectangle of the valid image.
    */
   const ossimIrect& imageRect() const;

   /**
    *  Returns the rectangle of where the image is within the raster file.
    *  In most cases this will be the same as the rawImageRect. (zero based)
    */
   const ossimIrect& validImageRect() const;

   /**
    *  Zero based rectangle of the raw image.
    */
   const ossimIrect& rawImageRect() const;

   /**
    * Returns the offset of this image's origin to a full-image's origin, in
    * the case where this image is a sub-image of a larger full-image.
    */
   const ossimIpt& subImageOffset() const;

   /**
    *  Returns the size of header in bytes.
    */
   ossim_int32 headerSize() const;

   /**
    *  Returns the fill mode.  This is used to flip values on ingest.
    *  Valid Mode Enumerations:
    *  - 0  Do nothing to the pixels on ingest.
    *  - 1  Flip all zeroes to the minimum pixel value.
    *  - 2  Flip zeroes within the image line to minimum pixel value.
    *       This is used by edge walking code.
    */
   ossim_uint32 fillToNullsMode() const;

   /**
    *  Sets theSetNullsMode.
    */
   void setFillToNullsMode(ossim_uint32 mode);

   /**
    * @brief Number of bands. Inlined below.
    * @return Number of bands from theMetaData class member cast to an int32.
    */
   inline ossim_int32 numberOfBands() const;
   
   /**
    * @brief Bytes per pixel.  Inlined below.
    * @return Bytes per pixel from theMetaData class member cast to an int32.
    */   
   inline ossim_int32 bytesPerPixel() const;
   
   /**
    *  Returns the number of pixels from the edge of a line
    *  to set to the fill value.  This can be used to eliminate
    *  resampling error on the edge of an image and still keep
    *  the image demensions relative to the geometry file.
    */
   ossim_int32 pixelsToChop() const;
   
   /**
    *  Enumerated in InterleaveTypeLUT. Should be either bip,
    *  bil or bsq.  Defaulted to bip.
    */
   ossimInterleaveType interleaveType() const;

   /**
    *  Returns the number of lines within "theValidImageRect".
    */
   inline ossim_int32 validLines() const;

   /**
    *  Returns the number of lines within "theRawImageRect".
    */
   inline ossim_int32 rawLines() const;

   /**
    *  Returns the number of bytes in a raw line.
    */
   inline ossim_int32 bytesPerRawLine() const;

   /**
    *  Returns the number of bytes in a valid line.
    */
   inline ossim_int32 bytesPerValidLine() const;

   /**
    *  Returns the number of samples within "theValidImageRect".
    */
   inline ossim_int32 validSamples() const;

   /**
    *  Returns the number of samples within "theRawImageRect".
    */
   inline ossim_int32 rawSamples() const;

   /**
    *  Returns the offset in bytes to the first valid sample in the image.
    */
   inline std::streamoff offsetToFirstValidSample() const;
   
   const std::vector<ossimFilename>& getImageFileList() const;

   void setImageFileList(const std::vector<ossimFilename>& list);

   /**
    * @brief Sets the image file list to file.  Performs a clear, prior to
    * setting.
    * @param file Image file.
    */
   void setImageFile(const ossimFilename& file);

   void setHeaderSize(ossim_int32 headerSize);
   
   void setInterleaveType(ossimInterleaveType il_type);

   virtual std::ostream& print(std::ostream& out) const;

   void setImageRect(const ossimIrect& imageRect);
   
   void setValidImageRect(const ossimIrect &imageRect);

   void setRawImageRect(const ossimIrect &imageRect);

   void setSubImageOffset(const ossimIpt& d);

   ossimByteOrder getImageDataByteOrder() const;

   void setImageDataByteOrder(ossimByteOrder byteOrder);
   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /**
    * @brief Takes image file and attempts to derive/find header file to
    * parse for general raster data, i.e. lines, samples, bands and so on.
    * @param image file.
    * @return true on success, false on error.
    */
   bool open( const ossimFilename& imageFile );

   /** @return Const reference to theMetaData class member. */
   inline const ossimImageMetaData& getImageMetaData() const;

   /** @return Reference to theMetaData class member. */
   inline ossimImageMetaData& getImageMetaData();

   /**
    * @brief Initialize from envi header file.
    * @param enviHeader
    * @return true on success, false on error.
    */  
   bool initializeFromEnviHdr( const ossimEnviHeader& enviHdr );

private:

   /**
    * @brief Initialize from header file.
    * @param headerFile
    * @return true on success, false on error.
    */
   bool initializeFromHdr( const ossimFilename& imageFile,
                           const ossimFilename& headerFile );

   /**
    * @brief Initialize from envi header file.
    * @param headerFile
    * @return true on success, false on error.
    */  
   bool initializeFromEnviHdr( const ossimFilename& headerFile );

   /**
    * @brief Initialize from omd file.
    * @param file
    * @return true on success, false on error.
    */  
   bool initializeFromXml( const ossimFilename& imageFile,
                           const ossimFilename& headerFile );

   /**
    * @brief Private method to get bits per pixel from file size,
    * lines, samples and bands.  Method created to avoid duplicate code
    * in "initizeFrom" methods.
    *
    * This assumes that "theRawImageRect" and "theNumberOfBands" has been
    * set.
    *
    * @param imageFile This image file.
    * @param Calculated bits per pixel.
    * @return bits per pixel or 0 on error.
    */
   ossim_int32 getBitsPerPixel( const ossimFilename& imageFile ) const;

   /** @brief Clears data members. */
   void clear();
   
   ossimImageMetaData          theMetaData;
   std::vector<ossimFilename>  theImageFileList;
   ossimInterleaveType         theInterleaveType;

   /**
    *  The zero based rectangle of the entire image not including any header.
    */
   ossimIrect theRawImageRect;

   /**
    *  The rectangle representing where the valid part of the image lies
    *  within "theRawImageRect".
    */
   ossimIrect theValidImageRect;

   /**
    *  The valid image rectangle normalized to be zero based.
    */
   ossimIrect theImageRect;

   /**
    *  The offset from the full-image origin to this image's origin
    */
   ossimIpt theSubImageOffset;

   /**
    *  Size of header in bytes.
    */
   ossim_int32 theHeaderSize;

   /**
    *  Mode of how to handle pixels on import.
    *   0 = Do nothing the pixels.
    *   1 = Set any zeroes to min value of radiometry.
    *   2 = Set zeroes on line edges only to nulls; any other zeroes are
    *       set to min value of radiometry.
    */
   ossimFillMode theSetNullsMode;

   /**
    *  Amount of pixels to chop for each line edge.
    */
   ossim_int32 thePixelsToChop;

   /**
    *  The byte order the image data is stored in.
    */
  ossimByteOrder theImageDataByteOrder;
};


//---
// Inlines:
// 
// These methods are used throughout raster read/write code; hence, inlines.
// Note that most of these are used in calculating std::streamoff which is
// a signed 64 bit integer on most machines; hence, the cast to an int from
// an unsigned int for convenience.
//---
inline const ossimImageMetaData& ossimGeneralRasterInfo::getImageMetaData() const
{
   return theMetaData;
}

inline ossimImageMetaData& ossimGeneralRasterInfo::getImageMetaData()
{
   return theMetaData;
}

inline ossim_int32 ossimGeneralRasterInfo::numberOfBands() const
{
   return static_cast<ossim_int32>( theMetaData.getNumberOfBands() );
}

inline ossim_int32 ossimGeneralRasterInfo::bytesPerPixel() const
{
   return static_cast<ossim_int32>( theMetaData.getBytesPerPixel() );
}

inline ossim_int32 ossimGeneralRasterInfo::validLines() const
{
   return static_cast<ossim_int32>(theValidImageRect.height());
}

inline ossim_int32 ossimGeneralRasterInfo::rawLines() const
{
   return static_cast<ossim_int32>(theRawImageRect.height());
}

inline ossim_int32 ossimGeneralRasterInfo::bytesPerRawLine() const
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

inline ossim_int32 ossimGeneralRasterInfo::bytesPerValidLine() const
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

inline ossim_int32 ossimGeneralRasterInfo::validSamples() const
{
   return static_cast<ossim_int32>(theValidImageRect.width());
}

inline ossim_int32 ossimGeneralRasterInfo::rawSamples() const
{
   return static_cast<ossim_int32>(theRawImageRect.width());
}

inline std::streamoff ossimGeneralRasterInfo::offsetToFirstValidSample() const
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
         ( validImageRect().ul().x * bytesPerPixel() );
   }
   else // BSQ
   {
      return headerSize() +
         ( bytesPerRawLine() * validImageRect().ul().y ) +
         ( validImageRect().ul().x * bytesPerPixel() );
   }
}

#endif
