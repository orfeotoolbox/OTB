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
//  $Id: ossimJpegWorldFileWriter.cpp,v 1.7 2005/08/08 22:00:24 dburken Exp $

#include <stdio.h>
#include <imaging/metadata/ossimJpegWorldFileWriter.h>
#include <base/common/ossimKeywordNames.h>
#include <base/common/ossimTrace.h>
#include <base/common/ossimRefPtr.h>
#include <base/data_types/ossimKeywordlist.h>
#include <projections/map_projections/ossimMapProjection.h>
#include <projections/map_projections/ossimMapProjectionInfo.h>
#include <projections/factory/ossimProjectionFactoryRegistry.h>
#include <imaging/formats/ossimImageHandler.h>
#include <imaging/ossimImageData.h>
#include <imaging/ossimImageSource.h>

RTTI_DEF1(ossimJpegWorldFileWriter,
          "ossimJpegWorldFileWriter",
          ossimMetadataFileWriter)

static const char DEFAULT_FILE_NAME[] = "output.jgw";
static ossimTrace traceDebug("ossimJpegWorldFileWriter:debug");

ossimJpegWorldFileWriter::ossimJpegWorldFileWriter()
   :
      ossimMetadataFileWriter()
{
}

ossimJpegWorldFileWriter::~ossimJpegWorldFileWriter()
{
}

bool ossimJpegWorldFileWriter::writeFile()
{
   if(!theInputConnection)
   {
      return false;
   }

   std::ofstream out(theFilename.c_str(), ios_base::out);
   if (!out)
   {
      return false;
   }

   // Get the geometry.
   ossimKeywordlist kwl;
   if (!theInputConnection->getImageGeometry(kwl))
   {
      return false;
   }

   // Make the projection.
   ossimRefPtr<ossimProjection> proj =
      ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
   if (!proj)
   {
      return false;
   }

   // Cast to map projection.
   ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, proj.get());
   if (!mapProj)
   {
      return false;
   }

   // Create the map projection info.
   ossimRefPtr<ossimMapProjectionInfo> projectionInfo
      = new ossimMapProjectionInfo(mapProj, theAreaOfInterest);

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
       << ul.u   << endl
       << ul.v   << endl;

   out.close();
   
   return true;
}

void ossimJpegWorldFileWriter::getMetadatatypeList(
   std::vector<ossimString>& metadatatypeList) const
{
   metadatatypeList.push_back(ossimString("jpeg_world_file")); 
}

bool ossimJpegWorldFileWriter::hasMetadataType(
   const ossimString& metadataType)const
{
   return (metadataType == "jpeg_world_file");
}
