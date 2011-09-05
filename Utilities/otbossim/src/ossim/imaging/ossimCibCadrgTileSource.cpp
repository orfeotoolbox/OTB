//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//********************************************************************
// $Id: ossimCibCadrgTileSource.cpp 19900 2011-08-04 14:19:57Z dburken $
#include <algorithm>

#include <ossim/imaging/ossimCibCadrgTileSource.h>

#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimNBandLutDataObject.h>
#include <ossim/imaging/ossimHistogramWriter.h>
#include <ossim/imaging/ossimImageHistogramSource.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/support_data/ossimRpfFrame.h>
#include <ossim/support_data/ossimRpfHeader.h>
#include <ossim/support_data/ossimRpfToc.h>
#include <ossim/support_data/ossimRpfTocEntry.h>
#include <ossim/support_data/ossimRpfCompressionSection.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimCylEquAreaProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimTrace.h>

static ossimTrace traceDebug = ossimTrace("ossimCibCadrgTileSource:debug");

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimCibCadrgTileSource.cpp 19900 2011-08-04 14:19:57Z dburken $";
#endif

RTTI_DEF1(ossimCibCadrgTileSource, "ossimCibCadrgTileSource", ossimImageHandler)

const ossim_uint32 ossimCibCadrgTileSource::CIBCADRG_FRAME_WIDTH  = 1536;
const ossim_uint32 ossimCibCadrgTileSource::CIBCADRG_FRAME_HEIGHT = 1536;

ossimCibCadrgTileSource::ossimCibCadrgTileSource()
   :ossimImageHandler(),
    theCompressedBuffer(0),
    theUncompressedBuffer(0),
    theNumberOfLines(0),
    theNumberOfSamples(0),
    theTile(0),
    theTableOfContents(0),
    theEntryToRender(0),
    theEntryNumberToRender(1),
    theTileSize(128, 128),
    theProductType(OSSIM_PRODUCT_TYPE_UNKNOWN),
    theSkipEmptyCheck(false)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimCibCadrgTileSource::ossimCibCadrgTileSource entered...\n";
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << "\n";
#endif      
   }
   theWorkFrame = new ossimRpfFrame;
   
   // a CADRG and CIBis a 64*64*12 bit buffer and must divide by 8 to
   // convert to bytes
   theCompressedBuffer   = new ossim_uint8[(64*64*12)/8];

   // whether CIB or CADRG we will alocate the buffer
   // to the larger of the 2 (CADRG is 3 bands)
   theUncompressedBuffer = new ossim_uint8[256*256*3];
                           
}

ossimCibCadrgTileSource::~ossimCibCadrgTileSource()
{
   if(theCompressedBuffer)
   {
      delete [] theCompressedBuffer;
      theCompressedBuffer = 0;
   }
   if(theUncompressedBuffer)
   {
      delete [] theUncompressedBuffer;
      theUncompressedBuffer = 0;
   }
   if(theWorkFrame)
   {
      delete theWorkFrame;
      theWorkFrame = 0;
   }
   close();
}

ossimString ossimCibCadrgTileSource::getShortName()const
{
   return ossimString("CIB/CADRG");
}

ossimString ossimCibCadrgTileSource::getLongName()const
{
   return ossimString("CIB/CADRG reader");
}


void ossimCibCadrgTileSource::close()
{
   deleteAll();
   ossimImageHandler::close();
}

bool ossimCibCadrgTileSource::isOpen()const
{
   return (theTableOfContents!=0);
}

bool ossimCibCadrgTileSource::open()
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimCibCadrgTileSource::open(): Entered....."
         << "\ntheImageFile: " << theImageFile << std::endl;
   }

   bool result = false;
   
   if(isOpen())
   {
      close();
   }

   theTableOfContents = new ossimRpfToc;
   
   if(theTableOfContents)
   {      
      if(theTableOfContents->parseFile(theImageFile) == ossimErrorCodes::OSSIM_OK)
      {
         if(theTableOfContents->getNumberOfEntries() > 0)
         {
            vector<ossimString> scaleList = getProductScaleList();
            if(scaleList.size() > 0)
            {
               std::vector<ossim_uint32> entryList;
               getEntryList(entryList);
               if(entryList.size() > 0)
               {
                  setCurrentEntry(entryList[0]);
                  
                  if(theEntryToRender)
                  {
                     // a CADRG is 1536x1536 per frame.
                     theNumberOfLines   = theEntryToRender->getNumberOfLines();
                     theNumberOfSamples = theEntryToRender->getNumberOfSamples();
                  }

                  if(theEntryToRender->getProductType().trim().upcase() == "CADRG")
                  {
                     theProductType = OSSIM_PRODUCT_TYPE_CADRG;
                     result = true;
                  }
                  else if(theEntryToRender->getProductType().trim().upcase() == "CIB")
                  {
                     theProductType = OSSIM_PRODUCT_TYPE_CIB;
                     result = true;
                  }
                  if ( result )
                  {
                     // This initializes tiles and buffers.
                     allocateForProduct();
                  }
               }
            }
         }
      }
   }

   if(!result)
   {
      theImageFile.clear();
      close();
   }

#if 0 /* 20100414 - drb */
   else
   {
      //---
      // Adjust image rect so not to go over the -180 to 180 and -90 to 90
      // bounds.
      //---
      // Note this did not do any boundary checking and was in conflict with setTocEntryToRender
      // method calculation of lines and samps.
      // setActualImageRect();

      std::ifstream in(theImageFile.c_str(), std::ios::in|std::ios::binary);
      if(in.good()&&theTableOfContents->getRpfHeader())
      {
      }
      completeOpen();

      theTile = ossimImageDataFactory::instance()->create(this, this);
      theTile->initialize();
   }
#endif

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimCibCadrgTileSource::open(): Leaving at line " << __LINE__
         << " result=" << (result?"true":"false") << std::endl;
   }

   return result;
}

void ossimCibCadrgTileSource::setSkipEmptyCheck( bool bSkipEmptyCheck )
{
   theSkipEmptyCheck = bSkipEmptyCheck;
}

ossimRefPtr<ossimImageData> ossimCibCadrgTileSource::getTile(
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

bool ossimCibCadrgTileSource::getTile(ossimImageData* result,
                                      ossim_uint32 resLevel)
{
   bool status = false;

   //---
   // Not open, this tile source bypassed, or invalid res level,
   // return a blank tile.
   //---
   if( isOpen() && isSourceEnabled() && isValidRLevel(resLevel) &&
       result && (result->getNumberOfBands() == getNumberOfOutputBands()) &&
       (theProductType != OSSIM_PRODUCT_TYPE_UNKNOWN) )
   {
      if ( resLevel > 0 )
      {
         //---
         // Check for overview tile.  Some overviews can contain r0 so always
         // call even if resLevel is 0 (if overview is not virtual).  Method 
         // returns true on success, false on error.
         //---
         status = getOverviewTile(resLevel, result);
      }

      if (!status) // Did not get an overview tile.
      {
         status = true;

         ossimIrect rect = result->getImageRectangle();

         ossimIrect imageRect = getImageRectangle();

         if ( rect.intersects(imageRect) )
         {
            //---
            // Start with a blank tile in case there is not total coverage
            // for rect.
            //---
            result->makeBlank();
   
            vector<ossimFrameEntryData> frames = getIntersectingEntries(rect);
            if(frames.size() > 0)
            {
               //---
               // Now lets render each frame.  Note we will have to find
               // subframes
               // that intersect the rectangle of interest for each frame.
               //---
               fillTile(rect, frames, result);
               
               // Revalidate tile status.
               result->validate();
            }
         }
         else
         {
            result->makeBlank();
         }
      }
   }
   
   return status;
}

ossim_uint32 ossimCibCadrgTileSource::getNumberOfInputBands()const
{
   switch(theProductType)
   {
      case  OSSIM_PRODUCT_TYPE_UNKNOWN:
      {
         return 0;
      }
      case OSSIM_PRODUCT_TYPE_CIB:
      {
         return 1;
      }
      case OSSIM_PRODUCT_TYPE_CADRG:
      {
         return 3;
      }
   }
   
   return 0;
}

ossim_uint32 ossimCibCadrgTileSource::getNumberOfOutputBands()const
{
   switch(theProductType)
   {
      case  OSSIM_PRODUCT_TYPE_UNKNOWN:
      {
         return 0;
      }
      case OSSIM_PRODUCT_TYPE_CIB:
      {
         return 1;
      }
      case OSSIM_PRODUCT_TYPE_CADRG:
      {
         return 3;
      }
   }
   
   return 0;
}

ossim_uint32 ossimCibCadrgTileSource::getNumberOfLines(ossim_uint32 reduced_res_level) const
{
   if (reduced_res_level == 0)
   {
      return theNumberOfLines;
   }
   else if (theOverview.valid())
   {
      return theOverview->getNumberOfLines(reduced_res_level);
   }

   return 0;
}

ossim_uint32 ossimCibCadrgTileSource::getNumberOfSamples(ossim_uint32 reduced_res_level) const
{
   if (reduced_res_level == 0)
   {
      return theNumberOfSamples;
   }
   else if (theOverview.valid())
   {
      return theOverview->getNumberOfSamples(reduced_res_level);
   }

   return 0;
}

ossimIrect ossimCibCadrgTileSource::getImageRectangle(ossim_uint32 reduced_res_level) const
{
   return ossimIrect(0,                         // upper left x
                     0,                         // upper left y
                     getNumberOfSamples(reduced_res_level) - 1,  // lower right x
                     getNumberOfLines(reduced_res_level)   - 1); // lower right y                     
}
   
ossimRefPtr<ossimImageGeometry> ossimCibCadrgTileSource::getImageGeometry()
{
   if ( !theGeometry )
   {
      // Check for external geom:
      theGeometry = getExternalImageGeometry();
      
      if ( !theGeometry && theEntryToRender )
      {
         theGeometry = theEntryToRender->getImageGeometry();
      }

      // Set image things the geometry object should know about.
      initImageParameters( theGeometry.get() );
   }

   return theGeometry;
}
   
ossimScalarType ossimCibCadrgTileSource::getOutputScalarType() const
{
   return OSSIM_UCHAR;
}

ossim_uint32 ossimCibCadrgTileSource::getTileWidth() const
{
   return theTileSize.x;
}
   
ossim_uint32 ossimCibCadrgTileSource::getTileHeight() const
{
   return theTileSize.y;
}

ossim_uint32 ossimCibCadrgTileSource::getCurrentEntry()const
{
   return (ossim_uint32)theEntryNumberToRender;
}

bool ossimCibCadrgTileSource::setCurrentEntry(ossim_uint32 entryIdx)
{
   bool result = false;

   theDecimationFactors.clear();
   
   // Clear the geometry.
   theGeometry = 0;
   
   // Must clear or openOverview will use last entries.
   theOverviewFile.clear();
   
   if(setEntryToRender(entryIdx))
   {
      completeOpen();
      result = true;
   }

   return result;
}

void ossimCibCadrgTileSource::getEntryList(std::vector<ossim_uint32>& entryList)const
{
   std::vector<ossimString> scaleList = getProductScaleList();
   ossim_uint32 scaleIdx = 0;
   ossim_uint32 entryIdx = 0;
   for(scaleIdx = 0; scaleIdx < scaleList.size(); ++scaleIdx)
   {
      std::vector<ossim_int32> eList = getProductEntryList(scaleList[scaleIdx]);
      
      for(entryIdx = 0; entryIdx < eList.size(); ++entryIdx)
      {
         entryList.push_back(eList[entryIdx]);
      }
   }

}

bool ossimCibCadrgTileSource::setEntryToRender(ossim_uint32 index)
{
   if(isOpen())
   {
      const ossimRpfTocEntry *temp = theTableOfContents->getTocEntry(index);

      if(temp)
      {
         if(!(temp->getBoundaryInformation().getCoverage().isGeographicRectNull()))
         {
            setTocEntryToRender(temp);
            theEntryNumberToRender = index;
            if(theEntryToRender)
            {
               if(theEntryToRender->getProductType().trim().upcase() == "CADRG")
               {
                  theProductType = OSSIM_PRODUCT_TYPE_CADRG;
               }
               else if(theEntryToRender->getProductType().trim().upcase() == "CIB")
               {
                  theProductType = OSSIM_PRODUCT_TYPE_CIB;
               }
               else
               {
                  theProductType = OSSIM_PRODUCT_TYPE_UNKNOWN;
               }
            }
            
            populateLut();
            
            return true;
         }
      }
   }
   
   return false;
}

void ossimCibCadrgTileSource::setTocEntryToRender(const ossimRpfTocEntry* entry)
{
   if(isOpen()&&entry)
   {
      theEntryToRender = entry;
      theNumberOfLines   = theEntryToRender->getNumberOfLines();
      theNumberOfSamples = theEntryToRender->getNumberOfSamples();
      theEntryNumberToRender = theTableOfContents->getTocEntryIndex(entry);
   }
}

ossim_int32 ossimCibCadrgTileSource::getCurrentEntryIndex()const
{
   return theEntryNumberToRender;
}

const ossimRpfTocEntry* ossimCibCadrgTileSource::getCurrentTocEntry()const
{
   return theEntryToRender;
}

const ossimRpfToc*  ossimCibCadrgTileSource::getToc()const
{
   return theTableOfContents;
}

bool ossimCibCadrgTileSource::isValidRLevel(ossim_uint32 reduced_res_level) const
{
   if (reduced_res_level == 0)
   {
      return true;
   }
   else if (theOverview.valid())
   {
      return theOverview->isValidRLevel(reduced_res_level);
   }
   else
   {
      return false;
   }
}

vector<ossimString> ossimCibCadrgTileSource::getProductScaleList()const
{
   vector<ossimString> result;
   
   if(isOpen())
   {
      ossim_int32 upperBound = theTableOfContents->getNumberOfEntries();

      for(ossim_int32 index = 0; index < upperBound; ++index)
      {
         const ossimRpfTocEntry* entry = theTableOfContents->getTocEntry(index);
         ossimString scale = entry->getBoundaryInformation().getScale();

         scale = scale.trim().upcase();
         if(scale!="")
         {
            // CIB will have product scales like 5M or 10M
            // and CADRG's will have product scales like
            // 1:50K and 1:250K ...
            // So if these patterns exist then add it to
            // the product list.
            if( ( std::find(scale.begin(), scale.end(), ':') != scale.end() )
                ||
                (scale[scale.size()-1]=='M'))
            {
               // only add it if it doesn't already exist
               // on the list.
               if(find(result.begin(), result.end(), scale) == result.end())
               {
                  result.push_back(scale);
               }
            }
         }
      }
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimCibCadrgTileSource::getProductScaleList DEBUG:"
         << endl;

      std::vector<ossimString>::const_iterator i = result.begin();
      ossim_uint32 index = 0;
      while (i != result.end())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "scale[" << index << "]:  " << (*i) << endl;
         ++index;
         ++i;
      }
   }
   
   return result;
}

vector<ossim_int32> ossimCibCadrgTileSource::getProductEntryList(const ossimString& productScale)const
{
   vector<ossim_int32> result;
   
   if(isOpen())
   {
      ossim_int32 upperBound = theTableOfContents->getNumberOfEntries();

      for(ossim_int32 index = 0; index < upperBound; ++index)
      {
         const ossimRpfTocEntry* entry = theTableOfContents->getTocEntry(index);

         if(entry)
         {
            // If the "skip empty check" is true, we don't check to see if the
            // actual frame images exist. Otherwise the check is carried out,
            // which is the default situation.
            bool bIsEmpty = (theSkipEmptyCheck==false) ? entry->isEmpty() : false;

            // if the entry is not empty then add it to the list.
            if( bIsEmpty == false )
            {
               ossimString scale = entry->getBoundaryInformation().getScale();
               scale = scale.trim().upcase();
               if(scale==productScale)
               {
                  result.push_back(index);
               }
            }
         }
      }
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimCibCadrgTileSource::getProductEntryList DEBUG:"
         << endl;

      std::vector<ossim_int32>::const_iterator i = result.begin();
      ossim_uint32 index = 0;
      while (i != result.end())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "scale[" << index << "]:  " << (*i) << endl;
         ++index;
         ++i;
      }
   }
   
   return result;
   
}

vector<ossimCibCadrgTileSource::ossimFrameEntryData> ossimCibCadrgTileSource::getIntersectingEntries(const ossimIrect& rect)
{
   vector<ossimFrameEntryData> result;

   // make sure we have the Toc entry to render
   if(!isOpen()) return result;
   
   ossimIrect imageRect = getImageRectangle();
   if(rect.intersects(imageRect))
   {
      ossimIrect clipRect  = rect.clipToRect(imageRect);
      ossimIrect frameRect(clipRect.ul().x/CIBCADRG_FRAME_WIDTH,
                           clipRect.ul().y/CIBCADRG_FRAME_HEIGHT,
                           clipRect.lr().x/CIBCADRG_FRAME_WIDTH,
                           clipRect.lr().y/CIBCADRG_FRAME_HEIGHT);

      ossimRpfFrameEntry tempEntry;

      
     for(ossim_int32 row = frameRect.ul().y; row <= frameRect.lr().y; ++row)
      {
         for(ossim_int32 col = frameRect.ul().x; col <= frameRect.lr().x; ++col)
         {
            if(theEntryToRender->getEntry((theEntryToRender->getNumberOfFramesVertical()-1) - row,
                                          col,
                                          tempEntry))
            {
               if(tempEntry.exists())
               {
                   result.push_back(ossimFrameEntryData(row,
                                                        col,
                                                        row*CIBCADRG_FRAME_HEIGHT,
                                                        col*CIBCADRG_FRAME_WIDTH,
                                                        tempEntry));
               }
            }
         }
      }
   }
   
   return result;
}

void ossimCibCadrgTileSource::fillTile(
   const ossimIrect& tileRect,
   const vector<ossimFrameEntryData>& framesInvolved,
   ossimImageData* tile)
{
   ossim_uint32 idx = 0;
   for(idx = 0;
       idx < framesInvolved.size();
       ++idx)
   {

      if(theWorkFrame->parseFile(framesInvolved[idx].theFrameEntry.getFullPath())
         == ossimErrorCodes::OSSIM_OK)
      {
         // we will fill a subtile.  We pass in which frame it is and the position of the frame.
         // the actual pixel will be 1536*row and 1536 *col.
         if(theProductType == OSSIM_PRODUCT_TYPE_CIB)
         {
            fillSubTileCib(*theWorkFrame,
                           tileRect,
                           framesInvolved[idx],
                           tile);
         }
         else
         {
            fillSubTileCadrg(*theWorkFrame,
                             tileRect,
                             framesInvolved[idx],
                             tile);
         }
      }
   }
}

void ossimCibCadrgTileSource::fillSubTileCadrg(
   const ossimRpfFrame&  aFrame,
   const ossimIrect& tileRect,
   const ossimFrameEntryData& frameEntryData,
   ossimImageData* tile)
{
   // first let's grab the absolute position of the frame rectangle in pixel space
   ossimIrect frameRect(frameEntryData.thePixelCol,
                        frameEntryData.thePixelRow,
                        frameEntryData.thePixelCol + CIBCADRG_FRAME_WIDTH  - 1,
                        frameEntryData.thePixelRow + CIBCADRG_FRAME_HEIGHT - 1);

   
   // now clip it to the tile
   ossimIrect clipRect = tileRect.clipToRect(frameRect);
   
   const ossimRpfCompressionSection* compressionSection = aFrame.getCompressionSection();
   
   if(!compressionSection)
   {
      return;
   }

   const vector<ossimRpfColorGrayscaleTable>& colorTable =
      aFrame.getColorGrayscaleTable();

   // ESH 03/2009 -- Partial fix for ticket #646.
   // Crash fix on reading RPFs: Make sure the colorTable vector 
   // has entries before trying to make use of them. 
   int numTables = (int)colorTable.size();
   if ( numTables <= 0 )
   {
      return;
   }

   ossim_uint8 *tempRows[3];
   
   tempRows[0] = theUncompressedBuffer;
   tempRows[1] = (theUncompressedBuffer + 256*256);
   tempRows[2] = (tempRows[1] + 256*256);
   
   // find the shift to 0,0
   ossimIpt tempDelta(clipRect.ul().x - frameEntryData.thePixelCol,
                      clipRect.ul().y - frameEntryData.thePixelRow);
   
   
   // In order to compute the subframe we will need the corner offsets of
   // the upper left of the frame and the upper left of the clip rect.  The
   // clip rect should be completely within the frame.  This just translates the value
   // to make the upper left of the frame be 0,0.
   //
   ossimIrect offsetRect(tempDelta.x,
                         tempDelta.y,
                         tempDelta.x + clipRect.width()-1,
                         tempDelta.y + clipRect.height()-1);
   
   // each subframe is 64x64.  We will actually use 256x256 since
   // we will be uncompressing them.  Note CADRG is a 256x256 tile
   // compressed to 64x64x12 bit data
   //
    ossimIrect subFrameRect(offsetRect.ul().x/256,
                            offsetRect.ul().y/256,
                            (offsetRect.lr().x)/256,
                            (offsetRect.lr().y)/256);
   
   ossim_uint32 readPtr  = 0;

   ossim_int32 row = 0;
   ossim_int32 col = 0;
   ossim_uint32 i = 0;
   ossim_uint32 j = 0;
   ossim_int32 upperY = subFrameRect.lr().y;
   ossim_int32 upperX = subFrameRect.lr().x;
   ossim_int32 lowerY = subFrameRect.ul().y;
   ossim_int32 lowerX = subFrameRect.ul().x; 
   for(row = lowerY; row <= upperY; ++row)
   {
      for(col = lowerX; col <= upperX; ++col)
      {
         readPtr = 0;
         if(aFrame.fillSubFrameBuffer(theCompressedBuffer, 0, row, col))
         {
            for (i = 0; i < 256; i += 4)
            {
               for (j = 0; j < 256; j += 8)
               {
                  ossim_uint16 firstByte  = theCompressedBuffer[readPtr++] & 0xff;
                  ossim_uint16 secondByte = theCompressedBuffer[readPtr++] & 0xff;
                  ossim_uint16 thirdByte  = theCompressedBuffer[readPtr++] & 0xff;
                  
                  //because dealing with half-bytes is hard, we
                  //uncompress two 4x4 tiles at the same time. (a
                  //4x4 tile compressed is 12 bits )
                  // this little code was grabbed from openmap software.
                  
                  /* Get first 12-bit value as index into VQ table */
                  // I think we need to swap
                  ossim_uint16 val1 = (firstByte << 4) | (secondByte >> 4);
                  
                  /* Get second 12-bit value as index into VQ table*/
                  ossim_uint16 val2 = ((secondByte & 0x000F) << 8) | thirdByte;
                  
                  for (ossim_uint32 t = 0; t < 4; ++t)
                  {
                     for (ossim_uint32 e = 0; e < 4; ++e)
                     {
                        ossim_uint16 tableVal1 = compressionSection->getTable()[t].theData[val1*4 + e] & 0xff;
                        ossim_uint16 tableVal2 = compressionSection->getTable()[t].theData[val2*4 + e] & 0xff;

                        ossim_uint32 pixindex = ((i+t)*256) +
                                                 (j + e);
                        const ossim_uint8* color1 = colorTable[0].getStartOfData(tableVal1);
                        const ossim_uint8* color2 = colorTable[0].getStartOfData(tableVal2);
                        
                        
                        tempRows[0][pixindex] = color1[0];
                        tempRows[1][pixindex] = color1[1];
                        tempRows[2][pixindex] = color1[2];
                        
                        tempRows[0][pixindex+4] = color2[0];
                        tempRows[1][pixindex+4] = color2[1];
                        tempRows[2][pixindex+4] = color2[2];
                     } //for e
                  } //for t
               }  /* for j */
            } //for i
         }
         else
         {
            memset(theUncompressedBuffer, 0, 256*256*3);
         }
         ossim_int32 tempCol = col*256;
         ossim_int32 tempRow = row*256;
         ossimIrect subRectToFill(frameRect.ul().x + tempCol,
                                  frameRect.ul().y + tempRow,
                                  frameRect.ul().x + tempCol + 255,
                                  frameRect.ul().y + tempRow + 255);
         tile->loadTile(theUncompressedBuffer,
                        subRectToFill,
                        OSSIM_BSQ);
      }
   }
}

void ossimCibCadrgTileSource::fillSubTileCib(
   const ossimRpfFrame&  aFrame,
   const ossimIrect& tileRect,
   const ossimFrameEntryData& frameEntryData,
   ossimImageData* tile)
{
   // first let's grab the absolute position of the frame rectangle in pixel
   // space
   ossimIrect frameRect(frameEntryData.thePixelCol,
                        frameEntryData.thePixelRow,
                        frameEntryData.thePixelCol + CIBCADRG_FRAME_WIDTH  - 1,
                        frameEntryData.thePixelRow + CIBCADRG_FRAME_HEIGHT - 1);

   
   // now clip it to the tile
   ossimIrect clipRect = tileRect.clipToRect(frameRect);

   const ossimRpfCompressionSection* compressionSection = aFrame.getCompressionSection();
   
   if(!compressionSection)
   {
      return;
   }

   const vector<ossimRpfColorGrayscaleTable>& colorTable =
      aFrame.getColorGrayscaleTable();

   // ESH 03/2009 -- Partial fix for ticket #646.
   // Crash fix on reading RPFs: Make sure the colorTable vector 
   // has entries before trying to make use of them. 
   int numTables = (int)colorTable.size();
   if ( numTables <= 0 )
   {
      return;
   }

   // check to see if it does overlap.  If it doesn't then the width and height
   // will be a single point
   {
      ossim_uint8 *tempRow;

      tempRow = theUncompressedBuffer;
      
      // find the shift to 0,0
      ossimIpt tempDelta(clipRect.ul().x - frameEntryData.thePixelCol,
                         clipRect.ul().y - frameEntryData.thePixelRow);
      
      // In order to compute the subframe we will need the corner offsets of
      // the upper left of the frame and the upper left of the clip rect.  The
      // clip rect should be completely within the frame.  This just translates the value
      // to make the upper left of the frame be 0,0.
      //
      ossimIrect offsetRect(tempDelta.x,
                            tempDelta.y,
                            tempDelta.x + clipRect.width()-1,
                            tempDelta.y + clipRect.height()-1);

      // each subframe is 64x64.  We will actually use 256x256 since
      // we will be uncompressing them.  Note CADRG is a 256x256 tile
      // compressed to 64x64x12 bit data
      //
      ossimIrect subFrameRect(offsetRect.ul().x/256,
                              offsetRect.ul().y/256,
                              (offsetRect.lr().x)/256,
                              (offsetRect.lr().y)/256);

      ossim_int32 row = 0;
      ossim_int32 col = 0;
      ossim_uint32 i = 0;
      ossim_uint32 j = 0;
      ossim_uint32 readPtr = 0;
      for(row = subFrameRect.ul().y; row <= subFrameRect.lr().y; ++row)
      {
         for(col = subFrameRect.ul().x; col <= subFrameRect.lr().x; ++col)
         {
            readPtr = 0;
            if(aFrame.fillSubFrameBuffer(theCompressedBuffer, 0, row, col))
            {
               for (i = 0; i < 256; i += 4)
               {
                  for (j = 0; j < 256; j += 8)
                  {
                     ossim_uint16 firstByte  = theCompressedBuffer[readPtr++] & 0xff;
                     ossim_uint16 secondByte = theCompressedBuffer[readPtr++] & 0xff;
                     ossim_uint16 thirdByte  = theCompressedBuffer[readPtr++] & 0xff;
                     
                     //because dealing with half-bytes is hard, we
                     //uncompress two 4x4 tiles at the same time. (a
                     //4x4 tile compressed is 12 bits )
                     // this little code was grabbed from openmap software.
                     
                     /* Get first 12-bit value as index into VQ table */
                     // I think we need to swap
                     ossim_uint16 val1 = (firstByte << 4) | (secondByte >> 4);
                     
                     /* Get second 12-bit value as index into VQ table*/
                     ossim_uint16 val2 = ((secondByte & 0x000F) << 8) | thirdByte;

                     for (ossim_uint32 t = 0; t < 4; ++t)
                     {
                        for (ossim_uint32 e = 0; e < 4; ++e)
                        {
                           ossim_uint16 tableVal1 = compressionSection->getTable()[t].theData[val1*4 + e] & 0xff;
                           ossim_uint16 tableVal2 = compressionSection->getTable()[t].theData[val2*4 + e] & 0xff;

                           ossim_uint32 pixindex = ((i+t)*256) + (j + e);
                           const ossim_uint8* color1 = colorTable[0].getStartOfData(tableVal1);
                           const ossim_uint8* color2 = colorTable[0].getStartOfData(tableVal2);

                           
                           tempRow[pixindex]      = color1[0];
                           tempRow[pixindex + 4]  = color2[0];
                        } //for e
                     } //for t
                  }  /* for j */
               } //for i
            }
            else
            {
               memset(theUncompressedBuffer, 0, 256*256);
            }
            ossim_int32 tCol = col*256;
            ossim_int32 tRow = row*256;
            ossimIrect subRectToFill(frameRect.ul().x + tCol,
                                     frameRect.ul().y + tRow,
                                     frameRect.ul().x + tCol + 255,
                                     frameRect.ul().y + tRow + 255);
            tile->loadTile(theUncompressedBuffer,
                           subRectToFill,
                           OSSIM_BSQ);
         }
      }
   }  
}

void ossimCibCadrgTileSource::allocateForProduct()
{
   if(theProductType ==  OSSIM_PRODUCT_TYPE_UNKNOWN)
   {
      return;
   }
   if(theUncompressedBuffer)
   {
      delete [] theUncompressedBuffer;
      theUncompressedBuffer = 0;
   }
   if(theCompressedBuffer)
   {
      delete [] theCompressedBuffer;
      theCompressedBuffer = 0;
   }
   
   // a CADRG and CIBis a 64*64*12 bit buffer and must divide by 8 to
   // convert to bytes
   theCompressedBuffer   = new ossim_uint8[(64*64*12)/8];
   if(theProductType == OSSIM_PRODUCT_TYPE_CIB)
   {
      theUncompressedBuffer = new ossim_uint8[256*256];
   }
   else
   {
      theUncompressedBuffer = new ossim_uint8[256*256*3];
   }
   
   theTile = ossimImageDataFactory::instance()->create(this, this);
   theTile->initialize();
}

const ossimRpfTocEntry* ossimCibCadrgTileSource::findFirstFrame()
{
   vector<ossimString> products = getProductScaleList();
   vector<ossim_int32> indexProducts;

   if(products.size() > 0)
   {
      indexProducts = getProductEntryList(products[0]);

      if(indexProducts.size()>0)
      {
         theEntryNumberToRender = indexProducts[0];
         
         return theTableOfContents->getTocEntry(indexProducts[0]);
      }
   }
   
   return 0;
}


void ossimCibCadrgTileSource::deleteAll()
{
   theOverview = 0;
   if(theTableOfContents)
   {
      delete theTableOfContents;
      theTableOfContents = 0;
   }
}

bool ossimCibCadrgTileSource::saveState(ossimKeywordlist& kwl,
                                     const char* prefix)const
{
   bool result = ossimImageHandler::saveState(kwl, prefix);

   kwl.add(prefix, 
	   "entry",
	   theEntryNumberToRender,
	   true);

   return result;
}
   
bool ossimCibCadrgTileSource::loadState(const ossimKeywordlist& kwl,
                                        const char* prefix)
{
   const char* MODULE = "ossimCibCadrgTileSource::loadState";

   if(traceDebug())
   {
      CLOG << "Entering..." << endl;
   }
   bool result = ossimImageHandler::loadState(kwl, prefix);

   if(!result)
   {
	   if(traceDebug())
	   {
          CLOG << "Leaving..." << endl;
       }
      return false;
   }
   const char* lookup = 0;
   lookup = kwl.find(ossimString(prefix), "entry");
   ossim_int32 entry = ossimString(lookup).toInt32();

   // if an entry is specified then
   // call the open with an entry number
   if(lookup)
   {
      if(traceDebug())
      {
         CLOG << "Leaving..." << endl;
      }
      result = ossimImageHandler::open(theImageFile);
      setCurrentEntry(entry);
      return result;
   }

   result = ossimImageHandler::open(theImageFile);
   
   return result;
}

ossim_uint32 ossimCibCadrgTileSource::getImageTileWidth() const
{
   return 256;
}

ossim_uint32 ossimCibCadrgTileSource::getImageTileHeight() const
{
   return 256;
}

bool ossimCibCadrgTileSource::isCib() const
{
   return (theProductType==OSSIM_PRODUCT_TYPE_CIB);
}

bool ossimCibCadrgTileSource::isCadrg() const
{
   return (theProductType==OSSIM_PRODUCT_TYPE_CADRG);
}

ossimString ossimCibCadrgTileSource::getProductScale() const
{
   ossimString result;
   
   const ossimRpfTocEntry* entry =
      theTableOfContents->getTocEntry(theEntryNumberToRender);

   if (entry)
   {
      result = entry->getBoundaryInformation().getScale();
   }

   return result;
}

ossimString ossimCibCadrgTileSource::getSecurityClassification()const
{
   ossimString result;
   
   const ossimRpfHeader* header =
      theTableOfContents->getRpfHeader();

   if (header)
   {
      result = header->getSecurityClassification();
   }

   return result;
}

ossimRefPtr<ossimProperty> ossimCibCadrgTileSource::getProperty(const ossimString& name)const
{
   if(name == "file_type")
   {
      if(theProductType == OSSIM_PRODUCT_TYPE_CIB)
      {
         return new ossimStringProperty("file_type", "CIB");
      }
      else if(theProductType == OSSIM_PRODUCT_TYPE_CADRG)
      {
         return new ossimStringProperty("file_type", "CADRG");
      }
      return 0;
   }
   return ossimImageHandler::getProperty(name);
}

void ossimCibCadrgTileSource::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageHandler::getPropertyNames(propertyNames);
   propertyNames.push_back("file_type");
   const ossimRpfHeader* header =
      theTableOfContents->getRpfHeader();
   
   if(header)
   {
      std::ifstream in(theImageFile.c_str(), std::ios::in|std::ios::binary);
      
   }
}

void ossimCibCadrgTileSource::populateLut()
{
   theLut = 0;
   if(theEntryToRender)
   {
      ossim_uint32 w = theEntryToRender->getNumberOfFramesHorizontal();
      ossim_uint32 h = theEntryToRender->getNumberOfFramesVertical();
      ossim_uint32 wi, hi;
      bool found = false;
      ossimRpfFrameEntry tempEntry;
      ossimRpfFrame aFrame;
      for(wi = 0; ((wi < w)&&(!found)); ++wi)
      {
         for(hi = 0; ((hi < h)&&(!found)); ++hi)
         {
            theEntryToRender->getEntry(hi,
                                       wi,
                                       tempEntry);
            if(tempEntry.getFullPath().exists())
            {
               found = true;
            }
         }
      }
      if(aFrame.parseFile(tempEntry.getFullPath())
         == ossimErrorCodes::OSSIM_OK)
      {
         const vector<ossimRpfColorGrayscaleTable>& colorTable =
            aFrame.getColorGrayscaleTable();

         // ESH 03/2009 -- Partial fix for ticket #646.
         // Crash fix on reading RPFs: Make sure the colorTable vector 
         // has entries before trying to make use of them. 
         int numTables = (int)colorTable.size();

         ossim_uint32 numElements = (numTables > 0) ? colorTable[0].getNumberOfElements() : 0;
         if(numElements > 0)
         {
            if((theProductType == OSSIM_PRODUCT_TYPE_CIB)||
               (theProductType == OSSIM_PRODUCT_TYPE_CADRG))
            {
               theLut = new ossimNBandLutDataObject(numElements,
                                                    3,
                                                    OSSIM_UINT8);
            }
            else
            {
               theLut = 0;
               return;
            }
            ossim_uint32 idx = 0;

            for(idx = 0; idx < numElements;++idx)
            {
               const ossim_uint8* startOfData = colorTable[0].getStartOfData(idx);
               switch(theProductType)
               {
                  case OSSIM_PRODUCT_TYPE_CIB:
                  {
                     (*theLut)[idx][0] = (ossimNBandLutDataObject::LUT_ENTRY_TYPE)(startOfData[0]);
                     (*theLut)[idx][1] = (ossimNBandLutDataObject::LUT_ENTRY_TYPE)(startOfData[0]);
                     (*theLut)[idx][2] = (ossimNBandLutDataObject::LUT_ENTRY_TYPE)(startOfData[0]);
                     break;
                  }
                  case OSSIM_PRODUCT_TYPE_CADRG:
                  {
                     (*theLut)[idx][0] = (ossimNBandLutDataObject::LUT_ENTRY_TYPE)(startOfData[0]);
                     (*theLut)[idx][1] = (ossimNBandLutDataObject::LUT_ENTRY_TYPE)(startOfData[1]);
                     (*theLut)[idx][2] = (ossimNBandLutDataObject::LUT_ENTRY_TYPE)(startOfData[2]);
                     break;
                  }
                  default:
                  {
                     break;
                  }
               }
            }
         }
      }
   }
}

void ossimCibCadrgTileSource::updatePropertiesToFirstValidFrame()
{
   if(theEntryToRender)
   {
      ossim_uint32 w = theEntryToRender->getNumberOfFramesHorizontal();
      ossim_uint32 h = theEntryToRender->getNumberOfFramesVertical();
      ossim_uint32 wi, hi;
      bool found = false;
      ossimRpfFrameEntry tempEntry;
      ossimRpfFrame aFrame;
      for(wi = 0; ((wi < w)&&(!found)); ++wi)
      {
         for(hi = 0; ((hi < h)&&(!found)); ++hi)
         {
            theEntryToRender->getEntry(hi,
                                       wi,
                                       tempEntry);
            if(tempEntry.getFullPath().exists())
            {
               found = true;
            }
         }
      }
      if(found)
      {
         if(aFrame.parseFile(tempEntry.getFullPath()) == ossimErrorCodes::OSSIM_OK)
         {
            
         }
      }	
   }
}
