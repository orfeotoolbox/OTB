//*************************************************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Oscar Kramer
//
// Description: Contains implementation of class for computing a mask from an input image.
//   The mask is stored in a compressed format where one byte represents 8 bit-masks for masking
//   8 image pixels. 
//*************************************************************************************************
//  $Id: ossimBitMaskWriter.cpp 3081 2011-12-22 16:34:12Z oscar.kramer $

#include <ossim/imaging/ossimBitMaskWriter.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimVisitor.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageHandler.h>

const char* ossimBitMaskWriter::MASK_FILE_MAGIC_NUMBER = "OSSIM_BIT_MASK";
const char* ossimBitMaskWriter::BM_STARTING_RLEVEL_KW = "starting_rlevel";

//*************************************************************************************************
// Constructor accepts handler to the source imagery being used to compute the mask.
//*************************************************************************************************
ossimBitMaskWriter::ossimBitMaskWriter()
   :
   m_flipper(0),
   m_startingResLevel(0)
{
}

//*************************************************************************************************
// Destructor
//*************************************************************************************************
ossimBitMaskWriter::~ossimBitMaskWriter()
{
   // Make sure the mask file was written out before disappearing:
   if (isOpen())
      close();
}

//*************************************************************************************************
// Returns true if buffers allocated:
//*************************************************************************************************
bool ossimBitMaskWriter::isOpen() const
{
   return (m_buffers.size() != 0);
}

//*************************************************************************************************
// Returns true if buffers allocated:
//*************************************************************************************************
bool ossimBitMaskWriter::open()
{
   // Nothing to do yet. Eventually want to open the output stream here and write the buffer after
   // each R-level is completed, so we don't need to keep vector of buffers in memory.
   return true; 
}

//*************************************************************************************************
// Reset
//*************************************************************************************************
void ossimBitMaskWriter::reset()
{
   // Wipe the mask buffers:
   vector<ossim_uint8*>::iterator iter = m_buffers.begin();
   while (iter != m_buffers.end())
   {
      delete [] (*iter);
      iter++;
   }
   m_buffers.clear();
   m_bufferSizes.clear();
}

//*************************************************************************************************
//! Sets the NULL pixel value to consider when computing mask:
//*************************************************************************************************
void ossimBitMaskWriter::setBogusPixel(double pixel_value)
{
   if (!m_flipper.valid())
      initializeFlipper();

   m_flipper->setTargetValue(pixel_value);
}

//*************************************************************************************************
//! Sets the range of pixels (inclusive) to be regarded as NULL pixels when computing mask
//*************************************************************************************************
void ossimBitMaskWriter::setBogusPixelRange(double min, double max)
{
   if (!m_flipper.valid())
      initializeFlipper();

   m_flipper->setTargetRange(min, max);
}

//*************************************************************************************************
// Given a source's tile, derives the alpha mask and saves it in buffer for later writing to disk.
//*************************************************************************************************
void ossimBitMaskWriter::generateMask(ossimRefPtr<ossimImageData> tile, ossim_uint32 rLevel)
{
   static const ossim_uint8 MASK_BITS_0[] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};
   static const ossim_uint8 MASK_BITS_1[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

   // We don't start doing anything until starting res or higher requested:
   ossim_uint32 mask_rlevel = rLevel - m_startingResLevel;
   if (!tile.valid())
      return;

   // We should have had this done by now, but just in case:
   if (!m_flipper.valid())
      initializeFlipper();

   // The flipper is used to identify null pixels since it has more sophisticated filtering 
   // than available from the tile object:
   switch (tile->getScalarType())
   {
   case OSSIM_UCHAR:
      m_flipper->flipPixels(ossim_uint8(0), tile.get(), rLevel);
      break;
   case OSSIM_UINT16:
   case OSSIM_USHORT11:
      m_flipper->flipPixels(ossim_uint16(0), tile.get(), rLevel);
      break;
   case OSSIM_SSHORT16:
      m_flipper->flipPixels(ossim_sint16(0), tile.get(), rLevel);
      break;
   case OSSIM_UINT32:
      m_flipper->flipPixels(ossim_uint32(0), tile.get(), rLevel);
      break;
   case OSSIM_SINT32:
      m_flipper->flipPixels(ossim_sint32(0), tile.get(), rLevel);
      break;
   case OSSIM_FLOAT32:
   case OSSIM_NORMALIZED_FLOAT:
      m_flipper->flipPixels(float(0), tile.get(), rLevel);
      break;
   case OSSIM_NORMALIZED_DOUBLE:
   case OSSIM_FLOAT64:
      m_flipper->flipPixels(ossim_float64(0), tile.get(), rLevel);
      break;
   case OSSIM_SCALAR_UNKNOWN:
   default:
      ossimNotify(ossimNotifyLevel_WARN) << "ossimBitMaskWriter::generateMask()-- "
         "Unsupported scalar type!" << endl;
      break;
   }

   ossimIpt image_size = computeImageSize(rLevel, tile.get());
   ossim_uint32 num_mask_cols = (image_size.x+7)/8; // size of mask buffer after compression
   ossim_uint32 num_mask_rows = image_size.y;
   ossim_uint8* maskbuf = 0;

   // Check if mask buffer for this R-level has already been allocated:
   if (m_buffers.size() <= mask_rlevel)
   {
      ossim_uint32 size_of_maskbuf = num_mask_rows * num_mask_cols;
      maskbuf = new ossim_uint8[size_of_maskbuf];
      memset(maskbuf, 0, size_of_maskbuf);
      m_buffers.push_back(maskbuf);
      m_bufferSizes.push_back(ossimIpt(num_mask_cols, num_mask_rows));
   }
   else
      maskbuf = m_buffers[mask_rlevel];

   ossim_uint32 mask_index=0, tile_index=0, start_bit=0;
   ossimIrect tile_rect (tile->getImageRectangle());
   ossimIpt ul (tile_rect.ul());
   ossimIpt lr (tile_rect.lr());

   // Scan each pixel in the source tile and decide on mask value:
   for (int y=ul.y; (y<=lr.y)&&(y<image_size.y); y++)
   {
      mask_index = y*num_mask_cols + ul.x/8;
      start_bit = ul.x % 8; // may not start on even mask byte boundary

      for (int x=ul.x; (x<=lr.x); /* incremented in bit loop below */ )         
      {
         if ( x < image_size.x )
         {
            // Process 8 samples and pack resultant mask into one byte:
            maskbuf[mask_index] = 0;
            for (ossim_uint32 mask_bit=start_bit; mask_bit<8; ++mask_bit)
            {
               // Decide whether to mask depending on pixel flipper outputting a NULL pixel. 
               // if (tile->isNull(tile_index++))
               if (tile->isNull(tile_index++))                  
                  maskbuf[mask_index] &=  MASK_BITS_0[mask_bit];
               else
                  maskbuf[mask_index] |= MASK_BITS_1[mask_bit];
               
               // Advance the pixel column and check for end of image rect:
               ++x;
               if ((x >= image_size.x) || (x > lr.x))
                  break;
            }
            ++mask_index; // advance the mask buffer index 1 after processing 8 tile samples
            start_bit = 0;
         }
         else
         {
            ++x;
            
            ++tile_index;
         }
      }
   } // Finished looping over all pixels in input tile

   return;
}

//*************************************************************************************************
//! Writes the mask file to path specified. Returns TRUE if successful.
//*************************************************************************************************
void ossimBitMaskWriter::close()
{
   static const char* MODULE = "ossimBitMaskWriter::writeMask()";
   static const ossimString MASK_EXTENSION ("mask");

   // Open the output file. Use default name if none provided.
   if (theOutputName.empty()) // Couldn't figure out the name
   {
      ossimNotify(ossimNotifyLevel_WARN)<<MODULE<<" -- Error encountered trying to create mask file" 
         " for writing because output file name was never initialized.";
      return;
   }

   ofstream maskFileStream (theOutputName.chars(), ios::out|ios::binary);
   if (!maskFileStream.is_open())
   {
      ossimNotify(ossimNotifyLevel_WARN)<<MODULE<<" -- Error encountered trying to create mask file" 
         "<"<<theOutputName<<"> for writing. Cannot write mask.";
      return;
   }

   // Write the header info:
   ossim_uint32 num_rlevels = (ossim_uint32) m_buffers.size();
   maskFileStream << MASK_FILE_MAGIC_NUMBER <<" "<< m_startingResLevel <<" "<< num_rlevels<<" ";
   for (ossim_uint32 r=0; r<num_rlevels; r++)
      maskFileStream << m_bufferSizes[r].x <<" "<< m_bufferSizes[r].y <<" "; 
   maskFileStream << ends;

   // Loop over each res level and write buffers to disk:
   for (ossim_uint32 rlevel = 0; rlevel<num_rlevels; ++rlevel)
   {
      ossim_uint32 bufsize = m_bufferSizes[rlevel].x * m_bufferSizes[rlevel].y;
      maskFileStream.write((char*)(m_buffers[rlevel]), bufsize);
   }

   maskFileStream.close();
   reset();

   return;
}

//*************************************************************************************************
//! Constructor accepts keywordlist with all info necessary to compute a mask. The caller will
//! still need to call writeMask().
//*************************************************************************************************
bool ossimBitMaskWriter::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   // static const char* MODULE = "ossimBitMaskWriter::writeMask(kwl)";

   initializeFlipper();

   // Fetch optional output filename:
   theOutputName = kwl.find(prefix, ossimKeywordNames::OUTPUT_FILE_KW);

   // Initialize the pixel flipper. This filter is used to identify pixels targeted for masking by
   // remapping them to the null pixel:
   m_flipper->loadState(kwl, prefix);

   // Read Starting res level:
   m_startingResLevel = 0;
   ossimString kw_value = kwl.find(prefix, BM_STARTING_RLEVEL_KW);
   if (!kw_value.empty())
      m_startingResLevel = kw_value.toUInt32();

   return true;
}

//*************************************************************************************************
// 
//*************************************************************************************************
void ossimBitMaskWriter::initializeFlipper()
{
   if (!m_flipper.valid())
   {
      m_flipper = new ossimPixelFlipper();
      m_flipper->setReplacementMode(ossimPixelFlipper::REPLACE_ONLY_FULL_TARGETS);
      m_flipper->setTargetValue(0);
      m_flipper->setReplacementValue(0);
   }

   // This method gets called when an input connection is made as well as other times, so keep an
   // eye out for valid input connection:
   if (theInputObjectList.size() != 0)
   {
      m_flipper->connectMyInputTo(theInputObjectList[0].get());
   }
}

//*************************************************************************************************
// 
//*************************************************************************************************
bool ossimBitMaskWriter::canConnectMyInputTo(ossim_int32 /*myInputIndex*/, 
                                             const ossimConnectableObject* object) const
{
   const ossimImageSource* ois = dynamic_cast<const ossimImageSource*>(object);
   return (ois != NULL);
}

//*************************************************************************************************
// 
//*************************************************************************************************
ossim_int32 ossimBitMaskWriter::connectMyInputTo (ossimConnectableObject *inputObject, 
                                                  bool makeOutputConnection, 
                                                  bool createEventFlag)
{
   ossimImageSource* input_source = dynamic_cast<ossimImageSource*>(inputObject);
   if (input_source == NULL)
      return -1;

   // Base class takes care of connection with this object:
   ossimConnectableObject::connectMyInputTo(input_source, makeOutputConnection, createEventFlag);

   // This input should be an image handler, but may need to search the objects inputs in case
   // it is chained:
   ossimImageHandler* handler = dynamic_cast<ossimImageHandler*>(input_source);
   if (handler == NULL)
   {
      // Need to search:
      ossimTypeNameVisitor visitor(ossimString("ossimImageHandler"),
                                   true,
                                   ossimVisitor::VISIT_CHILDREN|ossimVisitor::VISIT_INPUTS);
      
      input_source->accept(visitor);
      ossimRefPtr<ossimObject> obj = visitor.getObject();
      if ( obj.valid() )
      {
         handler = dynamic_cast<ossimImageHandler*>( obj.get() );
      }
   }

   // Should have a handler:
   if (!handler)
   { 
      disconnectAllInputs();
      return -1;
   }

   // A handler has been identified. Need the filename:
   ossimFilename imageFile =  handler->getFilename();
   if (imageFile.contains("ovr.tmp"))
   {
      // The handler is actually an overview, need some massaging to work since the overview may
      // not have an R0 entry and R1 rect must be requested instead:
      m_imageSize = handler->getBoundingRect(1).size() * 2; 
      imageFile = imageFile.noExtension();
   }
   else
   {
      // This is normal image handler so it is OK to request R0 rect:
      m_imageSize = handler->getBoundingRect(0).size(); 
   }
   if (theOutputName.empty())
      theOutputName = imageFile.setExtension("mask");

   // Check if the pixel flipper was initialized:
   if (m_flipper.valid())
      m_flipper->connectMyInputTo(input_source);
   else
      initializeFlipper();

   return 0;
}

//*************************************************************************************************
//! Since overviews may not yet exist when the mask is being written, we must compute the
//! size of the mask buffer based on the original R0 image size.
//*************************************************************************************************
ossimIpt ossimBitMaskWriter::computeImageSize(ossim_uint32 rlevel, ossimImageData* tile) const
{
   if (rlevel == 0)
      return m_imageSize;

   ossimIpt isize (m_imageSize);
   for (ossim_uint32 r=1; r<=rlevel; r++)
   {
      isize.x = (isize.x + 1) / 2;
      isize.y = (isize.y + 1) / 2;
   }

   // Adjust size n X direction to even mask boundary:
   ossimIpt tile_size (tile->getWidth(), tile->getHeight());
   isize.x = ((int)(isize.x+7)/8) * 8;

   return isize;
}

//*************************************************************************************************
// For imagery that already has overviews built, but with artifact edge pixels (such as JP2-
// compressed data), it is preferred to build the mask overviews directly from the R0 mask.
// This method will build the specified number of R-levels (including R0) from the last defined
// mask buffer. Returns TRUE if successful.
// disk.
//*************************************************************************************************
bool ossimBitMaskWriter::buildOverviews(ossim_uint32 total_num_rlevels)
{
   // First establish the remaining number of R-levels to generate, and the last valid R-level
   // currently in memory:
   if (m_buffers.size() == 0)
      return false;
   if (m_buffers.size() == total_num_rlevels)
      return true; // nothing to do
   ossim_uint32 ref_rlevel = m_startingResLevel + (ossim_uint32) m_buffers.size() - 1;
   ossim_uint32 ovr_rlevel = ref_rlevel + 1;

   ossimIpt ref_size (m_bufferSizes[ref_rlevel-m_startingResLevel]);
   ossim_uint8 *ref_buf = m_buffers[ref_rlevel-m_startingResLevel];
   ossim_uint32 ref_index = 0, ovr_index = 0;
   ossim_uint8 *ovr_buf = 0;
   ossim_uint32 size_of_refbuf = ref_size.x * ref_size.y;

   // Loop over all remaining res levels:
   while (ovr_rlevel != total_num_rlevels)
   {
      // Allocate the buffer at this R-level:
      ossimIpt ovr_size ((ref_size.x+1)/2, (ref_size.y+1)/2);
      ossim_uint32 size_of_ovrbuf = ovr_size.x * ovr_size.y;
      if (size_of_ovrbuf == 0)
         return false;
      ovr_buf = new ossim_uint8[size_of_ovrbuf];
      memset(ovr_buf, 0, size_of_ovrbuf);
      m_buffers.push_back(ovr_buf);
      m_bufferSizes.push_back(ovr_size);

      // Loop over each output overview pixel, considering the underlying rlevel (equivalent to
      // nearest-neighbor resampling for overview at 2X decimation):
      for (int y=0; y<ovr_size.y; y++)
      {
         ref_index = 2*y*ref_size.x;
         ovr_index = y*ovr_size.x;

         for (int x=0; x<ovr_size.x; x++)
         {
            ossim_uint8 a = 0;
            ossim_uint8 b = 0;
            if (ref_index < size_of_refbuf)
            {
               a = ref_buf[ref_index++];
               if ((x < (ovr_size.x-1)) || !(ref_size.x & 1))
                  b = ref_buf[ref_index++];
            }
            ovr_buf[ovr_index++] = ( (a & 0x80)       |
                                    ((a & 0x20) << 1) |
                                    ((a & 0x08) << 2) |
                                    ((a & 0x02) << 3) |
                                    ((b & 0x80) >> 4) |
                                    ((b & 0x20) >> 3) |
                                    ((b & 0x08) >> 2) |
                                    ((b & 0x02) >> 1));

         }
      }

      // Advance to next rlevel:
      ref_buf = ovr_buf;
      ref_size = ovr_size;
      size_of_refbuf = size_of_ovrbuf;
      ++ovr_rlevel;
   }

   return true;
}

