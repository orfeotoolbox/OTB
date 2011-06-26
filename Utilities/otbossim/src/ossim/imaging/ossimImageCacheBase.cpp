//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Mingjie Su
//
//********************************************************************
// $Id: ossimImageCacheBase.cpp 2685 2011-06-07 16:24:41Z david.burken $
#include <algorithm>
using namespace std;

#include <ossim/imaging/ossimImageCacheBase.h>

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

static ossimTrace traceDebug = ossimTrace("ossimImageCacheBase:debug");

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimImageCacheBase.cpp 2685 2011-06-07 16:24:41Z david.burken $";
#endif

RTTI_DEF1(ossimImageCacheBase, "ossimImageCacheBase", ossimImageHandler)

ossimImageCacheBase::ossimImageCacheBase()
   :
   ossimImageHandler(),
   m_actualImageRect(),
   m_numberOfLines(0),
   m_numberOfSamples(0),
   m_numberOfBands(0),
   m_fileNames(),
   m_tileSize(128, 128),
   m_workFrame(new ossimRpfFrame),
   m_bBox_LL_Lon(0.0),
   m_bBox_LL_Lat(0.0),
   m_bBox_UR_Lon(0.0),
   m_bBox_UR_Lat(0.0),
   m_numOfFramesVertical(0),
   m_numOfFramesHorizontal(0),
   m_frame_width(0),
   m_frame_height(0),
   m_frameEntryArray()
{
  if (traceDebug())
  {
    ossimNotify(ossimNotifyLevel_DEBUG)
      << "ossimImageCacheBase::ossimImageCacheBase entered...\n";
#ifdef OSSIM_ID_ENABLED
    ossimNotify(ossimNotifyLevel_DEBUG)
      << "OSSIM_ID:  " << OSSIM_ID << "\n";
#endif      
  }

  m_actualImageRect.makeNan();

}

ossimImageCacheBase::~ossimImageCacheBase()
{
  if(m_workFrame)
  {
    delete m_workFrame;
    m_workFrame = 0;
  }
  close();
}

void ossimImageCacheBase::close()
{
  deleteAll();
}

bool ossimImageCacheBase::isOpen()const
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

bool ossimImageCacheBase::buildFrameEntryArray(ossimFilename imageFile, 
                                               ossim_uint32 frameWidth, 
                                               ossim_uint32 frameHeight)
{
   static const char MODULE[] = "ossimImageCacheBase::buildFrameEntryArray";
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered..." << endl;
   }

   // use to check if tiles have overlap pixels
   ossim_float64 avgLat = 0.0;
   ossim_float64 avgLon = 0.0;

   m_frame_width = frameWidth;
   m_frame_height = frameHeight;

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
               
               m_numberOfBands = ossimString(bandStr).toInt32();
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

               avgLon = std::fabs(bBox_lon_diff/m_numOfFramesHorizontal);
               avgLat = std::fabs(bBox_lat_diff/m_numOfFramesVertical);
               
               m_numberOfLines   = m_numOfFramesVertical*m_frame_height;
               m_numberOfSamples = m_numOfFramesHorizontal*m_frame_width;

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

         if (std::fabs(tmpFrameLon - avgLon) > 0.002)
         {
            ossimNotify(ossimNotifyLevel_WARN) << fileName << " has overlap pixels with other image." << std::endl;
            return false;
         }
         
         ossim_float64 tmpRowPostion = std::fabs(m_bBox_UR_Lat - ur_lat);
         ossim_float64 tmpFrameLat = std::fabs(ur_lat - ll_lat);

         if (std::fabs(tmpFrameLat - avgLat) > 0.002)
         {
            ossimNotify(ossimNotifyLevel_WARN) << fileName << " has overlap pixels with other image." << std::endl;
            return false;
         }
         
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

void ossimImageCacheBase::checkLongitude(ossim_float64& leftLon,
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

ossim_uint32 ossimImageCacheBase::getNumberOfInputBands()const
{
   return m_numberOfBands;
}

ossim_uint32 ossimImageCacheBase::getNumberOfOutputBands()const
{
  return m_numberOfBands;
}

ossim_uint32 ossimImageCacheBase::getNumberOfLines(ossim_uint32 reduced_res_level) const
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

ossim_uint32 ossimImageCacheBase::getNumberOfSamples(ossim_uint32 reduced_res_level) const
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

void ossimImageCacheBase::setActualImageRect()
{
   m_actualImageRect = ossimIrect(0,0,m_numberOfLines, m_numberOfSamples);
}

ossimIrect ossimImageCacheBase::getImageRectangle(ossim_uint32 reduced_res_level) const
{
   return ossimIrect(0,                         // upper left x
                     0,                         // upper left y
                     getNumberOfSamples(reduced_res_level) - 1,  // lower right x
                     getNumberOfLines(reduced_res_level)   - 1); // lower right y                     
}



ossim_uint32 ossimImageCacheBase::getTileWidth() const
{
   return m_tileSize.x;
}

ossim_uint32 ossimImageCacheBase::getTileHeight() const
{
   return m_tileSize.y;
}

void ossimImageCacheBase::getEntryList(std::vector<ossim_uint32>& entryList)const
{
   entryList.push_back(0);
}

bool ossimImageCacheBase::isValidRLevel(ossim_uint32 reduced_res_level) const
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

vector<ossimImageCacheBase::ossimFrameEntryData> ossimImageCacheBase::getIntersectingEntries(const ossimIrect& rect)
{
   vector<ossimFrameEntryData> result;

   // make sure we have the Toc entry to render
   if(!isOpen()) return result;

   ossimIrect imageRect = getImageRectangle();
   if(rect.intersects(imageRect))
   {
      ossimIrect clipRect  = rect.clipToRect(imageRect);
      ossimIrect frameRect(clipRect.ul().x/m_frame_width,
                           clipRect.ul().y/m_frame_height,
                           clipRect.lr().x/m_frame_width,
                           clipRect.lr().y/m_frame_height);

      for(ossim_int32 row = frameRect.ul().y; row <= frameRect.lr().y; ++row)
      {
         for(ossim_int32 col = frameRect.ul().x; col <= frameRect.lr().x; ++col)
         {
            ossimRpfFrameEntry tempEntry = m_frameEntryArray[row][col];
            if(tempEntry.exists())
            {
               result.push_back(ossimFrameEntryData(row,
                                                    col,
                                                    row*m_frame_height,
                                                    col*m_frame_width,
                                                    tempEntry));
            }
         }
      }
   }

   return result;
}

void ossimImageCacheBase::deleteAll()
{
   theOverview = 0;
}

bool ossimImageCacheBase::saveState(ossimKeywordlist& kwl,
                                        const char* prefix)const
{
   bool result = ossimImageHandler::saveState(kwl, prefix);

   return result;
}

bool ossimImageCacheBase::loadState(const ossimKeywordlist& kwl,
                                        const char* prefix)
{
   const char* MODULE = "ossimImageCacheBase::loadState";

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

ossim_uint32 ossimImageCacheBase::getImageTileWidth() const
{
   return 256;
}

ossim_uint32 ossimImageCacheBase::getImageTileHeight() const
{
   return 256;
}

void ossimImageCacheBase::establishDecimationFactors()
{
   theDecimationFactors.clear();
   
   // Just needed to set the first R level here, the base class can do the rest:
   ossimImageHandler::establishDecimationFactors();
}

ossimRefPtr<ossimImageGeometry> ossimImageCacheBase::getImageGeometry()
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

