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
// This class parses a ER Mapper raster file format header.
//
//********************************************************************
// $Id: ossimERS.h,v 1.7 2004/10/01 15:42:10 dburken Exp $

#ifndef ossimERS_HEADER
#define ossimERS_HEADER
#include <iostream>
using namespace std;

#include <cstdio>
#include <base/common/ossimErrorStatusInterface.h>
#include <base/common/ossimConstants.h>
#include <base/data_types/ossimString.h>
#include <base/data_types/ossimFilename.h>
#include <base/data_types/ossimKeywordlist.h>

class ossimERS : public ossimErrorStatusInterface
{
public:
   ossimERS(const char* file);
   ~ossimERS() {};

   void clearFields();
   // DatasetHeader 
   double      theVersion;
   ossimString theFilename;
   ossimString theDescription;
   
//   tm lastupdated;
   ossimString theSensorname;
//   tm sensedate;
   ossim_int32       theHeaderOffset;				// for non-ermapper file
   ossimString theDatasetType;		// ERStorage
   ossimString theDatatype;			// Raster
   ossimString theByteorder;			//MSBFirst or LSBFirst
   ossimString theComments;				// arbitrary comment field
   
   //  CoordSpace 
   ossimString theDatum;				/* geodetic datum */
   ossimString theProjection;
   ossimString theCoordSysType;
   ossimString theUnits;
   double      theRotation;

   ossimUnitType theTieUnitType;
   // RasterInfo 
   ossimScalarType theCelltype;
   ossim_int32 theCellsizeof;
   bool theHasNullCells;
   double theNullCell;
   double theCellSizeX;
   double theCellSizeY;
   ossim_int32 theLine;
   ossim_int32 theSample;
   double theOriginX;
   double theOriginY;
   ossim_int32 theBands;
   vector<ossimString> theBandID;
   
   virtual std::ostream& print(std::ostream& out) const;
   
   bool toOssimProjectionGeom(ossimKeywordlist& kwl,
                              const char* prefix=NULL)const;
   ossimString toOssimProjection()const;
   ossimString toOssimDatum()const;
   ossim_int32 errorStatus() const { return theErrorStatus; }
   
private:
   // prevent use
   ossimERS();
   void parseError(char *msg);
   vector<ossimString> parseLine(std::istream& in);
   void parseHeader(std::istream& fptr);
   void parseRasterInfo(std::istream& fptr);
   void parseCoordinateSpace(std::istream& fptr);
};

#endif
