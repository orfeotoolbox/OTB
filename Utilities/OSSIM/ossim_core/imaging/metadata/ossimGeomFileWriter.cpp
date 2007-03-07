//*******************************************************************
// Copyright (C) 2003 Storage Area Networks, Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Kenneth Melero (kmelero@sanz.com)
//
//*******************************************************************
//  $Id: ossimGeomFileWriter.cpp,v 1.9 2006/01/06 16:57:04 dburken Exp $

#include <imaging/metadata/ossimGeomFileWriter.h>
#include <base/common/ossimKeywordNames.h>
#include <base/common/ossimTrace.h>
#include <base/data_types/ossimKeywordlist.h>
#include <projections/map_projections/ossimMapProjection.h>
#include <projections/map_projections/ossimMapProjectionInfo.h>
#include <projections/factory/ossimProjectionFactoryRegistry.h>
#include <imaging/formats/ossimImageHandler.h>
#include <imaging/ossimImageData.h>
#include <imaging/ossimImageSource.h>

RTTI_DEF1(ossimGeomFileWriter,
          "ossimGeomFileWriter",
          ossimMetadataFileWriter)

static const char DEFAULT_FILE_NAME[] = "output.geom";

static ossimTrace traceDebug("ossimGeomFileWriter:debug");

ossimGeomFileWriter::ossimGeomFileWriter()
   :
      ossimMetadataFileWriter()
{}

ossimGeomFileWriter::~ossimGeomFileWriter()
{}

bool ossimGeomFileWriter::writeFile()
{
   // Get the geometry from the input.
   ossimKeywordlist kwl;
   theInputConnection->getImageGeometry(kwl);

   // Create the projection.
   ossimRefPtr<ossimProjection> proj =
      ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
   if (!proj.valid())
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimGeomFileWriter::writeFile DEBUG:"
            << "\nCreate projection failed!"
            << "\nGeometry kwl: " << kwl
            << endl;
      }

      return false;
   }
   
   ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, proj.get());
   if (!mapProj)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimGeomFileWriter::writeFile DEBUG:"
            << "\nNot a map projection!"
            << endl;
      }

      return false;
   }

   // Create the projection info.
   ossimRefPtr<ossimMapProjectionInfo> projectionInfo
      = new ossimMapProjectionInfo(mapProj, theAreaOfInterest);
   
   // Set the tie points.
   projectionInfo->getGeom(kwl);

   // Write it to disk.
   return kwl.write(theFilename.c_str());
}

void ossimGeomFileWriter::getMetadatatypeList(
   std::vector<ossimString>& metadatatypeList) const
{
   metadatatypeList.push_back(ossimString("ossim_geometry")); 
}

bool ossimGeomFileWriter::hasMetadataType(
   const ossimString& metadataType)const
{
   return (metadataType == "ossim_geometry");
}
