//*******************************************************************
// 
// License:  See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
//         Orginally written by Oscar Kramer (SoBe Software)
//
// Description: Container class for LandSat7 Fast Format header files.
//
//********************************************************************
// $Id: ossimFfL7.h 15766 2009-10-20 12:37:09Z gpotts $

#ifndef ossimFfL7_HEADER
#define ossimFfL7_HEADER

#include <vector>
#include <iostream>

#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimPropertyInterface.h>

#include <ossim/base/ossimString.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimRefPtr.h>

class ossimProperty;
class ossimDate;


// ***************************************************************************
// CLASS: ossimFfL7
// ***************************************************************************

class OSSIMDLLEXPORT ossimFfL7 : public ossimReferenced,
                                 public ossimErrorStatusInterface,
                                 public ossimPropertyInterface
{
   friend std::ostream& operator<<(std::ostream& os, const ossimFfL7& head);

public:
   ossimFfL7  ();
   ossimFfL7  (const char* headerFile);
   
   void   dump(std::ostream& os) const;
      //> Provides a readable dump of this object.
      //<
   void   writeHeader(std::ostream& os) const;
      //> Writes the full header to the output stream in proper L7A format.
      //  NOT YET IMPLEMENTED
      //<
   int    path() const;
      //> Returns the path as an int.
      //  Currently uses the1stPathRowNumber string.
      //<
   int    row() const;
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
   double getBias(long bandIdx)const;

   /*!
    * Initializes "bias" with "theBias" from the radiometry record.
    * Contains eight bands; although, only bands present will be initialized.
    */
   void getGain(vector<double>& gain) const;
   double getGain(long bandIdx)const;

   double getParam(ossim_uint32 i)const;

   virtual unsigned int getBandCount()const;
   
   long getJulianDay()const;

   /** @return The acquisition date string. */
   ossimString getAcquisitionDate() const;

   /**
    * @brief Gets the date as an ossimDate.
    * @param date to initialize.
    */
   void getAcquisitionDate(ossimDate& date)const;

   /** @return The satellite name "theSatName". */
   ossimString getSatelliteName() const;

   /** @return The filename for band. */
   ossimFilename getBandFilename(ossim_uint32 idx)const;

   /** @return The number of lines per band. */
   int getLinesPerBand() const;

   /** @return The number of pixels per line (samples). */
   int getPixelsPerLine() const;

   /**
    * @brief Gets the sun elevation in degrees.
    * @param elevation The elevation to initialize.
    */
   void getSunElevation(double& elevation) const;

   /**
    * @brief Gets the sun azimuth in degrees.
    * @param azimuth The azimuth to initialize.
    */
   void getSunAzimuth(double& azimuth) const;

   /**
    * @brief Gets a property for matching name.
    * @param name The name of the property to get.
    * @return Returns property matching "name".
    */
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;

   /**
    * @brief Gets a list of property names available.
    * @param propertyNames The list to push back names to.
    */
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

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

protected:
   virtual ~ossimFfL7();

private:
   
   void initialize();
   void readAdminRecord(FILE* fptr);
   void readRadiomRecord(FILE* fptr);
   void readGeomRecord(FILE* fptr);
   int  convertGeoPoint(FILE*, ossimGpt&);

};

#endif
