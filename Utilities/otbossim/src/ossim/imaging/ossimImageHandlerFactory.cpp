//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id: ossimImageHandlerFactory.cpp 18002 2010-08-30 18:01:10Z gpotts $
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
#include <ossim/imaging/ossimRpfCacheTileSource.h>
#include <ossim/imaging/ossimQbTileFilesHandler.h>
#include <ossim/base/ossimRegExp.h>

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

ossimImageHandler* ossimImageHandlerFactory::open(const ossimFilename& fileName)const
{
   static const char* M = "ossimImageHandlerFactory::open(filename) -- ";
   if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG) << M <<" Entering..." << std::endl;

   ossimFilename copyFilename = fileName;
   if (traceDebug())
      ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Attempting to open file <"<<fileName<<">"<<std::endl;

   ossimRefPtr<ossimImageHandler> result = 0;
   while (true)
   {
      // Check for empty file.
      copyFilename.trim();
      if (copyFilename.empty()) break;

      // for all of our imagehandlers the filename must exist.
      // if we have any imagehandlers that require an encoded string and is contrlled in this factory then
      // we need to move this.
//      if (!copyFilename.exists())  break;

      ossimString ext = copyFilename.ext().downcase();
      if(ext == "gz")
         copyFilename = copyFilename.setExtension("");

      // Try opening from extension logic first (this is faster than instantiating each type).
//      if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying via extension...";
//      result = openFromExtension(copyFilename);
//      if (result.valid())  break;

      //---
      // If here do it the brute force way by going down the list of available
      // readers...
      //---
      if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying RPF Cache Image...";
      result = new ossimRpfCacheTileSource;
      if (result->open(copyFilename)) break;

      if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying JPEG...";
      result = new ossimJpegTileSource;
      if (result->open(copyFilename)) break;

      // this must be checked first before the TIFF handler
      if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying Quickbird TIFF...";
      result = new ossimQuickbirdTiffTileSource;
      if (result->open(copyFilename)) break;

      if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying TIFF...";
      result = new ossimTiffTileSource;
      if (result->open(copyFilename))  break;

      if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying CIB/CADRG...";
      result = new ossimCibCadrgTileSource;
      if (result->open(copyFilename))  break;

      if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying DOQQ...";
      result = new ossimDoqqTileSource;
      if (result->open(copyFilename))  break;

      if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying DTED...";
      result = new ossimDtedTileSource;
      if (result->open(copyFilename))  break;

      // this must be checked first before the NITF raw handler
      if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying Quickbird Nitf...";
      result = new ossimQuickbirdNitfTileSource;
      if (result->open(copyFilename))  break;

      if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying NITF...";
      result = new ossimNitfTileSource;
      if (result->open(copyFilename))  break;

      if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying USGS Dem...";
      result = new ossimUsgsDemTileSource;
      if (result->open(copyFilename))  break;

      if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying Landsat...";
      result = new ossimLandsatTileSource;
      if (result->open(copyFilename))  break;

      if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying VPF...";
      result = new ossimVpfTileSource;
      if (result->open(copyFilename))  break;

      if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying ERS...";
      result = new ossimERSTileSource;
      if (result->open(copyFilename))  break;

      //---
      // The srtm and general raser readers were picking up j2k overviews because the
      // matching base file has an "omd" file that the raster reader can load
      // so added extension check.  (drb - 20100709)
      //---
      if (copyFilename.ext() != "ovr")
      {
         // Note:  SRTM should be in front of general raster...
         if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying SRTM...";
         result = new ossimSrtmTileSource;
         if (result->open(copyFilename))  break;

         if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying General Raster...";
         result = new ossimGeneralRasterTileSource;
         if (result->open(copyFilename))  break;
      }

      if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying ADRG...";
      result = new ossimAdrgTileSource();
      if (result->open(copyFilename))  break;

      if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying ossimQbTileFilesHandler...";
      result = new ossimQbTileFilesHandler();
      if (result->open(copyFilename))  break;

      if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying CCF...";
      result = new ossimCcfTileSource();
      if (result->open(copyFilename))  break;

      if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying TileMap...";
      result = new ossimTileMapTileSource();
      if (result->open(copyFilename))  break;

      result = 0;
      break;
   }

   if (traceDebug())
   {
      if (result.valid())
         ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "   SUCCESS" << std::endl;
      else
         ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "   Open FAILED" << std::endl;
   }

   return result.release();
}

ossimImageHandler* ossimImageHandlerFactory::open(const ossimKeywordlist& kwl,
                                                  const char* prefix)const
{
   static const char* M = "ossimImageHandlerFactory::open(kwl,prefix) -- ";
   if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG) << M <<" entered..." << std::endl;

   ossimRefPtr<ossimImageHandler> result = 0;
   while (true)
   {
      if(traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << M<< "trying adrg...";
      result = new ossimAdrgTileSource();
      if (result->loadState(kwl, prefix))  break;

      if(traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << M<< "trying ccf...";
      result  = new ossimCcfTileSource();
      if (result->loadState(kwl, prefix))  break;

      if(traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << M << "trying CIB/CADRG...";
      result  = new ossimCibCadrgTileSource();
      if (result->loadState(kwl, prefix))  break;

      if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "trying RpfCache...";
      result  = new ossimRpfCacheTileSource();
      if (result->loadState(kwl, prefix))  break;

      if(traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << M << "trying DOQQ...";
      result  = new ossimDoqqTileSource();
      if (result->loadState(kwl, prefix))  break;

      if(traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << M<< "trying DTED...";
      result  = new ossimDtedTileSource();
      if (result->loadState(kwl, prefix))  break;

      if(traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << M<< "trying jpeg...";
      result  = new ossimJpegTileSource();
      if (result->loadState(kwl, prefix))  break;

      if(traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << M<< "trying Quickbird NITF...";
      result = new ossimQuickbirdNitfTileSource;
      if (result->loadState(kwl, prefix))  break;

      if(traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << M << "trying Nitf...";
      result  = new ossimNitfTileSource();
      if (result->loadState(kwl, prefix))  break;

      // Must be before tiff...
      if(traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << M<< "trying Quickbird TIFF...";
      result = new ossimQuickbirdTiffTileSource;
      if (result->loadState(kwl, prefix))  break;

      if(traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << M<< "trying tiff...";
      result  = new ossimTiffTileSource();
      if (result->loadState(kwl, prefix))  break;

      if(traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << M<< "trying USGS Dem...";
      result  = new ossimUsgsDemTileSource();
      if (result->loadState(kwl, prefix))  break;

      if(traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << M<< "trying Landsat...";
      result  = new ossimLandsatTileSource();
      if (result->loadState(kwl, prefix))  break;

      if(traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << M<< "trying VPF...";
      result = new ossimVpfTileSource;
      if (result->loadState(kwl, prefix))  break;

      if(traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << M<< "trying ERS...";
      result = new ossimERSTileSource;
      if (result->loadState(kwl, prefix))  break;

      // Note:  SRTM should be in front of general raster...
      if(traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << M<< "trying SRTM..."<< std::endl;
      result  = new ossimSrtmTileSource();
      if (result->loadState(kwl, prefix))  break;

      if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG) << M << "trying General Raster..."<< std::endl;
      result  = new ossimGeneralRasterTileSource();
      if (result->loadState(kwl, prefix))  break;

      if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"trying ossimQbTileFilesHandler..."<<std::endl;
      result = new ossimQbTileFilesHandler;
      if (result->loadState(kwl, prefix))  break;

      if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "Trying TileMap...";
      result = new ossimTileMapTileSource();
      if (result->loadState(copyFilename))  break;

      result = 0;
      break;
   }

   if (traceDebug())
   {
      if (result.valid())
         ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "   SUCCESS" << std::endl;
      else
         ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "   Open FAILED" << std::endl;
   }

   return result.release();
}

#if 0
ossimImageHandler* ossimImageHandlerFactory::openFromExtension(const ossimFilename& fileName) const
{
   
   static const char* M = "ossimImageHandlerFactory::openFromExtension() -- ";
   if(traceDebug()) 
      ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Attempting to open <"<<fileName<<">"<<std::endl;

   ossimString ext = fileName.ext().downcase();
   ossimRefPtr<ossimImageHandler> result = 0;
   
   
   while (true)
   {
      // OVR can be combined with "tif" once we get rid of ossimQuickbirdTiffTileSource
      if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Trying OVR..."<<std::endl;
      if (ext == "ovr")
      {
         result = new ossimTiffTileSource;
         if(result->open(fileName)) break;
      }

      if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Trying TIF or TIFF..."<<std::endl;
      if ( (ext == "tif") || (ext == "tiff") )
      {
         // this must be checked first before the TIFF handler
         result = new ossimQuickbirdTiffTileSource;
         if(result->open(fileName)) break;
         result = new ossimTiffTileSource;
         if(result->open(fileName)) break;
      }

      if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Trying NTF or NITF..."<<std::endl;
      if ( (ext == "ntf") || (ext == "nitf") )
      {
         // this must be checked first before the NITF raw handler
         result = new ossimQuickbirdNitfTileSource;
         if(result->open(fileName)) break;
         result = new ossimNitfTileSource;
         if(result->open(fileName)) break;
      }

      if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Trying RPF..."<<std::endl;
      if ( (fileName == "rpf"))
      {
         result = new ossimRpfCacheTileSource;
         if(result->open(fileName)) break;
      }

      if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Trying TOC..."<<std::endl;
      if ( (fileName == "a.toc") || (ext == "toc"))
      {
         result = new ossimCibCadrgTileSource;
         if(result->open(fileName)) break;
      }

      if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Trying JPG or JPEG..."<<std::endl;
      if ( (ext == "jpg") || (ext == "jpeg") )
      {
         result = new ossimJpegTileSource;
         if(result->open(fileName)) break;
      }

      if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Trying DOQ or DOQQ..."<<std::endl;
      if ( (ext == "doq") || (ext == "doqq") )
      {
         result = new ossimDoqqTileSource;
         if(result->open(fileName)) break;
      }

      if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Trying DTn..."<<std::endl;
      if ( (ext == "dt2") || (ext == "dt1") || (ext == "dt3") ||
         (ext == "dt4") || (ext == "dt5") || (ext == "dt0") )
      {
         result = new ossimDtedTileSource;
         if(result->open(fileName)) break;
      }

      if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Trying HGT..."<<std::endl;
      if (ext == "hgt")
      {
         result = new ossimSrtmTileSource;
         if(result->open(fileName)) break;
      }  

      if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Trying DEM..."<<std::endl;
      if (ext == "dem")
      {
         result = new ossimUsgsDemTileSource;
         if(result->open(fileName)) break;
      }

      if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Trying FST..."<<std::endl;
      if (ext == "fst")
      {
         result = new ossimLandsatTileSource;
         if(result->open(fileName)) break;
      }

      if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Trying RAS or RAW..."<<std::endl;
      if ( (ext == "ras") || (ext == "raw") )
      {
         result = new ossimGeneralRasterTileSource;
         if(result->open(fileName)) break;
      }

      if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Trying IMG..."<<std::endl;
      if (ext == "img")
      {
         result  = new ossimAdrgTileSource;
         if(result->open(fileName)) break;
      }

      if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Trying CCF..."<<std::endl;
      if (ext == "ccf")
      {
         result = new ossimCcfTileSource;
         if(result->open(fileName)) break;
      }

      if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Trying TIL..."<<std::endl;
      if (ext == "til")
      {
         result = new ossimQbTileFilesHandler;
         if(result->open(fileName)) break;
      }

      result = 0;
      break;
   }
   if (traceDebug())
   {
      if (result.valid())
         ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "   SUCCESS" << std::endl;
      else
         ossimNotify(ossimNotifyLevel_DEBUG)<<M<< "   Open FAILED" << std::endl;
   }
   return result.release();
}
#endif
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
   if(STATIC_TYPE_NAME(ossimRpfCacheTileSource) == typeName)
   {
     return new ossimRpfCacheTileSource();
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
   if(STATIC_TYPE_NAME(ossimQbTileFilesHandler) == typeName)
   {
      return new ossimQbTileFilesHandler();
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
   extensionList.push_back("dt3");
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
   extensionList.push_back("til");
   extensionList.push_back("otb");
}

void ossimImageHandlerFactory::getImageHandlersBySuffix(ossimImageHandlerFactoryBase::ImageHandlerList& result, const ossimString& ext)const
{
   static const char* M = "ossimImageHandlerFactory::getImageHandlersBySuffix() -- ";
   // OVR can be combined with "tif" once we get rid of ossimQuickbirdTiffTileSource
   if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Trying OVR..."<<std::endl;
   ossimString testExt = ext.downcase();
   if (testExt == "ovr")
   {
      result.push_back(new ossimTiffTileSource);
      return;
   }
   
   if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Testing TIF or TIFF..."<<std::endl;
   if ( (testExt == "tif") || (testExt == "tiff") )
   {
      // this must be checked first before the TIFF handler
      result.push_back(new ossimQuickbirdTiffTileSource);
      result.push_back(new ossimTiffTileSource);
      return;
   }
   
   if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Testing NTF or NITF..."<<std::endl;
   if ( (testExt == "ntf") || (testExt == "nitf") )
   {
      // this must be checked first before the NITF raw handler
      result.push_back(new ossimQuickbirdNitfTileSource);
      result.push_back(new ossimNitfTileSource);
      return;
   }
   
   if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Testing RPF..."<<std::endl;
   if ( (testExt == "rpf"))
   {
      result.push_back(new ossimRpfCacheTileSource);
      return;
   }
   
   if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Testing TOC..."<<std::endl;
   if ( testExt == "toc")
   {
      result.push_back(new ossimCibCadrgTileSource);
      return;
   }
   
   if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Testing JPG or JPEG..."<<std::endl;
   if ( (testExt == "jpg") || (testExt == "jpeg") )
   {
      result.push_back(new ossimJpegTileSource);
      return;
   }
   
   if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Testing DOQ or DOQQ..."<<std::endl;
   if ( (testExt == "doq") || (testExt == "doqq") )
   {
      result.push_back(new ossimDoqqTileSource);
      return;
   }
   
   if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Testing DTn..."<<std::endl;
   ossimString regExpStr = "dt[0-9]";
   ossimRegExp regExp(regExpStr);
   if(regExp.find(testExt))
   {
      result.push_back(new ossimDtedTileSource);
      return;
   }
   
   if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Testing HGT..."<<std::endl;
   if (testExt == "hgt")
   {
      result.push_back(new ossimSrtmTileSource);
      return;
   }  
   
   if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Testing DEM..."<<std::endl;
   if (testExt == "dem")
   {
      result.push_back(new ossimUsgsDemTileSource);
      return;
   }
   
   if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Testing FST..."<<std::endl;
   if (testExt == "fst")
   {
      result.push_back(new ossimLandsatTileSource);
      return;
   }
   
   if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Testing RAS or RAW..."<<std::endl;
   if ( (testExt == "ras") || (testExt == "raw") )
   {
      result.push_back(new ossimGeneralRasterTileSource);
      return;
   }
   
   if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Testing IMG..."<<std::endl;
   if (testExt == "img")
   {
      result.push_back(new ossimAdrgTileSource);
      return;
   }
   
   if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Testing CCF..."<<std::endl;
   if (testExt == "ccf")
   {
      result.push_back(new ossimCcfTileSource);
      return;
   }
   
   if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Testing TIL..."<<std::endl;
   if (testExt == "til")
   {
      result.push_back(new ossimQbTileFilesHandler);
      return;
   }
   if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)<<M<<"Testing OTB..."<<std::endl;
   if (testExt == "otb")
   {
      result.push_back(new ossimTileMapTileSource);
      return;
   }
}

void ossimImageHandlerFactory::getImageHandlersByMimeType(ossimImageHandlerFactoryBase::ImageHandlerList& result, const ossimString& mimeType)const
{
   ossimString test(mimeType.begin(), mimeType.begin()+6);
   if(test == "image/")
   {
      ossimString mimeTypeTest(mimeType.begin() + 6, mimeType.end());
      getImageHandlersBySuffix(result, mimeTypeTest);
      if(mimeTypeTest == "dted")
      {
         result.push_back(new ossimDtedTileSource);
      }
   }
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
   if(STATIC_TYPE_NAME(ossimTileMapTileSource) == typeName)
   {
      return new ossimTileMapTileSource();
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
   typeList.push_back(STATIC_TYPE_NAME(ossimRpfCacheTileSource));
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
   typeList.push_back(STATIC_TYPE_NAME(ossimQbTileFilesHandler));
}
