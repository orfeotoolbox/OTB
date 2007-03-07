//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Ken Melero (kmelero@imagelinks.com)
//         Orginally written by Oscar Kramer (SoBe Software)
// Description: Container class for LandSat7 Fast Format header files.
//
// ********************************************************************
// $Id: ossimFfL5.cpp,v 1.1 2005/11/10 15:03:51 gpotts Exp $
//
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;
#include <time.h>

#include <base/data_types/ossimString.h>
#include <base/common/ossimDate.h>
#include <support_data/ff_l7/ossimFfL5.h>
#include <base/context/ossimNotifyContext.h>
#include <support_data/ff_revb/ossimFfRevb.h>

ossimFfL5::ossimFfL5()
   : ossimFfL7()
{
}

ossimFfL5::ossimFfL5(const char* headerFile)
   : ossimFfL7()    
{   
   // open the header file:
   readHeaderRevB(headerFile);  
}

ossimFfL5::~ossimFfL5()
{
}


void ossimFfL5::readHeaderRevB(const ossimString& header_name)
{
   theErrorStatus = ossimErrorCodes::OSSIM_ERROR; // assume error, prove not at end;
   
   ossimFfRevb h(header_name);
   if (h.errorStatus() != ossimFfRevb::OSSIM_OK) return;
   
   strcpy(theRequestNumber,   h.theProductOrderNumber);
   strcpy(theLocationCode,    h.thePathRowNumber);
   strcpy(theAcquisitionDate, h.theAcquisitionDate);
   strcpy(theSatName,         h.theSatNumber);
   strcpy(theSensorName,      h.theInstrumentType);
   strcpy(theSensorMode,      "");
   theOffNadirAngle =         h.theOrientationAngle;
   strcpy(theProductType,     h.theProductType);
   strcpy(theProductSize,     h.theProductSize);
   strcpy(theProcessingType,  h.theProcessingType);
   strcpy(theResampAlgorithm, h.theResampAlgorithm);
   thePixelsPerLine         = h.thePixelsPerLine;
   theLinesPerBand          = h.theLinesPerImage;
   theRecordSize            = h.theRecordSize;
   theGsd                   = h.theGsd;
   theOutputBitsPerPixel    = 8;
   theAcquiredBitsPerPixel  = 8;
   strcpy(theBandsPresentString, h.theBandsPresentString);
   
   //generate band names   
   int i=0;
   while((theBandsPresentString[i]>='0') && (theBandsPresentString[i]<='9'))
   {
      sprintf(theBandFileNames[i],"band%c.dat",theBandsPresentString[i]);
      ++i;
   }
   int nbb=i;

   std::vector<ossimString> arrayPathRow = ossimString(theLocationCode).split("/");
   if(arrayPathRow.size() == 2)
   {
      thePathNumber = arrayPathRow[0].toInt();
      theRowNumber = arrayPathRow[1].beforePos(3).toInt();
   }
   
   //extract gains/biases from max/min radiance
   double min,max;
   theBias.clear();
   theGain.clear();
   for(i=0;i<nbb;++i)
   {
      if (sscanf( h.theBandRadiance[i]," %lf/%lf",&max,&min) != 2) return;
      theBias.push_back(min);
      theGain.push_back(max/254.0-min/255.0);
   }
   
   strcpy(theMapProjectionName      ,h.theMapProjName);
   strcpy(theEllipsoid              ,h.theEllipsoid); 
   strcpy(theDatum, "WGS-84");

   // added to read in all 15 parameters instead of the first two.
   
   for(long parameter=0;parameter < 15; ++parameter)
   {
      ossimString temp(h.theUsgsProjParam[parameter]);
      theProjectionParams[parameter] = temp.gsub('D','E').toDouble();
   }
   
   theUsgsMapZone            = h.theUsgsMapZone;
   
   char temps[256];
   sprintf(temps,"%s %s",h.theUlLon, h.theUlLat);
   if (convertGeoPoint(temps, theUL_Corner) != ossimErrorCodes::OSSIM_OK) return;
   
   sprintf(temps,"%s %s",h.theUrLon, h.theUrLat);
   if (convertGeoPoint(temps, theUR_Corner) != ossimErrorCodes::OSSIM_OK) return;
   
   sprintf(temps,"%s %s",h.theLrLon, h.theLrLat);
   if (convertGeoPoint(temps, theLR_Corner) != ossimErrorCodes::OSSIM_OK) return;
   
   sprintf(temps,"%s %s",h.theLlLon, h.theLlLat);
   if (convertGeoPoint(temps, theLL_Corner) != ossimErrorCodes::OSSIM_OK) return;        
   
   sprintf(temps,"%s %s",h.theCenterLon, h.theCenterLat);
   if (convertGeoPoint(temps, theCenterGP) != ossimErrorCodes::OSSIM_OK) return;        
      
   theCenterImagePoint.x     = h.theCenterSample;
   theCenterImagePoint.y     = h.theCenterLine;
      
   theHorizontalOffset       = h.theOffset;
   theOrientationAngle       = h.theOrientationAngle;
   theSunElevation           = h.theSunElevation;
   theSunAzimuth             = h.theSunAzimuth;

   // The header was successfully extracted:
   theErrorStatus = ossimErrorCodes::OSSIM_OK;   
   return;
}
 
int ossimFfL5::convertGeoPoint(const char* sptr, ossimGpt& geo_point)
{
   int    londeg;
   int    lonmin;
   double lonsec;
   char   lonhem;
   int    latdeg;
   int    latmin;
   double latsec;
   char   lathem;
   
   int converted = sscanf(sptr, "%03d%02d%7lf%c %02d%02d%7lf%c",
                          &londeg, &lonmin, &lonsec, &lonhem,
                          &latdeg, &latmin, &latsec, &lathem);

   if (converted != 8)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   geo_point.lat = (double)latdeg + (double)latmin/60.0 + latsec/3600.0;
   if (lathem == 'S')
      geo_point.lat *= -1.0;
   
   geo_point.lon= (double)londeg + (double)lonmin/60.0 + lonsec/3600.0;
   if (lonhem == 'W')
      geo_point.lon *= -1.0;

   return ossimErrorCodes::OSSIM_OK;
}
