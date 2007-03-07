//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero (kmelero@imagelinks.com)
//         Orginally written by Oscar Kramer (SoBe Software)
// Description: Container class for LandSat7 Fast Format header files.
//
//********************************************************************
// $Id: ossimFfL7.cpp,v 1.22 2005/11/10 15:01:41 gpotts Exp $

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;
#include <time.h>

#include <base/data_types/ossimString.h>
#include <base/common/ossimDate.h>
#include <support_data/ff_l7/ossimFfL7.h>
#include <base/context/ossimNotifyContext.h>

ossimFfL7::ossimFfL7()
   :
      ossimErrorStatusInterface()
{
   initialize();
}

ossimFfL7::ossimFfL7(const char* headerFile)
   :
      ossimErrorStatusInterface()
{
   initialize();
   
   // open the header file:
   FILE* fptr = fopen(headerFile, "r");
   if (!fptr)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
   }

   // Begin reading records:
   if (!theErrorStatus)
      readAdminRecord(fptr);
   if (!theErrorStatus)
      readRadiomRecord(fptr);
   if (!theErrorStatus)
      readGeomRecord(fptr);
   if(fptr)
   {
      fclose(fptr);
   }
}
ossimFfL7::~ossimFfL7()
{}
 
ossimString ossimFfL7::pathRow() const
{
   ossimString tmp = "p";
   tmp += theLocationCode;
   tmp.gsub("/", "r");
   tmp.gsub(" ", "");

   return tmp;
}

void ossimFfL7::dump(ostream& os) const
{
   os << setiosflags(ios::left | ios::fixed)
      << "\nAdministrative Record:"
      << setw(30) << "\ntheRequestNumber: " 
      << theRequestNumber;

   os << setw(30) << "\nLocationCode:     " << theLocationCode
      << setw(30) << "\nAcquisitionDate:  " << theAcquisitionDate
      << setw(30) << "\nSatelliteName:    " << theSatName
      << setw(30) << "\nSensorName:       " << theSensorName
      << setw(30) << "\nSensorMode:       " << theSensorMode
      << setw(30) << "\nOffNadirAngle:    "
      << setprecision(2) << theOffNadirAngle;

   os << setw(30) << "\nProductType: " 
      << theProductType
      << setw(30) << "\nProductSize: " 
      << theProductSize
      << setw(30) << "\nProcessingType: " 
      << theProcessingType 
      << setw(30) << "\nResampAlgorithm: " 
      << theResampAlgorithm
      << setw(30) << "\nPixelsPerLine: " 
      << thePixelsPerLine
      << setw(30) << "\nLinesPerImage: " 
      << theLinesPerBand
      << setw(30) << "\nRecordSize: " 
      << theRecordSize
      << setw(30) << "\nGsd: " 
      << setprecision(2) << theGsd
      << setw(30) << "\nOutputBitsPerPixel: " 
      << theOutputBitsPerPixel
      << setw(30) << "\nAcquiredBitsPerPixel: " 
      << theAcquiredBitsPerPixel
      << setw(30) << "\nBandsPresentString: " 
      << theBandsPresentString;
   
   for (int i=0; i<6; i++)
      os << "\nBandFileNames["<<i<<"]:             " <<theBandFileNames[i];
   os << endl;

   os << setprecision(15) << setw(30) << "\nRadiometric record:" << endl;
   ossim_uint32 band = 0;
   vector<ossim_float64>::const_iterator bi = theBias.begin();
   while (bi != theBias.end())
   {
      os << "band[" << band << "] bias:  " << (*bi) << endl;
      ++bi;
      ++band;
   }

   band = 0;
   bi = theGain.begin();
   while (bi != theGain.end())
   {
      os << "band[" << band << "] gain:  " << (*bi) << endl;
      ++bi;
      ++band;
   }
   
   os << endl
      << "Geometric Record:"
      << setw(30) << "\nMapProjectionName:" 
      << theMapProjectionName
      << setw(30) << "\nEllipsoid:" 
      << theEllipsoid
      << setw(30) << "\nDatum:" 
      << theDatum;

   for (int i = 0; i < 15; i++)
   {
      os << "\nProjectionParams[" << setw(2) << i << setw(10) << "]:  "
         << theProjectionParams[i];
   }

   os << setprecision(3) 
      << setw(30) << "\nUL Corner:" << theUL_Corner
      << setw(30) << "\nUR Corner:" << theUR_Corner
      << setw(30) << "\nLR Corner:" << theLR_Corner
      << setw(30) << "\nLL Corner:" << theLL_Corner
      << setw(30) << "\nCenter Point (Ground):" << theCenterGP
      << setw(30) << "\nCenter Point (Image):" << theCenterImagePoint
      << setw(30) << "\nHorizontalOffset:" 
      << theHorizontalOffset
      << setprecision(2) << setw(30) << "\nOrientationAngle:" 
      << theOrientationAngle
      << setw(30) << "\nSunElevationAngle:" 
      << setprecision(1) << theSunElevation
      << setw(30) << "\nSunAzimuth:" <<  theSunAzimuth
      << resetiosflags(ios::left)
      << endl << endl;
}

void ossimFfL7::writeHeader(ostream& os) const
{
   ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimFfL7::writeHeader: This method is not yet implemented. Ignoring request..."
                                      << std::endl;
   os << "WARNING ossimFfL7::writeHeader: This method is not yet implemented. Ignoring request..."
      << std::endl;
}

void ossimFfL7::readAdminRecord(FILE* fptr)
{
   static const int OFFSET = -1;
   int converted;
   theErrorStatus = ossimErrorCodes::OSSIM_ERROR; // assume error, prove not at end;

   fseek(fptr, OFFSET + 9, SEEK_SET);
   converted = fscanf(fptr, "%20c", theRequestNumber);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 35, SEEK_SET);
   converted = fscanf(fptr, "%17c", theLocationCode);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 71, SEEK_SET);
   converted = fscanf(fptr, "%8c", theAcquisitionDate);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 92, SEEK_SET);
   converted = fscanf(fptr, "%10c", theSatName);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 111, SEEK_SET);
   converted = fscanf(fptr, "%10c", theSensorName);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 135, SEEK_SET);
   converted = fscanf(fptr, "%6c", theSensorMode);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 154, SEEK_SET);
   converted = fscanf(fptr, "%6lf", &theOffNadirAngle);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 655, SEEK_SET);
   converted = fscanf(fptr, "%18c", theProductType);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 688, SEEK_SET);
   converted = fscanf(fptr, "%10c", theProductSize);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 741, SEEK_SET);
   converted = fscanf(fptr, "%11c", theProcessingType);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 765, SEEK_SET);
   converted = fscanf(fptr, "%2c", theResampAlgorithm);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 843, SEEK_SET);
   converted = fscanf(fptr, "%5d", &thePixelsPerLine);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 865, SEEK_SET);
   converted = fscanf(fptr, "%5d", &theLinesPerBand);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 932, SEEK_SET);
   converted = fscanf(fptr, "%9d", &theRecordSize);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 954, SEEK_SET);
   converted = fscanf(fptr, "%6lf", &theGsd);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 984, SEEK_SET);
   converted = fscanf(fptr, "%2d", &theOutputBitsPerPixel);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 1012, SEEK_SET);
   converted = fscanf(fptr, "%2d", &theAcquiredBitsPerPixel);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 1056, SEEK_SET);
   converted = fscanf(fptr, "%32c", theBandsPresentString);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 1131, SEEK_SET);
   converted = fscanf(fptr, "%29c", theBandFileNames[0]);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 1170, SEEK_SET);
   converted = fscanf(fptr, "%29c", theBandFileNames[1]);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 1211, SEEK_SET);
   converted = fscanf(fptr, "%29c", theBandFileNames[2]);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 1250, SEEK_SET);
   converted = fscanf(fptr, "%29c", theBandFileNames[3]);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 1291, SEEK_SET);
   converted = fscanf(fptr, "%29c", theBandFileNames[4]);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 1330, SEEK_SET);
   converted = fscanf(fptr, "%29c", theBandFileNames[5]);
   if (converted != 1) return;

   std::vector<ossimString> arrayPathRow = ossimString(theLocationCode).split("/");
   if(arrayPathRow.size() == 2)
   {
      thePathNumber = arrayPathRow[0].toInt();
      theRowNumber = arrayPathRow[1].beforePos(3).toInt();
   }
   
   // The Admin record was successfully extracted:
   theErrorStatus = ossimErrorCodes::OSSIM_OK;   
   return;
}


//***************************************************************************
// PRIVATE METHOD: ossimFfL7::readRadiomRecord()
//  
//***************************************************************************
void ossimFfL7::readRadiomRecord(FILE* fptr)
{
   int converted;
   char radiom_record[1537];
   
   if (!theErrorStatus)
   {
      fseek(fptr, 1536, SEEK_SET);
      converted = fscanf(fptr, "%1536c", radiom_record);
      if (converted != 1) theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
   }

   // Get the gains and biases.
   theBias.clear();
   theGain.clear();

   const ossim_uint32 MAX = 128;
   char temp_chars[MAX];
   istringstream is(radiom_record);

   // Eat the first line. "GAINS AND BIASES IN ASCENDING BAND NUMBER ORDER"
   is.getline(temp_chars, MAX);
   
   const char SPACE     = 0x20;
   const char LINE_FEED = 0x0a;
   
   // LS7 header has room for eight bands per spec.   
   for (ossim_uint32 band = 0; band < 8; ++band)
   {
      is.getline(temp_chars, MAX);
      bool blank_line = true;
      for (ossim_uint32 i = 0; i < MAX; ++i)
      {
         if (temp_chars[i] == LINE_FEED) break;  // end of line...

         if (temp_chars[i] != SPACE)
         {
            blank_line = false; // has bias and gain...
            break;
         }
      }

      if (blank_line == true) continue; // go to next line...
      
      double d = 0.0;
      istringstream is2(temp_chars);
      is2 >> d;
      theBias.push_back(d);
      is2 >> d;
      theGain.push_back(d);
   }
}

//***************************************************************************
// PRIVATE METHOD: ossimFfL7::readGeomRecord()
//  
//***************************************************************************
void ossimFfL7::readGeomRecord(FILE* fptr)
{
   static const int OFFSET = 3071;
   int converted;
   theErrorStatus = ossimErrorCodes::OSSIM_ERROR; // assume error, prove not at end;

   fseek(fptr, OFFSET + 32, SEEK_SET);
   converted = fscanf(fptr, "%4c", theMapProjectionName);
   if (converted != 1) return;

   fseek(fptr, OFFSET + 48, SEEK_SET);
   converted = fscanf(fptr, "%18c", theEllipsoid);
   if (converted != 1) return;
      
   strcpy(theDatum, "WGS-84");

   // added to read in all 15 parameters instead of the first two.
   long currentOffset = OFFSET + 110;
   for(long parameter=0;parameter < 15; ++parameter)
   {
      fseek(fptr, currentOffset, SEEK_SET);
      converted = fscanf(fptr, "%24lf", &theProjectionParams[parameter]);
      if (converted != 1) return;

      currentOffset +=25;
   }
      
   
   fseek(fptr, OFFSET + 521, SEEK_SET);
   converted = fscanf(fptr, "%6d", &theUsgsMapZone);
   if (converted != 1) return;
      
   fseek(fptr, OFFSET + 566, SEEK_SET);
   if(convertGeoPoint(fptr, theUL_Corner)) return;
      
   fseek(fptr, OFFSET + 646, SEEK_SET);
   if(convertGeoPoint(fptr, theUR_Corner)) return;
      
   fseek(fptr, OFFSET + 726, SEEK_SET);
   if(convertGeoPoint(fptr, theLR_Corner)) return;
      
   fseek(fptr, OFFSET + 806, SEEK_SET);
   if(convertGeoPoint(fptr, theLL_Corner)) return;
      
   fseek(fptr, OFFSET + 890, SEEK_SET);
   if(convertGeoPoint(fptr, theCenterGP)) return;
      
   fseek(fptr, OFFSET + 945, SEEK_SET);

   int x;
   int y;
   
   converted = fscanf(fptr, "%5d %5d", &x, &y);
   if (converted != 2) return;
   
   theCenterImagePoint.x = x;
   theCenterImagePoint.y = y;
      
   fseek(fptr, OFFSET + 969, SEEK_SET);
   converted = fscanf(fptr, "%6d", &theHorizontalOffset);
   if (converted != 1) return;
      
   fseek(fptr, OFFSET + 995, SEEK_SET);
   converted = fscanf(fptr, "%6lf", &theOrientationAngle);
   if (converted != 1) return;
      
   fseek(fptr, OFFSET + 1062, SEEK_SET);
   converted = fscanf(fptr, "%4lf", &theSunElevation);
   if (converted != 1) return;
      
   fseek(fptr, OFFSET + 1086, SEEK_SET);
   converted = fscanf(fptr, "%5lf", &theSunAzimuth);
   if (converted != 1) return;

   // successfully reached the end of Geom record:
   theErrorStatus = ossimErrorCodes::OSSIM_OK;

   return;
}


//***************************************************************************
// PRIVATE METHOD: ossimFfL7::convertGeoPoint(char* buffer);
//  
//***************************************************************************
int ossimFfL7::convertGeoPoint(FILE* fptr, ossimGpt& geo_point)
{
   int    londeg;
   int    lonmin;
   double lonsec;
   char   lonhem;
   int    latdeg;
   int    latmin;
   double latsec;
   char   lathem;
   
   int converted = fscanf(fptr, "%03d%02d%7lf%c %02d%02d%7lf%c",
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
   
//***************************************************************************
// PRIVATE METHOD: ossimFfL7::initialize()
// Initializes all fields to blanks (or 0's) and null terminates strings.
//***************************************************************************
void ossimFfL7::initialize()
{
   static const char source[] = "";

   // The Admin Record:
   strncpy(theRequestNumber, source, 21);
   strncpy(theLocationCode, source, 18);
   thePathNumber = 0;
   theRowNumber = 0;
   strncpy(theAcquisitionDate, source, 20);
   strncpy(theSatName, source, 11);
   strncpy(theSensorName, source, 11);
   strncpy(theSensorMode, source, 7);
   theOffNadirAngle = 0.0;      
   strncpy(theProductType, source, 19);
   strncpy(theProductSize, source, 11);
   strncpy(theProcessingType, source, 12);
   strncpy(theResampAlgorithm, source, 3);
   thePixelsPerLine = 0;
   theLinesPerBand = 0;
   theRecordSize = 0;
   theGsd = 0.0;
   theOutputBitsPerPixel = 0;
   theAcquiredBitsPerPixel = 0;
   strncpy(theBandsPresentString, source, 33);
   strncpy(theBandFileNames[0], source, 30);
   strncpy(theBandFileNames[1], source, 30);
   strncpy(theBandFileNames[2], source, 30);
   strncpy(theBandFileNames[3], source, 30);
   strncpy(theBandFileNames[4], source, 30);
   strncpy(theBandFileNames[5], source, 30);

   // The Radiometry Record:
   theBias.clear();
   theGain.clear();

   // The Geometry Record:
   strncpy(theMapProjectionName, source, 5);
   strncpy(theEllipsoid, source, 19);
   strncpy(theDatum, source, 7);
   for (int i=0; i<15; i++)
      theProjectionParams[i] = 0.0;
   theUsgsMapZone = 0;

   theHorizontalOffset  = 0;   
   theOrientationAngle  = 0.0; 
   theSunElevation      = 0.0; 
   theSunAzimuth        = 0.0; 

   return;
}

void ossimFfL7::getBias(vector<double>& bias) const
{
   bias = theBias;
}

void ossimFfL7::getGain(vector<double>& gain) const
{
   gain = theGain;
}


long ossimFfL7::getJulianDay()const
{
   ossimDate date(ossimString(theAcquisitionDate+4,
                              theAcquisitionDate+6).toLong(),
                  ossimString(theAcquisitionDate+6,
                              theAcquisitionDate+8).toLong(),
                  ossimString(theAcquisitionDate,
                              theAcquisitionDate+4).toLong());
   
   return static_cast<long>(date.getJulian());
}

unsigned int ossimFfL7::getBandCount()const
{
   ossimString tmp(theBandsPresentString);
   tmp.trim(); //remove spaces
   return tmp.length();
//   return strlen(tmp.chars()); //beurk! should implement length in ossimString
}
 
//***************************************************************************
// Function:
// ostream& operator<<(ostream& os, const ossimFfL7& head)
//***************************************************************************
ostream& operator<<(ostream& os, const ossimFfL7& head)
{
   head.dump(os);
   return os;
}

