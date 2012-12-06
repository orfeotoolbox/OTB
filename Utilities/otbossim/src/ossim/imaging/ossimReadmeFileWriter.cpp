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
//  $Id: ossimReadmeFileWriter.cpp 21631 2012-09-06 18:10:55Z dburken $

#include <ossim/imaging/ossimReadmeFileWriter.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageSource.h>
#include <iostream>
using namespace std;

RTTI_DEF1(ossimReadmeFileWriter,
          "ossimReadmeFileWriter",
          ossimMetadataFileWriter)

static const char DEFAULT_FILE_NAME[] = "output_readme.txt";
static ossimTrace traceDebug("ossimReadmeFileWriter:debug");

//**************************************************************************************************
// 
//**************************************************************************************************
ossimReadmeFileWriter::ossimReadmeFileWriter()
:
ossimMetadataFileWriter()
{
}

//**************************************************************************************************
// 
//**************************************************************************************************
ossimReadmeFileWriter::~ossimReadmeFileWriter()
{
}

//**************************************************************************************************
// 
//**************************************************************************************************
bool ossimReadmeFileWriter::writeFile()
{
   static const char MODULE[] = "ossimReadmeFileWriter::writeFile";

   if(!theInputConnection)
   {
      if (traceDebug())
      {
         CLOG << "DEBUG:"
              << "\nNo input connection!  Returning..."
              << endl;
      }

      return false;
   }

   std::ofstream out(theFilename.c_str(), ios_base::out);
   if (!out)
      return false;

   // Fetch the map projection of the input image if it exists:
   const ossimMapProjection* mapProj = 0;
   ossimRefPtr<ossimImageGeometry> imgGeom = theInputConnection->getImageGeometry();
   if ( imgGeom.valid() )
   {
      const ossimProjection* proj = imgGeom->getProjection();
      mapProj = PTR_CAST(ossimMapProjection, proj);
   }
   if (!mapProj)
   {
      out.close();
      return false;
   }

   ossimMapProjectionInfo* projectionInfo = new ossimMapProjectionInfo(mapProj, theAreaOfInterest);
   if(projectionInfo)
   {
      out << setiosflags(ios::fixed)
          << setiosflags(ios::left)
          << setw(16) << "Image: "
          << projectionInfo->getImageInfoString();
      
      out << setw(17) << "\nLines:"
          << projectionInfo->linesPerImage()
          << setw(17) << "\nSamples:"
          << projectionInfo->pixelsPerLine()
          << setw(17) << "\nPixel Size x:"
          << setprecision(8) 
          << projectionInfo->getMetersPerPixel().x 
          << " meters";
      
      out << ", " 
          << setprecision(8) 
          << projectionInfo->getUsSurveyFeetPerPixel().x
          << " U.S. feet";
      
      out << setw(17) << "\nPixel Size y:"
          << setprecision(8) 
          << projectionInfo->getMetersPerPixel().y 
          << " meters";
      
      out << ", " 
          << setprecision(8) 
          << projectionInfo->getUsSurveyFeetPerPixel().y
          << " U.S. feet";
      
      const ossimString type  = TYPE_NAME(mapProj);
      const ossimString datum = mapProj->getDatum()->code();
      const ossimString ellipsoid = mapProj->getEllipsoid().code();

      if(type)
         out << setw(17) << "\nProjection:" << type;

      const ossimUtmProjection* utmProj = PTR_CAST(ossimUtmProjection, mapProj);
      if (utmProj)
         out << setw(17) << "\nUTM map zone:" << utmProj->getZone();
      
      if(datum && ellipsoid)
         out << setw(17) << "\nDatum:"      << datum
             << setw(17) << "\nEllipsoid:"
             << ellipsoid;
      
      out << resetiosflags(ios::left) << endl;
      
      const ossimString NORTH = "N";
      const ossimString SOUTH = "S";
      const ossimString EAST  = "E";
      const ossimString WEST  = "W";
      
      ossimString tmpString;
      double tmpDouble;
      
      // HACK: Easiest way to get projection info for optional params is via the old keywordlist:
      ossimKeywordlist kwl;
      mapProj->saveState(kwl);
      const char* parallel1   = kwl.find(ossimKeywordNames::STD_PARALLEL_1_KW);
      const char* parallel2   = kwl.find(ossimKeywordNames::STD_PARALLEL_2_KW);
      const char* scaleFactor = kwl.find(ossimKeywordNames::SCALE_FACTOR_KW);
      
      
      ossimString proj_name = mapProj->getClassName();
      if ( ( proj_name.contains("Lambert")           ) ||
           ( proj_name.contains("Albers")            ) ||
           ( proj_name.contains("TransverseMercator" ) ) )
      {
         double falseEasting  =  mapProj->getFalseEasting();
         double falseNorthing =  mapProj->getFalseNorthing();
 
         double origin_lat = mapProj->origin().latd();
         tmpDouble = fabs(origin_lat);
         tmpString = (origin_lat < 0.0) ? SOUTH : NORTH;
         out << setiosflags(ios::left)
             << "\n"
             << setw(18) << "Origin Latitude:"
             << resetiosflags(ios::left)
             << setw(21) << setprecision(12) << tmpDouble 
             << tmpString << endl;
         
         double origin_lon = mapProj->origin().lond();
         tmpDouble = fabs(origin_lon);
         tmpString = (origin_lon < 0.0) ? WEST : EAST;
         out << setiosflags(ios::left)
             << setw(18) << "Origin Longitude:"
             << resetiosflags(ios::left)
             << setw(21) << tmpDouble << tmpString
             << setiosflags(ios::left)
             << "\n"
             << setw(23) << "False Easting:"
             << resetiosflags(ios::left)
             << setw(17) << setprecision(4) 
             << falseEasting;
         
         out << " meters, " << setprecision(4)
             << ossim::mtrs2usft(falseEasting) 
             << " U.S. feet";
         
         out << endl;
	 
         out << setiosflags(ios::left)
             << setw(23) << "False Northing:"
             << resetiosflags(ios::left)
             << setw(17) << setprecision(4) 
             << falseNorthing;
         
         out << " meters, " << setprecision(4)
             << ossim::mtrs2usft(falseNorthing) 
             << " U.S. feet";
         
         out << endl;
	 
      } // End of if "Alber Lambert or TM.
      
      if (proj_name.contains("TransverseMercator"))
      {

         out << setiosflags(ios::left)
             << "\n"
             << setw(18) << "Scale Factor:"
             << resetiosflags(ios::left)
             << setw(21) << setprecision(10) 
             << ossimString(scaleFactor).toDouble()
             << "\n" << endl;
      }
      
      if ( (proj_name.contains("Lambert") ) ||
           (proj_name.contains("Albers")  ) )
      {
         double stdpar1 = ossimString(parallel1).toDouble();
         tmpDouble = fabs(stdpar1);
         tmpString = (stdpar1 < 0.0) ? SOUTH : NORTH;
         out << setiosflags(ios::left) 
             << setw(23) << "Standard Parallel #1:"
             << resetiosflags(ios::left)
             << setw(16) << setprecision(12) << tmpDouble
             << tmpString << endl;
         
         double stdpar2 = ossimString(parallel2).toDouble();
         tmpDouble = fabs(stdpar2);
         tmpString = (stdpar2 < 0.0) ? SOUTH : NORTH;
         out << setiosflags(ios::left)
             << setw(23) << "Standard Parallel #2:"
             << resetiosflags(ios::left)
             << setw(16) << setprecision(12) 
             << tmpDouble << tmpString
             << "\n" << endl;
      }
      
      ossimString cornerString[4] = { "Upper Left",
                                      "Upper Right",
                                      "Lower Right",
                                      "Lower Left" };
	
      //---
      // Add a note to the readme file stating what the corner coordinates
      // are relative to.
      //---
      if (projectionInfo->getPixelType() == OSSIM_PIXEL_IS_AREA)
      {
         out 
            << "\nCorner coordinates relative to outer edge of pixel.\n\n";
      }
      else
      {
         out
            << "\nCorner coordinates relative to center of pixel.\n\n";
      }
      
      out << "Corner Coordinates:\n\n";
      
      //---
      // Upper Left Corner Points
      //---
      tmpDouble = fabs(projectionInfo->ulGroundPt().latd());
      tmpString = (projectionInfo->ulGroundPt().latd() < 0.0) ? SOUTH : NORTH; 
      out << cornerString[0]  << endl;
      out << setw(14) << "Latitude:  "
          << setw(16) << setprecision(12) << tmpDouble 
          << tmpString << endl;
      tmpDouble = fabs(projectionInfo->ulGroundPt().lond());
      tmpString = (projectionInfo->ulGroundPt().lond() < 0.0) ? WEST : EAST;
      out << setw(14) << "Longitude:  "
          << setw(16) << tmpDouble << tmpString << endl;
      
      out << setw(14) << "Easting:  "
          << setw(17) << setprecision(4) 
          << projectionInfo->ulEastingNorthingPt().x;
      
      out << " meters, " << setprecision(4)
          << ossim::mtrs2usft(projectionInfo->ulEastingNorthingPt().x)
          << " U.S. feet";
      
      out << endl
          << setw(14) << "Northing:  "
          << setw(17) << projectionInfo->ulEastingNorthingPt().y;
      
      out << " meters, " << setprecision(4)
          << ossim::mtrs2usft(projectionInfo->ulEastingNorthingPt().y)
          << " U.S. feet";
      
      out << endl << endl;
      
      //---
      // Upper Right Corner Points
      //---
      tmpDouble = fabs(projectionInfo->urGroundPt().latd());
      tmpString = (projectionInfo->urGroundPt().latd() < 0.0) ? SOUTH : NORTH; 
      out << cornerString[1]  << endl;
      out << setw(14) << "Latitude:  "
          << setw(16) << setprecision(12) << tmpDouble
          << tmpString << endl;
      tmpDouble = fabs(projectionInfo->urGroundPt().lond());
      tmpString = (projectionInfo->urGroundPt().lond() < 0.0) ? WEST : EAST;
      out << setw(14) << "Longitude:  "
          << setw(16) << tmpDouble << tmpString << endl;
      
      out << setw(14) << "Easting:  "
          << setw(17) << setprecision(4)
          << projectionInfo->urEastingNorthingPt().x;
      
      out << " meters, " << setprecision(4)
          << ossim::mtrs2usft(projectionInfo->urEastingNorthingPt().x)
          << " U.S. feet";
      
      out << endl
          << setw(14) << "Northing:  "
          << setw(17) << projectionInfo->urEastingNorthingPt().y;
      
      out << " meters, " << setprecision(4)
          << ossim::mtrs2usft(projectionInfo->urEastingNorthingPt().y)
          << " U.S. feet";
      
      out << endl << endl;
      
      //---
      // Lower Right Corner Points
      //---
      tmpDouble = fabs(projectionInfo->lrGroundPt().latd());
      tmpString = (projectionInfo->lrGroundPt().latd() < 0.0) ? SOUTH : NORTH; 
      out << cornerString[2]  << endl;
      out << setw(14) << "Latitude:  "
          << setw(16) << setprecision(12) << tmpDouble 
          << tmpString << endl;
      tmpDouble = fabs(projectionInfo->lrGroundPt().lond());
      tmpString = (projectionInfo->lrGroundPt().lond() < 0.0) ? WEST : EAST;
      out << setw(14) << "Longitude:  "
          << setw(16) << tmpDouble << tmpString << endl;
      
      out << setw(14) << "Easting:  "
          << setw(17) << setprecision(4) 
          << projectionInfo->lrEastingNorthingPt().x;
      
      out << " meters, " << setprecision(4)
          << ossim::mtrs2usft(projectionInfo->lrEastingNorthingPt().x)
          << " U.S. feet";
      
      out << endl
          << setw(14) << "Northing:  "
          << setw(17) << projectionInfo->lrEastingNorthingPt().y;
      
      out << " meters, " << setprecision(4)
          << ossim::mtrs2usft(projectionInfo->lrEastingNorthingPt().y)
          << " U.S. feet";
      
      out << endl << endl;
      
      //---
      // Lower Left Corner Points
      //---
      tmpDouble = fabs(projectionInfo->llGroundPt().latd());
      tmpString = (projectionInfo->llGroundPt().latd() < 0.0) ? SOUTH : NORTH; 
      out << cornerString[3]  << endl;
      out << setw(14) << "Latitude:  "
          << setw(16) << setprecision(12) << tmpDouble
          << tmpString << endl;
      tmpDouble = fabs(projectionInfo->llGroundPt().lond());
      tmpString = (projectionInfo->llGroundPt().lond() < 0.0) ? WEST : EAST;
      out << setw(14) << "Longitude:  "
          << setw(16) << tmpDouble << tmpString << endl;
      
      out << setw(14) << "Easting:  "
          << setw(17) << setprecision(4)
          <<  projectionInfo->llEastingNorthingPt().x;
      
      out << " meters, " << setprecision(4)
          << ossim::mtrs2usft(projectionInfo->llEastingNorthingPt().x)
          << " U.S. feet";
      
      out << endl
          << setw(14) << "Northing:  "
          << setw(17) << projectionInfo->llEastingNorthingPt().y;
      
      out << " meters, " << setprecision(4)
          << ossim::mtrs2usft(projectionInfo->llEastingNorthingPt().y)
          << " U.S. feet";
      
      out << endl << endl;
      
      out << "Center Coordinates:\n\n";
      
      tmpDouble = fabs(projectionInfo->centerGroundPt().latd());
      
      tmpString = (projectionInfo->centerGroundPt().latd() < 0.0) ? SOUTH : NORTH;
      
      out << setw(14) << "Latitude:  "
          << setw(16) << setprecision(12) << tmpDouble
          << tmpString << endl;
      
      tmpDouble = fabs(projectionInfo->centerGroundPt().lond());
      
      tmpString = (projectionInfo->centerGroundPt().lond() < 0.0) ? WEST : EAST;
      
      out << setw(14) << "Longitude:  "
          << setw(16) << tmpDouble << tmpString << endl;
      
      out << setw(14) << "Easting:  "
          << setw(17) << setprecision(4)
          << projectionInfo->centerEastingNorthingPt().x;
      
      out << " meters, " << setprecision(4)
          << ossim::mtrs2usft(projectionInfo->centerEastingNorthingPt().x)
          << " U.S. feet";
      
      out << endl
          << setw(14) << "Northing:  "
          << setw(17) << projectionInfo->centerEastingNorthingPt().y;
      
      out << " meters, " << setprecision(4)
          << ossim::mtrs2usft(projectionInfo->centerEastingNorthingPt().y)
          << " U.S. feet";
      
      out << endl << endl;
      
      out 
         << "\nNOTE:  Lat/Lon values are in decimal degrees.";
      
      out 
         << "\n       Eastings/Northings are in meters.";
      
      out << endl;
      
      delete projectionInfo;
   }
   
   out.close();
   return true;
}

//**************************************************************************************************
// 
//**************************************************************************************************
void ossimReadmeFileWriter::getMetadatatypeList(std::vector<ossimString>& metadatatypeList) const
{
   metadatatypeList.push_back(ossimString("ossim_readme")); 
}

//**************************************************************************************************
// 
//**************************************************************************************************
bool ossimReadmeFileWriter::hasMetadataType(const ossimString& metadataType) const
{
   return (metadataType == "ossim_readme");
}
