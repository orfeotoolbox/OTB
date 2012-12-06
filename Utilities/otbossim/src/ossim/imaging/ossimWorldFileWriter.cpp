//*******************************************************************
// Copyright (C) 2003 Storage Area Networks, Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Kenneth Melero (kmelero@sanz.com)
//
//*******************************************************************
//  $Id: ossimWorldFileWriter.cpp 21631 2012-09-06 18:10:55Z dburken $

#include <ossim/imaging/ossimWorldFileWriter.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimStatePlaneProjectionInfo.h>
// #include <ossim/projection/ossimStatePlaneProjectionFactory.h>
#include <ossim/base/ossimUnitConversionTool.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimImageSource.h>


RTTI_DEF1(ossimWorldFileWriter,
          "ossimWorldFileWriter",
          ossimMetadataFileWriter)

static ossimTrace traceDebug("ossimWorldFileWriter:debug");

ossimWorldFileWriter::ossimWorldFileWriter()
   :
   ossimMetadataFileWriter(),
   theUnits(OSSIM_METERS)
{
}

ossimWorldFileWriter::~ossimWorldFileWriter()
{
}

bool ossimWorldFileWriter::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   const char* lookup = kwl.find(prefix, "linear_units");
   if(lookup)
   {
      ossimUnitType units = (ossimUnitType)ossimUnitTypeLut::instance()->
         getEntryNumber(lookup, true);
      if ( (units == OSSIM_METERS) ||
           (units == OSSIM_FEET)   ||
           (units == OSSIM_US_SURVEY_FEET) )
      {
         theUnits = units;
      }
   }

   return ossimMetadataFileWriter::loadState(kwl, prefix);
}

//**************************************************************************************************
// Outputs projection information to the output file. Returns TRUE if successful.
//**************************************************************************************************
bool ossimWorldFileWriter::writeFile()
{
   if(!theInputConnection)
      return false;

   std::ofstream out(theFilename.c_str(), ios_base::out);
   if (!out)
      return false;
   
   // Fetch the map projection of the input image if it exists:
   const ossimMapProjection* mapProj = 0;
   ossimRefPtr<ossimImageGeometry> imgGeom = theInputConnection->getImageGeometry();
   if( imgGeom.valid() )
   {
      const ossimProjection* proj = imgGeom->getProjection();
      mapProj = PTR_CAST(ossimMapProjection, proj);
   }
   if (!mapProj)
   {
      out.close();
      return false;
   }

   // Convert projection info to proper units:
   ossimDpt gsd = mapProj->getMetersPerPixel();
   ossimDpt ul  = mapProj->getUlEastingNorthing();

   // ESH 05/2008 -- If the pcs code has been given, we
   // make use of the implied units.
   theUnits = mapProj->getProjectionUnits();
   if (theUnits == OSSIM_FEET)
   {
      gsd.x = ossimUnitConversionTool(gsd.x, OSSIM_METERS).getFeet();
      gsd.y = ossimUnitConversionTool(gsd.y, OSSIM_METERS).getFeet();
      ul.x  = ossimUnitConversionTool(ul.x,  OSSIM_METERS).getFeet();
      ul.y  = ossimUnitConversionTool(ul.y,  OSSIM_METERS).getFeet();
   }
   else if (theUnits == OSSIM_US_SURVEY_FEET)
   {
      gsd.x = ossimUnitConversionTool(gsd.x, OSSIM_METERS).getUsSurveyFeet();
      gsd.y = ossimUnitConversionTool(gsd.y, OSSIM_METERS).getUsSurveyFeet();
      ul.x  = ossimUnitConversionTool(ul.x,  OSSIM_METERS).getUsSurveyFeet();
      ul.y  = ossimUnitConversionTool(ul.y,  OSSIM_METERS).getUsSurveyFeet();
   }

   // output projection info to file:
   out << setiosflags(ios::fixed) << setprecision(15)
      << gsd.x  << endl
      << 0.0    << endl // rotation value X
      << 0.0    << endl // rotation value y
      << -gsd.y << endl
      << ul.x   << endl
      << ul.y   << endl;

   out.close();
   return true;
}

//**************************************************************************************************
void ossimWorldFileWriter::getMetadatatypeList(
   std::vector<ossimString>& metadatatypeList) const
{
   metadatatypeList.push_back(ossimString("tiff_world_file"));
   metadatatypeList.push_back(ossimString("jpeg_world_file")); 
}

bool ossimWorldFileWriter::hasMetadataType(
   const ossimString& metadataType)const
{
   if ( (metadataType == "tiff_world_file") ||
        (metadataType == "jpeg_world_file") )
   {
      return true;
   }
   
   return false;
}

void ossimWorldFileWriter::setLinearUnits(ossimUnitType units)
{
   if ( (units == OSSIM_UNIT_UNKNOWN) ||
        (units == OSSIM_METERS)  ||
        (units == OSSIM_FEET)    ||
        (units == OSSIM_US_SURVEY_FEET) )
   {
      theUnits = units;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimWorldFileWriter::setLinearUnits WARNING!"
         << "\nUnsupported units passed to method:  "
         << ossimUnitTypeLut::instance()->getEntryString(units)
         << "\nUnits unchanged..."
         << std::endl;
   }
}
