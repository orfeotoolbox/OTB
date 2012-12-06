//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//********************************************************************
// $Id: ossimRpfCacheTileSource.cpp 1361 2010-08-26 00:24:22Z david.burken $
#include <algorithm>
using namespace std;

#include <ossim/imaging/ossimRpfCacheTileSource.h>

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
#include <ossim/support_data/ossimRpfToc.h>
#include <ossim/support_data/ossimRpfTocEntry.h>
#include <ossim/support_data/ossimRpfCompressionSection.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimCylEquAreaProjection.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimPolygon.h>

static ossimTrace traceDebug = ossimTrace("ossimRpfCacheTileSource:debug");

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimRpfCacheTileSource.cpp 1361 2010-08-26 00:24:22Z david.burken $";
#endif

RTTI_DEF1(ossimRpfCacheTileSource, "ossimRpfCacheTileSource", ossimImageHandler)

const ossim_uint32 ossimRpfCacheTileSource::CIBCADRG_FRAME_WIDTH  = 1536;
const ossim_uint32 ossimRpfCacheTileSource::CIBCADRG_FRAME_HEIGHT = 1536;

ossimRpfCacheTileSource::ossimRpfCacheTileSource()
   :
   ossimImageHandler(),

   // a CADRG and CIBis a 64*64*12 bit buffer and must divide by 8 to
   // convert to bytes   
   m_compressedBuffer(new ossim_uint8[(64*64*12)/8]),

   // whether CIB or CADRG we will alocate the buffer
   // to the larger of the 2 (CADRG is 3 bands) 
   m_uncompressedBuffer(new ossim_uint8[256*256*3]),
   
   m_numberOfLines(0),
   m_numberOfSamples(0),
   m_tile(0),
   m_fileNames(),
   m_tileSize(128, 128),
   m_productType(OSSIM_PRODUCT_TYPE_UNKNOWN),
   m_workFrame(new ossimRpfFrame),
   m_bBox_LL_Lon(0.0),
   m_bBox_LL_Lat(0.0),
   m_bBox_UR_Lon(0.0),
   m_bBox_UR_Lat(0.0),
   m_numOfFramesVertical(0),
   m_numOfFramesHorizontal(0),
   m_frameEntryArray()
{
  if (traceDebug())
  {
    ossimNotify(ossimNotifyLevel_DEBUG)
      << "ossimRpfCacheTileSource::ossimRpfCacheTileSource entered...\n";
#ifdef OSSIM_ID_ENABLED
    ossimNotify(ossimNotifyLevel_DEBUG)
      << "OSSIM_ID:  " << OSSIM_ID << "\n";
#endif      
  }

  m_actualImageRect.makeNan();

}

ossimRpfCacheTileSource::~ossimRpfCacheTileSource()
{
  if(m_compressedBuffer)
  {
    delete [] m_compressedBuffer;
    m_compressedBuffer = 0;
  }
  if(m_uncompressedBuffer)
  {
    delete [] m_uncompressedBuffer;
    m_uncompressedBuffer = 0;
  }
  if(m_workFrame)
  {
    delete m_workFrame;
    m_workFrame = 0;
  }
  close();
}

ossimString ossimRpfCacheTileSource::getShortName()const
{
  return ossimString("RpfCache");
}

ossimString ossimRpfCacheTileSource::getLongName()const
{
  return ossimString("RpfCache reader");
}


void ossimRpfCacheTileSource::close()
{
  deleteAll();
}

bool ossimRpfCacheTileSource::isOpen()const
{
  //return (theTableOfContents!=0);
  ossimString ext = theImageFile.ext().downcase();

  if(ext == "rpf")
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool ossimRpfCacheTileSource::open()
{
  if(traceDebug())
  {
    ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRpfCacheTileSource::open(): Entered....." << std::endl;
  }

  ossimFilename imageFile = theImageFile;
  bool result = true;

  if(isOpen() == false)
  {
    close();
    result = false;
  }

  if(result)
  {
    if (m_fileNames.size() == 0 && m_frameEntryArray.size() == 0)
    {
      if (buildFrameEntryArray(imageFile) == false)
      {
        return false;
      }
    }
    
    setCurrentEntry(0);
    //---
    // Adjust image rect so not to go over the -180 to 180 and -90 to 90
    // bounds.
    //---
    setActualImageRect();

    // Set the base class image file name.
    theImageFile = imageFile;
    m_tile = ossimImageDataFactory::instance()->create(this, this);
    m_tile->initialize();
  }

  if(traceDebug())
  {
    ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRpfCacheTileSource::open(): Leaving at line" << __LINE__ << std::endl;
  }

  return result;
}

bool ossimRpfCacheTileSource::buildFrameEntryArray(ossimFilename imageFile)
{
   static const char MODULE[] = "ossimRpfCacheTileSource::buildFrameEntryArray";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered..." << endl;
   }

   std::ifstream in((imageFile).c_str() );
   
   std::string line;
   ossim_uint32 index = 0; // used throughout
   while( in.good() )
   {
      // Read in a line.
      std::getline(in, line);

      ossimString tmpStr = ossimString(line);
      if (!tmpStr.empty())
      {
         if (index == 0)
         {
            std::vector<ossimString> box_lat_lon;
            tmpStr.split(box_lat_lon, "|");
            
            if (box_lat_lon.size() > 2)
            {
               std::vector<ossimString> ll;
               std::vector<ossimString> ur;
               
               box_lat_lon[0].split(ll, ",");
               box_lat_lon[1].split(ur, ",");
               ossimString bandStr = box_lat_lon[2];
               
               m_bBox_LL_Lon = ll[0].toFloat64();
               m_bBox_LL_Lat = ll[1].toFloat64();
               m_bBox_UR_Lon = ur[0].toFloat64();
               m_bBox_UR_Lat = ur[1].toFloat64();
               
               checkLongitude(m_bBox_LL_Lon, m_bBox_UR_Lon);
               
               ossim_int32 bands = ossimString(bandStr).toInt32();
               if (bands == 1)
               {
                  m_productType = OSSIM_PRODUCT_TYPE_CIB;
               }
               else if (bands == 3)
               {
                  m_productType = OSSIM_PRODUCT_TYPE_CADRG;
               }
            }
            else
            {
               return false;
            }
         }
         else if (index == 1)
         {
            m_fileNames.push_back(tmpStr);
            std::vector<ossimString> frame_lat_lon = tmpStr.split("|");
            if (frame_lat_lon.size() > 2)
            {
               ossimString ll_lon_lat = frame_lat_lon[1];
               ossimString ur_lon_lat = frame_lat_lon[2];
               ossim_float64 ll_lon = ossimString(ll_lon_lat.split(",")[0]).toDouble();
               ossim_float64 ll_lat = ossimString(ll_lon_lat.split(",")[1]).toDouble();
               ossim_float64 ur_lon = ossimString(ur_lon_lat.split(",")[0]).toDouble();
               ossim_float64 ur_lat = ossimString(ur_lon_lat.split(",")[1]).toDouble();

               checkLongitude(ll_lon, ur_lon);
               
               ossim_float64 bBox_lat_diff = std::fabs(m_bBox_UR_Lat - m_bBox_LL_Lat);
               ossim_float64 bBox_lon_diff = std::fabs(m_bBox_UR_Lon - m_bBox_LL_Lon);
               
               ossim_float64 lat_diff = std::fabs(ur_lat - ll_lat);
               ossim_float64 lon_diff = std::fabs(ur_lon - ll_lon);
               
               m_numOfFramesVertical   = static_cast<ossim_uint32>(bBox_lat_diff/lat_diff + 0.5);
               m_numOfFramesHorizontal = static_cast<ossim_uint32>(bBox_lon_diff/lon_diff + 0.5);
               
               m_numberOfLines   = m_numOfFramesVertical*CIBCADRG_FRAME_HEIGHT;
               m_numberOfSamples = m_numOfFramesHorizontal*CIBCADRG_FRAME_WIDTH;

               if ( traceDebug() )
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << "\nscene height in degrees: " << bBox_lat_diff
                     << "\nscene width in degrees:  " << bBox_lon_diff
                     << "\nframe height in degrees: " << lat_diff
                     << "\nframe width in degrees:  " << lon_diff
                     << "\nframes vertical:         " << m_numOfFramesVertical
                     << "\nframes horizontal:       " << m_numOfFramesHorizontal
                     << "\nlines:                   " << m_numberOfLines
                     << "\nsamples:                 " << m_numberOfSamples << "\n";
               }
            }
            else
            {
               return false;
            }
         }
         else
         {
            m_fileNames.push_back(tmpStr);
         }
         
      } // matches: if (!tmpStr.empty())
      
      ++index;
      
   } // matches: while( in.good() )
   in.close();

   m_frameEntryArray.resize(m_numOfFramesVertical);
   for(index = 0; index < m_frameEntryArray.size(); ++index)
   {
      m_frameEntryArray[index].resize(m_numOfFramesHorizontal);
   }

   for(index = 0; index < m_fileNames.size(); ++index)
   {
      ossimString tmpStr = m_fileNames[index];
      std::vector<ossimString> frameInfos = tmpStr.split("|");
      if (frameInfos.size() > 1)
      {
         ossimString fileName = frameInfos[0];
         ossimString ll_lon_lat = frameInfos[1];
         ossimString ur_lon_lat = frameInfos[2];
         double ll_lon = ossimString(ll_lon_lat.split(",")[0]).toDouble();
         double ll_lat = ossimString(ll_lon_lat.split(",")[1]).toDouble();
         double ur_lon = ossimString(ur_lon_lat.split(",")[0]).toDouble();
         double ur_lat = ossimString(ur_lon_lat.split(",")[1]).toDouble();
         
         checkLongitude(ll_lon, ur_lon);
         
         ossim_float64 tmpColPostion = std::fabs(ll_lon - m_bBox_LL_Lon);
         ossim_float64 tmpFrameLon = std::fabs(ur_lon - ll_lon);
         
         ossim_float64 tmpRowPostion = std::fabs(m_bBox_UR_Lat - ur_lat);
         ossim_float64 tmpFrameLat = std::fabs(ur_lat - ll_lat);
         
         ossim_uint32 colNum = static_cast<ossim_uint32>(tmpColPostion/tmpFrameLon + 0.5);
         ossim_uint32 rowNum = static_cast<ossim_uint32>(tmpRowPostion/tmpFrameLat + 0.5);
         
         if (colNum >= m_numOfFramesHorizontal)
         {
            colNum = m_numOfFramesHorizontal-1; // Clamp to last column.
         }
         
         if (rowNum >= m_numOfFramesVertical)
         {
            rowNum = m_numOfFramesVertical-1; // Clamp to last row.
         }
         
         ossimRpfFrameEntry tempEntry;
         tempEntry.setEntry(ossimFilename(""), fileName);
         m_frameEntryArray[rowNum][colNum] = tempEntry;
      }
      else
      {
         return false;
      }
   }

   return true;
}

void ossimRpfCacheTileSource::checkLongitude(ossim_float64& leftLon,
                                             const ossim_float64& rightLon) const
{
   //---
   // Test for scene coordinates being 180 to 180 and adjust leftLon to -180
   // if so.
   //
   // NOTE:
   // Setting tolerance to 1/7200 about 15 meters.
   // Not sure if this is too loose or not. (drb)
   //---
   const ossim_float64 TOLERANCE = 0.000138889; // 1/7200 about 15 meters.

   if ( ossim::almostEqual(leftLon, 180.0, TOLERANCE) )
   {
      if ( ossim::almostEqual(rightLon, 180.0, TOLERANCE) )
      {
         leftLon = -180.0;
      }
   }
}

ossimRefPtr<ossimImageData> ossimRpfCacheTileSource::getTile(
  const  ossimIrect& rect, ossim_uint32 resLevel)
{
  if (m_tile.valid())
  {
    // Image rectangle must be set prior to calling getTile.
    m_tile->setImageRectangle(rect);

    if ( getTile( m_tile.get(), resLevel ) == false )
    {
      if (m_tile->getDataObjectStatus() != OSSIM_NULL)
      {
        m_tile->makeBlank();
      }
    }
  }

  return m_tile;
}

bool ossimRpfCacheTileSource::getTile(ossimImageData* result,
                                      ossim_uint32 resLevel)
{
   bool status = false;
   
   //---
   // Not open, this tile source bypassed, or invalid res level,
   // return a blank tile.
   //---
   if( isOpen() && isSourceEnabled() && isValidRLevel(resLevel) &&
       result && (result->getNumberOfBands() == getNumberOfOutputBands()) &&
       (m_productType != OSSIM_PRODUCT_TYPE_UNKNOWN) )
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

ossim_uint32 ossimRpfCacheTileSource::getNumberOfInputBands()const
{
  switch(m_productType)
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

ossim_uint32 ossimRpfCacheTileSource::getNumberOfOutputBands()const
{
  switch(m_productType)
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

ossim_uint32 ossimRpfCacheTileSource::getNumberOfLines(ossim_uint32 reduced_res_level) const
{
   if (reduced_res_level == 0)
   {
      return m_numberOfLines;
   }
   else if (theOverview.valid())
   {
      return theOverview->getNumberOfLines(reduced_res_level);
   }
   
   return 0;
}

ossim_uint32 ossimRpfCacheTileSource::getNumberOfSamples(ossim_uint32 reduced_res_level) const
{
   if (reduced_res_level == 0)
   {
      return m_numberOfSamples;
   }
   else if (theOverview.valid())
   {
      return theOverview->getNumberOfSamples(reduced_res_level);
   }
   
   return 0;
}

void ossimRpfCacheTileSource::setActualImageRect()
{
   m_actualImageRect = ossimIrect(0,0,m_numberOfLines, m_numberOfSamples);
}

ossimIrect ossimRpfCacheTileSource::getImageRectangle(ossim_uint32 reduced_res_level) const
{
   return ossimIrect(0,                         // upper left x
                     0,                         // upper left y
                     getNumberOfSamples(reduced_res_level) - 1,  // lower right x
                     getNumberOfLines(reduced_res_level)   - 1); // lower right y                     
}

ossimRefPtr<ossimImageGeometry> ossimRpfCacheTileSource::getImageGeometry()
{
   if (theGeometry.valid()) return theGeometry;
   
   // datum
   // WGS 84
   ossimKeywordlist kwl;
   const char* prefix = 0; // legacy
   kwl.add(prefix,
           ossimKeywordNames::DATUM_KW,
           "WGE",
           true);   
   
   ossimGpt ul(m_bBox_UR_Lat,m_bBox_LL_Lon);
   ossimGpt ll(m_bBox_LL_Lat,m_bBox_LL_Lon);
   ossimGpt ur(m_bBox_UR_Lat,m_bBox_UR_Lon);
   ossimGpt lr(m_bBox_LL_Lat,m_bBox_UR_Lon);

   double latInterval = fabs(ul.latd() - lr.latd())/ getNumberOfLines();
   double lonInterval = fabs(ul.lond() - ur.lond())/ getNumberOfSamples();

   kwl.add(prefix,
           ossimKeywordNames::UL_LAT_KW,
           ul.latd(),//-(latInterval/2.0),
           true);

   kwl.add(prefix,
           ossimKeywordNames::UL_LON_KW,
           ul.lond(),//+(lonInterval/2.0),
           true);

   kwl.add(prefix,
           ossimKeywordNames::LL_LAT_KW,
           ll.latd(),//+(latInterval/2.0),
           true);

   kwl.add(prefix,
           ossimKeywordNames::LL_LON_KW,
           ll.lond(),//+(lonInterval/2.0),
           true);

   kwl.add(prefix,
           ossimKeywordNames::LR_LAT_KW,
           lr.latd(),//+(latInterval/2.0),
           true);

   kwl.add(prefix,
           ossimKeywordNames::LR_LON_KW,
           lr.lond(),//-(lonInterval/2.0),
           true);

   kwl.add(prefix,
           ossimKeywordNames::UR_LAT_KW,
           ur.latd(),//-(latInterval/2.0),
           true);

   kwl.add(prefix,
           ossimKeywordNames::UR_LON_KW,
           ur.lond(),//-(latInterval/2.0),
           true);

   kwl.add(prefix,
           ossimKeywordNames::NUMBER_INPUT_BANDS_KW,
           getNumberOfInputBands(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::NUMBER_OUTPUT_BANDS_KW,
           getNumberOfOutputBands(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::NUMBER_LINES_KW,
           getNumberOfLines(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::NUMBER_SAMPLES_KW,
           getNumberOfSamples(),
           true);


   //---
   // Make a projection to get the easting / northing of the tie point and
   // the scale in meters.  This will only be used by the CIB.
   //---
   kwl.add(prefix,
           ossimKeywordNames::DATUM_KW,
           "WGE",
           true);

   ossimGpt origin((ul.latd()+lr.latd())*.5,
                   (ul.lond()+lr.lond())*.5,
                   0.0);

   double deltaLatPerPixel = latInterval;
   double deltaLonPerPixel = lonInterval;

   ossimDpt tie;

   tie.lat = ul.latd() - deltaLatPerPixel/2.0;
   tie.lon = ul.lond() + deltaLonPerPixel/2.0;

   kwl.add(prefix, 
           ossimKeywordNames::TIE_POINT_XY_KW,
           tie.toString(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT,
           deltaLatPerPixel,
           true);

   kwl.add(prefix,
           ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON,
           deltaLonPerPixel,
           true);

   kwl.add(prefix,
           ossimKeywordNames::ORIGIN_LATITUDE_KW,
           origin.latd(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::CENTRAL_MERIDIAN_KW,
           origin.lond(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::TIE_POINT_LAT_KW,
           ul.latd()-(deltaLatPerPixel/2.0),
           true);

   kwl.add(prefix,
           ossimKeywordNames::TIE_POINT_LON_KW,
           ul.lond()+(deltaLonPerPixel/2.0),
           true);

   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           "ossimEquDistCylProjection",
           true);
  
   // Capture this for next time.
   theGeometry = new ossimImageGeometry;
   theGeometry->loadState(kwl, prefix);

   // Set image things the geometry object should know about.
   initImageParameters( theGeometry.get() ); 

   return theGeometry;
}

ossimScalarType ossimRpfCacheTileSource::getOutputScalarType() const
{
   return OSSIM_UCHAR;
}

ossim_uint32 ossimRpfCacheTileSource::getTileWidth() const
{
   return m_tileSize.x;
}

ossim_uint32 ossimRpfCacheTileSource::getTileHeight() const
{
   return m_tileSize.y;
}

bool ossimRpfCacheTileSource::setCurrentEntry(ossim_uint32 entryIdx)
{
   // Clear the geometry.
   theGeometry = 0;

   // Must clear or openOverview will use last entries.
   theOverviewFile.clear();

   if(!setEntryToRender(entryIdx))
   {
      return false;
   }
   completeOpen();

   return true;
}

void ossimRpfCacheTileSource::getEntryList(std::vector<ossim_uint32>& entryList)const
{
   entryList.push_back(0);
}

bool ossimRpfCacheTileSource::setEntryToRender(ossim_uint32 /* index */)
{
   if(isOpen())
   { 
      populateLut();
      return true;
   }

   return false;
}

bool ossimRpfCacheTileSource::isValidRLevel(ossim_uint32 reduced_res_level) const
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

vector<ossimRpfCacheTileSource::ossimFrameEntryData> ossimRpfCacheTileSource::getIntersectingEntries(const ossimIrect& rect)
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

      for(ossim_int32 row = frameRect.ul().y; row <= frameRect.lr().y; ++row)
      {
         for(ossim_int32 col = frameRect.ul().x; col <= frameRect.lr().x; ++col)
         {
            ossimRpfFrameEntry tempEntry = m_frameEntryArray[row][col];
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

   return result;
}

void ossimRpfCacheTileSource::fillTile(
   const ossimIrect& tileRect,
   const vector<ossimFrameEntryData>& framesInvolved,
   ossimImageData* tile)
{
   ossim_uint32 idx = 0;
   for(idx = 0;
       idx < framesInvolved.size();
       ++idx)
   {

      if(m_workFrame->parseFile(framesInvolved[idx].theFrameEntry.getFullPath())
         == ossimErrorCodes::OSSIM_OK)
      {
         // we will fill a subtile.  We pass in which frame it is and the position of the frame.
         // the actual pixel will be 1536*row and 1536 *col.
         if(m_productType == OSSIM_PRODUCT_TYPE_CIB)
         {
            fillSubTileCib(*m_workFrame,
                           tileRect,
                           framesInvolved[idx],
                           tile);
         }
         else
         {
            fillSubTileCadrg(*m_workFrame,
                             tileRect,
                             framesInvolved[idx],
                             tile);
         }
      }
   }
}

void ossimRpfCacheTileSource::fillSubTileCadrg(
   const ossimRpfFrame&  aFrame,
   const ossimIrect& tileRect,
   const ossimFrameEntryData& frameEntryData,
   ossimImageData* tile)
{;
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

   tempRows[0] = m_uncompressedBuffer;
   tempRows[1] = (m_uncompressedBuffer + 256*256);
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
         if(aFrame.fillSubFrameBuffer(m_compressedBuffer, 0, row, col))
         {
            for (i = 0; i < 256; i += 4)
            {
               for (j = 0; j < 256; j += 8)
               {
                  ossim_uint16 firstByte  = m_compressedBuffer[readPtr++] & 0xff;
                  ossim_uint16 secondByte = m_compressedBuffer[readPtr++] & 0xff;
                  ossim_uint16 thirdByte  = m_compressedBuffer[readPtr++] & 0xff;

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
            memset(m_uncompressedBuffer, 0, 256*256*3);
         }
         ossim_int32 tempCol = col*256;
         ossim_int32 tempRow = row*256;
         ossimIrect subRectToFill(frameRect.ul().x + tempCol,
                                  frameRect.ul().y + tempRow,
                                  frameRect.ul().x + tempCol + 255,
                                  frameRect.ul().y + tempRow + 255);
         tile->loadTile(m_uncompressedBuffer,
                        subRectToFill,
                        OSSIM_BSQ);
      }
   }
}

void ossimRpfCacheTileSource::fillSubTileCib(
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

      tempRow = m_uncompressedBuffer;

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
            if(aFrame.fillSubFrameBuffer(m_compressedBuffer, 0, row, col))
            {
               for (i = 0; i < 256; i += 4)
               {
                  for (j = 0; j < 256; j += 8)
                  {
                     ossim_uint16 firstByte  = m_compressedBuffer[readPtr++] & 0xff;
                     ossim_uint16 secondByte = m_compressedBuffer[readPtr++] & 0xff;
                     ossim_uint16 thirdByte  = m_compressedBuffer[readPtr++] & 0xff;

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
               memset(m_uncompressedBuffer, 0, 256*256);
            }
            ossim_int32 tCol = col*256;
            ossim_int32 tRow = row*256;
            ossimIrect subRectToFill(frameRect.ul().x + tCol,
                                     frameRect.ul().y + tRow,
                                     frameRect.ul().x + tCol + 255,
                                     frameRect.ul().y + tRow + 255);
            tile->loadTile(m_uncompressedBuffer,
                           subRectToFill,
                           OSSIM_BSQ);
         }
      }
   }  
}

void ossimRpfCacheTileSource::allocateForProduct()
{
   if(m_productType ==  OSSIM_PRODUCT_TYPE_UNKNOWN)
   {
      return;
   }
   if(m_uncompressedBuffer)
   {
      delete [] m_uncompressedBuffer;
      m_uncompressedBuffer = 0;
   }
   if(m_compressedBuffer)
   {
      delete [] m_compressedBuffer;
      m_compressedBuffer = 0;
   }

   // a CADRG and CIBis a 64*64*12 bit buffer and must divide by 8 to
   // convert to bytes
   m_compressedBuffer   = new ossim_uint8[(64*64*12)/8];
   if(m_productType == OSSIM_PRODUCT_TYPE_CIB)
   {
      m_uncompressedBuffer = new ossim_uint8[256*256];
   }
   else
   {
      m_uncompressedBuffer = new ossim_uint8[256*256*3];
   }

   m_tile = ossimImageDataFactory::instance()->create(this, this);
   m_tile->initialize();
}

void ossimRpfCacheTileSource::deleteAll()
{
   theOverview = 0;
}

bool ossimRpfCacheTileSource::saveState(ossimKeywordlist& kwl,
                                        const char* prefix)const
{
   bool result = ossimImageHandler::saveState(kwl, prefix);

   return result;
}

bool ossimRpfCacheTileSource::loadState(const ossimKeywordlist& kwl,
                                        const char* prefix)
{
   const char* MODULE = "ossimRpfCacheTileSource::loadState";

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

ossim_uint32 ossimRpfCacheTileSource::getImageTileWidth() const
{
   return 256;
}

ossim_uint32 ossimRpfCacheTileSource::getImageTileHeight() const
{
   return 256;
}

bool ossimRpfCacheTileSource::isCib() const
{
   return (m_productType==OSSIM_PRODUCT_TYPE_CIB);
}

bool ossimRpfCacheTileSource::isCadrg() const
{
   return (m_productType==OSSIM_PRODUCT_TYPE_CADRG);
}

ossimRefPtr<ossimProperty> ossimRpfCacheTileSource::getProperty(const ossimString& name)const
{
   if(name == "file_type")
   {
      if(m_productType == OSSIM_PRODUCT_TYPE_CIB)
      {
         return new ossimStringProperty("file_type", "CIB");
      }
      else if(m_productType == OSSIM_PRODUCT_TYPE_CADRG)
      {
         return new ossimStringProperty("file_type", "CADRG");
      }
      return 0;
   }
   return ossimImageHandler::getProperty(name);
}

void ossimRpfCacheTileSource::populateLut()
{
   theLut = 0;
   if(m_fileNames.size() > 0)
   {
      // bool found = false;
      ossimRpfFrameEntry tempEntry;
      ossimRpfFrame aFrame;
      ossimFilename fileName = m_fileNames[0].split("|")[0];
      // if (fileName.exists())
      // {
      //    found = true;
      // }

      if(aFrame.parseFile(fileName)
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
            if((m_productType == OSSIM_PRODUCT_TYPE_CIB)||
               (m_productType == OSSIM_PRODUCT_TYPE_CADRG))
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
               switch(m_productType)
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

void ossimRpfCacheTileSource::establishDecimationFactors()
{
   theDecimationFactors.clear();
   
   // Just needed to set the first R level here, the base class can do the rest:
   ossimImageHandler::establishDecimationFactors();
}

