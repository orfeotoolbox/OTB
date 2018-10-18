/*
 * Copyright (C) 2010-2016 by David Burken
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include <ossimPluginReaderFactory.h>
#include <ossimRadarSat2TiffReader.h>
#include <ossimTerraSarTiffReader.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>

namespace ossimplugins
{


   static const ossimTrace traceDebug("ossimPluginReaderFactory:debug");

   RTTI_DEF1(ossimPluginReaderFactory,
             "ossimPluginReaderFactory",
             ossimImageHandlerFactoryBase);

   ossimPluginReaderFactory* ossimPluginReaderFactory::theInstance = 0;

   ossimPluginReaderFactory::~ossimPluginReaderFactory()
   {
      theInstance = nullptr;
   }

   ossimPluginReaderFactory* ossimPluginReaderFactory::instance()
   {
      if(!theInstance)
      {
         theInstance = new ossimPluginReaderFactory;
      }
      return theInstance;
   }

   ossimImageHandler* ossimPluginReaderFactory::open(
      const ossimFilename& fileName, bool openOverview)const
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimPluginReaderFactory::open(filename) DEBUG: entered..."
            << "\ntrying ossimRadarSat2TiffReader"
            << std::endl;
      }

      ossimRefPtr<ossimImageHandler> reader = new ossimRadarSat2TiffReader();
      if(reader->open(fileName) == false)
      {
         reader = 0;
      }

      if ( !reader.valid() )
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "\ntrying ossimTerraSarTiffReader"
               << std::endl;
         }
      
         reader = new ossimTerraSarTiffReader();
         if(reader->open(fileName) == false)
         {
            reader = 0;
         }
      }

      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimPluginReaderFactory::open(filename) DEBUG: leaving..."
            << std::endl;
      }

      return reader.release();
   }

   ossimImageHandler* ossimPluginReaderFactory::open(const ossimKeywordlist& kwl,
                                                     const char* prefix)const
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimPluginReaderFactory::open(kwl, prefix) DEBUG: entered..."
            << "Trying ossimRadarSat2TiffReader"
            << std::endl;
      }

      ossimRefPtr<ossimImageHandler> reader = new ossimRadarSat2TiffReader;
      if(reader->loadState(kwl, prefix) == false)
      {
         reader = 0;
      }

      if ( !reader.valid() )
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimPluginReaderFactory::open(kwl, prefix) DEBUG: entered..."
               << "Trying ossimTerraSarTiffReader"
               << std::endl;
         }
         reader = new ossimTerraSarTiffReader;
         if(reader->loadState(kwl, prefix) == false)
         {
            reader = 0;
         }
      }

      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimPluginReaderFactory::open(kwl, prefix) DEBUG: leaving..."
            << std::endl;
      }

      return reader.release();
   }

   ossimObject* ossimPluginReaderFactory::createObject(
      const ossimString& typeName)const
   {
      ossimRefPtr<ossimObject> result = 0;
      if(typeName == "ossimRadarSat2TiffReader")
      {
         result = new ossimRadarSat2TiffReader;
      }
      else if(typeName == "ossimTerraSarTiffReader")
      {
         result = new ossimTerraSarTiffReader;
      }

      return result.release();
   }

   ossimObject* ossimPluginReaderFactory::createObject(
      const ossimKeywordlist& kwl, const char* prefix)const
   {
      return this->open(kwl, prefix);
   }

   void ossimPluginReaderFactory::getTypeNameList(
      std::vector<ossimString>& typeList)const
   {
      typeList.push_back(ossimString("ossimRadarSat2TiffReader"));
      typeList.push_back(ossimString("ossimTerraSarTiffReader"));
   }

   void ossimPluginReaderFactory::getSupportedExtensions(
      ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const
   {
      extensionList.push_back(ossimString("xml"));
   }
}
