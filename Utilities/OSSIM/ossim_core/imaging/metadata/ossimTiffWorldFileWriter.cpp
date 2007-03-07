//*******************************************************************
// Copyright (C) 2003 Storage Area Networks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Kenneth Melero (kmelero@sanz.com)
//
//*******************************************************************
//  $Id: ossimTiffWorldFileWriter.cpp,v 1.8 2005/08/08 22:00:24 dburken Exp $

#include <imaging/metadata/ossimTiffWorldFileWriter.h>
#include <base/common/ossimKeywordNames.h>
#include <base/common/ossimTrace.h>
#include <base/data_types/ossimKeywordlist.h>
#include <projections/map_projections/ossimMapProjection.h>
#include <projections/map_projections/ossimMapProjectionInfo.h>
#include <projections/factory/ossimProjectionFactoryRegistry.h>
#include <imaging/formats/ossimImageHandler.h>
#include <imaging/ossimImageData.h>
#include <imaging/ossimImageSource.h>

RTTI_DEF1(ossimTiffWorldFileWriter,
          "ossimTiffWorldFileWriter",
          ossimMetadataFileWriter)

static const char DEFAULT_FILE_NAME[] = "output.tfw";
static ossimTrace traceDebug("ossimTiffWorldFileWriter:debug");

ossimTiffWorldFileWriter::ossimTiffWorldFileWriter()
   :
      ossimMetadataFileWriter()
{
}

ossimTiffWorldFileWriter::~ossimTiffWorldFileWriter()
{
}

bool ossimTiffWorldFileWriter::writeFile()
{
   bool result = true;
   if(!theInputConnection)
   {
      return false;
   }

   std::ofstream out(theFilename.c_str(), ios_base::out);
   if (!out)
   {
      return false;
   }
   
   ossimKeywordlist kwl;

   theInputConnection->getImageGeometry(kwl);
   ossimProjection* proj;
   proj = ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
   ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, proj);

   if(mapProj)
   {
      ossimMapProjectionInfo* projectionInfo
         = new ossimMapProjectionInfo(mapProj, theAreaOfInterest);

      projectionInfo->setPixelType(thePixelType);
      
      if(projectionInfo)
      {
         ossimDpt gsd;
         ossimDpt ul;

         if(projectionInfo->getProjection()->isGeographic())
         {
            gsd = projectionInfo->getDecimalDegreesPerPixel();
            ul  = projectionInfo->ulGroundPt();
         }
         else
         {
            gsd = projectionInfo->getMetersPerPixel();
            ul  = projectionInfo->ulEastingNorthingPt();
         }

         out << setiosflags(ios::fixed) << setprecision(15)
             << gsd.x  << endl
             << 0.0    << endl // rotation value X
             << 0.0    << endl // rotation value y
             << -gsd.y << endl
             << ul.x   << endl
             << ul.y   << endl;
         
         delete projectionInfo;
      }
   }
   else
   {
      result = false;
   }
   if(proj)
   {
      delete proj;
      proj = NULL;
   }

   out.close();
   
   return result;
}

void ossimTiffWorldFileWriter::getMetadatatypeList(
   std::vector<ossimString>& metadatatypeList) const
{
   metadatatypeList.push_back(ossimString("tiff_world_file")); 
}

bool ossimTiffWorldFileWriter::hasMetadataType(
   const ossimString& metadataType)const
{
   return (metadataType == "tiff_world_file");
}
