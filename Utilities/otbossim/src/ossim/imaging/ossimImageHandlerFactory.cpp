//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id: ossimImageHandlerFactory.cpp 15833 2009-10-29 01:41:53Z eshirschorn $
#include <ossim/imaging/ossimImageHandlerFactory.h>
#include <ossim/imaging/ossimAdrgTileSource.h>
#include <ossim/imaging/ossimCcfTileSource.h>
#include <ossim/imaging/ossimCibCadrgTileSource.h>
#include <ossim/imaging/ossimDoqqTileSource.h>
#include <ossim/imaging/ossimDtedTileSource.h>
#include <ossim/imaging/ossimNitfTileSource.h>
#include <ossim/imaging/ossimQuickbirdNitfTileSource.h>
#include <ossim/imaging/ossimSrtmTileSource.h>
#include <ossim/imaging/ossimQuickbirdTiffTileSource.h>
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/imaging/ossimUsgsDemTileSource.h>
#include <ossim/imaging/ossimLandsatTileSource.h>
#include <ossim/imaging/ossimGeneralRasterTileSource.h>
#include <ossim/imaging/ossimERSTileSource.h>
#include <ossim/imaging/ossimVpfTileSource.h>
#include <ossim/imaging/ossimTileMapTileSource.h>
#include <ossim/imaging/ossimVirtualImageHandler.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimJpegTileSource.h>
//#include <ossim/imaging/ossimRadarSatTileSource.h>
// #include <ossim/imaging/ossimRadarSat2TileSource.h>
// #include <ossim/imaging/ossimTerraSarTileSource.h>

static const ossimTrace traceDebug("ossimImageHandlerFactory:debug");

RTTI_DEF1(ossimImageHandlerFactory, "ossimImageHandlerFactory", ossimImageHandlerFactoryBase);

ossimImageHandlerFactory* ossimImageHandlerFactory::theInstance = 0;
ossimImageHandlerFactory::~ossimImageHandlerFactory()
{
   theInstance = (ossimImageHandlerFactory*)0;
}

ossimImageHandlerFactory* ossimImageHandlerFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimImageHandlerFactory;

      // let's turn off tiff error reporting
      TIFFSetErrorHandler(0);
      TIFFSetWarningHandler(0);
   }

   return theInstance;
}

ossimImageHandler* ossimImageHandlerFactory::open(
   const ossimFilename& fileName)const
{
   ossimFilename copyFilename = fileName;
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandlerFactory::open(filename) DEBUG: entered..."
         << std::endl
         << "Attempting to open file " << copyFilename << std::endl;
   }
   ossimRefPtr<ossimImageHandler> result = 0;

   // Check for empty file.
   copyFilename.trim();
   if (copyFilename.empty())
   {
      return result.release();
   }

   // for all of our imagehandlers the filename must exist.
   // if we have any imagehandlers that require an encoded string and is contrlled in this factory then
   // we need to move this.
   if(!copyFilename.exists()) return 0;

   ossimString ext = copyFilename.ext().downcase();
   
   if(ext == "gz")
   {
      copyFilename = copyFilename.setExtension("");
   }

   // Try opening from extension logic first.
   result = openFromExtension(copyFilename);
   {
      if (result.valid())
      {
         return result.release();
      }
   }

   //---
   // If here do it the brute force way by going down the list of available
   // readers...
   //---

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying OSSIM Virtual Image" << std::endl;
   }
   result = new ossimVirtualImageHandler;
   if(result->open(copyFilename))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying JPEG" << std::endl;
   }
   result = new ossimJpegTileSource;
   if(result->open(copyFilename))
   {
      return result.release();
   }
   result = 0;


   // test if TileMap
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying TileMap"
         << std::endl;
   }
   result = new ossimTileMapTileSource;
   if(result->open(copyFilename))
   {
      return result.release();
   }
   result = 0;

      // test if Radarsat
//   if(traceDebug())
//   {
//      ossimNotify(ossimNotifyLevel_DEBUG)
//         << "Radarsat"
//         << std::endl;
//   }
//   result = new ossimRadarSatTileSource;
//   if(result->open(copyFilename))
//   {
//      return result.release();
//   }
//   result = 0;

    // test if Radarsat2
//    if(traceDebug())
//    {
//       ossimNotify(ossimNotifyLevel_DEBUG)
//          << "Radarsat2"
//          << std::endl;
//    }
//    result = new ossimRadarSat2TileSource;
//    if(result->open(copyFilename))
//    {
//       return result.release();
//    }
//    result = 0;

      // test if TerraSAR
//    if(traceDebug())
//    {
//       ossimNotify(ossimNotifyLevel_DEBUG)
//          << "trying TerraSAR"
//          << std::endl;
//    }
//    result = new ossimTerraSarTileSource;
//    if(result->open(copyFilename))
//    {
//       return result.release();
//    }
//    result = 0;

   // this must be checked first before the TIFF handler
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying Quickbird TIFF"
         << std::endl;
   }
   result = new ossimQuickbirdTiffTileSource;
   if(result->open(copyFilename))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying tif" << std::endl;
   }
   // test for tiff
   result = new ossimTiffTileSource;
   if(result->open(copyFilename))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying CIB/CADRG"
         << std::endl;
   }
   result = new ossimCibCadrgTileSource;
   if(result->open(copyFilename))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying DOQQ" << std::endl;
   }
   result = new ossimDoqqTileSource;
   if(result->open(copyFilename))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying DTED" << std::endl;
   }
   result = new ossimDtedTileSource;
   if(result->open(copyFilename))
   {
      return result.release();
   }

   result = 0;

   // this must be checked first before the NITF raw handler

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying Quickbird Nitf"
         << std::endl;
   }
   result = new ossimQuickbirdNitfTileSource;
   if(result->open(copyFilename))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying Nitf" << std::endl;
   }
   result = new ossimNitfTileSource;
   if(result->open(copyFilename))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying USGS Dem"
         << std::endl;
   }
   result = new ossimUsgsDemTileSource;
   if(result->open(copyFilename))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying Landsat"
         << std::endl;
   }
   result = new ossimLandsatTileSource;
   if(result->open(copyFilename))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying Vpf" << std::endl;
   }
   result = new ossimVpfTileSource;
   if(result->open(copyFilename))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying ERS" << std::endl;
   }
   result = new ossimERSTileSource;
   if(result->open(copyFilename))
   {
      return result.release();
   }
   result = 0;

   // Note:  SRTM should be in front of general raster...
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying SRTM"
         << std::endl;
   }
   result = new ossimSrtmTileSource;
   if(result->open(copyFilename))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying General Raster"
         << std::endl;
   }
   result = new ossimGeneralRasterTileSource;
   if(result->open(copyFilename))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
      << "trying adrg" << std::endl;
   }
   
   // test if ADRG
   result  = new ossimAdrgTileSource();
   
   if(result->open(copyFilename))
   {
      return result.release();
   }
   result = 0;
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
      << "trying ccf" << std::endl;
   }
   // test if ccf
   result = new ossimCcfTileSource();
   if(result->open(copyFilename))
   {
      return result.release();
   }
   
   result = 0;
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandlerFactory::open(filename) DEBUG: returning..." << std::endl;
   }
   return (ossimImageHandler*)0;
}

ossimImageHandler* ossimImageHandlerFactory::open(const ossimKeywordlist& kwl,
                                                  const char* prefix)const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandlerFactory::open(kwl, prefix) DEBUG: entered..."
         << std::endl;
   }
   ossimRefPtr<ossimImageHandler> result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying adrg" << std::endl;
   }
   result = new ossimAdrgTileSource();

   if(result->loadState(kwl, prefix))
   {
      return result.release();
   }
   result = 0;
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying ccf" << std::endl;
   }
   result  = new ossimCcfTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying CIB/CADRG"
         << std::endl;
   }
   result  = new ossimCibCadrgTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying DOQQ" << std::endl;
   }
   result  = new ossimDoqqTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying DTED" << std::endl;
   }
   result  = new ossimDtedTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying jpeg" << std::endl;
   }
   result  = new ossimJpegTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying Quickbird NITF"
         << std::endl;
   }
   result = new ossimQuickbirdNitfTileSource;
   if(result->loadState(kwl, prefix))
   {
      return result.release();
   }
   result = 0;


   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying Nitf" << std::endl;
   }
   result  = new ossimNitfTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result.release();
   }
   result = 0;

   // TileMap
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying TileMap"
         << std::endl;
   }
   result = new ossimTileMapTileSource;
   if(result->loadState(kwl, prefix))
   {
      return result.release();
   }

   result = 0;

      // RadarSat
//   if(traceDebug())
//   {
//      ossimNotify(ossimNotifyLevel_DEBUG)
//         << "trying RadarSat"
//         << std::endl;
//   }
//   result = new ossimRadarSatTileSource;
//   if(result->loadState(kwl, prefix))
//   {
//      return result.release();
//   }
//
//   result = 0;

  // RadarSat2
//    if(traceDebug())
//    {
//       ossimNotify(ossimNotifyLevel_DEBUG)
//          << "trying RadarSat2"
//          << std::endl;
//    }
//    result = new ossimRadarSat2TileSource;
//    if(result->loadState(kwl, prefix))
//    {
//       return result.release();
//    }
//
//    result = 0;

      // TerraSAR
//    if(traceDebug())
//    {
//       ossimNotify(ossimNotifyLevel_DEBUG)
//          << "trying TerraSAR"
//          << std::endl;
//    }
//    result = new ossimTerraSarTileSource;
//    if(result->loadState(kwl, prefix))
//    {
//       return result.release();
//    }
//
//    result = 0;

   // Must be before tiff...
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying Quickbird TIFF"
         << std::endl;
   }
   result = new ossimQuickbirdTiffTileSource;
   if(result->loadState(kwl, prefix))
   {
      return result.release();
   }
   
   result = 0;
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying tiff" << std::endl;
   }
   result  = new ossimTiffTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying USGS Dem"
         << std::endl;
   }
   result  = new ossimUsgsDemTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying Landsat" << std::endl;
   }
   result  = new ossimLandsatTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying VPF" << std::endl;
   }
   result = new ossimVpfTileSource;
   if(result->loadState(kwl, prefix))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying ERS" << std::endl;
   }
   result = new ossimERSTileSource;
   if(result->loadState(kwl, prefix))
   {
      return result.release();
   }
   result = 0;
  // Note:  SRTM should be in front of general raster...
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying SRTM"
         << std::endl;
   }
   result  = new ossimSrtmTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying General Raster"
         << std::endl;
   }
   result  = new ossimGeneralRasterTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result.release();
   }
   result = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandlerFactory::open(kwl, prefix) DEBUG: returning..."
         << std::endl;
   }
   return (ossimImageHandler*)0;
}

ossimImageHandler* ossimImageHandlerFactory::openFromExtension(
   const ossimFilename& fileName) const
{
   ossimString ext = fileName.ext().downcase();

   ossimRefPtr<ossimImageHandler> result = 0;

   //---
   // Ovr can be combined with "tif" once we get rid of
   // ossimQuickbirdTiffTileSource
   //---
   if (ext == "ovr")
   {
      result = new ossimTiffTileSource;
      if(result->open(fileName))
      {
         return result.release();
      }
      result = 0;
   }
   
   if ( (ext == "tif") || (ext == "tiff") )
   {
      // this must be checked first before the TIFF handler
      result = new ossimQuickbirdTiffTileSource;
      if(result->open(fileName))
      {
         return result.release();
      }
      result = 0;

      result = new ossimTiffTileSource;
      if(result->open(fileName))
      {
         return result.release();
      }
      result = 0;
   }

   if ( (ext == "ntf") || (ext == "nitf") )
   {
      // this must be checked first before the NITF raw handler
      result = new ossimQuickbirdNitfTileSource;
      if(result->open(fileName))
      {
         return result.release();
      }
      result = 0;

      result = new ossimNitfTileSource;
      if(result->open(fileName))
      {
         return result.release();
      }
      result = 0;
   }

   if ( (fileName == "a.toc") || (ext == "toc") )
   {
      result = new ossimCibCadrgTileSource;
      if(result->open(fileName))
      {
         return result.release();
      }
      result = 0;
   }
   
   if ( (ext == "jpg") || (ext == "jpeg") )
   {
      result = new ossimJpegTileSource;
      if(result->open(fileName))
      {
         return result.release();
      }
      result = 0;
   }
   
   if ( (ext == "doq") || (ext == "doqq") )
   {
      result = new ossimDoqqTileSource;
      if(result->open(fileName))
      {
         return result.release();
      }
      result = 0;
   }

   if ( (ext == "dt2") || (ext == "dt1") || (ext == "dt3") ||
        (ext == "dt4") || (ext == "dt5") || (ext == "dt0") )
   {
      result = new ossimDtedTileSource;
      if(result->open(fileName))
      {
         return result.release();
      }
      result = 0;
   }

   if (ext == "hgt")
   {
      result = new ossimSrtmTileSource;
      if(result->open(fileName))
      {
         return result.release();
      }
      result = 0;
   }  

   if (ext == "dem")
   {
      result = new ossimUsgsDemTileSource;
      if(result->open(fileName))
      {
         return result.release();
      }
      result = 0;
   }

   if (ext == "fst")
   {
      result = new ossimLandsatTileSource;
      if(result->open(fileName))
      {
         return result.release();
      }
      result = 0;
   }

   if ( (ext == "ras") || (ext == "raw") )
   {
      result = new ossimGeneralRasterTileSource;
      if(result->open(fileName))
      {
         return result.release();
      }
      result = 0;
   }

   if (ext == "img")
   {
      result  = new ossimAdrgTileSource();
      if(result->open(fileName))
      {
         return result.release();
      }
      result = 0;
   }

   if (ext == "ccf")
   {
      result = new ossimCcfTileSource();
      if(result->open(fileName))
      {
         return result.release();
      }
      result = 0;
   }

   return result.release();
}

ossimObject* ossimImageHandlerFactory::createObject(const ossimString& typeName)const
{
   if(STATIC_TYPE_NAME(ossimAdrgTileSource) == typeName)
   {
      return new ossimAdrgTileSource();
   }
   if(STATIC_TYPE_NAME(ossimCcfTileSource) == typeName)
   {
      return new ossimCcfTileSource();
   }
   if(STATIC_TYPE_NAME(ossimCibCadrgTileSource) == typeName)
   {
      return new ossimCibCadrgTileSource();
   }
   if(STATIC_TYPE_NAME(ossimDoqqTileSource) == typeName)
   {
      return new ossimDoqqTileSource();
   }
   if(STATIC_TYPE_NAME(ossimDtedTileSource) == typeName)
   {
      return new ossimDtedTileSource();
   }
   if(STATIC_TYPE_NAME(ossimJpegTileSource) == typeName)
   {
      return new ossimJpegTileSource();
   }
   if(STATIC_TYPE_NAME(ossimNitfTileSource) == typeName)
   {
      return new ossimNitfTileSource();
   }
   if(STATIC_TYPE_NAME(ossimTiffTileSource) == typeName)
   {
      return new ossimTiffTileSource();
   }
   if(STATIC_TYPE_NAME(ossimUsgsDemTileSource) == typeName)
   {
      return new ossimUsgsDemTileSource();
   }
   if(STATIC_TYPE_NAME(ossimLandsatTileSource) == typeName)
   {
      return new ossimLandsatTileSource();
   }
   if(STATIC_TYPE_NAME(ossimERSTileSource) == typeName)
   {
      return new ossimERSTileSource();
   }
   if(STATIC_TYPE_NAME(ossimSrtmTileSource) == typeName)
   {
      return new ossimSrtmTileSource();
   }
   if(STATIC_TYPE_NAME(ossimGeneralRasterTileSource) == typeName)
   {
      return new ossimGeneralRasterTileSource();
   }

   if(STATIC_TYPE_NAME(ossimTileMapTileSource) == typeName)
   {
      return new ossimTileMapTileSource();
   }

   return (ossimObject*)0;
}

void ossimImageHandlerFactory::getSupportedExtensions(ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const
{
   extensionList.push_back("img");
   extensionList.push_back("ccf"); 
   extensionList.push_back("toc");
   extensionList.push_back("tif");
   extensionList.push_back("tiff");
   extensionList.push_back("doq");
   extensionList.push_back("doqq");
   extensionList.push_back("dt0");
   extensionList.push_back("dt1");
   extensionList.push_back("dt2");
   extensionList.push_back("jpg");
   extensionList.push_back("jpeg");
   extensionList.push_back("dem");
   extensionList.push_back("fst");
   extensionList.push_back("hdr");
   extensionList.push_back("ras");
   extensionList.push_back("hgt");
   extensionList.push_back("nsf");
   extensionList.push_back("nitf");
   extensionList.push_back("ntf");
   extensionList.push_back("otb");
}

ossimObject* ossimImageHandlerFactory::createObject(const ossimKeywordlist& kwl,
                                                    const char* prefix)const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandlerFactory::createObject(kwl, prefix) DEBUG:"
         << " entering ..." << std::endl;
   }
   ossimObject* result = (ossimObject*)0;
   const char* type = kwl.find(prefix, ossimKeywordNames::TYPE_KW);

   if(type)
   {
      if (ossimString(type).trim() == STATIC_TYPE_NAME(ossimImageHandler))
      {
         const char* lookup = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);

         if (lookup)
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << "BEBUG: filename " << lookup << std::endl;
            }
            // Call the open that takes a filename...
            result = this->open(kwl, prefix);//ossimFilename(lookup));
         }
      }
      else
      {
         result = createObject(ossimString(type));
         if(result)
         {
            result->loadState(kwl, prefix);
         }
      }
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimImageHandlerFactory::createObject(kwl, prefix) DEBUG: returning result ..." << std::endl;
   }
   return result;
}

void ossimImageHandlerFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossimAdrgTileSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimCcfTileSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimCibCadrgTileSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimDoqqTileSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimDtedTileSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimJpegTileSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimNitfTileSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimTiffTileSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimUsgsDemTileSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimLandsatTileSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimERSTileSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimSrtmTileSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimGeneralRasterTileSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimTileMapTileSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimQuickbirdNitfTileSource));
   typeList.push_back(STATIC_TYPE_NAME(ossimQuickbirdTiffTileSource));
}
