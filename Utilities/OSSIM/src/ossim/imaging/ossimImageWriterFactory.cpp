//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimImageWriterFactory.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/imaging/ossimImageWriterFactory.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimTiffWriter.h>
#include <ossim/imaging/ossimJpegWriter.h>
#include <ossim/imaging/ossimGeneralRasterWriter.h>
#include <ossim/imaging/ossimNitfWriter.h>
#include <ossim/base/ossimImageTypeLut.h>

ossimImageWriterFactory* ossimImageWriterFactory::theInstance = (ossimImageWriterFactory*)NULL;


ossimImageWriterFactory* ossimImageWriterFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimImageWriterFactory;
      ossimImageWriterFactoryRegistry::instance()->
         registerFactory(theInstance);
   }

   return theInstance;
}

ossimImageWriterFactory::~ossimImageWriterFactory()
{
   theInstance = (ossimImageWriterFactory*)NULL;
}

ossimImageFileWriter *ossimImageWriterFactory::createWriterFromExtension(const ossimString& fileExtension)const
{
   if((fileExtension == "tif")||
      (fileExtension == "tiff"))
   {
      return new ossimTiffWriter;
   }
   else if((fileExtension == "jpg")||
           (fileExtension == "jpeg"))
   {
      return new ossimJpegWriter;
   }
   else if(fileExtension == "ras")
   {
      return new ossimGeneralRasterWriter;
   }
   else if((fileExtension == "ntf")||
           (fileExtension == "nitf"))
   {
      return new ossimNitfWriter;
   }
   
   return NULL;
}

ossimImageFileWriter*
ossimImageWriterFactory::createWriter(const ossimKeywordlist& kwl,
                                      const char *prefix)const
{
   ossimString type = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   ossimImageFileWriter* result = (ossimImageFileWriter*)NULL;
   
   if(type != "")
   {
      result = createWriter(type);
      if (result)
      {
         if (result->hasImageType(type))
         {
            ossimKeywordlist kwl2(kwl);
            kwl2.add(prefix,
                     ossimKeywordNames::IMAGE_TYPE_KW,
                     type,
                     true);
         
            result->loadState(kwl2, prefix);
         }
         else
         {
            result->loadState(kwl, prefix);
         }
      }
   }

   return result;
}

ossimImageFileWriter*
ossimImageWriterFactory::createWriter(const ossimString& typeName)const
{
   // Check for tiff writer.
   ossimImageFileWriter* writer = new ossimTiffWriter;
   if (writer->getClassName() == typeName )
   {
      return writer;
   }
   else
   {
      // See if the type name is supported by the writer.
      if ( writer->hasImageType(typeName) )
      {
         writer->setOutputImageType(typeName);
         return writer;
      }
   }
   delete writer; // Not a tiff writer.

   // Check for jpeg writer.
   writer = new ossimJpegWriter;
   if ( writer->getClassName() == typeName )
   {
      return writer;
   }
   else
   {
      // See if the type name is supported by the writer.
      if ( writer->hasImageType(typeName) )
      {
         writer->setOutputImageType(typeName);
         return writer;
      }
   }
   delete writer; // Not a jpeg writer.

   // Check for general raster writer.
   writer = new ossimGeneralRasterWriter;
   if ( writer->getClassName() == typeName )
   {
      return writer;
   }
   else
   {
      // See if the type name is supported by the writer.
      if ( writer->hasImageType(typeName) )
      {
         writer->setOutputImageType(typeName);
         return writer;
      }
   }
   delete writer; // Not a general raster writer.   

   // Check for nitf writer.
   writer = new ossimNitfWriter;
   if ( writer->getClassName() == typeName )
   {
      return writer;
   }
   else
   {
      // See if the type name is supported by the writer.
      if ( writer->hasImageType(typeName) )
      {
         writer->setOutputImageType(typeName);
         return writer;
      }
   }
   delete writer; // Not a nitf writer.   

   writer = NULL;

   return writer; // Return a null writer.
}

ossimObject* ossimImageWriterFactory::createObject(const ossimKeywordlist& kwl,
                                                   const char *prefix)const
{
   return createWriter(kwl, prefix);
}

ossimObject* ossimImageWriterFactory::createObject(const ossimString& typeName)const
{
   return createWriter(typeName);
}


void ossimImageWriterFactory::getExtensions(std::vector<ossimString>& result)const
{
   result.push_back("ras");
   result.push_back("tif");
   result.push_back("jpg");
   result.push_back("ntf");
}

void ossimImageWriterFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossimTiffWriter));
   typeList.push_back(STATIC_TYPE_NAME(ossimJpegWriter));
   typeList.push_back(STATIC_TYPE_NAME(ossimGeneralRasterWriter));
   typeList.push_back(STATIC_TYPE_NAME(ossimNitfWriter));
}

void ossimImageWriterFactory::getImageTypeList(std::vector<ossimString>& imageTypeList)const
{
   // Add the tiff writer types.
   ossimImageFileWriter* writer = new ossimTiffWriter;
   writer->getImageTypeList(imageTypeList);
   delete writer;

   // Add the jpeg writer types.
   writer = new ossimJpegWriter;
   writer->getImageTypeList(imageTypeList);
   delete writer;

   // Add the general raster writer types.
   writer = new ossimGeneralRasterWriter;
   writer->getImageTypeList(imageTypeList);
   delete writer;
   writer = NULL;

   // Add the nitf writer types.
   writer = new ossimNitfWriter;
   writer->getImageTypeList(imageTypeList);
   delete writer;
   writer = NULL;
}
