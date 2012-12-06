//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  Andrew Huang
//
// Description:
// 
// Class declaration for ossimERSFileWriter.  Meta data class for
// writing an ERS header file.
//
//----------------------------------------------------------------------------
// $Id: ossimERSFileWriter.cpp 21631 2012-09-06 18:10:55Z dburken $

#include <ossim/imaging/ossimERSFileWriter.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimMapProjectionInfo.h>


RTTI_DEF1(ossimERSFileWriter,
          "ossimERSFileWriter",
          ossimMetadataFileWriter)

ossimERSFileWriter::ossimERSFileWriter()
   :
   ossimMetadataFileWriter(),
   theHdr()
{
}

ossimERSFileWriter::~ossimERSFileWriter()
{
}

bool ossimERSFileWriter::loadState(const ossimKeywordlist& kwl,
                                   const char* prefix)
{
   // To pick up output filename.
   return ossimMetadataFileWriter::loadState(kwl, prefix);
}

bool ossimERSFileWriter::writeFile()
{
   ossimString tmp;
   
   theHdr.theDatasetType	= "ERStorage";
   theHdr.theDatatype		= "Raster";
   theHdr.theBands		= theInputConnection->getNumberOfOutputBands();
   theHdr.theSample		= theAreaOfInterest.width();
   theHdr.theLine		= theAreaOfInterest.height();
   
   ossimByteOrder byteorder = ossim::byteOrder();
   if (byteorder == OSSIM_BIG_ENDIAN)
   {
      theHdr.theByteorder = "MSBFirst";
   }
   else
   {
      theHdr.theByteorder = "LSBFirst";
   }
   
   theHdr.theCelltype = theInputConnection->getOutputScalarType();
   
   // Get the geometry from the input.
   ossimMapProjection* mapProj = 0;
   ossimRefPtr<ossimImageGeometry> inputGeom = theInputConnection->getImageGeometry();
   if ( inputGeom.valid() ) mapProj = PTR_CAST(ossimMapProjection, inputGeom->getProjection());
   if (mapProj)
   {
      // Create the projection info.
      ossimRefPtr<ossimMapProjectionInfo> projectionInfo
         = new ossimMapProjectionInfo(mapProj, theAreaOfInterest);

      // Set the tie points in the keyword list.
      ossimKeywordlist kwl;
      projectionInfo->getGeom(kwl);

      // Get the projection type.
      const char* lookup;
      ossimString projection;
      lookup = kwl.find(ossimKeywordNames::TYPE_KW);
      if (lookup)
      {
         projection = lookup;
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimERSFileWriter::writeFile WARNING:"
            << "\nNo projection type found!\nReturning..."
            << std::endl;
         return false; // Have to have the projection type!
      }

      if (projection == "ossimUtmProjection")
      {
         ossimString utm;
         lookup = kwl.find(ossimKeywordNames::HEMISPHERE_KW);
         if (lookup)
         {
            utm = lookup;
            utm += "UTM";
         }
         else
            return false;

         lookup = kwl.find(ossimKeywordNames::ZONE_KW);
         if (lookup)
            utm += lookup;
         else
            return false;

         theHdr.theProjection = utm;
         theHdr.theCoordSysType = "EN";


         //we get tie points
         lookup = kwl.find(ossimKeywordNames::TIE_POINT_XY_KW);
         tmp = lookup;
         if (lookup)
         {
            tmp = tmp.trim("(");
            tmp = tmp.trim(")");
            ossimString a = tmp.before(",", 0);
            ossimString b = tmp.after(",", 0);
            theHdr.theOriginX = a.toDouble();
            theHdr.theOriginY = b.toDouble();
         }
         //set the tie point unit type to meters
         theHdr.theTieUnitType = OSSIM_METERS;

      }
      else if (projection == "ossimEquDistCylProjection")
      {
         theHdr.theProjection = "GEODETIC";
         theHdr.theCoordSysType = "LL";

         //we get tie points
         lookup = kwl.find(ossimKeywordNames::TIE_POINT_XY_KW);
         tmp = lookup;
         if (lookup)
         {
            tmp = tmp.trim("(");
            tmp = tmp.trim(")");
            ossimString a = tmp.before(",", 0);
            ossimString b = tmp.after(",", 0);
            theHdr.theOriginX = a.toDouble();
            theHdr.theOriginY = b.toDouble();
         }
         //set the tie point unit type to degrees
         theHdr.theTieUnitType = OSSIM_DEGREES;

      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimERSFileWriter::writeFile WARNING:"
            << "\nOnly LatLon and UTM supported!\nReturning..."
            << std::endl;
         return false;		
      }

      // Get the datum.
      ossimString datum = "WGS-84";
      lookup = kwl.find(ossimKeywordNames::DATUM_KW);
      if (lookup)
      {
         ossimString os = lookup;
         if (os == "WGE")
         {
            theHdr.theDatum = "WGS84";
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimERSFileWriter::writeFile WARNING:"
               << "\nOnly WGS84 supported!\nReturning..."
               << std::endl;
            return false; // Datum has to be WGS84 for now...
         }
      }

      //get cell size
      lookup = kwl.find(ossimKeywordNames::PIXEL_SCALE_XY_KW);
      tmp = lookup;
      if (lookup)
      {
         tmp = tmp.trim("(");
         tmp = tmp.trim(")");
         ossimString a = tmp.before(",", 0);
         ossimString b = tmp.after(",", 0);

         theHdr.theCellSizeX = a.toDouble();
         theHdr.theCellSizeY = b.toDouble();
      }

      //get pixel is area or point
      if(thePixelType == OSSIM_PIXEL_IS_POINT)
      {
         theHdr.theOriginX -= theHdr.theCellSizeX/2.0;
         theHdr.theOriginY += theHdr.theCellSizeY/2.0;
      }
   }

   return theHdr.writeFile(theFilename);
}

void ossimERSFileWriter::getMetadatatypeList(
   std::vector<ossimString>& metadatatypeList) const
{
   metadatatypeList.push_back(ossimString("ers_header")); 
}

bool ossimERSFileWriter::hasMetadataType(
   const ossimString& metadataType)const
{
   return (metadataType == "ers_header");
}
