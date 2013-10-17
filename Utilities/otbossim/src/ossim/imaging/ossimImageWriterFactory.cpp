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
// $Id: ossimImageWriterFactory.cpp 22242 2013-04-20 02:16:06Z gpotts $

#include <ossim/imaging/ossimImageWriterFactory.h>
#include <ossim/base/ossimImageTypeLut.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimGeneralRasterWriter.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/imaging/ossimJpegWriter.h>
#include <ossim/imaging/ossimNitfWriter.h>
#include <ossim/imaging/ossimNitf20Writer.h>
#include <ossim/imaging/ossimPdfWriter.h>
#include <ossim/imaging/ossimTiffWriter.h>

ossimImageWriterFactory* ossimImageWriterFactory::theInstance = (ossimImageWriterFactory*)NULL;


ossimImageWriterFactory* ossimImageWriterFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimImageWriterFactory;
      ossimImageWriterFactoryRegistry::instance()->registerFactory(theInstance);
   }

   return theInstance;
}

ossimImageWriterFactory::~ossimImageWriterFactory()
{
   theInstance = (ossimImageWriterFactory*)NULL;
}

ossimImageFileWriter *ossimImageWriterFactory::createWriterFromExtension(
   const ossimString& fileExtension)const
{
   ossimImageFileWriter* result = 0;

   ossimString ext = fileExtension;
   ext.downcase();
   
   if( (ext == "tif")|| (ext == "tiff") )
   {
      result = new ossimTiffWriter;
   }
   else if( (ext == "jpg")|| (ext == "jpeg") )
   {
      result = new ossimJpegWriter;
   }
   else if( (ext == "ras") || (ext == "bsq") )
   {
      // Default is OSSIM_GENERAL_RASTER_BSQ
      result = new ossimGeneralRasterWriter;
   }
   else if(ext == "bil")
   {
      result = new ossimGeneralRasterWriter;
      result->setOutputImageType(OSSIM_GENERAL_RASTER_BIL);
   }
   else if(ext == "bip")
   {
      result = new ossimGeneralRasterWriter;
      result->setOutputImageType(OSSIM_GENERAL_RASTER_BIP);
   }
   else if((ext == "ntf")||
           (ext == "nitf"))
   {
      result = new ossimNitfWriter;
   }
   else if( ext == "pdf" )
   {
      result = new ossimPdfWriter;
   }
   
   return result;
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
   ossimString mimeType = typeName;
   mimeType = mimeType.downcase();
	
   // Check for tiff writer.
   ossimRefPtr<ossimImageFileWriter> writer = createFromMimeType(mimeType);
	
   if(writer.valid())
   {
      return writer.release();
   }
 
   writer = new ossimTiffWriter;
   
   if (STATIC_TYPE_NAME(ossimTiffWriter) == typeName )
   {
      return writer.release();
   }
   else
   {
      // See if the type name is supported by the writer.
      if ( writer->hasImageType(typeName) )
      {
         writer->setOutputImageType(typeName);
         return writer.release();
      }
   }

   // Check for jpeg writer.
   writer = new ossimJpegWriter;
   if ( writer->getClassName() == typeName )
   {
      return writer.release();
   }
   else
   {
      // See if the type name is supported by the writer.
      if ( writer->hasImageType(typeName) )
      {
         writer->setOutputImageType(typeName);
         return writer.release();
      }
   }
	
   // Check for general raster writer.
   writer = new ossimGeneralRasterWriter;
   if ( writer->getClassName() == typeName )
   {
      return writer.release();
   }
   else
   {
      // See if the type name is supported by the writer.
      if ( writer->hasImageType(typeName) )
      {
         writer->setOutputImageType(typeName);
         return writer.release();
      }
   }

   // Check for nitf writer.
   writer = new ossimNitfWriter;
   if ( writer->getClassName() == typeName )
   {
      return writer.release();
   }
   else
   {
      // See if the type name is supported by the writer.
      if ( writer->hasImageType(typeName) )
      {
         writer->setOutputImageType(typeName);
         return writer.release();
      }
   }

   // Check for nitf writer.
   writer = new ossimNitf20Writer;
   if ( writer->getClassName() == typeName )
   {
      return writer.release();
   }
   else
   {
      // See if the type name is supported by the writer.
      if ( writer->hasImageType(typeName) )
      {
         writer->setOutputImageType(typeName);
         return writer.release();
      }
   }

   // Check for pdf writer.
   writer = new ossimPdfWriter;
   if ( writer->getClassName() == typeName )
   {
      return writer.release();
   }
   else
   {
      // See if the type name is supported by the writer.
      if ( writer->hasImageType(typeName) )
      {
         writer->setOutputImageType(typeName);
         return writer.release();
      }
   }
   
   writer = 0; // Not a nitf writer.   

   return writer.release(); // Return a null writer.
}

ossimImageFileWriter* ossimImageWriterFactory::createFromMimeType(
   const ossimString& mimeType)const
{
   if((mimeType == "image/jpeg")||
      (mimeType == "image/jpg"))
   {
      ossimJpegWriter* writer = new ossimJpegWriter;
      writer->setOutputImageType("jpeg");
      return writer;
   }
   else if((mimeType == "image/tiff")||
           (mimeType == "image/tif")||
           (mimeType == "image/gtif")||
           (mimeType == "image/gtiff"))
   {
      ossimTiffWriter* writer = new ossimTiffWriter;
      writer->setOutputImageType("tiff_tiled_band_separate");
      return writer;
   }
   else if((mimeType == "image/nitf") ||
           (mimeType == "image/ntf"))
   {
      ossimNitfWriter* writer = new ossimNitfWriter;
      writer->setOutputImageType("nitf_block_band_separate");
      return writer;
   }
   else if((mimeType == "image/ras"))
   {
      ossimGeneralRasterWriter* writer = new ossimGeneralRasterWriter;
      writer->setOutputImageType("general_raster_bsq");
      return writer;
   }
   else if((mimeType == "application/pdf")||
           (mimeType == "image/pdf"))
   {
      ossimPdfWriter* writer = new ossimPdfWriter;
      writer->setOutputImageType("ossim_pdf");
      return writer;
   }
   return 0;
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
   result.push_back("bsq");
   result.push_back("bil");
   result.push_back("bip");
   result.push_back("tif");
   result.push_back("jpg");
   result.push_back("ntf");
   result.push_back("pdf");
}

void ossimImageWriterFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossimTiffWriter));
   typeList.push_back(STATIC_TYPE_NAME(ossimJpegWriter));
   typeList.push_back(STATIC_TYPE_NAME(ossimGeneralRasterWriter));
   typeList.push_back(STATIC_TYPE_NAME(ossimNitfWriter));
   typeList.push_back(STATIC_TYPE_NAME(ossimNitf20Writer));
   typeList.push_back(STATIC_TYPE_NAME(ossimPdfWriter));   
}

void ossimImageWriterFactory::getImageFileWritersBySuffix(
   ossimImageWriterFactoryBase::ImageFileWriterList& result, const ossimString& ext)const
{
   ossimString testExt = ext.downcase();
   if(testExt == "tiff" || testExt == "tif")
   {
      result.push_back(new ossimTiffWriter);
   }
   else if(testExt == "ntf" || testExt == "nitf")
   {
      result.push_back(new ossimNitfWriter);
   }
   else if(testExt == "jpg" || testExt == "jpeg")
   {
      result.push_back(new ossimJpegWriter);
   }
   else if(testExt == "ras" || testExt == "bsq" || testExt == "bil" || testExt == "bip")
   {
      result.push_back(new ossimGeneralRasterWriter);
   }
   else if( testExt == "pdf" )
   {
      result.push_back( new ossimPdfWriter );
   }
}

void ossimImageWriterFactory::getImageFileWritersByMimeType(
   ossimImageWriterFactoryBase::ImageFileWriterList& result, const ossimString& mimeType ) const
{
   ossimString testExt = mimeType.downcase();
   testExt = ossimString(testExt.begin()+6, testExt.end());
   
   getImageFileWritersBySuffix(result, testExt);
}

void ossimImageWriterFactory::getImageTypeList( std::vector<ossimString>& imageTypeList ) const
{
   // Add the tiff writer types.
   ossimRefPtr<ossimImageFileWriter> writer = new ossimTiffWriter;
   writer->getImageTypeList(imageTypeList);

   // Add the jpeg writer types.
   writer = new ossimJpegWriter;
   writer->getImageTypeList(imageTypeList);

   // Add the general raster writer types.
   writer = new ossimGeneralRasterWriter;
   writer->getImageTypeList(imageTypeList);

   // Add the nitf writer types.
   writer = new ossimNitfWriter;
   writer->getImageTypeList(imageTypeList);
   
   // Add the nitf writer types.
   writer = new ossimNitf20Writer;
   writer->getImageTypeList(imageTypeList);
   
   // Add the pdf writer types.
   writer = new ossimPdfWriter;
   writer->getImageTypeList(imageTypeList);
   
   writer = 0;
}
