//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Contains class definition for ossimCcfTileSource.
//*******************************************************************
//  $Id: ossimCcfTileSource.cpp 17195 2010-04-23 17:32:18Z dburken $

#include <algorithm>
#include <ossim/imaging/ossimCcfTileSource.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimInterleaveTypeLut.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>

using namespace std;

RTTI_DEF1(ossimCcfTileSource, "ossimCcfTileSource", ossimImageHandler);

//***
// Define Trace flags for use within this file:
//***

static ossimTrace traceExec  ("ossimCcfTileSource:exec");
static ossimTrace traceDebug ("ossimCcfTileSource:debug");

// For interleave type enum to string conversions.
static const ossimInterleaveTypeLut ILUT;

ossimCcfTileSource::ossimCcfTileSource()
   :
      ossimImageHandler(),
      theCcfHead(),
      theTile(NULL),
      theChipBuffer(NULL),
      theFileStr(NULL),
      theOutputBandList(1),
      theByteOrder(ossim::byteOrder())
{}

ossimCcfTileSource::~ossimCcfTileSource()
{
  close();
}

ossimRefPtr<ossimImageData> ossimCcfTileSource::getTile(
   const  ossimIrect& rect, ossim_uint32 resLevel)
{
   if (theTile.valid())
   {
      // Image rectangle must be set prior to calling getTile.
      theTile->setImageRectangle(rect);
      
      if ( getTile( theTile.get(), resLevel ) == false )
      {
         if (theTile->getDataObjectStatus() != OSSIM_NULL)
         {
            theTile->makeBlank();
         }
      }
   }

   return theTile;
}

bool ossimCcfTileSource::getTile(ossimImageData* result,
                                 ossim_uint32 resLevel)
{
   bool status = false;

   //---
   // Not open, this tile source bypassed, or invalid res level,
   // return a blank tile.
   //---
   if( isOpen() && isSourceEnabled() && isValidRLevel(resLevel) &&
       result && (result->getNumberOfBands() == getNumberOfOutputBands()) )
   {
      result->ref();  // Increment ref count.

      //---
      // Check for overview tile.  Some overviews can contain r0 so always
      // call even if resLevel is 0.  Method returns true on success, false
      // on error.
      //---
      status = getOverviewTile(resLevel, result);
      
      if (!status) // Did not get an overview tile.
      {
         status = true;
         
         ossimIrect tile_rect = result->getImageRectangle();
         
         ossimIrect image_rect = theCcfHead.imageRect(resLevel);
   
         //---
         // See if any point of the requested tile is in the image.
         //---
         if ( tile_rect.intersects(image_rect) )
         {
            ossimIrect clip_rect = tile_rect.clipToRect(image_rect);
            
            if ( !tile_rect.completely_within(clip_rect) )
            {
               // Start with a blank tile.
               result->makeBlank();
            }
            
            // Load the tile buffer with data from the ccf.
            status = fillBuffer(tile_rect,
                                clip_rect,
                                image_rect,
                                resLevel,
                                result);
            if (status)
            {
               result->validate();
            }
         }
      }
      
      result->unref();  // Decrement ref count.
   }

   return status;
}

//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimCcfTileSource::fillBuffer(const ossimIrect& tile_rect,
                                    const ossimIrect& clip_rect,
                                    const ossimIrect& image_rect,
                                    ossim_uint32 reduced_res_level,
                                    ossimImageData* tile)
{
   bool status = false;
   
   //***
   // Determine the pixel type and make the appropriate tiles.
   //***
   switch (theCcfHead.pixelType())
   {
      case OSSIM_UCHAR:
         status = fillUcharBuffer(tile_rect,
                                  clip_rect,
                                  image_rect,
                                  reduced_res_level,
                                  tile);
         break;
      case OSSIM_USHORT11:
      case OSSIM_USHORT16:
         status = fillUshortBuffer(tile_rect,
                                   clip_rect,
                                   image_rect,
                                   reduced_res_level,
                                   tile);
         break;
      default:
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimCcfTileSource fillBuffer ERROR:"
            << "\nUnsupported pixel type!"
            << "\nBuffer not filled.  Returning error." << endl;
         break;
   }

   return status;
}

//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimCcfTileSource::fillUcharBuffer(const ossimIrect& tile_rect,
                                         const ossimIrect& clip_rect,
                                         const ossimIrect& /* image_rect */,
                                         ossim_uint32 reduced_res_level,
                                         ossimImageData* tile)
{
   if(!isOpen()) return false;
      
   const ossim_uint32 TILE_SIZE_PER_BAND     = tile->getSizePerBand();
   const ossim_uint32 CHIP_SIZE_PER_BAND     = theCcfHead.chipSizePerBand();
   const ossim_uint32 SAMPLES_PER_CHIP       = theCcfHead.samplesPerChip();
   const ossim_uint32 LINES_PER_CHIP         = theCcfHead.linesPerChip();
   const ossim_uint32 BYTES_PER_CHUNK        = theCcfHead.bytesPerChunk();
   const ossim_uint32 BYTES_PER_CHIP         = theCcfHead.bytesPerChip();
   const ossim_uint32 CHIPS_IN_SAMPLE_DIR    = theCcfHead.sampleChipsPerChunk();
   const ossim_uint32 CHIPS_IN_LINE_DIR      = theCcfHead.lineChipsPerChunk();
   const ossim_uint32 NUMBER_OF_INPUT_BANDS  = theCcfHead.numberOfBands();
   const ossim_uint32 CHUNKS_IN_SAMPLE_DIR   = theCcfHead.chunksInSampleDir(reduced_res_level);
   const ossim_uint32 CHUNK_SIDE_SIZE        = SAMPLES_PER_CHIP *
                                        CHIPS_IN_SAMPLE_DIR;
   const ossim_uint32 CHIP_OFFSET_TO_END     = SAMPLES_PER_CHIP - 1;

   //***
   // Shift the upper left corner of the "clip_rect" to the an even chunk
   // boundry.
   //***
   ossimIpt chunkOrigin = clip_rect.ul();
   adjustToStartOfChunk(chunkOrigin);

   //***
   // Calculate the number of chunks needed in the line/sample directions.
   //***
   ossim_uint32 size_in_x
      = static_cast<ossim_uint32>(clip_rect.lr().x - chunkOrigin.x + 1);
   ossim_uint32 size_in_y
      = static_cast<ossim_uint32>(clip_rect.lr().y - chunkOrigin.y + 1);
   ossim_uint32 w = (CHIPS_IN_SAMPLE_DIR*SAMPLES_PER_CHIP);
   ossim_uint32 h = (CHIPS_IN_LINE_DIR*LINES_PER_CHIP);
   ossim_uint32 chunks_in_x_dir = size_in_x / w  + (size_in_x % w  ? 1 : 0);
   ossim_uint32 chunks_in_y_dir = size_in_y / h + (size_in_y % h ? 1 : 0);
   
   
   //***
   // Get the start of data for the "reduced_res_level" passed in.
   //***
   streampos startOfData = theCcfHead.startOfData(reduced_res_level);
   ossimIpt ulChunkPt = chunkOrigin;

   // Chunk loop in line direction.
   for (ossim_uint32 y=0; y<chunks_in_y_dir; ++y)
   {
      ulChunkPt.x = chunkOrigin.x;

      // Chunk loop in sample direction.
      for (ossim_uint32 x=0; x<chunks_in_x_dir; ++x)
      {
         //***
         // Sequence through the chips in the chunk.
         // Grab the ccf data and stuff the tile buffer.
         //***
         ossimIpt chip_pt = ulChunkPt;

         // Chip loop in line direction.
         for (ossim_uint32 y_chip=0; y_chip<CHIPS_IN_LINE_DIR; ++y_chip) 
         {
            chip_pt.x = ulChunkPt.x;  // Reset x back to front of chunk.

            // Chip loop in sample direction.
            for (ossim_uint32 x_chip=0; x_chip<CHIPS_IN_SAMPLE_DIR; ++x_chip)
            {
               ossimIrect chip_rect(chip_pt.x,
                               chip_pt.y,
                               chip_pt.x + CHIP_OFFSET_TO_END,
                               chip_pt.y + CHIP_OFFSET_TO_END);
               
               if (chip_rect.intersects(clip_rect))
               {
                  //***
                  // Some point in the chip intersect the tile so grab the
                  // data.
                  //***

                  //***
                  // Get the seek position for the chunk.
                  //***
                  streampos tmp_offset
                     = ( ulChunkPt.y / CHUNK_SIDE_SIZE * BYTES_PER_CHUNK *
                         CHUNKS_IN_SAMPLE_DIR ) +
                     ( ulChunkPt.x / CHUNK_SIDE_SIZE * BYTES_PER_CHUNK );

                  streampos seek_position = startOfData + tmp_offset;

                  // Now move it to the chip.
                  tmp_offset
                     = ( y_chip *  BYTES_PER_CHIP * CHIPS_IN_SAMPLE_DIR ) +
                     (x_chip * BYTES_PER_CHIP);

                  seek_position += tmp_offset;

                  // Seek to the chip
                  theFileStr->seekg(seek_position, ios::beg);

                  //***
                  // Read the chip from the ccf file into the chip buffer.
                  // This will get all the bands.  Bands are interleaved by
                  // chip.
                  //***
                  if (!theFileStr->read((char*)theChipBuffer, BYTES_PER_CHIP))
                  {
                     theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                     cerr << "ossimCcfTileSource fillUchBuffer READ ERROR!"
                          << "\nReturning from method." << endl;
                     return false;
                  }
                  
                  ossimIrect chip_clip_rect = clip_rect.clipToRect(chip_rect);

                  //***
                  // Band loop in sample direction.  Bands are interleaved by
                  // chip.
                  //***
                  for (ossim_uint32 band=0; band<NUMBER_OF_INPUT_BANDS; ++band)
                  {
                     //***
                     // Get a pointer positioned at the first valid pixel in
                     // the chip.
                     //***
                     ossim_uint8* s
                        = theChipBuffer +
                        band * CHIP_SIZE_PER_BAND + 
                        (chip_clip_rect.ul().y - chip_rect.ul().y) *
                        SAMPLES_PER_CHIP + chip_clip_rect.ul().x -
                        chip_rect.ul().x;
                                
                     //***
                     // Get a pointer positioned at the first valid pixel in
                     // the tile.
                     //***
                     ossim_uint8* d
                        = (ossim_uint8*)tile->getBuf() +
                        band * TILE_SIZE_PER_BAND +
                        (chip_clip_rect.ul().y - tile_rect.ul().y) *
                        tile->getWidth() + chip_clip_rect.ul().x -
                        tile_rect.ul().x;      

                     // Line loop through a chip.
                     for (ossim_int32 chip_line = chip_clip_rect.ul().y;
                          chip_line <= chip_clip_rect.lr().y;
                          chip_line++)
                     {
                        // Sample loop through a chip.
                        ossim_uint32 i=0;
                        for (ossim_int32 chip_sample = chip_clip_rect.ul().x;
                             chip_sample <= chip_clip_rect.lr().x;
                             chip_sample++)
                        {
                           d[i] = s[i];
                           i++;
                        }

                        // Increment the pointers by one line.
                        s += SAMPLES_PER_CHIP;
                        d += tile->getWidth();
                     }
                     
                  } // End of band loop.
                  
               } // End of if (chip_rect.intersects(clip_rect))
               
               chip_pt.x += SAMPLES_PER_CHIP;
               
            } // End of chip loop in the sample direction.

           chip_pt.y += LINES_PER_CHIP; 
            
         } // End of chip loop in the line direction.
         
         ulChunkPt.x += CHUNK_SIDE_SIZE;
         
      }  // End of chunk loop in the sample direction.

      ulChunkPt.y += CHUNK_SIDE_SIZE;
      
   }  // End of chunk loop in the line direction.

   return true;
}

//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimCcfTileSource::fillUshortBuffer(const ossimIrect& tile_rect,
                                          const ossimIrect& clip_rect,
                                          const ossimIrect& /* image_rect */,
                                          ossim_uint32 reduced_res_level,
                                          ossimImageData* tile)
{
   const ossim_uint32 TILE_SIZE_PER_BAND     = tile->getSizePerBand();
   const ossim_uint32 CHIP_SIZE_PER_BAND     = theCcfHead.chipSizePerBand();
   const ossim_uint32 SAMPLES_PER_CHIP       = theCcfHead.samplesPerChip();
   const ossim_uint32 LINES_PER_CHIP         = theCcfHead.linesPerChip();
   const ossim_uint32 BYTES_PER_CHUNK        = theCcfHead.bytesPerChunk();
   const ossim_uint32 BYTES_PER_CHIP         = theCcfHead.bytesPerChip();
   const ossim_uint32 CHIPS_IN_SAMPLE_DIR    = theCcfHead.sampleChipsPerChunk();
   const ossim_uint32 CHIPS_IN_LINE_DIR      = theCcfHead.lineChipsPerChunk();
   const ossim_uint32 NUMBER_OF_INPUT_BANDS  = theCcfHead.numberOfBands();
   const ossim_uint32 CHUNKS_IN_SAMPLE_DIR   = theCcfHead.
                                        chunksInSampleDir(reduced_res_level);
   const ossim_uint32 CHUNK_SIDE_SIZE        = SAMPLES_PER_CHIP *
                                        CHIPS_IN_SAMPLE_DIR;
   const ossim_uint32 CHIP_OFFSET_TO_END     = SAMPLES_PER_CHIP - 1;
   
   //***
   // Shift the upper left corner of the "clip_rect" to the an even chunk
   // boundry.
   //***
   ossimIpt chunkOrigin = clip_rect.ul();
   adjustToStartOfChunk(chunkOrigin);

   //***
   // Calculate the number of chunks needed in the line/sample directions.
   //***
   
   ossim_uint32 size_in_x = clip_rect.lr().x - chunkOrigin.x + 1;
   ossim_uint32 size_in_y = clip_rect.lr().y - chunkOrigin.y + 1;
   ossim_uint32 w = (CHIPS_IN_SAMPLE_DIR*SAMPLES_PER_CHIP);
   ossim_uint32 h = (CHIPS_IN_LINE_DIR*LINES_PER_CHIP);
   ossim_uint32 chunks_in_x_dir = size_in_x / w  + (size_in_x % w  ? 1 : 0);
   ossim_uint32 chunks_in_y_dir = size_in_y / h + (size_in_y % h ? 1 : 0);
   //***
   // Get the start of data for the "reduced_res_level" passed in.
   //***
   streampos startOfData = theCcfHead.startOfData(reduced_res_level);
   ossimIpt ulChunkPt = chunkOrigin;

   // Chunk loop in line direction.
   for (ossim_uint32 y=0; y<chunks_in_y_dir; ++y)
   {
      ulChunkPt.x = chunkOrigin.x;

      // Chunk loop in sample direction.
      for (ossim_uint32 x=0; x<chunks_in_x_dir; ++x)
      {
         //***
         // Sequence through the chips in the chunk.
         // Grab the ccf data and stuff the tile buffer.
         //***
         ossimIpt chip_pt = ulChunkPt;

         // Chip loop in line direction.
         for (ossim_uint32 y_chip=0; y_chip<CHIPS_IN_LINE_DIR; ++y_chip) 
         {
            chip_pt.x = ulChunkPt.x;  // Reset x back to front of chunk.

            // Chip loop in sample direction.
            for (ossim_uint32 x_chip=0; x_chip<CHIPS_IN_SAMPLE_DIR; ++x_chip)
            {
               ossimIrect chip_rect(chip_pt.x,
                               chip_pt.y,
                               chip_pt.x + CHIP_OFFSET_TO_END,
                               chip_pt.y + CHIP_OFFSET_TO_END);
               
               if (chip_rect.intersects(clip_rect))
               {
                  //***
                  // Some point in the chip intersect the tile so grab the
                  // data.
                  //***

                  //***
                  // Get the seek position for the chunk.
                  //***
                  streampos tmp_offset
                     = ( ulChunkPt.y / CHUNK_SIDE_SIZE * BYTES_PER_CHUNK *
                         CHUNKS_IN_SAMPLE_DIR ) +
                     ( ulChunkPt.x / CHUNK_SIDE_SIZE * BYTES_PER_CHUNK );

                  streampos seek_position = startOfData + tmp_offset;

                  // Now move it to the chip.
                  tmp_offset
                     = ( y_chip *  BYTES_PER_CHIP * CHIPS_IN_SAMPLE_DIR ) +
                     (x_chip * BYTES_PER_CHIP);

                  seek_position += tmp_offset;

                  // Seek to the chip
                  theFileStr->seekg(seek_position, ios::beg);

                  //***
                  // Read the chip from the ccf file into the chip buffer.
                  // This will get all the bands.  Bands are interleaved by
                  // chip.
                  //***
                  if (!theFileStr->read((char*)theChipBuffer, BYTES_PER_CHIP))
                  {
                     theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                     cerr << "ossimCcfTileSource fillUshortBuffer READ ERROR!"
                          << "Returning from method." << endl;
                     return false;
                  }
                  
                  ossimIrect chip_clip_rect = clip_rect.clipToRect(chip_rect);

                  //***
                  // Band loop in sample direction.  Bands are interleaved by
                  // chip.
                  //***
                  for (ossim_uint32 band=0; band<NUMBER_OF_INPUT_BANDS; band++)
                  {
                     //***
                     // Get a pointer positioned at the first valid pixel in
                     // the chip.
                     //***
                     ossim_uint16* s
                        = (ossim_uint16*)theChipBuffer +
                        band * CHIP_SIZE_PER_BAND + 
                        (chip_clip_rect.ul().y - chip_rect.ul().y) *
                        SAMPLES_PER_CHIP + chip_clip_rect.ul().x -
                        chip_rect.ul().x;       
                     
                     //***
                     // Get a pointer positioned at the first valid pixel in
                     // the tile.
                     //***
                     ossim_uint16* d
                        = (ossim_uint16*)tile->getBuf() +
                        band * TILE_SIZE_PER_BAND +
                        (chip_clip_rect.ul().y - tile_rect.ul().y) *
                        tile->getWidth() + chip_clip_rect.ul().x -
                        tile_rect.ul().x;
                     
                     // Line loop through a chip.
                     for (ossim_int32 chip_line = chip_clip_rect.ul().y;
                          chip_line <= chip_clip_rect.lr().y;
                          ++chip_line)
                     {
                        // Sample loop through a chip.
                        ossim_uint32 i=0;
                        for (ossim_int32 chip_sample = chip_clip_rect.ul().x;
                             chip_sample <= chip_clip_rect.lr().x;
                             ++chip_sample)
                        {
                           if (theByteOrder == OSSIM_LITTLE_ENDIAN)
                           {
                              //***
                              // CCF data alway stored in big endian byte
                              // order so swap the bytes.
                              //***
                              d[i] = (s[i] << 8) | (s[i] >> 8);
                           }
                           else
                           {
                              d[i] = s[i];
                           }
                           
                           ++i;
                        }

                        // Increment the pointers by one line.
                        s += SAMPLES_PER_CHIP;
                        d += tile->getWidth();
                     }
                     
                  } // End of band loop.
                  
               } // End of if (chip_rect.intersects(clip_rect))
               
               chip_pt.x += SAMPLES_PER_CHIP;
               
            } // End of chip loop in the sample direction.

           chip_pt.y += LINES_PER_CHIP; 
            
         } // End of chip loop in the line direction.
         
         ulChunkPt.x += CHUNK_SIDE_SIZE;
         
      }  // End of chunk loop in the sample direction.

      ulChunkPt.y += CHUNK_SIDE_SIZE;
      
   }  // End of chunk loop in the line direction.

   return true;
}
   
            
//*******************************************************************
// Public Method:
//*******************************************************************
ossimIrect
ossimCcfTileSource::getImageRectangle(ossim_uint32 reduced_res_level) const
{
   return theCcfHead.imageRect(reduced_res_level);
}

//*******************************************************************
// Public Method:
//*******************************************************************
bool ossimCcfTileSource::setOutputBandList(const vector<ossim_uint32>& outputBandList)
{
   static const char MODULE[] = "ossimCcfTileSource::setOutputBandList";
   if (traceExec())  CLOG << "entering..." << endl;
   
   if (outputBandList.size() != theOutputBandList.size())
   {
      //***
      // Verify that each individual band does not bust the range of input
      // bands.
      //***
      for (ossim_uint32 i=0; i<outputBandList.size(); i++)
      {
         if (outputBandList[i] > (getNumberOfInputBands() - 1))
         {
            setErrorStatus();

            cerr << MODULE << " ERROR:"
                 << "\nOutput band number in list is greater than the "
                 << "number of bands in the image source!"
                 << "\noutputBandList[" << i << "]:  "
                 << "\nHighest availabe band:  "
                 << (getNumberOfInputBands() - 1)
                 << "\nError status has been set!  Returning..."
                 << endl;
            return false;            
         }
      }

      // Remove the old tiles and reallocate with the new size.
      theTile = NULL;
   }

   theOutputBandList = outputBandList;  // Assign the new list.

   if (traceExec())  CLOG << "returning..." << endl;
   return true;
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimCcfTileSource::saveState(ossimKeywordlist& kwl,
                                   const char* prefix) const
{
   // Currently nothing to do here.
   return ossimImageHandler::saveState(kwl, prefix);
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimCcfTileSource::loadState(const ossimKeywordlist& kwl,
                                   const char* prefix)
{
   if (ossimImageHandler::loadState(kwl, prefix))
   {
      if (open())
      {
         return true;
      }
   }

   return false;
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimCcfTileSource::open()
{
   static const char MODULE[] = "ossimCcfTileSource::open";
   if (traceExec())  CLOG << "entering..." << endl;

   if(isOpen())
   {
      close();
   }
   
   if (traceDebug())
   {
      CLOG << "DEBUG -- "
           << "\n\t theImageFile: " << theImageFile << endl;
   }
   
   if (theCcfHead.parseCcfHeader(theImageFile.c_str()) == false)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      if (traceExec())  CLOG << "returning with error..." << endl;
      return false;
   }

   if (traceDebug())
   {
      CLOG << "DEBUG -- "
           << "\n\t theImageFile: " << theImageFile
           << "\n\t theCcfHead: " << theCcfHead
           << endl;
   }

   theFileStr = new std::ifstream;
   // Open up the file for reading.
   theFileStr->open(theImageFile.c_str(), ios::in | ios::binary);
   
   if (!(*theFileStr))
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      
      cerr << MODULE << " ERROR!"
           << "\nCould not open file:  " << theImageFile.c_str()
           << "\nReturning..." << endl;
      if (traceExec())  CLOG << "returning with error..." << endl;

      delete theFileStr;
      theFileStr = NULL;
      return false;
   }

   theTile = ossimImageDataFactory::instance()->create(this, this);
   theTile->initialize();
     
   theChipBuffer  = new ossim_uint8[theCcfHead.bytesPerChip()];
   
   // Initialize the output band list.
   if (getNumberOfInputBands() > theOutputBandList.size())
   {
      theOutputBandList.resize(getNumberOfInputBands());
   }
   
   for (ossim_uint32 i=0; i<getNumberOfInputBands(); ++i)
   {
      theOutputBandList[i] = i; // One to one for initial setup.
   }
   
   completeOpen();
   
   if (traceDebug() && theTile.valid())
   {
      CLOG << "DEBUG:"
           << "\ntheTile:\n" << *theTile << endl;
   }

   if (traceExec())  CLOG << "returning..." << endl;
   return true;
}

void ossimCcfTileSource::close()
{
   theTile = NULL;
   if (theChipBuffer)
   {
      delete [] theChipBuffer;
      theChipBuffer = NULL;
   }
   if(theFileStr)
   {
      theFileStr->close();
      theFileStr->clear();
      delete theFileStr;
      theFileStr = NULL;
   }
}

//*******************************************************************
// Public method:
//*******************************************************************
ossimScalarType ossimCcfTileSource::getOutputScalarType() const
{
   return theCcfHead.pixelType();
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimCcfTileSource::getTileWidth() const
{
   return ( theTile.valid() ? theTile->getWidth() : 0 );
}

//*******************************************************************
// Public method:
//*******************************************************************
ossim_uint32 ossimCcfTileSource::getTileHeight() const
{
   return ( theTile.valid() ? theTile->getHeight() : 0 );
}

ossim_uint32 ossimCcfTileSource::getImageTileWidth() const
{
   return 32;
}

ossim_uint32 ossimCcfTileSource::getImageTileHeight() const
{
   return 32;
}

void ossimCcfTileSource::initVerticesFromHeader()
{
   theValidImageVertices.clear();
   
   const vector<ossimIpt>& validImageVertices
      = theCcfHead.getValidImageVertices();
   
   ossim_uint32 upper = (ossim_uint32)validImageVertices.size();
   if(!upper) return;
   for(ossim_uint32 index = 0; index < upper; ++index)
   {
      theValidImageVertices.push_back(validImageVertices[index]);
   }

   if (traceDebug())
   {
      cout << "ossimCcfTileSource::initVerticesFromHeader DEBUG:"
           << "\nValid image vertices:"
           << endl;
      for (ossim_uint32 i=0; i<upper; ++i)
      {
         cout << "\npoint[" << i << "]:  " << theValidImageVertices[i];
      }
      cout << endl;
   }
}

bool ossimCcfTileSource::isEcgGeom(const ossimFilename& filename)const
{
   ossimFilename file = filename;
   file.setExtension("geom");
   
   if(file.exists())
   {
      char bytes[3];
      ifstream in(file.c_str());
      
      in.read((char*)bytes, 3);
      ossimString testEcg(bytes,
                          bytes + 3);
      testEcg = testEcg.downcase();
      if(in&&(testEcg == "ecg"))
      {
         return true;
      }
   }
   return false;
}

ossimString ossimCcfTileSource::getShortName()const
{
   return ossimString("ccf");
}

ossimString ossimCcfTileSource::getLongName()const
{
   return ossimString("ccf reader");
}

ossim_uint32 ossimCcfTileSource::getNumberOfInputBands() const
{
   return theCcfHead.numberOfBands();
}

ossim_uint32 ossimCcfTileSource::getNumberOfOutputBands()const
{
   return getNumberOfInputBands();
}

ossim_uint32 ossimCcfTileSource::getNumberOfLines(ossim_uint32 reduced_res_level) const
{
   return theCcfHead.numberOfLines(reduced_res_level);
}

ossim_uint32 ossimCcfTileSource::getNumberOfSamples(ossim_uint32 reduced_res_level) const
{
   return theCcfHead.numberOfSamples(reduced_res_level);
}

ossim_uint32 ossimCcfTileSource::getNumberOfDecimationLevels() const
{
   return theCcfHead.numberOfReducedResSets();
}

bool ossimCcfTileSource::isOpen()const
{
   return (theFileStr != NULL);
}

ossimRefPtr<ossimProperty> ossimCcfTileSource::getProperty(const ossimString& name)const
{
	if(name == "file_type")
	{
		
		return new ossimStringProperty(name, "CCF");
	}
	return ossimImageHandler::getProperty(name);
}

void ossimCcfTileSource::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
	ossimImageHandler::getPropertyNames(propertyNames);
	propertyNames.push_back("file_type");
}

void ossimCcfTileSource::adjustToStartOfChunk(ossimIpt& pt) const
{
   pt.x &= 0xffffff00;
   pt.y &= 0xffffff00;
}

void ossimCcfTileSource::adjustToStartOfChip(ossimIpt& pt) const
{
   pt.x &= 0xffffffe0;
   pt.y &= 0xffffffe0;
}
