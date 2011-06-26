//*************************************************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Oscar Kramer
//
//*************************************************************************************************
//  $Id: ossimBitMaskWriter.h 2644 2011-05-26 15:20:11Z oscar.kramer $

#ifndef ossimBitMaskWriter_HEADER
#define ossimBitMaskWriter_HEADER

#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimOutputSource.h>
#include <ossim/imaging/ossimPixelFlipper.h>
#include <vector>

class ossimFilename;
class ossimKeywordlist;
class ossimImageHandler;
class ossimBitMaskTileSource;

//*************************************************************************************************
//! 
//! Class for computing a mask from an input image source and writing the mask file to disk.
//!
//! This class takes care of all details associated with computing, writing and reading the custom,
//! bit-compressed OSSIM bit mask file format. 
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
//! The mask is computed here using the specified values for bogus pixels. If a pixel in the source
//! image is in the range of bogus values, a mask of 0 is assigned to that pixel, otherwise 1. 
//!
//*************************************************************************************************
class OSSIM_DLL ossimBitMaskWriter : public ossimOutputSource
{
public:
   static const char* BM_STARTING_RLEVEL_KW;
   static const char* MASK_FILE_MAGIC_NUMBER;

   //! Default constructor typically used when reading a mask from disk.
   ossimBitMaskWriter();

   ~ossimBitMaskWriter();

   //! Sets the NULL pixel value to consider when computing mask:
   void setBogusPixel(double pixel_value);

   //! Sets the range of pixels (inclusive) to be regarded as NULL pixels when computing mask
   void setBogusPixelRange(double min, double max);

   //! Sets the starting resolution level for the mask.
   void setStartingResLevel(ossim_uint32 res_level) { m_startingResLevel = res_level; }

   //! Given a source's tile, derives the alpha mask and saves it in buffer for later writing to 
   //! disk.
   void generateMask(ossimRefPtr<ossimImageData> tile, ossim_uint32 rLevel);

   //! For imagery that already has overviews built, but with artifact edge pixels (such as JP2-
   //! compressed data), it is preferred to build the mask overviews directly from the R0 mask.
   //! This method will build the remaining number of R-levels from the last defined mask buffer
   //! such that the total number of R-levels (counting R0) are represented. Returns TRUE if successful.
   bool buildOverviews(ossim_uint32 total_num_rlevels);

   //! Computes and writes the mask file according to the specification in the KWL.
   //! Returns TRUE if successful.
   virtual bool loadState(const ossimKeywordlist& spec, const char* prefix=0);

   virtual bool isOpen() const;
   virtual bool open();
   virtual void close();

   virtual bool canConnectMyInputTo(ossim_int32 myInputIndex, 
                                    const ossimConnectableObject* object) const;
   virtual ossim_int32 connectMyInputTo (ossimConnectableObject *inputObject, 
                                         bool makeOutputConnection=true, 
                                         bool createEventFlag=true);

protected:
   friend class ossimBitMaskTileSource;

   //! Initializes the flipper (used for identifying pixels for masking) to default values
   void initializeFlipper();

   //! Deletes allocated buffers and resets all values to defaults.
   void reset();

   //! Since overviews may not yet exist when the mask is being written, we must compute the
   //! size of the source image based on the original R0 image size.
   ossimIpt computeImageSize(ossim_uint32 rlevel, ossimImageData* tile) const;

   ossimRefPtr<ossimPixelFlipper>  m_flipper;
   vector<ossim_uint8*>            m_buffers;
   vector<ossimIpt>                m_bufferSizes;
   ossim_uint32                    m_startingResLevel;
   ossimIpt                        m_imageSize; //!< Size of full res source image
};

#endif

