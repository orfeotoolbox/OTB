//*******************************************************************
// Copyright (C) 2003 Storage Area Networks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Kenneth Melero
//
//*******************************************************************
//  $Id: ossimImageMetaDataWriterFactory.cpp,v 1.11 2005/08/09 19:10:41 dburken Exp $

#include <imaging/factory/ossimImageMetaDataWriterFactory.h>

#include <imaging/metadata/ossimEnviHeaderFileWriter.h>
#include <imaging/metadata/ossimFgdcFileWriter.h>
#include <imaging/metadata/ossimGeomFileWriter.h>
#include <imaging/metadata/ossimJpegWorldFileWriter.h>
#include <imaging/metadata/ossimReadmeFileWriter.h>
#include <imaging/metadata/ossimTiffWorldFileWriter.h>

#include <base/common/ossimKeywordNames.h>

RTTI_DEF1(ossimImageMetaDataWriterFactory,
          "ossimImageMetaDataWriterFactory",
          ossimImageMetaDataWriterFactoryBase);

ossimImageMetaDataWriterFactory*
ossimImageMetaDataWriterFactory::theInstance = NULL;

ossimImageMetaDataWriterFactory::ossimImageMetaDataWriterFactory()
{
   theInstance = this;
}

ossimImageMetaDataWriterFactory::~ossimImageMetaDataWriterFactory()
{
   theInstance = NULL;
}

ossimImageMetaDataWriterFactory* ossimImageMetaDataWriterFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimImageMetaDataWriterFactory;
   }

   return theInstance;
}

ossimObject* ossimImageMetaDataWriterFactory::createObject(
   const ossimString& typeName) const
{
   // Use the type name to instantiate the class.
   ossimObject* result = (ossimObject*)NULL;
   
   if(STATIC_TYPE_NAME(ossimEnviHeaderFileWriter) == typeName)
   {
     result = new ossimEnviHeaderFileWriter;
   }
   else if(STATIC_TYPE_NAME(ossimFgdcFileWriter) == typeName)
   {
     result = new ossimFgdcFileWriter;
   }
   else if(STATIC_TYPE_NAME(ossimGeomFileWriter) == typeName)
   {
     result = new ossimGeomFileWriter;
   }
   else if(STATIC_TYPE_NAME(ossimJpegWorldFileWriter) == typeName)
   {
     result = new ossimJpegWorldFileWriter;
   }
   else if(STATIC_TYPE_NAME(ossimReadmeFileWriter) == typeName)
   {
     result = new ossimReadmeFileWriter;
   }
   else if(STATIC_TYPE_NAME(ossimTiffWorldFileWriter) == typeName)
   {
     result = new ossimTiffWorldFileWriter;
   }

   return result;
}

ossimObject* ossimImageMetaDataWriterFactory::createObject(
   const ossimKeywordlist& kwl, const char* prefix)const
{
   //---
   // Check the type keyword found in kwl.  Use the create by class name
   // above and then call load state.
   //---
   ossimObject* result = (ossimObject*)NULL;

   const char* type = kwl.find(prefix, ossimKeywordNames::TYPE_KW);

   if(type)
   {
      result = createObject(ossimString(type));
      if(result)
      {
         result->loadState(kwl, prefix);
      }
   }
   
   return result;
}

ossimRefPtr<ossimMetadataFileWriter>
ossimImageMetaDataWriterFactory::createWriter(const ossimString& type)const
{
   ossimRefPtr<ossimMetadataFileWriter> writer;
   
   writer = new ossimEnviHeaderFileWriter;
   if ( (writer->getClassName() == type) ||
        (writer->hasMetadataType(type)) )
   {
      return writer;
   }

   writer = new ossimFgdcFileWriter;
   if ( (writer->getClassName() == type) ||
        (writer->hasMetadataType(type)) )
   {
      return writer;
   }

   writer = new ossimGeomFileWriter;
   if ( (writer->getClassName() == type) ||
        (writer->hasMetadataType(type)) )
   {
      return writer;
   }
   
   writer = new ossimJpegWorldFileWriter;
   if ( (writer->getClassName() == type) ||
        (writer->hasMetadataType(type)) )
   {
      return writer;
   }

   writer = new ossimReadmeFileWriter;
   if ( (writer->getClassName() == type) ||
        (writer->hasMetadataType(type)) )
   {
      return writer;
   }

   writer = new ossimTiffWorldFileWriter;
   if ( (writer->getClassName() == type) ||
        (writer->hasMetadataType(type)) )
   {
      return writer;
   }

   // Not in factory.
   writer = NULL;
   return writer;
}

void ossimImageMetaDataWriterFactory::getTypeNameList(
   std::vector<ossimString>& typeList) const
{
   // add each object's RTTI name here
   typeList.push_back(STATIC_TYPE_NAME(ossimEnviHeaderFileWriter));
   typeList.push_back(STATIC_TYPE_NAME(ossimFgdcFileWriter));
   typeList.push_back(STATIC_TYPE_NAME(ossimGeomFileWriter));
   typeList.push_back(STATIC_TYPE_NAME(ossimJpegWorldFileWriter));
   typeList.push_back(STATIC_TYPE_NAME(ossimReadmeFileWriter));   
   typeList.push_back(STATIC_TYPE_NAME(ossimTiffWorldFileWriter));
}

void ossimImageMetaDataWriterFactory::getMetadatatypeList(
   std::vector<ossimString>& metadatatypeList) const
{
   ossimRefPtr<ossimMetadataFileWriter> writer;

   writer = new ossimEnviHeaderFileWriter;
   writer->getMetadatatypeList(metadatatypeList);

   writer = new ossimFgdcFileWriter;
   writer->getMetadatatypeList(metadatatypeList);
   
   writer = new ossimGeomFileWriter;
   writer->getMetadatatypeList(metadatatypeList);

   writer = new ossimJpegWorldFileWriter;
   writer->getMetadatatypeList(metadatatypeList);

   writer = new ossimReadmeFileWriter;
   writer->getMetadatatypeList(metadatatypeList);

   writer = new ossimTiffWorldFileWriter;
   writer->getMetadatatypeList(metadatatypeList);
}

ossimImageMetaDataWriterFactory::ossimImageMetaDataWriterFactory(
   const ossimImageMetaDataWriterFactory& rhs)
{
}

const ossimImageMetaDataWriterFactory&
ossimImageMetaDataWriterFactory::operator=(
   const ossimImageMetaDataWriterFactory& rhs)
{
   return *this;
}
