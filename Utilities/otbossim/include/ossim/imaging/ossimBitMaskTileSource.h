//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Oscar Kramer
//
//*******************************************************************
//  $Id: ossimBitMaskTileSource.h 2644 2011-05-26 15:20:11Z oscar.kramer $

#ifndef ossimBitMaskTileSource_HEADER
#define ossimBitMaskTileSource_HEADER

#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimBitMaskWriter.h>

//*************************************************************************************************
//! This is the image handler for providing mask pixels to an ossimMaskFilter object.
//!
//! This class takes care of reading the custom, bit-compressed OSSIM bit mask file format. 
//! 
//! When the developer needs to mask an image, the following needs to be done:
//!
//!   Creating the Mask:
//!   1. A mask file first needs to be created using ossimBitMaskWriter and a source image. The app
//!      "img2rr --create-mask" directs that program to use ossimBitMaskWriter to generate a mask  
//!      during overview generation. There is also create-bitmask app that will generate the mask.
//!   2. An OSSIM ".mask" file should exist on disk after step 0. This is typically the same base 
//!      name as the source image file being masked.
//!   
//!   Using the Mask:
//!   1. An ossimBitMaskTileSource (derived from ossimImageHandler) needs to be instantiated and
//!      the mask file name used to open it. 
//!   3. An ossimMaskFilter (derived from ossimImageCombiner) is the actual filter that combines
//!      the source image handler and the ossimBitMaskTileSource object to provide the resultant 
//!      masked tiles.
//!      
//! The OSSIM bit mask file contains a small header that consists of:
//!    magic number,
//!    starting res level,
//!    number of res levels
//!    num columns at starting res level,
//!    num rows at starting res level, 
//!    num columns at 2nd res level,
//!    num rows at 2nd res level, 
//!    etc., for all res levels
//!    <space> 
//!    <compressed binary mask data>
//!
//! The number of rows and columns represents the number of bytes in the mask, with each byte
//! containing 8 alpha mask bits representing 8 pixels in the original image.
//!
//! This class functions as an image handler, returning 255 for unmasked pixels, and 0 for masked 
//! pixels. 
//! 
//! For debug purposes, this code initializes the tile to all 128's so that the 
//! ossimMaskFilter can recognize uninitialized mask data.
//!
//*************************************************************************************************

class OSSIM_DLL ossimBitMaskTileSource : public ossimImageHandler
{
public:

   ossimBitMaskTileSource();

   //! Return a tile of masks. These are "bit masks", i.e., either 0 (hide) or 1 (show).
   //! The pixels' remaining 7 bits are cleared and should not be used.
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tile_rect, ossim_uint32 rLevel=0);
   
   virtual bool         isValidRLevel     (ossim_uint32 res_level)   const;
   virtual ossim_uint32 getNumberOfLines  (ossim_uint32 res_level=0) const;
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 res_level=0) const;

   virtual ossim_uint32    getNumberOfInputBands()  const { return 1; }
   virtual ossim_uint32    getNumberOfOutputBands() const { return 1; }
   virtual ossimScalarType getOutputScalarType()    const { return OSSIM_UINT8; }
   virtual ossim_uint32    getImageTileWidth()      const { return 0; }
   virtual ossim_uint32    getImageTileHeight()     const { return 0; }   

   virtual bool isOpen() const;
   virtual bool open();
   virtual void close();

   virtual double getNullPixelValue(ossim_uint32 /*band=0*/) const { return 0; }
   virtual double getMinPixelValue (ossim_uint32 /*band=0*/) const { return 0; }
   virtual double getMaxPixelValue (ossim_uint32 /*band=0*/) const { return 1.0; }

   //! There is no geometry associated with a mask file -- it is pixel-for-pixel mapped to
   //! an image file. The image file must have the valid geometry.
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry() { return NULL; }

   //! Overrides base class implementation.
   virtual ossim_uint32 getNumberOfDecimationLevels() const;

   //! For backward compatibility with older mask format ("Ming Mask"), provide for associating
   //! the mask with an image handler:
   void setAssociatedImageHandler(ossimImageHandler* handler);

   //! This class can be used during overview generation, in which case there will be a simultaneous
   //! mask writing. Instead of accessing a mask file (which may not yet exist), the mask buffer
   //! of the writer will be accessed directly instead of our own buffer.
   void setAssociatedMaskWriter(ossimBitMaskWriter* maskWriter);

protected:
   virtual ~ossimBitMaskTileSource();
   
   //! Returns true if this handler recognizes the file name extension as an OSSIM raster mask file.
   bool isSupportedExtension();

   virtual void establishDecimationFactors();

   ossimRefPtr<ossimImageData>  m_tile;
   vector<ossim_uint8*>         m_buffers;
   vector<ossimIpt>             m_bufferSizes;
   
   //! For backward compatibility with older mask format ("Ming Mask"), provide for associating
   //! the mask with an image handler:
   ossimRefPtr<ossimImageHandler>  m_handler;

   //! This class can be used during overview generation, in which case there will be a simultaneous
   //! mask writing. Instead of accessing a mask file (which may not yet exist), the mask buffer
   //! of the writer will be accessed directly instead of our own buffer.
   ossimRefPtr<ossimBitMaskWriter>  m_maskWriter;

   TYPE_DATA
};

#endif

