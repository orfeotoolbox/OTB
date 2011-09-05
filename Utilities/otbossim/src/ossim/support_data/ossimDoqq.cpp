//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
// 
// Description: This class parses a Digital Ortho Quarter Quad (DOQQ)
//              header.
//
//********************************************************************
// $Id: ossimDoqq.cpp 19900 2011-08-04 14:19:57Z dburken $

#include <string.h>
#include <ossim/support_data/ossimDoqq.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>

static ossimTrace traceDebug("ossimDoqq:debug");

//**************************************************************************
// CONSTRUCTOR
//**************************************************************************
ossimDoqq::ossimDoqq()
   :  theErrorStatus(OSSIM_ERROR)
{
}

//**************************************************************************
// CONSTRUCTOR
//**************************************************************************
ossimDoqq::ossimDoqq(ossimFilename file)
   :  theErrorStatus(OSSIM_ERROR)
{
   open(file);
   theDoqFile.close();
}

//**************************************************************************
// Opens the DOQ header and parses info.
//**************************************************************************
bool ossimDoqq::open(const ossimFilename& file)
{
   // Assume all kosher:
   theErrorStatus = OSSIM_OK;

   // Check first line of header to determine which version to parse.
   if (theDoqFile.is_open())
      theDoqFile.close();

   theDoqFile.open(file.c_str(), std::ios::in);
   char header[23];
   theDoqFile.seekg(0, std::ios::beg);
   theDoqFile.get(header, 22);
   header[22] = '\0';
   if(strcmp((const char*)header, "BEGIN_USGS_DOQ_HEADER") == 0)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimDoqq::ossimDoqq Loading DOQ version 2 header..."
            << std::endl;
      }

      ldstr_v2(theDoqFile);
   }
   else
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimDoqq::ossimDoqq  Loading DOQ version 1 header..."
            << std::endl;
      }

      ldstr_v1(theDoqFile);
   }

   // Check for error.
   if(theErrorStatus)
      return false;
   return true;
}

ossimDoqq::~ossimDoqq()
{
}

void ossimDoqq::ldstr_v2(std::istream& in)
{
   static const char MODULE[] = "ossimDoqq::ldstr_v2(istream& in)";

   if (!in)
   {
      theErrorStatus = OSSIM_ERROR;
      return;
   }

   char line[100];
   char dum1[30];
   char dum2[30];
   char dum3[30];
   char dum4[30];

   while((!strncmp(line, "END_USGS_HEADER", 15) == 0)&&
			(in.good()))
   {
      // Read in one line of header at a time.
      in.getline(line, 100);
      
      if(strncmp(line, "SAMPLES_AND_LINES", 17) == 0)
      {
         sscanf(line, "%s %s %s", dum1, dum2, dum3);
         theLine = atoi(dum3);
         theSample = atoi(dum2);
      }

      else if(strncmp(line, "HORIZONTAL_COORDINATE_SYSTEM", 28) == 0)
      {
         sscanf(line, "%s %s", dum1, dum2);
         theProjection = dum2;
      }
      
      else if(strncmp(line, "NW_QUAD_CORNER_XY", 17) == 0)
      {         
         sscanf(line, "%s %s %s", dum1, dum2, dum3);
         
         theUE = atof(dum2);
         theUN = atof(dum3);
      }
      
      else if(strncmp(line, "NE_QUAD_CORNER_XY", 17) == 0)
      {
         sscanf(line, "%s %s %s", dum1, dum2, dum3);
         theLE = atof(dum2);
         theLN = atof(dum3);
      }

      else if(strncmp(line, "COORDINATE_ZONE", 15) == 0)
      {
         sscanf(line, "%s %s", dum1, dum2);
         theUtmZone = atoi(dum2);
      }

      else if(strncmp(line, "SOURCE_IMAGE_DATE", 17) == 0)
      {
         sscanf(line, "%s %s %s %s", dum1, dum2, dum3, dum4);
         theAcqYear  = ossimString(dum2);
         theAcqMonth = ossimString(dum3);
         theAcqDay   = ossimString(dum4);
      }

      else if((strncmp(line, "XY_ORIGIN", 9) == 0))
      {
         sscanf(line, "%s %s %s", dum1, dum2, dum3);
         theEasting = atof(dum2);
         theNorthing = atof(dum3);        
      }

      else if((strncmp(line, "HORIZONTAL_DATUM", 16) == 0) && theDatum.empty())
      {
         ossimString datum;         
         sscanf(line, "%s %s", dum1, dum2);
         datum = dum2; 
         
         if(datum.contains("NAD27"))
            theDatum = "NAD";
         else
            theDatum = "NAR";
      }

      else if(strncmp(line, "BYTE_COUNT", 10) == 0)
      {
         ossimString header;         
         sscanf(line, "%s %s", dum1, dum2);
         header = dum2;
         
         theHeaderSize = atoi(header.chars());
      }

      else if(strncmp(line, "BAND_CONTENT", 12) == 0)
      {
         ossimString rgbType;        
         sscanf(line, "%s %s", dum1, dum2);
         rgbType = dum2;

         if(rgbType.contains("BLACK&WHITE"))
            theRgb = 1;
         else
            theRgb = 3;
      }

      else if(strncmp(line, "HORIZONTAL_RESOLUTION", 21) == 0)
      {
         ossimString gsd;
         sscanf(line, "%s %s", dum1, dum2);
         gsd = dum2;

         theGsd.x = gsd.toDouble();
         theGsd.y = gsd.toDouble();
      }

      else if(strncmp(line, "QUADRANGLE_NAME", 15) == 0)
      {
         sscanf(line, "%s %29c", dum1, dum2);
         dum2[29] = 0;
         theQuadName = dum2;
      }

      else if(strncmp(line, "QUADRANT", 8) == 0)
      {
         sscanf(line, "%s %s", dum1, dum2);
         theQuad = dum2;
      }

      else if(strncmp(line, "NATION", 6) == 0)
      {
         sscanf(line, "%s %s", dum1, dum2);
         theNation = dum2;
      }

      else if(strncmp(line, "STATE", 5) == 0)
      {
         sscanf(line, "%s %s", dum1, dum2);
         theState = dum2;
      }

      else if(strncmp(line, "RMSE_XY", 7) == 0)
      {
         sscanf(line, "%s %s", dum1, dum2);
         theRMSE = ossimString(dum2).toDouble();
      }

      else if(strncmp(line, "IMAGE_SOURCE", 12) == 0)
      {
         sscanf(line, "%s %29c", dum1, dum2);
         dum2[29] = 0;
         theImageSource = dum2;
      }

      else if(strncmp(line, "SOURCE_IMAGE_ID", 15) == 0)
      {
         sscanf(line, "%s %29c", dum1, dum2);
         dum2[29] = 0;
         theSourceImageID = dum2;
      }
   }

	if (!in.good())
	{
      theErrorStatus = OSSIM_ERROR;
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
			<< MODULE << " ERROR:\n"
			<< "\tHeader stream is bad."
			<< std::endl;
      }
		return;
	}

   // Check for valid lines and samples and header size.
   if(theLine <= 0 || theSample <= 0 || theHeaderSize <= 0)
   {
      theErrorStatus = OSSIM_ERROR;
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " ERROR:\n"
            << "\tInvalid lines or samples or header size."
            << std::endl;
      }
      return;
   }

   // Assign concatenated acquisition date:
   theAcqYearMonthDay = theAcqYear;
   theAcqYearMonthDay += "-";
   theAcqYearMonthDay += theAcqMonth;
   theAcqYearMonthDay += "-";
   theAcqYearMonthDay += theAcqDay;

}

void ossimDoqq::ldstr_v1(std::istream& in)
{
   static const char MODULE[] = "ossimDoqq::ldstr_v1(istream& in)";

   if (!in)
   {
      theErrorStatus = OSSIM_ERROR;
      return;
   }

   char tmp1[DATA_ORDER_SIZE+1];
   in.seekg(DATA_ORDER_OFFSET, std::ios::beg);
   in.get(tmp1, DATA_ORDER_SIZE+1);
   theDataOrder = tmp1;

   //***
   // Perform a sanity check on the data order just in case this isn't a
   // ossimDoqq file.
   //***  
   tmp1[DATA_ORDER_SIZE] = '\0';
   int data_order = atoi(tmp1);
   if ( (data_order != 1) && (data_order != 2) )
   {
      theErrorStatus = OSSIM_ERROR;

      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " NOTICE:\n"
            << "Invalid data ordering.  Not a doq?" << std::endl;
      }
   }
   
   char tmp2[LINE_SIZE+1];
   in.seekg(LINE_OFFSET, std::ios::beg);
   in.get(tmp2, LINE_SIZE+1);
   theLine = atoi(tmp2);

   char tmp3[SAMPLE_SIZE+1];
   in.seekg(SAMPLE_OFFSET,std::ios::beg);
   in.get(tmp3, SAMPLE_SIZE+1); 
   theSample = atoi(tmp3);

   // Check for valid lines and samples.
   if (theLine <= 0 || theSample <= 0)
   {
      theErrorStatus = OSSIM_ERROR;

      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " ERROR:\n"
            << "\tInvalid lines or samples."
            << std::endl;
      }
      
      return;
   }
   
   char tmp4[PROJECTION_SIZE+1];
   in.seekg(PROJECTION_OFFSET, std::ios::beg);
   in.get(tmp4, PROJECTION_SIZE+1);
   theProjection = tmp4;

   char tmp5[UTM_ZONE_SIZE+1];
   in.seekg(UTM_ZONE_OFFSET, std::ios::beg);
   in.get(tmp5, UTM_ZONE_SIZE+1);
   theUtmZone = atoi(tmp5);

   char tmp8[DATUM_SIZE+1];
   in.seekg(DATUM_OFFSET, std::ios::beg);
   in.get(tmp8, DATUM_SIZE+1);
   theDatum = tmp8;

   char rgbType[RGB_SIZE+1];
   in.seekg(RGB_OFFSET, std::ios::beg);
   in.get(rgbType, RGB_SIZE+1);


   if(atoi(rgbType) == 5)
   {
      theRgb = 3;
   }
   else
   {
      theRgb = 1;
   }
   
   theHeaderSize = (theSample * theRgb * 4);

   // Calculate the size of each record.
   theRecordSize = (theSample * theRgb);

   char tmp6[UL_EASTING_SIZE+1];
   in.seekg( (theRecordSize * 2) + UL_EASTING_OFFSET, std::ios::beg);
   in.get(tmp6, UL_EASTING_SIZE+1);

   char tmp7[UL_NORTHING_SIZE+1];
   in.seekg( (theRecordSize * 2) + UL_NORTHING_OFFSET, std::ios::beg);
   in.get(tmp7, UL_NORTHING_SIZE+1);
   
   // Get Easting and Northing.
   theEasting = convertStr(tmp6);
   theNorthing = convertStr(tmp7);

   char tmp10[GSD_SIZE+1];
   in.seekg( (theRecordSize*3) + GSD_X_OFFSET, std::ios::beg);
   in.get(tmp10, GSD_SIZE+1);
   theGsd.x = std::abs(ossimString(tmp10, tmp10+GSD_SIZE).toDouble());
   in.seekg( (theRecordSize*3) + GSD_Y_OFFSET, std::ios::beg);
   in.get(tmp10, GSD_SIZE+1);
   theGsd.y = std::abs(ossimString(tmp10, tmp10+GSD_SIZE).toDouble());
   
}

ossim_float64 ossimDoqq::convertStr(const char* str) const
{
   //***
   // The USGS Projection Parameters in a OssimDoqq header have a "D" to
   // denote the exponent.  This simply substitutes the "D" with an
   // "E" so that atof works properly.
   //***
   ossimString tmp = str;
   tmp.gsub("D", "E");
   return tmp.toFloat64();
}

std::ostream& ossimDoqq::print(std::ostream& out) const
{
   const char* prefix = "doqq.";
   out << prefix << "Quadrangle_Name: " << theQuadName << std::endl;
   out << prefix << "Quadrant: " << theQuad << std::endl;
   out << prefix << "Image_Source: " << theImageSource << std::endl;
   out << prefix << "Source_Image_ID: " << theSourceImageID << std::endl;
   out << prefix << "RMSE_XY: " << theRMSE << std::endl;
   out << prefix << "Line: " << theLine << std::endl;
   out << prefix << "Sample: " << theSample << std::endl;
   out << prefix << "Projection: " << theProjection << std::endl;
   out << prefix << "Datum: " << theDatum << std::endl;
   out << prefix << "Easting: " << theEasting << std::endl;
   out << prefix << "Northing: " << theNorthing << std::endl;
   out << prefix << "UN: " << theUN << std::endl;
   out << prefix << "UE: " << theUE << std::endl;
   out << prefix << "LN: " << theLN << std::endl;
   out << prefix << "LE: " << theLE << std::endl;
   out << prefix << "GSD: " << theGsd << std::endl;
   out << prefix << "Band: " << theRgb << std::endl;
   out << prefix << "Utm_Zone: " << theUtmZone << std::endl;
   
   return out;
}
