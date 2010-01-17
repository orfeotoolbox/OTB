//*******************************************************************
// Copyright (C) 2003 Storage Area Networks, Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Kenneth Melero (kmelero@sanz.com)
//
//*******************************************************************
//  $Id: ossimGeomFileWriter.cpp 15766 2009-10-20 12:37:09Z gpotts $

#include <ossim/imaging/ossimGeomFileWriter.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageSource.h>

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
   if(!theInputConnection) return false;
   ossimRefPtr<ossimImageGeometry> geom = theInputConnection->getImageGeometry();
   if(geom.valid())
   {
      ossimKeywordlist kwl;
      geom->saveState(kwl);
      return kwl.write(theFilename.c_str());
   }
   return false;
#if 0
   // Get the geometry from the input.
   const ossimMapProjection* mapProj = 0;
   const ossimImageGeometry* inputGeom = theInputConnection->getImageGeometry();
   if (inputGeom)
      mapProj = PTR_CAST(ossimMapProjection, inputGeom->getProjection());

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
   ossimKeywordlist kwl;
   projectionInfo->getGeom(kwl);

   // Write it to disk.
   return kwl.write(theFilename.c_str());
#endif
}

void ossimGeomFileWriter::getMetadatatypeList(std::vector<ossimString>& metadatatypeList) const
{
   metadatatypeList.push_back(ossimString("ossim_geometry")); 
}

bool ossimGeomFileWriter::hasMetadataType(
   const ossimString& metadataType)const
{
   return (metadataType == "ossim_geometry");
}
