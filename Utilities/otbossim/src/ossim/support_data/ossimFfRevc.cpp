//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Ken Melero (kmelero@imagelinks.com)
//         Orginally written by Dave Burken (dburken@imagelinks.com)
// Description: This class parses an EOSAT Fast Format rev c header.
//
//********************************************************************
// $Id: ossimFfRevc.cpp 13217 2008-07-23 19:19:13Z dburken $

#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

#include <cstdlib>
#include <cstring>

#include <ossim/support_data/ossimFfRevc.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimNotifyContext.h>

const int LOCATION_OFFSET [4] = { 23,
                                  183,
                                  343,
                                  503 };

const int PATH_ROW_NUMBER_OFFSET [4] = { 34,
                                         194,
                                         354,
                                         514 };

const int DATE_OFFSET [4] = { 70,
                              230,
                              390,
                              550 };

const int SAT_NAME_OFFSET [4] = { 91,
                                  251,
                                  411,
                                  571 };

const int SENSOR_NAME_OFFSET [4] = { 110,
                                     270,
                                     430,
                                     590 };

const int SENSOR_MODE_OFFSET [4] = { 134,
                                     294,
                                     454,
                                     614 };

const int OFF_NADIR_ANGLE_OFFSET [4] = { 153, 
                                         313,
                                         473,
                                         633 };


const int BIAS_OFFSET [8] = { 1616,
                              1696,
                              1776,
                              1856,
                              1936,
                              2016,
                              2096,
                              2176  };

const int GAIN_OFFSET [8] = { 1641,
                              1721,
                              1801,
                              1881,
                              1961,
                              2041,
                              2121,
                              2201  };

static const int PROJ_PARAM_OFFSET [15]  = { 3181,
                                      3206,
                                      3232,
                                      3257,
                                      3282,
                                      3312,
                                      3337,
                                      3362,
                                      3392,
                                      3417,
                                      3442,
                                      3472,
                                      3497,
                                      3522,
                                      3552  };

static const char BLANK_STRING[] = " ";

//***************************************************************************
// CONSTRUCTOR
//***************************************************************************
ossimFfRevc::ossimFfRevc()
   :
      theAdminRecord(),
      theRadiomRecord(),
      theGeoRecord(),
      theErrorStatus(OSSIM_OK)
{ }

//***************************************************************************
// CONSTRUCTOR:
// ossimFfRevc::ossimFfRevc(const char* headerFile)
//
// Takes a filename representing an IRS-1C Fast Format rev C header.
//***************************************************************************
ossimFfRevc::ossimFfRevc(const char* headerFile)
   :
   theErrorStatus(OSSIM_OK)
{
   ifstream is;

   is.open(headerFile);

   if (!is)
   {
      theErrorStatus = OSSIM_ERROR;

      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimFfRevc::ossimFfRevc(header): "
                                          << "Cannot open:  " << headerFile << "\n"
                                          << "Returning from constructor." << std::endl;

      return;
   }

   loadFromStream(is);

   is.close();

}


//***************************************************************************
// PUBLIC METHOD:
// int ossimFfRevc::path(int sceneNbr) const
//
// Convenience method to parse the path from the path/row string.  Returns
// zero if stream is not opened.  Current loacation field is:
// path/row/fraction/subscene in ppp/rrrffss format.
//***************************************************************************
int ossimFfRevc::path(int sceneNbr) const
{
   char tmpBuff[4];
   int path = 0;

   istringstream is(theAdminRecord.thePathRowNumber[sceneNbr]);

   if (is)
   {
      is.get(tmpBuff, 4);
      path = atoi(tmpBuff);
   }

   return path;
}


//***************************************************************************
// PUBLIC METHOD:
// int ossimFfRevc::row(int sceneNbr) const
//
// Convenience method to parse the row from the path/row string.  Returns
// zero if stream is not opened.  Current loacation field is:
// path/row/fraction/subscene in ppp/rrrffss format.
//***************************************************************************
int ossimFfRevc::row(int sceneNbr) const
{
   char tmpBuff[4];
   int row = 0;

   istringstream is(theAdminRecord.thePathRowNumber[sceneNbr]);

   if (is)
   {
      is.seekg(4, ios::beg);
      is.get(tmpBuff, 4);
      row = atoi(tmpBuff);
   }

   return row;
}


//***************************************************************************
// PUBLIC METHOD:
// int ossimFfRevc::fraction(int sceneNbr) const
//
// Convenience method to parse the fraction from the path/row string.  Returns
// zero if stream is not opened.  Current loacation field is:
// path/row/fraction/subscene in ppp/rrrffss format.
//***************************************************************************
int ossimFfRevc::fraction(int sceneNbr) const
{
   char tmpBuff[3];
   int fraction = 0;

   istringstream is(theAdminRecord.thePathRowNumber[sceneNbr]);

   if (is)
   {
      is.seekg(7, ios::beg);
      is.get(tmpBuff, 3);
      fraction = atoi(tmpBuff);
   }

   return fraction;
}


//***************************************************************************
// PUBLIC METHOD:
// int ossimFfRevc::subScene(int sceneNbr) const
//
// Convenience method to parse the subScene from the path/row string.  Returns
// zero if stream is not opened.  Current loacation field is:
// path/row/fraction/subscene in ppp/rrrffss format.
//***************************************************************************
ossimString ossimFfRevc::subScene(int sceneNbr) const
{
   char   tmpBuff[3];
   ossimString subScene;

   istringstream is(theAdminRecord.thePathRowNumber[sceneNbr]);

   if (is)
   {
      is.seekg(9, ios::beg);
      is.get(tmpBuff, 3);
      subScene = tmpBuff;
   }

   return subScene;
}

//***************************************************************************
// ossimFfRevc::pathRow(int sceneNbr)
//***************************************************************************
ossimString ossimFfRevc::pathRow(int sceneNbr) const
{
   ossimString tmp = "p";

   tmp += theAdminRecord.thePathRowNumber[sceneNbr];

   tmp.gsub("/", "r");

   tmp.gsub(" ", "");

   return tmp;
}

//***************************************************************************
// PUBLIC METHOD:
// void ossimFfRevc::print(ostream& os) const
//
// Prints data members.
//***************************************************************************
void ossimFfRevc::print(ostream& os) const
{
   os << setiosflags(ios::left | ios::fixed)
      << "Administrative Record:"
      << setw(30) << "\ntheProductOrderNumber:" 
      << theAdminRecord.theProductOrderNumber;

   int i;
   for (i = 0; i < NUMBER_OF_SCENES; i++)
   {
      os << "\nScene" << (i + 1) << setw(23) << " PathRowNumber:"
         << theAdminRecord.thePathRowNumber[i]
         << "\nScene" << (i + 1) << setw(23) << " AcquisitionDate:  "
         << theAdminRecord.theAcquisitionDate[i]
         << "\nScene" << (i + 1) << setw(23) << " SatelliteName:    "
         << theAdminRecord.theSatName[i]
         << "\nScene" << (i + 1) << setw(23) << " SensorName:       "
         << theAdminRecord.theSensorName[i]
         << "\nScene" << (i + 1) << setw(23) << " SensorMode:       "
         << theAdminRecord.theSensorMode[i]
         << "\nScene" << (i + 1) << setw(23) << " OffNadirAngle:    "
         << setprecision(2) << theAdminRecord.theOffNadirAngle[i];
   }

   os << setw(30) << "\nProductType:" 
      << theAdminRecord.theProductType
      << setw(30) << "\nProductSize:" 
      << theAdminRecord.theProductSize
      << setw(30) << "\nProcessingType:" 
      << theAdminRecord.theProcessingType 
      << setw(30) << "\nResampAlgorithm:" 
      << theAdminRecord.theResampAlgorithm
      << setw(30) << "\nTapeVolumeNumber:" 
      << theAdminRecord.theTapeVolumeNumber
      << setw(30) << "\nNumberVolumesPerTape:" 
      << theAdminRecord.theNumberVolumesPerTape
      << setw(30) << "\nPixelsPerLine:" 
      << theAdminRecord.thePixelsPerLine
      << setw(30) << "\nLinesPerImage:" 
      << theAdminRecord.theLinesPerImage
      << setw(30) << "\n1stLineInVolume:" 
      << theAdminRecord.the1stLineInVolume
      << setw(30) << "\nTapeBlockingFactor:" 
      << theAdminRecord.theTapeBlockingFactor
      << setw(30) << "\nRecordSize:" 
      << theAdminRecord.theRecordSize
      << setw(30) << "\nGsd:" 
      << setprecision(2) << theAdminRecord.theGsd
      << setw(30) << "\nOutputBitsPerPixel:" 
      << theAdminRecord.theOutputBitsPerPixel
      << setw(30) << "\nAcquiredBitsPerPixel:" 
      << theAdminRecord.theAcquiredBitsPerPixel
      << setw(30) << "\nBandsPresentString:" 
      << theAdminRecord.theBandsPresentString
      << setw(30) << "\nFormatVersion:" 
      << theAdminRecord.theFormatVersion
      << endl 
      << setw(30) << "Radiometric record:";

   os << setprecision(15);

   for (i = 0; i < NUMBER_OF_BANDS; i++)
   {
      os << "\nBand" << (i + 1) << setw(24) << " bias:" 
         << theRadiomRecord.theBias[i]
         << "\nBand" << (i + 1) << setw(24) << " gain:" 
         << theRadiomRecord.theGain[i];
   }

   os << endl
      << "Geometric Record:"
      << setw(30) << "\nMapProjectionName:" 
      << theGeoRecord.theMapProjectionName
      << setw(30) << "\nEllipsoid:" 
      << theGeoRecord.theEllipsoid
      << setw(30) << "\nDatum:" 
      << theGeoRecord.theDatum;

   for (i = 0; i < 15; i++)
   {
      os << "\nProjectionParams[" << setw(2) << i << setw(10) << "]:  "
         << theGeoRecord.theProjectionParams[i];
   }

   os << setprecision(3) 
      << setw(30) << "\nUlLon:" 
      << theGeoRecord.theUlLon
      << setw(30) << "\nUlLat:" 
      << theGeoRecord.theUlLat
      << setw(30) << "\nUlEasting:" 
      << theGeoRecord.theUlEasting
      << setw(30) << "\nUlNorthing:" 
      << theGeoRecord.theUlNorthing   
      << setw(30) << "\nUrLon:" 
      << theGeoRecord.theUrLon
      << setw(30) << "\nUrLat:" 
      << theGeoRecord.theUrLat
      << setw(30) << "\nUrEasting:" 
      << theGeoRecord.theUrEasting
      << setw(30) << "\nUrNorthing:" 
      << theGeoRecord.theUrNorthing
      << setw(30) << "\nLrLon:" 
      << theGeoRecord.theLrLon
      << setw(30) << "\nLrLat:" 
      << theGeoRecord.theLrLat
      << setw(30) << "\nLrEasting:" 
      << theGeoRecord.theLrEasting
      << setw(30) << "\nLrNorthing:" 
      << theGeoRecord.theLrNorthing
      << setw(30) << "\nLlLon:" 
      << theGeoRecord.theLlLon
      << setw(30) << "\nLlLat:" 
      << theGeoRecord.theLlLat
      << setw(30) << "\nLlEasting:" 
      << theGeoRecord.theLlEasting
      << setw(30) << "\nLlNorthing:" 
      << theGeoRecord.theLlNorthing
      << setw(30) << "\nCenterLon:" 
      << theGeoRecord.theCenterLon
      << setw(30) << "\nCenterLat:" 
      << theGeoRecord.theCenterLat
      << setw(30) << "\nCenterEasting:" 
      << theGeoRecord.theCenterEasting
      << setw(30) << "\nCenterNorthing:" 
      << theGeoRecord.theCenterNorthing
      << setw(30) << "\nCenterSample:" 
      << theGeoRecord.theCenterSample
      << setw(30) << "\nCenterLine:" 
      << theGeoRecord.theCenterLine
      << setw(30) << "\nHorizontalOffset:" 
      << theGeoRecord.theHorizontalOffset
      << setprecision(2) << setw(30) << "\nOrientationAngle:" 
      << theGeoRecord.theOrientationAngle
      << setw(30) << "\nSunElevationAngle:" 
      << setprecision(1) << theGeoRecord.theSunElevationAngle
      << setw(30) << "\nSunAzimuth:" <<  theGeoRecord.theSunAzimuth
      << resetiosflags(ios::left)
      << endl << endl;

}

//***************************************************************************
// PUBLIC METHOD:
// void ossimFfRevc::write(ostream& os) const
//
// Writes data members in EOSAT Fast Format Rev C format.
//***************************************************************************
void ossimFfRevc::write(ostream& os) const
{
   const char PRODUCT_ID_DESC [PRODUCT_ORDER_NUMBER_DESC_SIZE + 1]
      = "PRODUCT ID =";

   const char LOCATION_DESC [LOCATION_DESC_SIZE + 1]
      = " LOCATION =";

   const char DATE_DESC [DATE_DESC_SIZE + 1]
      = " ACQUISITION DATE =";
   
   const char SATELLITE_NAME_DESC [SAT_NAME_DESC_SIZE + 1]
      = "SATELLITE =";
   
   const char SENSOR_NAME_DESC [SENSOR_NAME_DESC_SIZE + 1]
      = " SENSOR =";
   
   const char SENSOR_MODE_DESC [SENSOR_MODE_DESC_SIZE + 1]
      = " SENSOR MODE =";
      
   const char LOOK_ANGLE_DESC [LOOK_ANGLE_DESC_SIZE + 1]
      = " LOOK ANGLE =";
   
   const char PRODUCT_TYPE_DESC [PRODUCT_TYPE_DESC_SIZE + 1]
      = "PRODUCT TYPE =";
   
   const char PRODUCT_SIZE_DESC [PRODUCT_SIZE_DESC_SIZE + 1]
      = " PRODUCT SIZE =";
   
   const char PROCESSING_TYPE_DESC [PROCESSING_TYPE_DESC_SIZE + 1]
      = "TYPE OF PROCESSING =";
   
   const char RESAMPLING_ALGO_DESC [RESAMPLING_ALGO_DESC_SIZE + 1]
      = " RESAMPLING =";
   
   const char TAPE_VOLUME_NUMBER_DESC [TAPE_VOLUME_NUMBER_DESC_SIZE + 1]
      = "VOLUME #/# IN SET =";
   
   const char PIXELS_PER_LINE_DESC [PIXELS_PER_LINE_DESC_SIZE + 1]
      = " PIXELS PER LINE =";
   
   const char LINES_PER_IMAGE_DESC [LINES_PER_IMAGE_DESC_SIZE + 1]
      = " LINES PER BAND =";

   const char  FIRST_LINE_DESC [FIRST_LINE_DESC_SIZE + 1]
      = "START LINE # =";
   
   const char BLOCKING_FACTOR_DESC [BLOCKING_FACTOR_DESC_SIZE + 1]
      = " BLOCKING FACTOR =";
   
   const char RECORD_LENGTH_DESC [RECORD_LENGTH_DESC_SIZE + 1]
      = " RECORD LENGTH =";
   
   const char PIXEL_GSD_DESC [PIXEL_GSD_DESC_SIZE + 1]
      = " PIXEL SIZE =";
   
   const char BITS_PER_PIXEL_DESC [BITS_PER_PIXEL_DESC_SIZE + 1]
      = "OUTPUT BITS PER PIXEL =";
   
   const char AQUIRED_BITS_PER_PIXEL_DESC
      [AQUIRED_BITS_PER_PIXEL_DESC_SIZE + 1] = " ACQUIRED BITS PER PIXEL =";
   
   const char BANDS_PRESENT_DESC [BANDS_PRESENT_DESC_SIZE + 1]
      = "BANDS PRESENT =";
   
   const char REV_DESC [REV_DESC_SIZE + 1]
      = "REV            ";
   
   const char BIAS_GAIN_DESC [BIAS_GAIN_DESC_SIZE + 1]
      = "BIASES AND GAINS IN THE BAND ORDER AS ON THIS TAPE";
   
   const char GEO_DESC [GEO_DESC_SIZE + 1]
      = "GEOMETRIC DATA";
   
   const char MAP_PROJECTION_NAME_DESC [MAP_PROJECTION_NAME_DESC_SIZE + 1]
      = " MAP PROJECTION =";
   
   const char ELLIPSOID_DESC [ELLIPSOID_DESC_SIZE + 1]
      = " ELLIPSOID =";
      
   const char DATUM_DESC [DATUM_DESC_SIZE + 1]
      = " DATUM =";
   
   const char PROJECTION_PARAMETER_DESC [PROJECTION_PARAMETER_DESC_SIZE + 1]
      = "USGS PROJECTION PARAMETERS =";
   
   const char UL_DESC [CORNER_DESC_SIZE + 1]
      = "UL =";

   const char UR_DESC [CORNER_DESC_SIZE + 1]
      = "UR =";
   
   const char LR_DESC [CORNER_DESC_SIZE + 1]
      = "LR =";
   
   const char LL_DESC [CORNER_DESC_SIZE + 1]
      = "LL =";
   
   const char CENTER_DESC [CENTER_DESC_SIZE + 1]
      = "CENTER =";
   
   const char HORIZONTAL_OFFSET_DESC [HORIZONTAL_OFFSET_DESC_SIZE + 1]
      = "OFFSET =";
   
   const char ORIENTATION_ANGLE_DESC [ORIENTATION_ANGLE_DESC_SIZE + 1]
      = " ORIENTATION ANGLE =";
   
   const char SUN_ELEVATION_DESC [SUN_ELEVATION_DESC_SIZE + 1]
      = "SUN ELEVATION ANGLE =";
   
   const char SUN_AZIMUTH_DESC [SUN_AZIMUTH_DESC_SIZE + 1]
      = " SUN AZIMUTH ANGLE =";

   const char SPACE[] = " ";

   //>
   // NOTE:  "seekp's" will be perormed as needed for safety.
   //<

   //>
   // Start at beginning of the stream.
   //<
   os.seekp(0, ios::beg);

   //>
   // Start of administrative record.
   //<

   os << setiosflags(ios::fixed)  // Disable scientific mode.

      << PRODUCT_ID_DESC

      << setw(PRODUCT_ORDER_NUMBER_SIZE)
      << theAdminRecord.theProductOrderNumber;

   //>
   // The admin record can contain info for up to four scenes.
   //<

   int i;
   for (i = 0; i < 4; i++)
   {
      os.seekp(LOCATION_OFFSET[i], ios::beg);

      os << LOCATION_DESC         
      
         << setw(PATH_ROW_NUMBER_SIZE) 
         << theAdminRecord.thePathRowNumber[i] 

         << DATE_DESC

         << setw(DATE_SIZE) 
         << theAdminRecord.theAcquisitionDate[i] 

         << setw(1) << SPACE << endl // End of line.

         << SATELLITE_NAME_DESC // Start of line.

         << setw(SAT_NAME_SIZE) 
         << theAdminRecord.theSatName[i] 

         << SENSOR_NAME_DESC
      
         << setw(SENSOR_NAME_SIZE) 
         << theAdminRecord.theSensorName[i]

         << SENSOR_MODE_DESC

         << setw(SENSOR_MODE_SIZE) 
         << theAdminRecord.theSensorMode[i]

         << LOOK_ANGLE_DESC

         << setw(OFF_NADIR_ANGLE_SIZE)
         << setiosflags(ios::right)
         << setprecision(2) 
         << theAdminRecord.theOffNadirAngle[i] // End of scene.
         << resetiosflags(ios::right)
         << endl 
         << setw(23) << SPACE; // End of line.
   } // End of scene loop.

   os.seekp(640, ios::beg); // Start of line.

   os << PRODUCT_TYPE_DESC

      << setiosflags(ios::left)
      
      << setw(PRODUCT_TYPE_SIZE)
      << theAdminRecord.theProductType

      << PRODUCT_SIZE_DESC

      << setw(PRODUCT_SIZE_SIZE)
      << theAdminRecord.theProductSize

      << setw(22) << SPACE << endl; // End of line.

   os.seekp(720, ios::beg);

   os << PROCESSING_TYPE_DESC

      << setw(PROCESSING_TYPE_SIZE)
      << theAdminRecord.theProcessingType

      << RESAMPLING_ALGO_DESC

      << setw(RESAMPLING_ALGO_SIZE)
      << theAdminRecord.theResampAlgorithm

      << resetiosflags(ios::left)

      << setw(33) << SPACE << endl; // End of line.

   os.seekp(800, ios::beg);  // Start of line.
   
   os << TAPE_VOLUME_NUMBER_DESC

      << setiosflags(ios::right)

      << setw(TAPE_VOLUME_NUMBER_SIZE) 
      << theAdminRecord.theTapeVolumeNumber
      << "/" << setw(VOLUMES_PER_TAPE_SIZE)
      << theAdminRecord.theNumberVolumesPerTape

      << PIXELS_PER_LINE_DESC

      << setw(PIXELS_PER_LINE_SIZE)
      << theAdminRecord.thePixelsPerLine 

      << LINES_PER_IMAGE_DESC

      << setw(LINES_PER_IMAGE_SIZE)
      << theAdminRecord.theLinesPerImage

      << "/"

      << setw(LINES_PER_IMAGE_SIZE)
      << theAdminRecord.theLinesPerImage

      << setw(4) << SPACE << endl;  // End of line.
  
   os.seekp(880, ios::beg);  // Start of line.

   os << setiosflags(ios::right)

      << FIRST_LINE_DESC

      << setw(LINES_PER_IMAGE_SIZE)
      << theAdminRecord.the1stLineInVolume

      << BLOCKING_FACTOR_DESC

      << setw(BLOCKING_FACTOR_SIZE)
      << theAdminRecord.theTapeBlockingFactor
 
      << RECORD_LENGTH_DESC

      << setw(RECORD_LENGTH_SIZE)
      << theAdminRecord.theRecordSize

      << PIXEL_GSD_DESC

      
      << setw(PIXEL_GSD_SIZE)
      << setprecision(2)
      << theAdminRecord.theGsd

      << endl;  // End of line.

   os.seekp(960, ios::beg);  // Start of line.

   os << BITS_PER_PIXEL_DESC

      << setw(BITS_PER_PIXEL_SIZE)
      << theAdminRecord.theOutputBitsPerPixel

      << AQUIRED_BITS_PER_PIXEL_DESC

      << setw(BITS_PER_PIXEL_SIZE)
      << theAdminRecord.theAcquiredBitsPerPixel

      << resetiosflags(ios::right)

      << setw(26) << SPACE << endl;  // End of line.

   os.seekp(1040, ios::beg);  // Start of line.

   os << BANDS_PRESENT_DESC

      << setw(BANDS_PRESENT_SIZE)
      << theAdminRecord.theBandsPresentString

      << setw(31) << SPACE << endl // End of line.
      << setw(79) << SPACE << endl
      << setw(79) << SPACE << endl
      << setw(79) << SPACE << endl
      << setw(79) << SPACE << endl
      << setw(79) << SPACE << endl;

   os.seekp(1520, ios::beg);
   
   os << REV_DESC

      << setw(FORMAT_VERSION_SIZE)
      << theAdminRecord.theFormatVersion;

   //***
   // End of administrative record(bytes 0 to 1535).
   //***

   //***
   // Beginning of radiometric record.
   //***

   os.seekp(1536, ios::beg);  

   os << BIAS_GAIN_DESC

      << setw(29) << SPACE << endl; 

   //***
   // Loop through the bands and get write out the bias and gain for each
   // band.
   //***
   
   os << resetiosflags(ios::left) << setiosflags(ios::right);

   for (i = 0; i < 8; i++)
   {
      os.seekp(BIAS_OFFSET[i], ios::beg);

      os << setprecision(15)
         
         << setw(BIAS_SIZE)
         << theRadiomRecord.theBias[i]

         << SPACE 

         << setw(GAIN_SIZE) 
         << theRadiomRecord.theGain[i]

         << setw(30) << SPACE << endl;
   } // End of loop through the bands.

   os << resetiosflags(ios::right);

   os.seekp(2256, ios::beg);

   os << setw(79) << SPACE << endl;
   os << setw(79) << SPACE << endl;
   os << setw(79) << SPACE << endl;
   os << setw(79) << SPACE << endl;
   os << setw(79) << SPACE << endl;
   os << setw(79) << SPACE << endl;
   os << setw(79) << SPACE << endl;
   os << setw(79) << SPACE << endl;
   os << setw(79) << SPACE << endl;
   os << setw(79) << SPACE << endl;
   os << setw(15) << SPACE << endl;

   //***
   // End of radiometric record(bytes 1536 to 3071).
   //***

   //**
   // Start of geometric record(bytes 3072 to 4607).
   //***

   os.seekp(3072, ios::beg);  // Start of record.

   os << GEO_DESC

      << MAP_PROJECTION_NAME_DESC

      << setiosflags(ios::left) // Alpha fields left justified.

      << setw(MAP_PROJECTION_NAME_SIZE)
      << theGeoRecord.theMapProjectionName

      << ELLIPSOID_DESC // Looks like this has changed.

      << setw(ELLIPSOID_SIZE)
      << theGeoRecord.theEllipsoid

      << DATUM_DESC

      << setw(DATUM_SIZE)
      << theGeoRecord.theDatum

      << resetiosflags(ios::left)
      
      << endl // End of line

      << PROJECTION_PARAMETER_DESC
      << SPACE;

   os.seekp(PROJ_PARAM_OFFSET[0], ios::beg);
   os << setprecision(15)
      << setiosflags(ios::right)
      
      << setw(PROJECTION_PARAMETER_SIZE) 
      << theGeoRecord.theProjectionParams[0]
      << SPACE;

   os.seekp(PROJ_PARAM_OFFSET[1], ios::beg);
   os << setw(PROJECTION_PARAMETER_SIZE) 
      << theGeoRecord.theProjectionParams[1]
      << setw(1) << SPACE << endl;

   //***
   // Code is duplicated every three projection parameter four times; 
   // hence, the double loop.
   //***
   for (i = 0; i < 4; i++)
   {
      for (int j = i * 3 + 2; j < (i * 3) + 5; j++)
      {
         os.seekp(PROJ_PARAM_OFFSET[j], ios::beg);

         os << setw(PROJECTION_PARAMETER_SIZE) 
            << theGeoRecord.theProjectionParams[j]
            << setw(1) << SPACE;

         os.seekp(PROJ_PARAM_OFFSET[j], ios::beg);
         os << setw(PROJECTION_PARAMETER_SIZE) 
            << theGeoRecord.theProjectionParams[j]
            << setw(1) << SPACE;

         os.seekp(PROJ_PARAM_OFFSET[j], ios::beg);
         os << setw(PROJECTION_PARAMETER_SIZE) 
            << theGeoRecord.theProjectionParams[j]
            << setw(5) << SPACE << endl;
      }
   }

   os.seekp(PROJ_PARAM_OFFSET[14], ios::beg);
   os << setw(PROJECTION_PARAMETER_SIZE) 
      << theGeoRecord.theProjectionParams[14]
      << setw(55) << SPACE << endl << resetiosflags(ios::right);


   os.seekp(3632, ios::beg);
   os << UL_DESC
 
      << SPACE
      
      << setw(LON_SIZE) << theGeoRecord.theUlLon

      << SPACE

      << setw(LAT_SIZE) << theGeoRecord.theUlLat

      << SPACE

      << setprecision(3)
      
      << setw(EASTING_SIZE) << setiosflags(ios::right) 
      << theGeoRecord.theUlEasting

      << SPACE

      << setw(NORTHING_SIZE) 
      << theGeoRecord.theUlNorthing

      << setw(20) << SPACE << endl << resetiosflags(ios::right);

      
   os.seekp(3712, ios::beg);
   os << UR_DESC
 
      << SPACE
      
      << setw(LON_SIZE) << theGeoRecord.theUrLon

      << SPACE

      << setw(LAT_SIZE) << theGeoRecord.theUrLat

      << SPACE

      << setw(EASTING_SIZE) << setiosflags(ios::right) 
      << theGeoRecord.theUrEasting

      << SPACE

      << setw(NORTHING_SIZE) 
      << theGeoRecord.theUrNorthing

      << setw(20) << SPACE << endl << resetiosflags(ios::right);

      
   os.seekp(3792, ios::beg);
   os << LR_DESC
 
      << SPACE
      
      << setw(LON_SIZE) << theGeoRecord.theLrLon

      << SPACE

      << setw(LAT_SIZE) << theGeoRecord.theLrLat

      << SPACE

      << setw(EASTING_SIZE) << setiosflags(ios::right) 
      << theGeoRecord.theLrEasting

      << SPACE

      << setw(NORTHING_SIZE) 
      << theGeoRecord.theLrNorthing

      << setw(20) << SPACE << endl << resetiosflags(ios::right);

      
   os.seekp(3872, ios::beg);
   os << LL_DESC
 
      << SPACE
      
      << setw(LON_SIZE) << theGeoRecord.theLlLon

      << SPACE

      << setw(LAT_SIZE) << theGeoRecord.theLlLat

      << SPACE

      << setw(EASTING_SIZE) << setiosflags(ios::right) 
      << theGeoRecord.theLlEasting

      << SPACE

      << setw(NORTHING_SIZE) 
      << theGeoRecord.theLlNorthing

      << setw(20) << SPACE << endl << resetiosflags(ios::right);

   os.seekp(3952, ios::beg);
   os << CENTER_DESC

      << SPACE

      << setw(LON_SIZE) 
      << theGeoRecord.theCenterLon
      
      << SPACE

      << setw(LAT_SIZE) 
      << theGeoRecord.theCenterLat

      << SPACE

      << setw(EASTING_SIZE) << setiosflags(ios::right) 
      << theGeoRecord.theCenterEasting

      << SPACE

      << setw(NORTHING_SIZE) 
      << theGeoRecord.theCenterNorthing

      << SPACE 

      << setw(CENTER_SAMPLE_SIZE) 
      << theGeoRecord.theCenterSample

      << SPACE 

      << setw(CENTER_LINE_SIZE) 
      << theGeoRecord.theCenterLine

      << setw(4) << SPACE << endl;

   os.seekp(4032, ios::beg);
   os << HORIZONTAL_OFFSET_DESC

      << setw(HORIZONTAL_OFFSET_SIZE) 
      << theGeoRecord.theHorizontalOffset

      << ORIENTATION_ANGLE_DESC

      << setw(ORIENTATION_ANGLE_SIZE) 
      << setprecision(2)
      << theGeoRecord.theOrientationAngle

      << setw(39) << SPACE << endl; // 41 in spec

   os.seekp(4112, ios::beg);
   os << SUN_ELEVATION_DESC

      << setw(SUN_ELEVATION_SIZE) 
      << setprecision(1) 
      << theGeoRecord.theSunElevationAngle

      << SUN_AZIMUTH_DESC

      << setw(SUN_AZIMUTH_SIZE) 
      << setprecision(1) 
      << theGeoRecord.theSunAzimuth  // End of data.

      << setw(29) << SPACE << endl 
      << setw(79) << SPACE << endl 
      << setw(79) << SPACE << endl 
      << setw(79) << SPACE << endl 
      << setw(79) << SPACE << endl
      << setw(79) << SPACE << endl
      << setw(15) << SPACE << endl << flush; // ? size

   //***
   // End of geometric record(bytes 3072 to 4607).
   //***
   
}


//***************************************************************************
// PRIVATE METHOD:
// ossimFfRevc::loadFromStream(istream& is)
//***************************************************************************
void ossimFfRevc::loadFromStream(istream& is)
{
   //***
   // See .h for enumerations for field sizes and offsets. 
   //***

   //***
   // NOTE:  Because of header inconsistencies all seeks will be relative to
   //        the beginning of the stream.
   //***

   //***
   // Temporary buffer for fields that need to be converted to integers or
   // floats.
   //***
   
   char tmpBuff[25];

   //***
   // Start of administrative record.
   //***

   is.seekg(PRODUCT_ORDER_NUMBER_OFFSET, ios::beg);

   is.get(theAdminRecord.theProductOrderNumber, 
          PRODUCT_ORDER_NUMBER_SIZE + 1,
          ' ');

   if (checkStream(is)) return;

   //***
   // Loop through the four scenes and get the data.
   //***

   int i;
   for (i = 0; i < NUMBER_OF_SCENES; i++)
   {
 
      is.seekg(PATH_ROW_NUMBER_OFFSET[i], ios::beg);

      is.get(theAdminRecord.thePathRowNumber[i], PATH_ROW_NUMBER_SIZE + 1);
   
      if (checkStream(is)) return;

      is.seekg(DATE_OFFSET[i], ios::beg);

      is.get(theAdminRecord.theAcquisitionDate[i], DATE_SIZE + 1);
   
      if (checkStream(is)) return;

      is.seekg(SAT_NAME_OFFSET[i], ios::beg);

      is.get(theAdminRecord.theSatName[i], SAT_NAME_SIZE + 1);
   
      if (checkStream(is)) return;

      is.seekg(SENSOR_NAME_OFFSET[i], ios::beg);

      is.get(theAdminRecord.theSensorName[i], SENSOR_NAME_SIZE + 1);
   
      if (checkStream(is)) return;

      is.seekg(SENSOR_MODE_OFFSET[i], ios::beg);

      is.get(theAdminRecord.theSensorMode[i], SENSOR_MODE_SIZE + 1);
  
      if (checkStream(is)) return;

      is.seekg(OFF_NADIR_ANGLE_OFFSET[i], ios::beg);

      is.get(tmpBuff, OFF_NADIR_ANGLE_SIZE + 1);

      if (checkStream(is)) return;

      theAdminRecord.theOffNadirAngle[i] = atof(tmpBuff);

   } // End of scene loop.
   
   is.seekg(PRODUCT_TYPE_OFFSET, ios::beg);

   is.get(theAdminRecord.theProductType, PRODUCT_TYPE_SIZE + 1);

   if (checkStream(is)) return;
 
   is.seekg(PRODUCT_SIZE_OFFSET, ios::beg);

   is.get(theAdminRecord.theProductSize, PRODUCT_SIZE_SIZE + 1);

   if (checkStream(is)) return;
   
   is.seekg(PROCESSING_TYPE_OFFSET, ios::beg);

   is.get(theAdminRecord.theProcessingType, PROCESSING_TYPE_SIZE + 1);

   if (checkStream(is)) return;
   
   is.seekg(RESAMPLING_ALGO_OFFSET, ios::beg);

   is.get(theAdminRecord.theResampAlgorithm, RESAMPLING_ALGO_SIZE + 1);

   if (checkStream(is)) return;
   
   is.seekg(TAPE_VOLUME_NUMBER_OFFSET, ios::beg);

   is.get(tmpBuff, TAPE_VOLUME_NUMBER_SIZE + 1, '/');

   if (checkStream(is)) return;

   theAdminRecord.theTapeVolumeNumber = atoi(tmpBuff);

   is.seekg(VOLUMES_PER_TAPE_OFFSET, ios::beg);

   is.get(tmpBuff, VOLUMES_PER_TAPE_SIZE + 1);

   if (checkStream(is)) return;

   theAdminRecord.theNumberVolumesPerTape = atoi(tmpBuff);
    
   is.seekg(PIXELS_PER_LINE_OFFSET, ios::beg);

   is.get(tmpBuff, PIXELS_PER_LINE_SIZE + 1);

   if (checkStream(is)) return;

   theAdminRecord.thePixelsPerLine = atoi(tmpBuff);
   
   is.seekg(LINES_PER_IMAGE_OFFSET, ios::beg);

   is.get(tmpBuff, LINES_PER_IMAGE_SIZE + 1);

   if (checkStream(is)) return;

   theAdminRecord.theLinesPerImage = atoi(tmpBuff);

   is.seekg(FIRST_LINE_IN_VOLUME_OFFSET, ios::beg);

   is.get(tmpBuff, FIRST_LINE_IN_VOLUME_SIZE + 1);

   if (checkStream(is)) return;

   theAdminRecord.the1stLineInVolume = atoi(tmpBuff);

   is.seekg(BLOCKING_FACTOR_OFFSET, ios::beg);

   is.get(tmpBuff, BLOCKING_FACTOR_SIZE + 1);

   if (checkStream(is)) return;

   theAdminRecord.theTapeBlockingFactor = atoi(tmpBuff);
   
   is.seekg(RECORD_LENGTH_OFFSET, ios::beg);

   is.get(tmpBuff, RECORD_LENGTH_SIZE + 1);

   if (checkStream(is)) return;

   theAdminRecord.theRecordSize = atoi(tmpBuff);
   
   is.seekg(PIXEL_GSD_OFFSET, ios::beg);

   is.get(tmpBuff, PIXEL_GSD_SIZE + 1);

   if (checkStream(is)) return;

   theAdminRecord.theGsd = atof(tmpBuff);
   
   is.seekg(BITS_PER_PIXEL_OFFSET, ios::beg);

   is.get(tmpBuff, BITS_PER_PIXEL_SIZE + 1);

   if (checkStream(is)) return;

   theAdminRecord.theOutputBitsPerPixel = atoi(tmpBuff);
   
   is.seekg(ACQUIRED_BITS_PER_PIXEL_OFFSET, ios::beg);

   is.get(tmpBuff, BITS_PER_PIXEL_SIZE + 1);

   if (checkStream(is)) return;

   theAdminRecord.theAcquiredBitsPerPixel = atoi(tmpBuff);
   
   is.seekg(BANDS_PRESENT_OFFSET, ios::beg);

   is.get(theAdminRecord.theBandsPresentString, BANDS_PRESENT_SIZE + 1);

   if (checkStream(is)) return;
   
   is.seekg(FORMAT_VERSION_OFFSET, ios::beg);

   is.get(theAdminRecord.theFormatVersion, FORMAT_VERSION_SIZE + 1);

   if (checkStream(is)) return;
   //***
   // End of administrative record.
   //***

   //***
   // Start of radiometric record.
   //***

   for (i = 0; i < NUMBER_OF_BANDS; i++)
   {
      is.seekg(BIAS_OFFSET[i], ios::beg);

      is.get(tmpBuff, BIAS_SIZE + 1);

      if (checkStream(is)) return;
   
      theRadiomRecord.theBias[i] = atof(tmpBuff);

      is.seekg(GAIN_OFFSET[i], ios::beg);

      is.get(tmpBuff, GAIN_SIZE + 1);

      if (checkStream(is)) return;
   
      theRadiomRecord.theGain[i] = atof(tmpBuff);
   }
   //***
   // End of radiometric record.
   //***

   //***
   // Start of geometric record.
   //***

   is.seekg(MAP_PROJECTION_NAME_OFFSET, ios::beg);

   is.get(theGeoRecord.theMapProjectionName, MAP_PROJECTION_NAME_SIZE + 1);

   if (checkStream(is)) return;

   is.seekg(ELLIPSOID_OFFSET, ios::beg);

   is.get(theGeoRecord.theEllipsoid, ELLIPSOID_SIZE + 1);

   if (checkStream(is)) return;

   is.seekg(DATUM_OFFSET, ios::beg);

   is.get(theGeoRecord.theDatum, DATUM_SIZE + 1, ' ');
   
   if (checkStream(is)) return;

   //***
   // Get the fifteen projection parameters.
   //***
   
   for(i = 0; i < 15; i++)
   {
      is.seekg(PROJ_PARAM_OFFSET[i], ios::beg);
   
      is.get(tmpBuff, PROJECTION_PARAMETER_SIZE + 1);

      if (checkStream(is)) return;

      theGeoRecord.theProjectionParams[i] = atof(tmpBuff);
   }

   //***
   // Start of upper left data:  longitude, latitude, easting, and northing. 
   //***
   is.seekg(UL_LON_OFFSET, ios::beg);

   is.get(theGeoRecord.theUlLon, LON_SIZE + 1);
   
   if (checkStream(is)) return;
   
   is.seekg(UL_LAT_OFFSET, ios::beg);

   is.get(theGeoRecord.theUlLat, LAT_SIZE + 1);
   
   if (checkStream(is)) return;
   
   is.seekg(UL_EASTING_OFFSET, ios::beg);

   is.get(tmpBuff, EASTING_SIZE + 1);

   if (checkStream(is)) return;

   theGeoRecord.theUlEasting = atof(tmpBuff);

   is.seekg(UL_NORTHING_OFFSET, ios::beg);

   is.get(tmpBuff, NORTHING_SIZE + 1);

   if (checkStream(is)) return;

   theGeoRecord.theUlNorthing = atof(tmpBuff);

   //***
   // End of upper left data.
   //***

   //***
   // Start of upper right data:  longitude, latitude, easting, and northing. 
   //***
   is.seekg(UR_LON_OFFSET, ios::beg);

   is.get(theGeoRecord.theUrLon, LON_SIZE + 1);
   
   if (checkStream(is)) return;
   
   is.seekg(UR_LAT_OFFSET, ios::beg);

   is.get(theGeoRecord.theUrLat, LAT_SIZE + 1);
   
   if (checkStream(is)) return;
   
   is.seekg(UR_EASTING_OFFSET, ios::beg);

   is.get(tmpBuff, EASTING_SIZE + 1);

   if (checkStream(is)) return;

   theGeoRecord.theUrEasting = atof(tmpBuff);

   is.seekg(UR_NORTHING_OFFSET, ios::beg);

   is.get(tmpBuff, NORTHING_SIZE + 1);

   if (checkStream(is)) return;

   theGeoRecord.theUrNorthing = atof(tmpBuff);

   //***
   // End of upper right data.
   //***

   //***
   // Start of lower right data:  longitude, latitude, easting, and northing. 
   //***
   is.seekg(LR_LON_OFFSET, ios::beg);

   is.get(theGeoRecord.theLrLon, LON_SIZE + 1);
   
   if (checkStream(is)) return;
   
   is.seekg(LR_LAT_OFFSET, ios::beg);

   is.get(theGeoRecord.theLrLat, LAT_SIZE + 1);
   
   if (checkStream(is)) return;
   
   is.seekg(LR_EASTING_OFFSET, ios::beg);

   is.get(tmpBuff, EASTING_SIZE + 1);

   if (checkStream(is)) return;

   theGeoRecord.theLrEasting = atof(tmpBuff);

   is.seekg(LR_NORTHING_OFFSET, ios::beg);

   is.get(tmpBuff, NORTHING_SIZE + 1);

   if (checkStream(is)) return;

   theGeoRecord.theLrNorthing = atof(tmpBuff);

   //***
   // End of lower right data.
   //***

   //***
   // Start of lower left data:  longitude, latitude, easting, and northing. 
   //***
   is.seekg(LL_LON_OFFSET, ios::beg);

   is.get(theGeoRecord.theLlLon, LON_SIZE + 1);
   
   if (checkStream(is)) return;
   
   is.seekg(LL_LAT_OFFSET, ios::beg);

   is.get(theGeoRecord.theLlLat, LAT_SIZE + 1);
   
   if (checkStream(is)) return;
   
   is.seekg(LL_EASTING_OFFSET, ios::beg);

   is.get(tmpBuff, EASTING_SIZE + 1);

   if (checkStream(is)) return;

   theGeoRecord.theLlEasting = atof(tmpBuff);

   is.seekg(LL_NORTHING_OFFSET, ios::beg);

   is.get(tmpBuff, NORTHING_SIZE + 1);

   if (checkStream(is)) return;

   theGeoRecord.theLlNorthing = atof(tmpBuff);

   //***
   // End of lower left data.
   //***

   //***
   // Start of scene center data:  longitude, latitude, easting, northing,
   // sample, line. 
   //***
   is.seekg(CENTER_LON_OFFSET, ios::beg);

   is.get(theGeoRecord.theCenterLon, LON_SIZE + 1);
   
   if (checkStream(is)) return;
   
   is.seekg(CENTER_LAT_OFFSET, ios::beg);

   is.get(theGeoRecord.theCenterLat, LAT_SIZE + 1);
   
   if (checkStream(is)) return;
   
   is.seekg(CENTER_EASTING_OFFSET, ios::beg);

   is.get(tmpBuff, EASTING_SIZE + 1);

   if (checkStream(is)) return;

   theGeoRecord.theCenterEasting = atof(tmpBuff);

   is.seekg(CENTER_NORTHING_OFFSET, ios::beg);

   is.get(tmpBuff, NORTHING_SIZE + 1);

   if (checkStream(is)) return;

   theGeoRecord.theCenterNorthing = atof(tmpBuff);

   is.seekg(CENTER_SAMPLE_OFFSET, ios::beg);

   is.get(tmpBuff, CENTER_SAMPLE_SIZE + 1);

   if (checkStream(is)) return;

   theGeoRecord.theCenterSample = atoi(tmpBuff);

   is.seekg(CENTER_LINE_OFFSET, ios::beg);

   is.get(tmpBuff, CENTER_LINE_SIZE + 1);

   if (checkStream(is)) return;

   theGeoRecord.theCenterLine = atoi(tmpBuff);

   //***
   // End of scene center data.
   //***
   
   is.seekg(HORIZONTAL_OFFSET_OFFSET, ios::beg);

   is.get(tmpBuff, HORIZONTAL_OFFSET_SIZE + 1);

   if (checkStream(is)) return;

   theGeoRecord.theHorizontalOffset = atoi(tmpBuff);
   
   is.seekg(ORIENTATION_ANGLE_OFFSET, ios::beg);

   is.get(tmpBuff, ORIENTATION_ANGLE_SIZE + 1);

   if (checkStream(is)) return;

   theGeoRecord.theOrientationAngle = atof(tmpBuff);
   
   is.seekg(SUN_ELEVATION_OFFSET, ios::beg);

   is.get(tmpBuff, SUN_ELEVATION_SIZE + 1);

   if (checkStream(is)) return;

   theGeoRecord.theSunElevationAngle = atof(tmpBuff);
   
   is.seekg(SUN_AZIMUTH_OFFSET, ios::beg);

   is.get(tmpBuff, SUN_AZIMUTH_SIZE + 1);

   if (checkStream(is)) return;

   theGeoRecord.theSunAzimuth = atof(tmpBuff);

   //***
   // End of geometric record.
   //***
   
}

//***************************************************************************
// PRIVATE METHOD:
// ossimFfRevc::checkStream(istream& is)
//
// Checks the stream.  If an error has occurred it sets theErrorStatus.
// Returns: theErrorStatus    ( 0 = OK,  1 = ERROR )
//***************************************************************************
int ossimFfRevc::checkStream(istream& is)
{
   //***
   // istreams set fault bits and the operator! is overloaded to return
   // true if an error has occurred.
   //***
   
   if (!is)
   {
      theErrorStatus = OSSIM_ERROR;

      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimFfRevc::checkStream: "
                                          << "Stream corrupted.   Returning from method."
                                          << std::endl;
   } 

   return theErrorStatus;
}
   
//***************************************************************************
// Function:
// ostream& operator<<(ostream& os, const ossimFfRevc& head)
//***************************************************************************
ostream& operator<<(ostream& os, const ossimFfRevc& head)
{
   head.print(os);

   return os;
}

//***************************************************************************
// CONSTRUCTOR:
// ossimFfRevc::AdminRecord::AdminRecord()
//
// Default constructor for AdminRecord nested class:
//***************************************************************************
ossimFfRevc::AdminRecord::AdminRecord()
   :
      theTapeVolumeNumber(1),
      theNumberVolumesPerTape(1),
      thePixelsPerLine(0),
      theLinesPerImage(0),
      the1stLineInVolume(1),
      theTapeBlockingFactor(1),
      theRecordSize(0),
      theGsd(0.0),
      theOutputBitsPerPixel(8),
      theAcquiredBitsPerPixel(6)
{
   const char tmpBuff[] = "";

   strcpy(theProductOrderNumber, tmpBuff);

   //>
   // There can be four scenes.
   //<
   
   for (int i = 0; i < 4; i++)
   {
      strcpy(thePathRowNumber[i], tmpBuff);
      strcpy(theAcquisitionDate[i], tmpBuff);
      strcpy(theSatName[i], tmpBuff);
      strcpy(theSensorName[i], tmpBuff);
      strcpy(theSensorMode[i], tmpBuff);
      theOffNadirAngle[i] = 0.0;
   }
   strcpy(theProductType, "MAP ORIENTED");
   strcpy(theProductSize, tmpBuff);
   strcpy(theProcessingType, "SYSTEMATIC");
   strcpy(theResampAlgorithm, "CC");
   strcpy(theBandsPresentString, "P");
   strcpy(theFormatVersion, "C");
}

//***************************************************************************
// CONSTRUCTOR:
// ossimFfRevc::RadiomRecord::RadiomRecord()
//
// Default constructor for RadiomRecord nested class:
//***************************************************************************
ossimFfRevc::RadiomRecord::RadiomRecord()
{
   for (int i = 0; i < 8; i++)
   {
      theBias[i] = 0.0;
      theGain[i] = 0.0;
   }
}
         
//***************************************************************************
// CONSTRUCTOR:
// ossimFfRevc::GeoRecord::GeomRecord()
//
// Default constructor for GeoRecord nested class:
//***************************************************************************
ossimFfRevc::GeoRecord::GeoRecord()
   :
      theUlEasting(0.0),
      theUlNorthing(0.0),
      theUrEasting(0.0),
      theUrNorthing(0.0),
      theLrEasting(0.0),
      theLrNorthing(0.0),
      theLlEasting(0.0),
      theLlNorthing(0.0),
      theCenterEasting(0.0),
      theCenterNorthing(0.0),
      theCenterSample(0),
      theCenterLine(0),
      theHorizontalOffset(0),
      theOrientationAngle(0.0),
      theSunElevationAngle(0.0),
      theSunAzimuth(0.0)
{
   for (int i = 0; i < 15; i++)
   {
      theProjectionParams[i] = 0.0;
   }

   const char tmpBuff[] = "";

   strcpy(theMapProjectionName, "UTM");
   strcpy(theEllipsoid, tmpBuff);
   strcpy(theDatum, tmpBuff);
   strcpy(theUlLon, tmpBuff);
   strcpy(theUlLat, tmpBuff);
   strcpy(theUrLon, tmpBuff);
   strcpy(theUrLat, tmpBuff);
   strcpy(theLrLon, tmpBuff);
   strcpy(theLrLat, tmpBuff);
   strcpy(theLlLon, tmpBuff);
   strcpy(theLlLat, tmpBuff);
   strcpy(theCenterLon, tmpBuff);
   strcpy(theCenterLat, tmpBuff);
}
