//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Oscar Kramer
//
// Description:
//
// Contains class definition for ossimBitMaskTileSource.
//*******************************************************************
//  $Id: ossimBitMaskTileSource.cpp 2644 2011-05-26 15:20:11Z oscar.kramer $

#include <math.h>

#include <ossim/imaging/ossimBitMaskTileSource.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimInterleaveTypeLut.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimBitMaskWriter.h>

RTTI_DEF1_INST(ossimBitMaskTileSource, "ossimBitMaskTileSource", ossimImageHandler);

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimBitMaskTileSource::ossimBitMaskTileSource()
   :
   ossimImageHandler(),
   m_tile(NULL)
{
}

//*************************************************************************************************
// Destructor must free the buffers for each res level
//*************************************************************************************************
ossimBitMaskTileSource::~ossimBitMaskTileSource()
{
   close();
}

//*************************************************************************************************
// Reset
//*************************************************************************************************
void ossimBitMaskTileSource::close()
{
   ossimImageHandler::close();  // base class

   // Wipe the mask buffers:
   vector<ossim_uint8*>::iterator iter = m_buffers.begin();
   while (iter != m_buffers.end())
   {
      delete [] (*iter);
      iter++;
   }
   m_buffers.clear();
   m_bufferSizes.clear();
   m_tile = NULL;
}

//*************************************************************************************************
// Opens an OSSIM Bit Mask raster file. This is a custom formatted file with header info
//**************************************************************************************************
bool ossimBitMaskTileSource::open()
{
   static const char MODULE[] = "ossimBitMaskTileSource::open()";

   if (!isSupportedExtension())
   {
      close();
      return false;
   }

   close();

   // Open the mask file and verify magic number, read header info as well and verify good values:
   std::ifstream fileStream(theImageFile.chars(), ios::in|ios::binary);
   if (!fileStream.good())
      return false;
   ossimString magic_number;
   int num_res_levels = 0;
   char space;
   fileStream >> magic_number;
   if (magic_number == ossimBitMaskWriter::MASK_FILE_MAGIC_NUMBER)
   {
      // This is a next-generation mask file with header:
      fileStream >> theStartingResLevel >> num_res_levels;
      for (int r=0; r<num_res_levels; r++)
      {
         ossimIpt buffer_size;
         fileStream >> buffer_size.x >> buffer_size.y;
         m_bufferSizes.push_back(buffer_size);
      }
      fileStream >> space;
   }
   else
   {
      // This is the old headerless "Ming mask" format. Need to query the source image for image size.
      // This format implies a starting res level of 1:
      if (!m_handler.valid())
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << MODULE << " ERROR:\nInput file <"<<theImageFile<<"> has no header information and "
            "no valid source image has been provided. Cannot process mask" << std::endl;
         fileStream.close();
         close();
         return false;
      }
      theStartingResLevel = 1;
      num_res_levels = m_handler->getNumberOfDecimationLevels() - 1;
      fileStream.seekg(0);

      // Need to compute buffer sizes since they are not available in the mask header:
      ossimIpt image_size = m_handler->getBoundingRect(1).size();
      ossimIpt buffer_size ((image_size.x+7)/8, image_size.y);
      for (int r=0; r<num_res_levels; r++)
      {
          m_bufferSizes.push_back(buffer_size);
          buffer_size.x = (buffer_size.x+1)/2;
          buffer_size.y = (buffer_size.y+1)/2;
      }
   }

   if (m_bufferSizes.empty())
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << MODULE << " ERROR:\nInput file <"<<theImageFile<<"> is not recognized or has invalid "
         "header information." << std::endl;
      fileStream.close();
      close();
      return false;
   }

   // Good to go, Need to now establish sizes for and offsets to each res level:
   for (int r=0; r<num_res_levels; ++r)
   {
      // Allocate buffer for this res level:
      ossim_uint32 size = m_bufferSizes[r].x * m_bufferSizes[r].y; 
      ossim_uint8* buffer = new ossim_uint8[size];
      m_buffers.push_back((ossim_uint8*) buffer);

      // Read from disk:
      fileStream.read((char*)buffer, size);
      if (fileStream.fail())
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << MODULE << " ERROR:"
            << "\nError encountered reading mask file <" << theImageFile << ">"<<std::endl;
         fileStream.close();
         close();
         return false;
      }
   }

   fileStream.close();

   // Allocate the mask tile. This will be 8-bits per pixel (i.e., decompressed):
   m_tile = ossimImageDataFactory::instance()->create(this, this);
   m_tile->initialize();

   // Computes decimation factors for additional reduced res sets:
   establishDecimationFactors();

   return true;
}

//**************************************************************************************************
// "Open" here means only that the mask buffers are valid and ready for access
//**************************************************************************************************
bool ossimBitMaskTileSource::isOpen() const
{
   return ((m_buffers.size() != 0) || (m_maskWriter.valid()));
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimBitMaskTileSource::isSupportedExtension()
{
   ossimString ext = theImageFile.ext();
   if (ext == "mask")
      return true;

   return false;
}

//*************************************************************************************************
// Unpacks the mask data into a destination tile
//*************************************************************************************************
ossimRefPtr<ossimImageData> ossimBitMaskTileSource::getTile(const ossimIrect& tile_rect, 
                                                            ossim_uint32 resLevel)
{
   static const ossim_uint8 MASK_BITS[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

   // Allocate the mask tile. This will be 8-bits per pixel (i.e., decompressed):
   if (!m_tile.valid())
   {
      m_tile = ossimImageDataFactory::instance()->create(this, this);
      m_tile->initialize();
   }

   // Initial screening:
   if (!(isOpen() && isSourceEnabled() && isValidRLevel(resLevel)))
      return NULL;

   // Check if requested rect falls inside valid mask area:
   if (!getImageRectangle(resLevel).intersects(tile_rect))
      return NULL;

   // Check if tile size matches requested size:
   if ((m_tile->getWidth() != tile_rect.width()) || (m_tile->getHeight() != tile_rect.height()))
   {
      m_tile->setWidthHeight(tile_rect.width(), tile_rect.height());
      m_tile->initialize();
   }
   m_tile->setImageRectangle(tile_rect);

   // Start with blank tile if rect partially intersects defined mask area:
   ossimIrect image_rect (getImageRectangle(resLevel));
   if (!tile_rect.completely_within(image_rect))
      m_tile->makeBlank();
   
   //##### START DEBUG CODE #########################
   // This else if identifies unassigned mask pixels being returned by this method incorrectly.
   // See ossimMaskFilter::executeMaskFilterSelection() (OLK 2/11)
   //m_tile->fill(128.0);//###
   //##### END DEBUG CODE #########################

   ossimIpt image_size (image_rect.size());
   ossim_uint32 num_mask_cols = (image_size.x+7)/8; 
   ossim_uint8* tilebuf = (ossim_uint8*) m_tile->getBuf(0);
   ossim_uint32 tile_index = 0, mask_index = 0, start_bit=0;
   ossimIpt ul (tile_rect.ul());
   ossimIpt lr (tile_rect.lr());

   // Decide if we use our own mask buffer (read from file) or borrow tha active mask writer's
   // buffer:
   ossim_uint8* maskbuf;
   if (m_maskWriter.valid())
      maskbuf = m_maskWriter->m_buffers[resLevel-theStartingResLevel];
   else
      maskbuf = m_buffers[resLevel-theStartingResLevel];

   // Nested loop to fill tile:
   for (int y=ul.y; (y<=lr.y)&&(y<image_size.y); y++)
   {
      mask_index = y*num_mask_cols + ul.x/8;
      start_bit = ul.x % 8; // may not start on even mask byte boundary
      for (int x=ul.x; x<=lr.x;)
      {
         if (x < image_size.x)
         {
            // fetch mask pixel and isolate bit of interest:
            for (ossim_uint32 mask_bit=start_bit; mask_bit<8; ++mask_bit)
            {
               if (maskbuf[mask_index] & MASK_BITS[mask_bit])
                  tilebuf[tile_index++] = 255;
               else
                  tilebuf[tile_index++] = 0;

               // Advance the pixel column and check for end of image rect:
               ++x;
               if ((x >= image_size.x) || (x>lr.x))
                  break;
            }

            ++mask_index; // advance the mask buffer index 1 after processing 8 tile samples
            start_bit = 0;
         }
         else
         {
            // Tile buffer extends beyond valid mask rect, so assume it is masked (0):
            tilebuf[tile_index++] = 0;
            ++x;
         }
      }
   }
   
   m_tile->validate();
   return m_tile;
}

//*******************************************************************
// Argument res_Level is relative to the full image
//*******************************************************************
bool ossimBitMaskTileSource::isValidRLevel(ossim_uint32 res_Level) const
{
   ossim_uint32 num_rlevels;
   if (m_maskWriter.valid())
      num_rlevels = (ossim_uint32) m_maskWriter->m_buffers.size();
   else
      num_rlevels = (ossim_uint32) m_buffers.size();

   return ((res_Level >= theStartingResLevel) && 
           ((res_Level-theStartingResLevel) < num_rlevels));
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimBitMaskTileSource::getNumberOfLines(ossim_uint32 res_Level) const
{
   if (!isValidRLevel(res_Level))
      return 0;

   if (m_maskWriter.valid())
      return m_maskWriter->m_bufferSizes[res_Level-theStartingResLevel].y;

   return m_bufferSizes[res_Level-theStartingResLevel].y;
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimBitMaskTileSource::getNumberOfSamples(ossim_uint32 res_Level) const
{
   if (!isValidRLevel(res_Level))
      return 0;

   ossim_uint32 n = 0;
   if (m_maskWriter.valid())
      n = m_maskWriter->m_bufferSizes[res_Level-theStartingResLevel].x;
   else
      n = m_bufferSizes[res_Level-theStartingResLevel].x;

   return n*8; // 8 pixels represented in one mask byte entry
}

//*******************************************************************
//! Overrides base class implementation.
//*******************************************************************
ossim_uint32 ossimBitMaskTileSource::getNumberOfDecimationLevels() const
{
   return (ossim_uint32) theDecimationFactors.size();
}

//*******************************************************************
//! Overrides base class implementation.
//*******************************************************************
void ossimBitMaskTileSource::establishDecimationFactors()
{
   theDecimationFactors.clear();
   ossim_uint32 num_res_levels = (ossim_uint32) m_buffers.size();
   ossimDpt decimation(1, 1);
   for (ossim_uint32 r=0; r<num_res_levels; r++)
   {
      theDecimationFactors.push_back(decimation);
      decimation = decimation/2.0;
   }
}

//*************************************************************************************************
//! This class can be used during overview generation, in which case there will be a simultaneous
//! mask writing. Instead of accessing a mask file (which may not yet exist), the mask buffer
//! of the writer will be accessed directly instead of our own buffer.
//*************************************************************************************************
void ossimBitMaskTileSource::setAssociatedMaskWriter(ossimBitMaskWriter* maskWriter)
{ 
   m_maskWriter = maskWriter; 
   if (m_maskWriter.valid())
      theStartingResLevel = m_maskWriter->m_startingResLevel;
}

//*************************************************************************************************
//! For backward compatibility with older mask format ("Ming Mask"), provide for associating
//! the mask with an image handler:
//*************************************************************************************************
void ossimBitMaskTileSource::setAssociatedImageHandler(ossimImageHandler* handler)
{
   m_handler = handler;
}

