//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Chong-Ket Chuah
// 
// Description:
// Implementation of ossimERS class for parsing a ER Mapper raster
// file format header.
//
//********************************************************************
// $Id: ossimERS.cpp,v 1.19 2004/10/01 15:42:10 dburken Exp $

#include <fstream>
#include <iostream>
using namespace std;

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <base/common/ossimCommon.h>
#include <base/common/ossimTrace.h>
#include <support_data/ers/ossimERS.h>
#include <base/common/ossimKeywordNames.h>
#include <base/context/ossimNotifyContext.h>

static ossimTrace traceDebug("ossimERS:degug");


ossimERS::ossimERS()
   :
      ossimErrorStatusInterface()
{
   clearFields();
}

ossimERS::ossimERS(const char* headerFile)
   :
      ossimErrorStatusInterface()
{
   clearFields();

   // open the header file:
   std::ifstream in;
   in.open(headerFile, ios::in | ios::binary);

   if (!in)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
   }

   // Begin parsing records:
   if (!theErrorStatus)
   {
      parseHeader(in);
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimERS DEBUG:"
         << *this
         << endl;
   }
}

void ossimERS::parseError(char *msg)
{
   theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
}

void ossimERS::parseRasterInfo(std::istream& in)
{
   vector<ossimString> tokens;

   bool done = false;
   while (!done)
   {
      tokens = parseLine(in);
      if (tokens.empty())
      {
         parseError("premature end of header file.");
         return;
      }
      if (tokens[0] == "CellType")
      {
         if (tokens[1] == "Unsigned8BitInteger")
         {
            theCelltype = OSSIM_UCHAR;
         }
         else if (tokens[1] == "Unsigned16BitInteger")
         {
            theCelltype = OSSIM_USHORT16;
         }
         else if (tokens[1] == "Signed16BitInteger")
         {
            theCelltype = OSSIM_SSHORT16;
         }
         else if (tokens[1] == "IEEE4ByteReal")
         {
            theCelltype = OSSIM_FLOAT;
         }
         else if (tokens[1] == "IEEE8ByteReal")
         {
            theCelltype = OSSIM_DOUBLE;
         }
         else
         {
            parseError("Unsupported CellType.");
         }
      }
      else if (tokens[0] == "NullCellValue")
      {
         theHasNullCells = true;
         theNullCell = tokens[1].toInt();	 // float nullcell value?
      }
      else if (tokens[0] == "Xdimension")
      {	// ignore "CellInfo"
         theCellSizeX = tokens[1].toDouble();
      }
      else if (tokens[0] == "Ydimension")
      {	// ignore "CellInfo"
         theCellSizeY = tokens[1].toDouble();
      }
      else if (tokens[0] == "NrOfLines")
      {
         theLine = tokens[1].toInt();
      }
      else if (tokens[0] == "NrOfCellsPerLine")
      {
         theSample = tokens[1].toInt();
      }
      else if (tokens[0] == "Eastings")
      {	// ignore "RegistrationCoord"
         theTieUnitType = OSSIM_METERS;
         theOriginX = tokens[1].toDouble();
      }
      else if (tokens[0] == "Northings")
      {	// ignore "RegistrationCoord"
         theTieUnitType = OSSIM_METERS;
         theOriginY = tokens[1].toDouble();
      }
      else if (tokens[0] == "Longitude")
      {	// ignore "RegistrationCoord"
                                                // need to check coordinatestype ("Cannot use Latitude with UTM projection")
         double deg, min, sec;
         sscanf (tokens[1].chars(), "%lg:%lg:%lg", &deg, &min, &sec);
         theOriginX = ossimAbs(deg) + min/60.0 + sec/3600.0;
         if (deg < 0.0)
         {
            theOriginX = -theOriginX;
         }
         theTieUnitType = OSSIM_DEGREES;
      }
      else if (tokens[0] == "Latitude")
      {	// ignore "RegistrationCoord"
         double deg, min, sec;
         sscanf (tokens[1].chars(), "%lg:%lg:%lg", &deg, &min, &sec);
         theOriginY = ossimAbs(deg) + min/60.0 + sec/3600.0;
         if (deg < 0.0)
         {
            theOriginY = -theOriginY;
         }
         theTieUnitType = OSSIM_DEGREES;
      }
      else if (tokens[0] == "NrOfBands")
      {
         theBands = tokens[1].toInt();
      }
      else if (tokens[0] == "Value")
      {	// ignore "BandID"
        // keep adding to the bandID vector
        // delete Value key then join the rest of tokens
         tokens.erase(tokens.begin());
         ossimString id;
         id.join(tokens, " ");
         theBandID.push_back(id);
      }
      else if (tokens[0] == "RasterInfo")
      {
         done = true;
      }
   }
}


void ossimERS::parseCoordinateSpace(std::istream& in)
{
   vector<ossimString> tokens;

   bool done = false;
   while (!done)
   {
      tokens = parseLine(in);
      if (tokens.empty())
      {
         parseError("premature end of header file.");
         return;
      }
      if (tokens[0] == "Datum")
      {
         theDatum = tokens[1];
         if (theDatum != "WGS84" && theDatum != "RAW")
         {
            parseError("Datum must be WGS84 or RAW");
         }
      }
      else if (tokens[0] == "Projection")
      {
         theProjection = tokens[1];
         // need to parse projection string
         /*(EqualStrings (tok(1,3), "UTM")) {
           coordinateSystem = 'U';
           if (tok(0) == 'N')
           northernHemisphere = 1;
           else
           northernHemisphere = 0;
           spString zoneStr = tok(4,2);
           sscanf (zoneStr.chars(), "%d", &zone);
         */
      }
      else if (tokens[0] == "CoordinateType")
      {
         // ignore it
      }
      else if (tokens[0] == "Units")
      {
         // ignore it for now
      }
      else if (tokens[0] == "Rotation")
      {
         double deg, min, sec;
         sscanf (tokens[1].chars(), "%lg:%lg:%lg", &deg, &min, &sec);
         theRotation = deg + min/60.0 + sec/3600.0;
      }
      else if (tokens[0] == "CoordinateSpace")
      {
         done = true;
      }
   }

   // requires more error checking on unit and rotation here
   return;
}

void ossimERS::parseHeader(std::istream& in)
{
   theErrorStatus = ossimErrorCodes::OSSIM_OK;
   char magicNumberTest[14];
   in.read(magicNumberTest, 13);
   if(ossimString(magicNumberTest,
                  magicNumberTest+13) != "DatasetHeader")
   {
      parseError("First line must be DatasetHeader");
      return;
   }
   in.seekg(0);
   vector<ossimString> tokens = parseLine(in);
   if(!tokens.size())
   {
      parseError("First line must be DatasetHeader");
      return;
   }
   if (tokens[0] != "DatasetHeader")
   {
      parseError("First line must be DatasetHeader");
      return;
   }

   bool done = false;
   while (!done)
   {
      tokens = parseLine(in);
      if (tokens.empty())
      {
         parseError("premature end of header file.");
         return;
      }
      if (tokens[0] == "Version")
      {
         theVersion = tokens[1].toDouble();
         if (theVersion < 4.0)
         {
            parseError("Version must be 4.0 or greater");
         }
      }
      else if (tokens[0] == "DataSetType")
      {
         theDatasetType = tokens[1];
         if (theDatasetType != "ERStorage")
         {
            parseError("DataSetType must be ERStorage");
         }
      }
      else if (tokens[0] == "DataType")
      {
         theDatatype = tokens[1];
         if (theDatatype != "Raster")
         {
            parseError("DataType must be Raster");
         }
      }
      else if (tokens[0] == "ByteOrder")
      {
         theByteorder = tokens[1];
         if (theByteorder != "MSBFirst" && theByteorder != "LSBFirst")
         {
            parseError("ByteOrder must either be MSBFirst or LSBFirst");
         }
      }
      else if (tokens[0] == "CoordinateSpace")
      {
         if (tokens[1] == "Begin")
         {
            parseCoordinateSpace(in);
         }
      }
      else if (tokens[0] == "RasterInfo")
      {
         if (tokens[1] == "Begin")
         {
            parseRasterInfo(in);
         }
      }
      else if (tokens[0] == "SenseDate")
      {
         // ignore for now
      }
      else if (tokens[0] == "Comments")
      {
         // ignore for now
      }
      else if (tokens[0] == "DatasetHeader")
      {
         done = true;
      }
   }

   if(theProjection.contains("GEODETIC"))
   {
      theTieUnitType = OSSIM_DEGREES;
   }

   return;
}


// read a line from the file, split it using "= \t\n" delimiters.
// if the first token is empty, if the line begins with a tab, delete
// the token.  The second token, the value for the key, may be quoted.
// trim the quotes.
vector<ossimString> ossimERS::parseLine(std::istream& in)
{
/*   const int bufSize = 500; */
   ossimString line;
   vector<ossimString> tokens;
   bool invalidCharHit = false;
   const int MAX_LENGTH = 10000;
   int tempCount = 0;
   // read a line, skipping empty line
   while (tokens.empty()&&(in)&&(tempCount < MAX_LENGTH))
   {
     tempCount = 1;
      char c = in.get();
      while( (c != '\n')&&
             (!in.eof())&&
             (!invalidCharHit))
      {
         if(c > 0x7e)
         {
            invalidCharHit = true;
         }
         else
         {
            line += (char)c;
            c = in.get();
	    ++tempCount;
         }
      }

      if(!invalidCharHit)
      {
         line.trim('\t');
         line.trim('\n');
         line.trim('\r');
         line.trim(' ');

         if(line != "")
         {
            tokens = line.split("= \t");
            if (tokens.size() > 1)
            {
               tokens[1].trim('\"');
               tokens.back().trim('\"');
            }
         }
      }
      else
      {
         tokens.clear();
         return tokens;
      }
   }
   if(in.bad()||(tempCount>=MAX_LENGTH))
     {
       tokens.clear();
     }

   return tokens;
}


std::ostream& ossimERS::print(std::ostream& out) const
{
   out << setw(30) << "\ndatasetType:      " << theDatasetType
       << setw(30) << "\ndatatype:	  " << theDatatype
       << setw(30) << "\nbyteorder:        " << theByteorder
       << setw(30) << "\ncomments:         " << theComments;
   
   //  CoordSpace
   out << setw(30) << "\ndatum:            " << theDatum
       << setw(30) << "\nprojection:       " << theProjection
       << setw(30) << "\ncoordsystype:     " << theCoordSysType
       << setw(30) << "\nunits:            " << theUnits
       << setw(30) << "\nrotation:         " << theRotation;
   
   
   // RasterInfo
   out << setw(30) << "\ncelltype:          " << theCelltype
       << setw(30) << "\nhasNullcells:      " << theHasNullCells
       << setw(30) << "\nnullcell:          " << theNullCell
       << setw(30) << "\ncellsizeX:         " << theCellSizeX
       << setw(30) << "\ncellsizeY:         " << theCellSizeY
       << setw(30) << "\ntheLine:           " << theLine
       << setw(30) << "\ntheSample:         " << theSample
       << setw(30) << "\ntheBands:          " << theBands
       << setw(30) << "\noriginX:           " << theOriginX
       << setw(30) << "\noriginY:           " << theOriginY;
   
   for (int i=0; i<(int)theBandID.size(); i++)
   {
      out << "\nBandID["<<i<<"]:             " << theBandID[i];
   }
   
   out << endl;
   
   return out;
}

bool ossimERS::toOssimProjectionGeom(ossimKeywordlist& kwl,
                                     const char* prefix)const
{
   ossimString proj  = toOssimProjection();
   ossimString datum = toOssimDatum();

   if(proj == "")
   {
      return false;
   }

   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           proj.c_str(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::DATUM_KW,
           datum.c_str(),
           true);

   // if it's UTM we have to extract out zone and hemisphere
   // from the projection name stored by ERMapper.
   //
   if(proj == "ossimUtmProjection")
   {
      ossimString zone(theProjection.begin()+4,
                       theProjection.end());
      ossimString hemisphere(theProjection.begin(),
                             theProjection.begin()+1);
      kwl.add(prefix,
              ossimKeywordNames::ZONE_KW,
              zone.c_str(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::HEMISPHERE_KW,
              hemisphere.c_str(),
              true);
   }

   if(theTieUnitType == OSSIM_METERS)
   {
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_EASTING_KW,
              theOriginX + theCellSizeX/2,
              true);
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_NORTHING_KW,
              theOriginY - theCellSizeY/2,
              true);
      kwl.add(prefix,
              ossimKeywordNames::METERS_PER_PIXEL_X_KW,
              theCellSizeX,
              true);
      kwl.add(prefix,
              ossimKeywordNames::METERS_PER_PIXEL_Y_KW,
              theCellSizeY,
              true);

   }
   else if(theTieUnitType == OSSIM_DEGREES)
   {
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_LAT_KW,
              theOriginY - theCellSizeY/2,
              true);
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_LON_KW,
              theOriginX + theCellSizeX/2,
              true);
      kwl.add(prefix,
              ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON,
              theCellSizeX,
              true);
      kwl.add(prefix,
              ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT,
              theCellSizeY,
              true);
   }

   return true;
}

ossimString ossimERS::toOssimProjection()const
{
   ossimString result = "";

   if(theProjection.contains("UTM"))
   {
      result = "ossimUtmProjection";
   }
   else if(theProjection.contains("GEODETIC"))
   {
      result = "ossimEquDistCylProjection";
   }

   return result;
}

ossimString ossimERS::toOssimDatum()const
{
   ossimString result = "WGE"; // wgs 84 default datum code

   if(theDatum != "WGS84")
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING Datum ossimERS::toOssimDatum: " << theDatum << " is not handled in ossimERS::toOssimDatum()\n"
                                            << "Please notify us with the datum name so we can add it\n";
   }

   return result;
}

//***************************************************************************
// PRIVATE METHOD: ossimERS::initialize()
// Initializes all fields to blanks (or 0's) and null terminates strings.
//***************************************************************************
void ossimERS::clearFields()
{
//   static const char source[] = "";

   theErrorStatus = ossimErrorCodes::OSSIM_OK;
   theVersion       = 0;
   theFilename      = "";
   theDescription   = "";
   theSensorname    = "";
   theHeaderOffset = 0;
   theDatasetType   = "";
   theDatatype      = "";
   theByteorder     = "";
   theComments      = "";
   theDatum         = "";
   theProjection    = "";
   theCoordSysType  = "";
   theUnits         = "";
   theRotation      = 0.0;
   theCelltype      = OSSIM_SCALAR_UNKNOWN;
   theCellsizeof    = 0;
   theHasNullCells  = false;
   theNullCell      = 0;
   theCellSizeX     = 0;
   theCellSizeY     = 0;
   theLine       = 0;
   theSample     = 0;
   theOriginX       = 0;
   theOriginY       = 0;
   theBands      = 0;
   theTieUnitType= OSSIM_UNIT_UNKNOWN;
   theBandID.clear();

   return;
}
