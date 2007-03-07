//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
// 
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero (kmelero@imagelinks.com)
//         Orginally written by Oscar Kramer (SoBe Software)
//
// Description: Container class for LandSat7 Fast Format header files.
//
//********************************************************************
// $Id: ossimFfL7.h,v 1.11 2005/11/10 15:02:02 gpotts Exp $

#ifndef ossimFfL7_HEADER
#define ossimFfL7_HEADER
#include <iostream>
using namespace std;

#include <cstdio>
#include <base/common/ossimErrorStatusInterface.h>
#include <base/data_types/ossimString.h>
#include <base/data_types/ossimDpt.h>
#include <base/data_types/ossimGpt.h>
#include <base/data_types/ossimIpt.h>

// Forward class declarations.

// ***************************************************************************
// CLASS: ossimFfL7
// ***************************************************************************

class OSSIMDLLEXPORT ossimFfL7 : public ossimErrorStatusInterface
{
   friend ostream& operator<<(ostream& os, const ossimFfL7& head);

public:
   ossimFfL7  ();
   ossimFfL7  (const char* headerFile);
   virtual ~ossimFfL7();
   
   void   dump(ostream& os) const;
      //> Provides a readable dump of this object.
      //<
   void   writeHeader(ostream& os) const;
      //> Writes the full header to the output stream in proper L7A format.
      //  NOT YET IMPLEMENTED
      //<
   int    path() const { return thePathNumber; }
      //> Returns the path as an int.
      //  Currently uses the1stPathRowNumber string.
      //<
   int    row() const { return theRowNumber; }
      //> Returns the row as an int from the pathRowString.
      //  Currently uses the1stPathRowNumber string.
      //<
   ossimString pathRow() const;
      //> Returns a string represnting the path and row as a name.
      //  i.e.: PPP/RRFFSS will be returned as pPPPrRRFFSS
      //  This method will remove any spaces.
      //<

   /*!
    * Initializes "bias" with "theBias" from the radiometry record.
    * Contains eight bands; although, only bands present will be initialized.
    */
   void getBias(vector<double>& bias) const;
   double getBias(long bandIdx)const
      {
         return theBias[bandIdx];
      }

   /*!
    * Initializes "bias" with "theBias" from the radiometry record.
    * Contains eight bands; although, only bands present will be initialized.
    */
   void getGain(vector<double>& gain) const;
   double getGain(long bandIdx)const
      {
         return theGain[bandIdx];
      }

   double getParam(ossim_uint32 i)const
      {
         if(i < 16)
         {
            return theProjectionParams[i];
         }
         return 0.0;
      }

   virtual unsigned int getBandCount()const;
   
   long getJulianDay()const;
   // The Admin Record:
   char   theRequestNumber[21];     // NNNYYMMDDSSSSUUUU
   char   theLocationCode[18];      // ppp/rrrffss
   int    thePathNumber;
   int    theRowNumber;
   char   theAcquisitionDate[20];   // yyyymmdd
   char   theSatName[11];           // "LANDSAT7"
   char   theSensorName[11];        // "ETM+"
   char   theSensorMode[7];         // "NORMAL"
   double theOffNadirAngle;         // degrees
   char   theProductType[19];       // "MAPbORIENTED"|"ORBITbORIENTED"
   char   theProductSize[11];       // ("FULLb"|"SUB"|"MULTI")+"SCENE"
   char   theProcessingType[12];    // "SYSTEMATIC"
   char   theResampAlgorithm[3];    // "CC"|"NN"|"MF"
   int    thePixelsPerLine;
   int    theLinesPerBand;
   int    theRecordSize;
   double theGsd;
   int    theOutputBitsPerPixel;
   int    theAcquiredBitsPerPixel;
   char   theBandsPresentString[33];
   char   theBandFileNames[8][30];

   // The Radiometry Record:
   vector<double> theBias;
   vector<double> theGain;

   // The Geometry Record:
   char    theMapProjectionName[5];  
   char    theEllipsoid[19];        // "WGS84"
   char    theDatum[7];             // "WGS84"
   double  theProjectionParams[15];
   int     theUsgsMapZone;

   ossimGpt theUL_Corner;
   ossimGpt theUR_Corner;
   ossimGpt theLR_Corner;
   ossimGpt theLL_Corner;
   ossimGpt theCenterGP;

   ossimIpt theCenterImagePoint;

   int     theHorizontalOffset;   // pixels
   double  theOrientationAngle;   // degrees
   double  theSunElevation;       // degrees
   double  theSunAzimuth;         // degrees

private:
   void        initialize();
   void        readAdminRecord(FILE* fptr);
   void        readRadiomRecord(FILE* fptr);
   void        readGeomRecord(FILE* fptr);
   int         convertGeoPoint(FILE*, ossimGpt&);

};

#endif
