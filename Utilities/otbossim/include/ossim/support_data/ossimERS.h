//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Chong-Ket Chuah
// Contributor: Anrew Huang
//
// Description: 
// This class parses a ER Mapper raster file format header.
//
//********************************************************************
// $Id: ossimERS.h 13216 2008-07-23 18:56:53Z dburken $

#ifndef ossimERS_HEADER
#define ossimERS_HEADER

#include <iosfwd>
#include <vector>

#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>

class ossimERS : public ossimErrorStatusInterface
{
public:

   ossimERS();
   ossimERS(const char* file);
   virtual ~ossimERS() {};

   void clearFields();
   // DatasetHeader 
   double      theVersion;
   ossimString theFilename;
   ossimString theDescription;
   
//   tm lastupdated;
   ossimString theSensorname;
//   tm sensedate;
   ossim_int32       theHeaderOffset; // for non-ermapper file
   ossimString theDatasetType;	      // ERStorage
   ossimString theDatatype;           // Raster
   ossimString theByteorder;          //MSBFirst or LSBFirst
   ossimString theComments;           // arbitrary comment field
   
   //  CoordSpace 
   ossimString theDatum; /* geodetic datum */
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
   std::vector<ossimString> theBandID;
   
   virtual std::ostream& print(std::ostream& out) const;
   
   bool writeFile(const ossimFilename& file);
   
   bool toOssimProjectionGeom(ossimKeywordlist& kwl,
                              const char* prefix=NULL)const;
   ossimString toOssimProjection()const;
   ossimString toOssimDatum()const;
   ossim_int32 errorStatus() const { return theErrorStatus; }
   
private:

   void parseError(const char *msg);
   std::vector<ossimString> parseLine(std::istream& in);
   void parseHeader(std::istream& fptr);
   void parseRasterInfo(std::istream& fptr);
   void parseCoordinateSpace(std::istream& fptr);
};

#endif
