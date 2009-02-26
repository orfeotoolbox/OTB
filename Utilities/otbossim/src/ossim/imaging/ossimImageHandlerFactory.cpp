//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
//----------------------------------------------------------------------------
// $Id: ossimImageHandlerFactory.cpp 13910 2008-12-03 21:13:09Z gpotts $
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
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimJpegTileSource.h>
#include <ossim/imaging/ossimRadarSatTileSource.h>
#include <ossim/imaging/ossimTerraSarTileSource.h>

static const ossimTrace traceDebug("ossimImageHandlerFactory:debug");

RTTI_DEF1(ossimImageHandlerFactory, "ossimImageHandlerFactory", ossimImageHandlerFactoryBase);

ossimImageHandlerFactory* ossimImageHandlerFactory::theInstance = 0;
ossimImageHandlerFactory::~ossimImageHandlerFactory()
{
   theInstance = (ossimImageHandlerFactory*)NULL;
}

ossimImageHandlerFactory* ossimImageHandlerFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimImageHandlerFactory;

      // let's turn off tiff error reporting
      TIFFSetErrorHandler(NULL);
      TIFFSetWarningHandler(NULL);
   }

   return theInstance;
}

ossimImageHandler* ossimImageHandlerFactory::open(const ossimFilename& fileName)const
{
   
   ossimFilename copyFilename = fileName;
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandlerFactory::open(filename) DEBUG: entered..."
         << std::endl
         << "Attempting to open file " << copyFilename << std::endl;
   }
   ossimImageHandler* result = NULL;

   // Check for empty file.
   copyFilename.trim();
   if (copyFilename.empty())
   {
      return result;
   }

   if(copyFilename.ext() == "gz")
   {
      copyFilename = copyFilename.setExtension("");
   }
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying JPEG" << std::endl;
   }
   result = new ossimJpegTileSource;
   if(result->open(copyFilename))
   {
      return result;
   }
   delete result;
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying adrg" << std::endl;
   }

   // test if ADRG
   result  = new ossimAdrgTileSource();

   if(result->open(copyFilename))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying ccf" << std::endl;
   }
   // test if ccf
   result = new ossimCcfTileSource();
   if(result->open(copyFilename))
   {
      return result;
   }

   delete result;

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
      return result;
   }
   delete result;

      // test if Radarsat
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "Radarsat"
         << std::endl;
   }
   result = new ossimRadarSatTileSource;
   if(result->open(copyFilename))
   {
      return result;
   }
   delete result;

      // test if TerraSAR
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying TerraSAR"
         << std::endl;
   }
   result = new ossimTerraSarTileSource;
   if(result->open(copyFilename))
   {
      return result;
   }
   delete result;

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
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying tif" << std::endl;
   }
   // test for tiff
   result = new ossimTiffTileSource;
   if(result->open(copyFilename))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying CIB/CADRG"
         << std::endl;
   }
   result = new ossimCibCadrgTileSource;
   if(result->open(copyFilename))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying DOQQ" << std::endl;
   }
   result = new ossimDoqqTileSource;
   if(result->open(copyFilename))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying DTED" << std::endl;
   }
   result = new ossimDtedTileSource;
   if(result->open(copyFilename))
   {
      return result;
   }

   delete result;

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
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying Nitf" << std::endl;
   }
   result = new ossimNitfTileSource;
   if(result->open(copyFilename))
   {
      return result;
   }
   delete result;


   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying USGS Dem"
         << std::endl;
   }
   result = new ossimUsgsDemTileSource;
   if(result->open(copyFilename))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying Landsat"
         << std::endl;
   }
   result = new ossimLandsatTileSource;
   if(result->open(copyFilename))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying Vpf" << std::endl;
   }
   result = new ossimVpfTileSource;
   if(result->open(copyFilename))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying ERS" << std::endl;
   }
   result = new ossimERSTileSource;
   if(result->open(copyFilename))
   {
      return result;
   }
   delete result;

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
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying General Raster"
         << std::endl;
   }
   result = new ossimGeneralRasterTileSource;
   if(result->open(copyFilename))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandlerFactory::open(filename) DEBUG: returning..." << std::endl;
   }
   return (ossimImageHandler*)NULL;
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
   ossimImageHandler* result = NULL;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying adrg" << std::endl;
   }
   result = new ossimAdrgTileSource();

   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying ccf" << std::endl;
   }
   result  = new ossimCcfTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying CIB/CADRG"
         << std::endl;
   }
   result  = new ossimCibCadrgTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying DOQQ" << std::endl;
   }
   result  = new ossimDoqqTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying DTED" << std::endl;
   }
   result  = new ossimDtedTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying jpeg" << std::endl;
   }
   result  = new ossimJpegTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying Quickbird NITF"
         << std::endl;
   }
   result = new ossimQuickbirdNitfTileSource;
   if(result->loadState(kwl, prefix))
   {
      return result;
   }

   delete result;


   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying Nitf" << std::endl;
   }
   result  = new ossimNitfTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;

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
      return result;
   }
   
   delete result;

      // RadarSat
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying RadarSat"
         << std::endl;
   }
   result = new ossimRadarSatTileSource;
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   
   delete result;

      // TerraSAR
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying TerraSAR"
         << std::endl;
   }
   result = new ossimTerraSarTileSource;
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   
   delete result;

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
      return result;
   }
   
   delete result;
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying tiff" << std::endl;
   }
   result  = new ossimTiffTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying USGS Dem"
         << std::endl;
   }
   result  = new ossimUsgsDemTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying Landsat" << std::endl;
   }
   result  = new ossimLandsatTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying VPF" << std::endl;
   }
   result = new ossimVpfTileSource;
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying ERS" << std::endl;
   }
   result = new ossimERSTileSource;
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;
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
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying General Raster"
         << std::endl;
   }
   result  = new ossimGeneralRasterTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandlerFactory::open(kwl, prefix) DEBUG: returning..."
         << std::endl;
   }
   return (ossimImageHandler*)NULL;
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

   return (ossimObject*)NULL;
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
   ossimObject* result = (ossimObject*)NULL;
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
