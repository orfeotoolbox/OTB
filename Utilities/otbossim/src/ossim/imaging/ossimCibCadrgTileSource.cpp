//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//********************************************************************
// $Id: ossimCibCadrgTileSource.cpp 10245 2007-01-12 21:51:13Z dburken $
#include <algorithm>
using namespace std;

#include <ossim/imaging/ossimCibCadrgTileSource.h>

#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimNBandLutDataObject.h>
#include <ossim/imaging/ossimHistogramWriter.h>
#include <ossim/imaging/ossimImageHistogramSource.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
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
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimCylEquAreaProjection.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimTrace.h>

static ossimTrace traceDebug = ossimTrace("ossimCibCadrgTileSource:debug");

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimCibCadrgTileSource.cpp 10245 2007-01-12 21:51:13Z dburken $";
#endif

RTTI_DEF1(ossimCibCadrgTileSource, "ossimCibCadrgTileSource", ossimImageHandler)

const ossim_uint32 ossimCibCadrgTileSource::CIBCADRG_FRAME_WIDTH  = 1536;
const ossim_uint32 ossimCibCadrgTileSource::CIBCADRG_FRAME_HEIGHT = 1536;

ossimCibCadrgTileSource::ossimCibCadrgTileSource()
   :ossimImageHandler(),
    theCompressedBuffer(NULL),
    theUncompressedBuffer(NULL),
    theNumberOfLines(0),
    theNumberOfSamples(0),
    theTile(NULL),
    theTableOfContents(NULL),
    theEntryToRender(NULL),
    theEntryNumberToRender(1),
    theTileSize(128, 128),
    theProductType(OSSIM_PRODUCT_TYPE_UNKNOWN)
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
   
   theActualImageRect.makeNan();

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
}

bool ossimCibCadrgTileSource::isOpen()const
{
   return (theTableOfContents!=NULL);
}

bool ossimCibCadrgTileSource::open()
{
   if ( theImageFile.isRelative() )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimCibCadrgTileSource::open DEBUG:"
            << "\ntheImageFile:  " << theImageFile
            << "\nHas a relative path which will not work with this object!"
            << "\nReturning false..."
            << endl;
      }
      return false;
   }

   ossimFilename imageFile = theImageFile;
   
   bool result = true;
   if(isOpen())
   {
      close();
   }
   
   theTableOfContents = new ossimRpfToc;
   
   if(theTableOfContents)
   {      
      if(theTableOfContents->parseFile(imageFile) == ossimErrorCodes::OSSIM_OK)
      {
         if(theTableOfContents->getNumberOfEntries() > 0)
         {
            vector<ossimString> scaleList = getProductScaleList();
            if(scaleList.size() > 0)
            {
               vector<ossim_int32>  entryList = getProductEntryList(scaleList[0]);
               if(entryList.size() > 0)
               {
                  //open(imageFile, entryList[0]);
                  setCurrentEntry(entryList[0]);
                  
                  if(theEntryToRender)
                  {
                     // a CADRG is 1536x1536 per frame.
                     theNumberOfLines   = theEntryToRender->getNumberOfFramesVertical()*CIBCADRG_FRAME_HEIGHT;
                     theNumberOfSamples = theEntryToRender->getNumberOfFramesHorizontal()*CIBCADRG_FRAME_WIDTH;
                  }
                  else // we didn't find a frame to render
                  {
                     result = false;
                  }
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
                     result = false;
                  }
                  allocateForProduct();
               }
               else
               {
                  result = false;
               }
            }
            else
            {
               result = false;
            }
         }
         else
         {
            result = false;
         }
      }
      else
      {
         result = false;
      }
   }
   else
   {
      result = false;
   }

   if(!result)
   {
      close();
   }
   else
   {
      //---
      // Adjust image rect so not to go over the -180 to 180 and -90 to 90
      // bounds.
      //---
      setActualImageRect();

      // Set the base class image file name.
      theImageFile = imageFile;

      completeOpen();

      theTile = ossimImageDataFactory::instance()->create(this, this);
      theTile->initialize();
   }

   return result;
}

ossimRefPtr<ossimImageData> ossimCibCadrgTileSource::getTile(
   const  ossimIrect& rect, ossim_uint32 resLevel)
{
   ossimIpt origin = rect.ul();
      
   if(!isOpen())
   {
      return ossimRefPtr<ossimImageData>();
   }
   ossimIrect imageRect = getImageRectangle();
   if( (!rect.intersects(imageRect) )||
       (theProductType == OSSIM_PRODUCT_TYPE_UNKNOWN) )
   {
      return ossimRefPtr<ossimImageData>();
   }
   
   theTile->setImageRectangle(rect);
   if (theOverview)
   {
      if (theOverview->hasR0() || resLevel)
      {
         return theOverview->getTile(rect, resLevel);
      }
   }

   //---
   // Start with a blank tile in case there is not total coverage for rect.
   //---
   theTile->makeBlank();
   
   vector<ossimFrameEntryData> frames = getIntersectingEntries(rect);
   if(frames.size() > 0)
   {
      //---
      // Now lets render each frame.  Note we will have to find subframes
      // that intersect the rectangle of interest for each frame.
      //---
      fillTile(rect, frames);

      // Revalidate tile status.
      theTile->validate();
   }
   
   return theTile;
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
   else if (theOverview)
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
   else if (theOverview)
   {
      return theOverview->getNumberOfSamples(reduced_res_level);
   }

   return 0;
}

void ossimCibCadrgTileSource::setActualImageRect()
{
   ossimRpfBoundaryRectRecord boundaryInfo = theEntryToRender->getBoundaryInformation();
   ossimGpt ul(boundaryInfo.getCoverage().getUlLat(),
               boundaryInfo.getCoverage().getUlLon());
//    ossimGpt ll(boundaryInfo.getCoverage().getLlLat(),
//                boundaryInfo.getCoverage().getLlLon());
//    ossimGpt ur(boundaryInfo.getCoverage().getUrLat(),
//                boundaryInfo.getCoverage().getUrLon());
   ossimGpt lr(boundaryInfo.getCoverage().getLrLat(),
               boundaryInfo.getCoverage().getLrLon());
   
   double latInterval = boundaryInfo.getCoverage().getVerticalInterval();
   double lonInterval = boundaryInfo.getCoverage().getHorizontalInterval();
   
   int lines   = irint(fabs(ul.lat - lr.lat)/latInterval);
   int samples = irint(fabs(ul.lon - lr.lon)/lonInterval);


   theNumberOfLines   = lines;
   theNumberOfSamples = samples;

   theActualImageRect = ossimIrect(0,0,theNumberOfLines, theNumberOfSamples);
   
}

ossimIrect ossimCibCadrgTileSource::getImageRectangle(ossim_uint32 reduced_res_level) const
{
   return ossimIrect(0,                         // upper left x
                     0,                         // upper left y
                     getNumberOfSamples(reduced_res_level) - 1,  // lower right x
                     getNumberOfLines(reduced_res_level)   - 1); // lower right y                     
}
   

   
bool ossimCibCadrgTileSource::getImageGeometry(ossimKeywordlist& kwl,
                                            const char* prefix)
{
   if(!theEntryToRender)
   {
      return false;
   }

   if (theGeometryKwl.getSize())
   {
      kwl = theGeometryKwl;
      return true;
   }

   // datum
   // WGS 84
   kwl.add(prefix,
           ossimKeywordNames::DATUM_KW,
           "WGE",
           true);   

   ossimRpfBoundaryRectRecord boundaryInfo = theEntryToRender->getBoundaryInformation();

   ossimGpt ul(boundaryInfo.getCoverage().getUlLat(),
               boundaryInfo.getCoverage().getUlLon());
   ossimGpt ll(boundaryInfo.getCoverage().getLlLat(),
               boundaryInfo.getCoverage().getLlLon());
   ossimGpt ur(boundaryInfo.getCoverage().getUrLat(),
               boundaryInfo.getCoverage().getUrLon());
   ossimGpt lr(boundaryInfo.getCoverage().getLrLat(),
               boundaryInfo.getCoverage().getLrLon());

   
//    double latInterval = fabs(ul.latd() - lr.latd())/ getNumberOfLines();
//    double lonInterval = fabs(ul.lond() - ur.lond())/ getNumberOfSamples();
   double latInterval = boundaryInfo.getCoverage().getVerticalInterval();
   double lonInterval = boundaryInfo.getCoverage().getHorizontalInterval();

     
   // double latInterval = boundaryInfo.getCoverage().getVerticalInterval();
   // double lonInterval = boundaryInfo.getCoverage().getHorizontalInterval();

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
   

   //***
   // Make a projection to get the easting / northing of the tie point and
   // the scale in meters.  This will only be used by the CIB.
   //***
//   const ossimDatum* datum = ossimDatumFactory::instance()->wgs84();

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

   tie.lat = boundaryInfo.getCoverage().getUlLat() - deltaLatPerPixel/2.0;
   tie.lon = boundaryInfo.getCoverage().getUlLon() + deltaLonPerPixel/2.0;
   
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
   
   
   int z = boundaryInfo.getZone();
   
   if (z == 74) z--; // Fix J to a zone.
   if (z > 64) z -= 64; // Below the equator
   else z -= 48; // Above the equator
   
   kwl.add(prefix,
           ossimKeywordNames::ZONE_KW,
            z,
            true);

    if(z!=9)
    {
       kwl.add(prefix,
               ossimKeywordNames::TYPE_KW,
	       "ossimEquDistCylProjection",
               true);
    }
    else
    {
       kwl.add(prefix,
               ossimKeywordNames::TYPE_KW,
               "ossimAzimEquDistProjection",
               true);
    }
    
    if(theProductType == OSSIM_PRODUCT_TYPE_CADRG)
    {
       
       ossimString scale = boundaryInfo.getScale();
       scale = scale.after(":").upcase();
       scale = scale.trim();

       double scaleValue = 1000000;
       if(scale == "5M")
       {
          scaleValue = 5000000;
       }
       else if(scale == "2M")
       {
          scaleValue = 2000000;
       }
       else if(scale == "1M")
       {
          scaleValue = 1000000;
       }
       else if(scale == "500K")
       {
          scaleValue = 500000;
       }
       else if(scale == "250K")
       {
          scaleValue = 250000; 
       }
       else if(scale == "200K")
       {
          scaleValue = 200000;
       }
       else if(scale == "100K")
       {
          scaleValue = 100000; 
       }
       else if(scale == "50K")
       {
          scaleValue = 50000;
       }
       else if(scale == "25K")
       {
          scaleValue = 25000;
       }
       else if(scale == "20K")
       {
          scaleValue = 20000;
       }
       else if(scale == "15K")
       {
          scaleValue = 15000;
       }
       else if(scale == "12,500")
       {
          scaleValue = 12500;
       }
       else if(scale == "10K")
       {
          scaleValue = 10000;
       }
       else
       {
          ossimNotify(ossimNotifyLevel_NOTICE)
             << "ossimCibCadrgTileSource::getImageGeometry: scale not "
             << "supported for projection --> " << scale << endl;
       }
       
       kwl.add(prefix,
               "map_scale",
               scaleValue,
               true);        
    }
    else if(theProductType ==  OSSIM_PRODUCT_TYPE_CIB)
    {
       ossimString scale = boundaryInfo.getScale();
       scale = scale.upcase();
       scale = scale.trim('M');

       // from the CIB spec the scale is equivalent to
       // a CADRG by the following formula
       //
       // 100*10^-6/<meter product>
       //
       // where <meter product is 10, 5 ....etc
       //
       double scaleValue = 1.0/((100.0/1000000.0) / scale.toDouble());

       kwl.add(prefix,
               "map_scale",
               scaleValue,
               true);        
    }

    setImageGeometry(kwl);
    
    return true;
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
   theGeometryKwl.clear();
   if(!setEntryToRender(entryIdx))
   {
      return false;
   }
   completeOpen();

   return true;
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
      // a CIB is 1536x1536 per frame.
      theEntryToRender = entry;
      theNumberOfLines   = theEntryToRender->getNumberOfFramesVertical()*CIBCADRG_FRAME_HEIGHT;
      theNumberOfSamples = theEntryToRender->getNumberOfFramesHorizontal()*CIBCADRG_FRAME_HEIGHT;
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
   else if (theOverview)
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
                (scale[(int)scale.size()-1]=='M'))
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
            // if the entry is not empty then add it to the list.
            if( entry->isEmpty() == false )
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

void ossimCibCadrgTileSource::fillTile(const ossimIrect& tileRect,
                                       const vector<ossimFrameEntryData>& framesInvolved)
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
                           framesInvolved[idx]);
         }
         else
         {
            fillSubTileCadrg(*theWorkFrame,
                             tileRect,
                             framesInvolved[idx]);
         }
      }
   }
}

void ossimCibCadrgTileSource::fillSubTileCadrg(const ossimRpfFrame&  aFrame,
                                               const ossimIrect& tileRect,
                                               const ossimFrameEntryData& frameEntryData)
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
   const vector<ossimRpfColorGrayscaleTable>& colorTable =
      aFrame.getColorGrayscaleTable();
   
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
         theTile->loadTile(theUncompressedBuffer,
                              subRectToFill,
                              OSSIM_BSQ);
      }
   }
}

void ossimCibCadrgTileSource::fillSubTileCib(const ossimRpfFrame&  aFrame,
                                               const ossimIrect& tileRect,
                                               const ossimFrameEntryData& frameEntryData)
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

      const vector<ossimRpfColorGrayscaleTable>& colorTable =
                                        aFrame.getColorGrayscaleTable();

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
            theTile->loadTile(theUncompressedBuffer,
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
      theUncompressedBuffer = NULL;
   }
   if(theCompressedBuffer)
   {
      delete [] theCompressedBuffer;
      theCompressedBuffer = NULL;
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
   
   return NULL;
}


void ossimCibCadrgTileSource::deleteAll()
{
   if(theOverview)
   {
      delete theOverview;
      theOverview = NULL;
   }
   if(theTableOfContents)
   {
      delete theTableOfContents;
      theTableOfContents = NULL;
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
      return false;
   }
   const char* lookup = NULL;
   
   lookup = kwl.find(prefix, "entry");
   ossim_int32 entry = ossimString(lookup).toLong();

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

         ossim_uint32 numElements = colorTable[0].getNumberOfElements();
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
