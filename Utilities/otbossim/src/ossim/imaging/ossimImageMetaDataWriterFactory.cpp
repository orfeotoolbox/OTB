//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Kenneth Melero
//
//*******************************************************************
//  $Id: ossimImageMetaDataWriterFactory.cpp 17206 2010-04-25 23:20:40Z dburken $

#include <ossim/imaging/ossimImageMetaDataWriterFactory.h>

#include <ossim/imaging/ossimEnviHeaderFileWriter.h>
#include <ossim/imaging/ossimERSFileWriter.h>
#include <ossim/imaging/ossimFgdcFileWriter.h>
#include <ossim/imaging/ossimGeomFileWriter.h>
#include <ossim/imaging/ossimReadmeFileWriter.h>
#include <ossim/imaging/ossimWorldFileWriter.h>

#include <ossim/base/ossimKeywordNames.h>

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
   if(STATIC_TYPE_NAME(ossimERSFileWriter) == typeName)
   {
     result = new ossimERSFileWriter;
   }
   else if(STATIC_TYPE_NAME(ossimFgdcFileWriter) == typeName)
   {
     result = new ossimFgdcFileWriter;
   }
   else if(STATIC_TYPE_NAME(ossimGeomFileWriter) == typeName)
   {
     result = new ossimGeomFileWriter;
   }
   else if(STATIC_TYPE_NAME(ossimReadmeFileWriter) == typeName)
   {
     result = new ossimReadmeFileWriter;
   }
   //---
   // Special case for backwards compatibility. ossimJpegWorldFileWriter and
   // ossimTiffWorldFileWriter same; hence, moved to ossimWorldFileWriter.
   //---
   else if( (STATIC_TYPE_NAME(ossimWorldFileWriter)  == typeName) ||
            (ossimString("ossimJpegWorldFileWriter") == typeName) ||
            (ossimString("ossimTiffWorldFileWriter") == typeName) )
   {
     result = new ossimWorldFileWriter;
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

   writer = new ossimERSFileWriter;
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
   
   writer = new ossimReadmeFileWriter;
   if ( (writer->getClassName() == type) ||
        (writer->hasMetadataType(type)) )
   {
      return writer;
   }

   //---
   // Special case for backwards compatibility. ossimJpegWorldFileWriter and
   // ossimTiffWorldFileWriter same; hence, moved to ossimWorldFileWriter.
   //---
   writer = new ossimWorldFileWriter;
   if ( (writer->getClassName() == type) ||
        (writer->hasMetadataType(type))  ||
        (ossimString("ossimJpegWorldFileWriter") == type) ||
        (ossimString("ossimTiffWorldFileWriter") == type) )
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
   typeList.push_back(STATIC_TYPE_NAME(ossimERSFileWriter));   
   typeList.push_back(STATIC_TYPE_NAME(ossimFgdcFileWriter));
   typeList.push_back(STATIC_TYPE_NAME(ossimGeomFileWriter));
   typeList.push_back(STATIC_TYPE_NAME(ossimReadmeFileWriter));   
   typeList.push_back(STATIC_TYPE_NAME(ossimWorldFileWriter));
}

void ossimImageMetaDataWriterFactory::getMetadatatypeList(
   std::vector<ossimString>& metadatatypeList) const
{
   ossimRefPtr<ossimMetadataFileWriter> writer;

   writer = new ossimEnviHeaderFileWriter;
   writer->getMetadatatypeList(metadatatypeList);

   writer = new ossimERSFileWriter;
   writer->getMetadatatypeList(metadatatypeList);

   writer = new ossimFgdcFileWriter;
   writer->getMetadatatypeList(metadatatypeList);
   
   writer = new ossimGeomFileWriter;
   writer->getMetadatatypeList(metadatatypeList);

   writer = new ossimReadmeFileWriter;
   writer->getMetadatatypeList(metadatatypeList);

   writer = new ossimWorldFileWriter;
   writer->getMetadatatypeList(metadatatypeList);
}

ossimImageMetaDataWriterFactory::ossimImageMetaDataWriterFactory(
   const ossimImageMetaDataWriterFactory& /* rhs */)
{
}

const ossimImageMetaDataWriterFactory&
ossimImageMetaDataWriterFactory::operator=(
   const ossimImageMetaDataWriterFactory& /* rhs */)
{
   return *this;
}
