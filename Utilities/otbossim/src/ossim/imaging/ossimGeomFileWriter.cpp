//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Kenneth Melero
//
//*******************************************************************
//  $Id: ossimGeomFileWriter.cpp 20506 2012-01-27 17:02:30Z dburken $

#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/imaging/ossimGeomFileWriter.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimProjection.h>

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
   bool status = false;
   if(theInputConnection)
   {
      ossimRefPtr<ossimImageGeometry> geom = theInputConnection->getImageGeometry();
      if(geom.valid())
      {
         //---
         // First check the ossimImageGeometry image size and adjust to area of interest if
         // necessary.  The ossimImageGeometry::applyScale method sometimes has rounding
         // issues so fix it here...
         //---
         if ( geom->getImageSize().hasNans() ||
              ( geom->getImageSize() != theAreaOfInterest.size() ) )
         {
            if ( traceDebug() )
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "ossimGeomFileWriter::writeFile DEBUG:"
                  << "\nAdjusting ossimImageGeometry size to reflect the area of interest."
                  << "\narea of interest size: " << theAreaOfInterest.size()
                  << "\nossimImageGeometry size: " << geom->getImageSize()
                  << std::endl;
            }
            geom->setImageSize( theAreaOfInterest.size() );
         }
               
         // Save the state to keyword list.
         ossimKeywordlist kwl;
         geom->saveState(kwl);

         const ossimMapProjection* mapProj =
            PTR_CAST(ossimMapProjection, geom->getProjection());

         if (mapProj)
         {
            const char* prefix = "projection.";
            
            ossimDpt tiePoint;
            if (mapProj->isGeographic())
            {
               // Get the ground tie point.
               ossimGpt gpt;
               mapProj->lineSampleToWorld(theAreaOfInterest.ul(), gpt);
               tiePoint = gpt;
               
               // Set the units to degrees.
               kwl.add(prefix,
                       ossimKeywordNames::TIE_POINT_UNITS_KW,
                       ossimUnitTypeLut::instance()->getEntryString(OSSIM_DEGREES),
                       true);
            }
            else
            {
               // Get the easting northing tie point.
               mapProj->lineSampleToEastingNorthing(theAreaOfInterest.ul(), tiePoint);
               
               
               // Set the units to meters.
               kwl.add(prefix,
                       ossimKeywordNames::TIE_POINT_UNITS_KW,
                       ossimUnitTypeLut::instance()->getEntryString(OSSIM_METERS),
                       true);
            }
            
            // Write the tie to keyword list.
            kwl.add(prefix,
                    ossimKeywordNames::TIE_POINT_XY_KW,
                    ossimDpt(tiePoint).toString().c_str(),
                    true);
            
         } // matches: if (mapProj)
         
         status = kwl.write(theFilename.c_str());
         
      } // matches: if(geom.valid())

   } // matches: if(theInputConnection)
   
   return status;
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
