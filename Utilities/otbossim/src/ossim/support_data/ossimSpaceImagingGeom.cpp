//*******************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: This class extends the stl's string class.
//
//********************************************************************
//  $Id: ossimSpaceImagingGeom.cpp 17206 2010-04-25 23:20:40Z dburken $
#include <ossim/support_data/ossimSpaceImagingGeom.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotifyContext.h>
#include <sstream>
#include <iostream>
using namespace std;

const char* ossimSpaceImagingGeom::SIG_PRODUCER_KW = "Producer";
const char* ossimSpaceImagingGeom::SIG_PROJECT_NAME_KW = "Project Name";
const char* ossimSpaceImagingGeom::SIG_FILENAME_KW = "Filename";
const char* ossimSpaceImagingGeom::SIG_BAND_KW = "Band";
const char* ossimSpaceImagingGeom::SIG_BITS_PER_PIXEL_KW = "Bits/Pixel";
const char* ossimSpaceImagingGeom::SIG_NUMBER_OF_BANDS_KW = "Number of Bands";
const char* ossimSpaceImagingGeom::SIG_DATUM_KW = "Datum";
const char* ossimSpaceImagingGeom::SIG_PROJECTION_KW = "Projection";
const char* ossimSpaceImagingGeom::SIG_SELECTED_PROJECTION_KW = "Selected Projection";
const char* ossimSpaceImagingGeom::SIG_ZONE_KW = "Zone";
const char* ossimSpaceImagingGeom::SIG_UL_EASTING_KW = "UL Map X (Easting)";
const char* ossimSpaceImagingGeom::SIG_UL_NORTHING_KW = "UL Map Y (Northing)";
const char* ossimSpaceImagingGeom::SIG_PIXEL_SIZE_X_KW = "Pixel Size X";
const char* ossimSpaceImagingGeom::SIG_PIXEL_SIZE_Y_KW = "Pixel Size Y";
const char* ossimSpaceImagingGeom::SIG_COLUMNS_KW = "Columns";
const char* ossimSpaceImagingGeom::SIG_ROWS_KW = "Rows";

ossimSpaceImagingGeom::ossimSpaceImagingGeom()
{
}

ossimSpaceImagingGeom::ossimSpaceImagingGeom(const ossimFilename& file,
                                             const char* prefix)
{
   setGeometry(file);
   thePrefix = prefix;
}

void ossimSpaceImagingGeom::setGeometry(const ossimFilename& file)
{
   theSpaceImagingGeometry.clear();

   theSpaceImagingGeometry.addFile(file.c_str());
}

void ossimSpaceImagingGeom::setGeometry(const ossimKeywordlist& kwl)
{
   theSpaceImagingGeometry.clear();

   theSpaceImagingGeometry = kwl;
}

void ossimSpaceImagingGeom::exportToOssim(ossimKeywordlist& kwl,
                                          const char* prefix)const
{
   const ossimKeywordlist::KeywordMap& geomMap = theSpaceImagingGeometry.getMap();
   ossimKeywordlist::KeywordMap::const_iterator node = geomMap.begin();
   
   while(node != geomMap.end())
   {
      ossimString keyword = spaceImagingToOssimKeyword((*node).first);
      ossimString value   = spaceImagingToOssimValue((*node).first,
                                                     (*node).second);
      if(keyword != "")
      {
         kwl.add(prefix,
                 keyword.c_str(),
                 value.c_str(),
                 true);
      }
      ++node;
   }
}

ossimString ossimSpaceImagingGeom::spaceImagingToOssimKeyword(const ossimString& spaceImagingKeyword)const
{
   if(spaceImagingKeyword == SIG_NUMBER_OF_BANDS_KW)
   {
      return ossimString (ossimKeywordNames::NUMBER_BANDS_KW);
   }
   else if(spaceImagingKeyword == SIG_DATUM_KW)
   {
      return ossimString(ossimKeywordNames::DATUM_KW);
   }
   else if((spaceImagingKeyword == SIG_PROJECTION_KW)||
		   (spaceImagingKeyword == SIG_SELECTED_PROJECTION_KW))
   {
      return ossimString(ossimKeywordNames::TYPE_KW);
   }
   else if(spaceImagingKeyword == SIG_ZONE_KW)
   {
      return ossimString(ossimKeywordNames::ZONE_KW);
   }
   else if(spaceImagingKeyword == SIG_UL_EASTING_KW)
   {
      return ossimString(ossimKeywordNames::TIE_POINT_EASTING_KW);
   }
   else if(spaceImagingKeyword == SIG_UL_NORTHING_KW)
   {
      return ossimString(ossimKeywordNames::TIE_POINT_NORTHING_KW);
   }
   else if(spaceImagingKeyword == SIG_PIXEL_SIZE_X_KW)
   {
      return ossimString(ossimKeywordNames::METERS_PER_PIXEL_X_KW);
   }
   else if(spaceImagingKeyword == SIG_PIXEL_SIZE_Y_KW)
   {
      return ossimString(ossimKeywordNames::METERS_PER_PIXEL_Y_KW);
   }
   else if(spaceImagingKeyword == SIG_COLUMNS_KW)
   {
      return ossimString(ossimKeywordNames::NUMBER_SAMPLES_KW);
   }
   else if(spaceImagingKeyword == SIG_ROWS_KW)
   {
      return ossimString(ossimKeywordNames::NUMBER_LINES_KW);
   }
   
   return ossimString("");
}

ossimString ossimSpaceImagingGeom::spaceImagingToOssimValue(
   const ossimString& spaceImagingKeyword,
   const ossimString& /* spaceImagingValue */)const
{
   const char* value = theSpaceImagingGeometry.find(thePrefix.c_str(),
                                                    spaceImagingKeyword.c_str());

   if(!value) return ossimString("");
   
   if(spaceImagingKeyword == SIG_DATUM_KW)
   {
      // better to have a lut for this.  For now I'll put it
      // here.  As the list gets larger let's move it to a
      // lut then.
      ossimString datum = value;
      datum = datum.trim().upcase();
      
      if( datum == "WGS84")
      {
         return ossimString("WGE");
      }
   }
   else if((spaceImagingKeyword == SIG_PROJECTION_KW)||
		   (spaceImagingKeyword == SIG_SELECTED_PROJECTION_KW))
   {
      ossimString proj = value;
      proj = proj.trim().upcase();
      
      if(proj == "UNIVERSAL TRANSVERSE MERCATOR")
      {
         return ossimString("ossimUtmProjection");
      }
      else
      {
		  ossimNotify(ossimNotifyLevel_WARN)  << "WARNING ossimSpaceImagingGeom::spaceImagingToOssimValue: Projection " << value << " doesn't have a converter yet!" << endl
		  << "Please add it" << endl;
      }
   }
   else if(spaceImagingKeyword == SIG_UL_EASTING_KW)
   {
      ossimString easting = value;
      easting = easting.trim();

      ostringstream os;
      os << easting;

      ossimString eastingValue;
      ossimString eastingUnits;

      istringstream is(os.str());
      is >> eastingValue >> eastingUnits;

      eastingUnits = eastingUnits.trim().upcase();
      if(eastingUnits!= "METERS")
      {
		  ossimNotify(ossimNotifyLevel_WARN)  << "WARNING ossimSpaceImagingGeom::spaceImagingToOssimValue: units " << eastingUnits << " not supported yet!" << endl;
      }

      return eastingValue;
   }
   else if(spaceImagingKeyword == SIG_UL_NORTHING_KW)
   {
      ossimString northing = value;
      northing = northing.trim();

      ostringstream os;
      os << northing;

      ossimString northingValue;
      ossimString northingUnits;

      istringstream is(os.str());
      is >> northingValue >> northingUnits;

      northingUnits = northingUnits.trim().upcase();
      if(northingUnits!= "METERS")
      {
		  ossimNotify(ossimNotifyLevel_WARN)  << "WARNING ossimSpaceImagingGeom::spaceImagingToOssimValue: units " << northingUnits << " not supported yet!" << endl;
      }
      
      return northingValue;
   }
   else if(spaceImagingKeyword == SIG_PIXEL_SIZE_X_KW)
   {
      ossimString gsd = value;
      gsd = gsd.trim();

      ostringstream os;
      os << gsd;

      ossimString gsdValue;
      ossimString gsdUnits;

      istringstream is(os.str());
      is >> gsdValue >> gsdUnits;

      gsdUnits = gsdUnits.trim().upcase();
      if(gsdUnits!= "METERS")
      {
		  ossimNotify(ossimNotifyLevel_WARN)  << "WARNING ossimSpaceImagingGeom::spaceImagingToOssimValue: units " << gsdUnits << " not supported yet!" << endl;
      }
      
      return gsdValue;
   }
   else if(spaceImagingKeyword == SIG_PIXEL_SIZE_Y_KW)
   {
      ossimString gsd = value;
      gsd = gsd.trim();

      ostringstream os;
      os << gsd;

      ossimString gsdValue;
      ossimString gsdUnits;

      istringstream is(os.str());
      is >> gsdValue >> gsdUnits;

      gsdUnits = gsdUnits.trim().upcase();
      if(gsdUnits!= "METERS")
      {
		  ossimNotify(ossimNotifyLevel_WARN)  << "WARNING ossimSpaceImagingGeom::spaceImagingToOssimValue: units " << gsdUnits << " not supported yet!" << endl;
      }
      
      return gsdValue;
   }
   
   return ossimString(value);
}

ossimString ossimSpaceImagingGeom::getBandName()const
{
   return theSpaceImagingGeometry.find(SIG_BAND_KW);
}

ossimFilename ossimSpaceImagingGeom::getFilename()const
{
   return theSpaceImagingGeometry.find(SIG_FILENAME_KW);
}

ossimString ossimSpaceImagingGeom::getProducer()const
{
   return theSpaceImagingGeometry.find(SIG_PRODUCER_KW);
}
