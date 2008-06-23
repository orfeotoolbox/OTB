//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id: ossimImageHandlerFactory.cpp 10104 2006-12-14 16:13:05Z gpotts $
#include <ossim/imaging/ossimImageHandlerSarFactory.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimRadarSatTileSource.h>
#include <ossim/imaging/ossimRadarSat2TileSource.h>
#include <ossim/imaging/ossimTerraSarTileSource.h>
//#include <ossim/imaging/ossimCosmoSkymedTileSource.h>
#include <ossim/imaging/ossimEnvisatAsarTileSource.h>
#include <ossim/imaging/ossimErsSarTileSource.h>



static const ossimTrace traceDebug("ossimImageHandlerSarFactory:debug");

RTTI_DEF1(ossimImageHandlerSarFactory, "ossimImageHandlerSarFactory", ossimImageHandlerFactoryBase);

ossimImageHandlerSarFactory* ossimImageHandlerSarFactory::theInstance = 0;

ossimImageHandlerSarFactory::~ossimImageHandlerSarFactory()
{
   theInstance = (ossimImageHandlerSarFactory*)NULL;
}

ossimImageHandlerSarFactory* ossimImageHandlerSarFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimImageHandlerSarFactory;
   }

   return theInstance;
}

ossimImageHandler* ossimImageHandlerSarFactory::open(const ossimFilename& fileName)const
{
   
   ossimFilename copyFilename = fileName;
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandlerSarFactory::open(filename) DEBUG: entered..."
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

	// Radarsat
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying RadarSat"
         << std::endl;
   }
   result = new ossimRadarSatTileSource;
   if(result->open(copyFilename))
   {
      return result;
   }
   delete result;

   // TerraSar
	if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying TerraSar"
         << std::endl;
   }
   result = new ossimTerraSarTileSource;
   if(result->open(copyFilename))
   {
      return result;
   }
   delete result;
  
   // Envisat ASAR
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying EnvisatAsar"
         << std::endl;
   }
   result = new ossimEnvisatAsarTileSource;
   if(result->open(copyFilename))
   {
      return result;
   }
   delete result;
   
   // Radarsat 2
	if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying RadarSat2"
         << std::endl;
   }
   result = new ossimRadarSat2TileSource;
   if(result->open(copyFilename))
   {
      return result;
   }
   delete result;

   // ERS
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying Ers Sar"
         << std::endl;
   }
   result = new ossimErsSarTileSource;
   if(result->open(copyFilename))
   {
      return result;
   }
   delete result;
   
   //// Cosmo Skymed
   //if(traceDebug())
   //{
   //   ossimNotify(ossimNotifyLevel_DEBUG)
   //      << "trying CosmoSkymed"
   //      << std::endl;
   //}
   //result = new ossimCosmoSkymedTileSource;
   //if(result->open(copyFilename))
   //{
   //   return result;
   //}
   //delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandlerFactory::open(filename) DEBUG: returning..." << std::endl;
   }
   return (ossimImageHandler*)NULL;
}

ossimImageHandler* ossimImageHandlerSarFactory::open(const ossimKeywordlist& kwl,
                                                  const char* prefix)const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandlerSarFactory::open(kwl, prefix) DEBUG: entered..."
         << std::endl;
   }
   ossimImageHandler* result = NULL;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying RadarSat"
         << std::endl;
   }
   result  = new ossimRadarSatTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;

	if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying TerraSar"
         << std::endl;
   }
   result  = new ossimTerraSarTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;

	if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying RadarSat2"
         << std::endl;
   }
   result  = new ossimRadarSat2TileSource();
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;

	if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying EnvisatAsar"
         << std::endl;
   }
   result  = new ossimEnvisatAsarTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying Ers Sar"
         << std::endl;
   }
   result  = new ossimErsSarTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;

   /*if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying CosmoSkymed"
         << std::endl;
   }
   result  = new ossimCosmoSkymedTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;*/

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandlerFactory::open(kwl, prefix) DEBUG: returning..."
         << std::endl;
   }
   return (ossimImageHandler*)NULL;
}

ossimObject* ossimImageHandlerSarFactory::createObject(const ossimString& typeName)const
{
   if(STATIC_TYPE_NAME(ossimRadarSatTileSource) == typeName)
   {
      return new ossimRadarSatTileSource();
   }
	if(STATIC_TYPE_NAME(ossimTerraSarTileSource) == typeName)
   {
      return new ossimTerraSarTileSource();
   }
	if(STATIC_TYPE_NAME(ossimEnvisatAsarTileSource) == typeName)
   {
      return new ossimEnvisatAsarTileSource();
   }
	/*if(STATIC_TYPE_NAME(ossimCosmoSkymedTileSource) == typeName)
   {
      return new ossimCosmoSkymedTileSource();
   }*/
   if(STATIC_TYPE_NAME(ossimErsSarTileSource) == typeName)
   {
      return new ossimErsSarTileSource();
   }
	if(STATIC_TYPE_NAME(ossimRadarSat2TileSource) == typeName)
   {
      return new ossimRadarSat2TileSource();
   }
   return (ossimObject*)NULL;
}

void ossimImageHandlerSarFactory::getSupportedExtensions(ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const
{
   extensionList.push_back("001");	// RadarSat, ERS
	extensionList.push_back("xml");	// TerraSar, RadarSat2
	//extensionList.push_back("h5");	// CosmoSkymed
	extensionList.push_back("n1");	// EnvisatAsar
}

ossimObject* ossimImageHandlerSarFactory::createObject(const ossimKeywordlist& kwl,
                                                    const char* prefix)const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandlerSarFactory::createObject(kwl, prefix) DEBUG:"
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
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimImageHandlerSarFactory::createObject(kwl, prefix) DEBUG: returning result ..." << std::endl;
   }
   return result;
}

void ossimImageHandlerSarFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
	typeList.push_back(STATIC_TYPE_NAME(ossimRadarSatTileSource));
	typeList.push_back(STATIC_TYPE_NAME(ossimTerraSarTileSource));
	typeList.push_back(STATIC_TYPE_NAME(ossimEnvisatAsarTileSource));
	//typeList.push_back(STATIC_TYPE_NAME(ossimCosmoSkymedTileSource));
	typeList.push_back(STATIC_TYPE_NAME(ossimErsSarTileSource));
	typeList.push_back(STATIC_TYPE_NAME(ossimRadarSat2TileSource));
}
