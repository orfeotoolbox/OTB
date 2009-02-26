//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Ken Melero (kmelero@imagelinks.com)
//         Orginally written by Dave Burken (dburken@imagelinks.com)
// Description: This class parses an EOSAT Fast Format rev b header.
//
//********************************************************************
// $Id: ossimFfRevb.cpp 13217 2008-07-23 19:19:13Z dburken $

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <ossim/support_data/ossimFfRevb.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimNotifyContext.h>

// File scope constants for seek positions.
const int RADIANCE_OFFSET [7] = { 300,
                                  317,
                                  334,
                                  351,
                                  368,
                                  385,
                                  402  };

static const int PROJ_PARAM_OFFSET [15]  = { 594,
                                      618,
                                      642,
                                      666,
                                      690,
                                      714,
                                      738,
                                      762,
                                      786,
                                      810,
                                      834,
                                      858,
                                      882,
                                      906,
                                      930 };


//***************************************************************************
// CONSTRUCTOR
//***************************************************************************
ossimFfRevb::ossimFfRevb()
   :
      the1stLineInVolume(1),
      theLinesPerVolume(0),
      theOrientationAngle(0.0),
      theUsgsProjNumber(0),
      theUsgsMapZone(0),
      theSemiMajorAxis(0.0),
      theSemiMinorAxis(0.0),
      theGsd(0.0),
      thePixelsPerLine(0),
      theLinesPerImage(0),
      theUlEasting(0.0),
      theUlNorthing(0.0),
      theUrEasting(0.0),
      theUrNorthing(0.0),
      theLrEasting(0.0),
      theLrNorthing(0.0),
      theLlEasting(0.0),
      theLlNorthing(0.0),
      theBlockingFactor(0),
      theRecordSize(0),
      theSunElevation(0),
      theSunAzimuth(0),
      theCenterEasting(0.0),
      theCenterNorthing(0.0),
      theCenterSample(0),
      theCenterLine(0),
      theOffset(0),
      theErrorStatus(OSSIM_OK)
{
   const char tmpBuff[] = "";

   strcpy(theProductOrderNumber, tmpBuff);
   strcpy(thePathRowNumber, tmpBuff);
   strcpy(theAcquisitionDate, tmpBuff);
   strcpy(theSatNumber, tmpBuff);
   strcpy(theInstrumentType, tmpBuff);
   strcpy(theProductType, "MAP ORIENTED");
   strcpy(theProductSize, tmpBuff);
   strcpy(theMapSheetName, tmpBuff);
   strcpy(theProcessingType, "TERRAIN");
   strcpy(theResampAlgorithm, "NN");

   int i;
   for (i=0; i<NUMBER_OF_BANDS; i++)
   {
      strcpy(theBandRadiance[i], tmpBuff);
   }

   strcpy(theVolumeNumber, tmpBuff);
   strcpy(theMapProjName, tmpBuff);

   for (i=0; i<NUMBER_OF_PROJECTION_PARAMETERS; i++)
   {
      strcpy(theUsgsProjParam[i], tmpBuff);
   }

   strcpy(theEllipsoid, tmpBuff);
   strcpy(theUlLon, tmpBuff);
   strcpy(theUlLat, tmpBuff);
   strcpy(theUrLon, tmpBuff);
   strcpy(theUrLat, tmpBuff);
   strcpy(theLrLon, tmpBuff);
   strcpy(theLrLat, tmpBuff);
   strcpy(theLlLon, tmpBuff);
   strcpy(theLlLat, tmpBuff);
   strcpy(theBandsPresentString, "P");
   strcpy(theCenterLon, tmpBuff);
   strcpy(theCenterLat, tmpBuff);
   strcpy(theFormatVersion, "B");
   
}

//***************************************************************************
// CONSTRUCTOR:
// ossimFfRevb::ossimFfRevb(const char* headerFile)
//
// Takes a filename representing an IRS-1C Fast Format rev C header.
//***************************************************************************
ossimFfRevb::ossimFfRevb(const char* headerFile)
   :
   theErrorStatus(OSSIM_OK)
{
   ifstream is;

   is.open(headerFile);

   if (!is)
   {
      theErrorStatus = OSSIM_ERROR;

      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimFfRevb::ossimFfRevb(header):"
                                          << "Cannot open header = " << headerFile << "\n"
                                          << "Returning from constructor." << std::endl;

      return;
   }

   loadFromStream(is);

   is.close();

}


//***************************************************************************
// PUBLIC METHOD:
// int ossimFfRevb::path(int sceneNbr) const
//
// Convenience method to parse the path from the path/row string.  Returns
// zero if stream is not opened.  Current loacation field is:
// path/row/fraction/subscene in ppp/rrrff format.
//***************************************************************************
int ossimFfRevb::path() const
{
   char tmpBuff[4];
   int path = 0;

   istringstream is(thePathRowNumber);

   if (is)
   {
      is.get(tmpBuff, 4);
      tmpBuff[3] = '\0';
      path = atoi(tmpBuff);
   }

   return path;
}


//***************************************************************************
// PUBLIC METHOD:
// int ossimFfRevb::row(int sceneNbr) const
//
// Convenience method to parse the row from the path/row string.  Returns
// zero if stream is not opened.  Current loacation field is:
// path/row/fraction/subscene in ppp/rrrff format.
//***************************************************************************
int ossimFfRevb::row() const
{
   char tmpBuff[4];
   int row = 0;

   istringstream is(thePathRowNumber);

   if (is)
   {
      is.seekg(4, ios::beg);
      is.get(tmpBuff, 4);
      tmpBuff[3] = '\0';
      row = atoi(tmpBuff);
   }

   return row;
}


//***************************************************************************
// PUBLIC METHOD:
// int ossimFfRevb::fraction(int sceneNbr) const
//
// Convenience method to parse the fraction from the path/row string.  Returns
// zero if stream is not opened.  Current loacation field is:
// path/row/fraction/subscene in ppp/rrrff format.
//***************************************************************************
int ossimFfRevb::fraction() const
{
   char tmpBuff[3];
   int fraction = 0;

   istringstream is(thePathRowNumber);

   if (is)
   {
      is.seekg(7, ios::beg);
      is.get(tmpBuff, 3);
      tmpBuff[2] = '\0';
      fraction = atoi(tmpBuff);
   }

   return fraction;
}

//***************************************************************************
// PUBLIC METHOD:
// double ossimFfRevb::projParam(int paramNumber) const
//
// Returns the projection parameter as a double from the string taken from
// the header.  There are fifteen parameters so the possible range for
// paramNumber is 0 to 14.
//***************************************************************************
double ossimFfRevb::projParam(int paramNumber) const
{
   if (paramNumber >= NUMBER_OF_PROJECTION_PARAMETERS)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimFfRevb::projParam: "
                                          << "Parameter out of range:  " << paramNumber
                                          << "\nValid range:  0 - 14" << std::endl;
      return 0.0;
   }

   //***
   // The USGS Projection Parameters in a rev b header have a "D" to
   // denote the exponent.  This simply substitutes the "D" with an
   // "E" so that atof works properly.
   //***
   ossimString tmp = theUsgsProjParam[paramNumber];
   tmp.gsub("D", "E");

   return atof(tmp.chars());
}

//***************************************************************************
// PUBLIC METHOD:
// void ossimFfRevb::print(ostream& os) const
//
// Prints data members.
//***************************************************************************
void ossimFfRevb::print(ostream& os) const
{
   os << setiosflags(ios::left | ios::fixed)
      << setw(30) << "\nimage_id:" 
      << theProductOrderNumber
      << setw(30) << "\npath_row_number:"
      << thePathRowNumber
      << setw(30) << "\nacquisition_date:"
      << theAcquisitionDate
      << setw(30) << "\nsatellite_name:"
      << theSatNumber
      << setw(30) << "\nsensor_name:"
      << theInstrumentType
      << setw(30) << "\nproduct_type:" 
      << theProductType
      << setw(30) << "\nproduct_size:" 
      << theProductSize
      << setw(30) << "\nprocessing_type:" 
      << theProcessingType 
      << setw(30) << "\nresamp_algorithm:" 
      << theResampAlgorithm;

   int i;
   for (i=0; i<NUMBER_OF_BANDS; i++)
   {
      os << "\nband" << i+1 << setw(25) << "_radiance:"
         << theBandRadiance[i];
   }

   os << setw(30) << "\ntape_volume_number:" 
      << theVolumeNumber
      << setw(30) << "\nfirst_line_in_volume:" 
      << the1stLineInVolume
      << setw(30) << "\nlines_per_volume:" 
      << theLinesPerVolume
      << setw(30) << "\norientation_angle:" 
      << setprecision(2) << theOrientationAngle
      << setw(30) << "\nprojection_type:" 
      << theMapProjName
      << setw(30) << "\nusgs_projection_number:" 
      << theUsgsProjNumber
      << setw(30) << "\nmap_zone:" 
      << theUsgsMapZone;

//    os << setprecision(15);
   os << setiosflags(ios::right);
   for (i=0; i<NUMBER_OF_PROJECTION_PARAMETERS; i++)
   {
      os << "\nprojection_parameter_" << i+1 << setw(10) << ":"
         << setw(24) << theUsgsProjParam[i];
   }
   os << resetiosflags(ios::right);
   

   os << setw(30) << "\nellipsoid:" 
      << theEllipsoid
      << setw(30) << "\nsemi_major_axis:"
      << setprecision(3) << setw(11) << theSemiMajorAxis
      << setw(30) << "\nsemi_minor_axis:"
      << setw(11) << theSemiMinorAxis
      << setw(30) << "\ngsd:" 
      << setprecision(2) << setw(5) << theGsd      
      << setw(30) << "\nnumber_samples:"
      << thePixelsPerLine
      << setw(30) << "\nnumber_lines:" 
      << theLinesPerImage
      << setprecision(3)
      << setw(30) << "\nul_longitude:" 
      << theUlLon
      << setw(30) << "\nul_latitude:" 
      << theUlLat
      << setw(30) << "\nul_easting:" 
      << setw(13) << theUlEasting
      << setw(30) << "\nul_northing:" 
      << setw(13) << theUlNorthing   
      << setw(30) << "\nur_longitude:" 
      << theUrLon
      << setw(30) << "\nur_latitude:" 
      << theUrLat
      << setw(30) << "\nur_easting:" 
      << setw(13) << theUrEasting
      << setw(30) << "\nur_northing:" 
      << setw(13) << theUrNorthing
      << setw(30) << "\nlr_longitude:" 
      << theLrLon
      << setw(30) << "\nlr_latitude:" 
      << theLrLat
      << setw(30) << "\nlr_easting:" 
      << setw(13) << theLrEasting
      << setw(30) << "\nlr_northing:" 
      << setw(13) << theLrNorthing
      << setw(30) << "\nll_longitude:" 
      << theLlLon
      << setw(30) << "\nll_latitude:" 
      << theLlLat
      << setw(30) << "\nll_easting:" 
      << setw(13) << theLlEasting
      << setw(30) << "\nll_northing:" 
      << setw(13) << theLlNorthing      
      << setw(30) << "\nbands_present_string:" 
      << theBandsPresentString
      << setw(30) << "\ntape_blocking_factor:" 
      << theBlockingFactor
      << setw(30) << "\nrecord_size:" 
      << theRecordSize
      << setw(30) << "\nillum_elevation:" 
      << theSunElevation
      << setw(30) << "\nillum_azimuth:"
      << theSunAzimuth
      << setw(30) << "\ncenter_longitude:" 
      << theCenterLon
      << setw(30) << "\ncenter_latitude:" 
      << theCenterLat
      << setw(30) << "\ncenter_easting:" 
      << setw(13) << theCenterEasting
      << setw(30) << "\ncenter_northing:" 
      << setw(13) << theCenterNorthing
      << setw(30) << "\ncenter_sample:" 
      << theCenterSample
      << setw(30) << "\ncenter_line:" 
      << theCenterLine
      << setw(30) << "\nhorizontal_offset:" 
      << theOffset
      << setw(30) << "\nformat_version:" 
      << theFormatVersion
      << std::endl;

}

//***************************************************************************
// PUBLIC METHOD:
// void ossimFfRevb::write(ostream& os) const
//
// Writes an EOSAT Fast Format Rev B formatted header.
//***************************************************************************
void ossimFfRevb::write(ostream& os) const
{
   const char PRODUCT_ID_DESC [PRODUCT_ORDER_NUMBER_DESC_SIZE + 1]
      = "PRODUCT =";

   const char WRS_DESC [WRS_DESC_SIZE + 1]
      = " WRS =";

   const char DATE_DESC [DATE_DESC_SIZE + 1]
      = " ACQUISITION DATE =";
   
   const char SATELLITE_NUMBER_DESC [SAT_NUMBER_DESC_SIZE + 1]
      = " SATELLITE =";
   
   const char INSTRUMENT_TYPE_DESC [INSTRUMENT_TYPE_DESC_SIZE + 1]
      = " INSTRUMENT =";
   
   const char PRODUCT_TYPE_DESC [PRODUCT_TYPE_DESC_SIZE + 1]
      = " PRODUCT TYPE =";
   
   const char PRODUCT_SIZE_DESC [PRODUCT_SIZE_DESC_SIZE + 1]
      = " PRODUCT SIZE =";
   
   const char PROCESSING_TYPE_DESC [PROCESSING_TYPE_DESC_SIZE + 1]
      = " TYPE OF GEODETIC PROCESSING =";
   
   const char RESAMPLING_ALGO_DESC [RESAMPLING_ALGO_DESC_SIZE + 1]
      = " RESAMPLING =";
   
   const char RADIANCE_DESC [RADIANCE_DESC_SIZE + 1]
      = " RAD GAINS/BIASES = ";

   const char VOLUME_NUMBER_DESC [VOLUME_NUMBER_DESC_SIZE + 1]
      = " TAPE SPANNING FLAG=";
   
   const char  FIRST_LINE_DESC [FIRST_LINE_DESC_SIZE + 1]
      = " START LINE #=";

   const char LINES_PER_VOLUME_DESC [LINES_PER_VOLUME_DESC_SIZE + 1]
      = " LINES PER VOL=";

   const char ORIENTATION_ANGLE_DESC [ORIENTATION_ANGLE_DESC_SIZE + 1]
      = " ORIENTATION =";

   const char MAP_PROJ_NAME_DESC [MAP_PROJ_NAME_DESC_SIZE + 1]
      = " PROJECTION =";
 
   const char USGS_PROJ_NUMBER_DESC [USGS_PROJ_NUMBER_DESC_SIZE + 1]
      = " USGS PROJECTION # =";
 
   const char USGS_MAP_ZONE_DESC [USGS_MAP_ZONE_DESC_SIZE + 1]
      = " USGS MAP ZONE =";
 
   const char PROJECTION_PARAMETER_DESC [USGS_PROJ_PARAMS_DESC_SIZE + 1]
      = " USGS PROJECTION PARAMETERS =";

   const char ELLIPSOID_DESC [ELLIPSOID_DESC_SIZE + 1]
      = " EARTH ELLIPSOID =";
      
   const char MAJOR_AXIS_DESC [MAJOR_AXIS_DESC_SIZE+ 1]
      = " SEMI-MAJOR AXIS =";
      
   const char MINOR_AXIS_DESC [MINOR_AXIS_DESC_SIZE+ 1]
      = " SEMI-MINOR AXIS =";
      
   const char PIXEL_GSD_DESC [PIXEL_GSD_DESC_SIZE + 1]
      = " PIXEL SIZE =";
   
   const char PIXELS_PER_LINE_DESC [PIXELS_PER_LINE_DESC_SIZE + 1]
      = " PIXELS PER LINE=";
   
   const char LINES_PER_IMAGE_DESC [LINES_PER_IMAGE_DESC_SIZE + 1]
      = " LINES PER IMAGE=";

   const char UL_DESC [CORNER_DESC_SIZE + 1]
      = " UL ";

   const char UR_DESC [CORNER_DESC_SIZE + 1]
      = " UR ";
   
   const char LR_DESC [CORNER_DESC_SIZE + 1]
      = " LR ";
   
   const char LL_DESC [CORNER_DESC_SIZE + 1]
      = " LL ";
   
   const char BANDS_PRESENT_DESC [BANDS_PRESENT_DESC_SIZE + 1]
      = " BANDS PRESENT =";
   
   const char BLOCKING_FACTOR_DESC [BLOCKING_FACTOR_DESC_SIZE + 1]
      = " BLOCKING FACTOR =";
   
   const char RECORD_LENGTH_DESC [RECORD_LENGTH_DESC_SIZE + 1]
      = " RECORD LENGTH =";
   
   const char SUN_ELEVATION_DESC [SUN_ELEVATION_DESC_SIZE + 1]
      = " SUN ELEVATION =";
   
   const char SUN_AZIMUTH_DESC [SUN_AZIMUTH_DESC_SIZE + 1]
      = " SUN AZIMUTH =";

   const char CENTER_DESC [CENTER_DESC_SIZE + 1]
      = " CENTER ";
   
   const char OFFSET_DESC [OFFSET_DESC_SIZE + 1]
      = " OFFSET=";
   
   const char REV_DESC [REV_DESC_SIZE + 1]
      = " REV";
      
   const char SPACE[] = " ";

   //***
   // Start at beginning of the stream.
   //***
   os.seekp(0, ios::beg);
   os << setiosflags(ios::fixed)  // Disable scientific mode.
      << setiosflags(ios::left)
       
      << PRODUCT_ID_DESC
      << setw(PRODUCT_ORDER_NUMBER_SIZE)
      << theProductOrderNumber

      << WRS_DESC         
      << setw(PATH_ROW_NUMBER_SIZE) 
      << thePathRowNumber

      << DATE_DESC
      << setw(DATE_SIZE) 
      << theAcquisitionDate

      << SATELLITE_NUMBER_DESC
      << setw(SAT_NUMBER_SIZE) 
      << theSatNumber 

      << INSTRUMENT_TYPE_DESC
      << setw(INSTRUMENT_TYPE_SIZE) 
      << theInstrumentType

      << PRODUCT_TYPE_DESC
      << setw(PRODUCT_TYPE_SIZE) 
      << theProductType

      << PRODUCT_SIZE_DESC
      << setw(PRODUCT_SIZE_SIZE) 
      << theProductSize

      << setw(MAP_SHEET_SIZE)
      << theMapSheetName

      << PROCESSING_TYPE_DESC
      << setw(PROCESSING_TYPE_SIZE) 
      << theProcessingType

      << RESAMPLING_ALGO_DESC
      << setw(RESAMPLING_ALGO_SIZE) 
      << theResampAlgorithm

      << RADIANCE_DESC;

   int i;
   for (i=0; i<NUMBER_OF_BANDS-1; i++) // Ouput six of the seven bands.
   {
      os << setw(RADIANCE_SIZE) << theBandRadiance[i] << SPACE;
   }

   os << setw(RADIANCE_SIZE) << theBandRadiance[6] // Last one no space.

      << VOLUME_NUMBER_DESC
      << setw(VOLUME_NUMBER_SIZE) 
      << theVolumeNumber

      << resetiosflags(ios::left)
      << setiosflags(ios::right)

      << FIRST_LINE_DESC
      << setw(FIRST_LINE_IN_VOLUME_SIZE) 
      << the1stLineInVolume

      << LINES_PER_VOLUME_DESC 
      << setw(LINES_PER_VOLUME_SIZE) 
      << theLinesPerVolume 

      << ORIENTATION_ANGLE_DESC 
      << setprecision(2) << setw(ORIENTATION_ANGLE_SIZE) 
      << theOrientationAngle  

      << MAP_PROJ_NAME_DESC
      << setw(MAP_PROJ_NAME_SIZE) 
      << theMapProjName

      << USGS_PROJ_NUMBER_DESC 
      << setw(USGS_PROJ_NUMBER_SIZE) 
      << theUsgsProjNumber 

      << USGS_MAP_ZONE_DESC 
      << setw(USGS_MAP_ZONE_SIZE) 
      << theUsgsMapZone 

      << PROJECTION_PARAMETER_DESC;

   for (i=0; i<NUMBER_OF_PROJECTION_PARAMETERS; i++)
   {
      os << setw(USGS_PROJ_PARAMS_SIZE)
         << theUsgsProjParam[i];
   }

   
   os << setiosflags(ios::left)

      << ELLIPSOID_DESC
      << setw(ELLIPSOID_SIZE)
      << theEllipsoid
      << resetiosflags(ios::left)
      << setprecision(3)

      << setiosflags(ios::right)

      << MAJOR_AXIS_DESC
      << setw(MAJOR_AXIS_SIZE) 
      << theSemiMajorAxis 

      << MINOR_AXIS_DESC
      << setw(MINOR_AXIS_SIZE) 
      << theSemiMinorAxis

      << setprecision(2)

      << PIXEL_GSD_DESC
      << setw(PIXEL_GSD_SIZE) 
      << theGsd
      
      << PIXELS_PER_LINE_DESC
      << setw(PIXELS_PER_LINE_SIZE) 
      << thePixelsPerLine
 
      << LINES_PER_IMAGE_DESC
      << setw(LINES_PER_IMAGE_SIZE) 
      << theLinesPerImage

      << setprecision(3)

      << UL_DESC
      << setw(LON_SIZE)
      << theUlLon
      << SPACE
      << setw(LAT_SIZE)
      << theUlLat
      << SPACE
      << setw(EASTING_SIZE)
      << theUlEasting
      << SPACE
      << setw(NORTHING_SIZE) 
      << theUlNorthing

      << UR_DESC
      << setw(LON_SIZE)
      << theUrLon
      << SPACE
      << setw(LAT_SIZE)
      << theUrLat
      << SPACE
      << setw(EASTING_SIZE)
      << theUrEasting
      << SPACE
      << setw(NORTHING_SIZE) 
      << theUrNorthing

      << LR_DESC
      << setw(LON_SIZE)
      << theLrLon
      << SPACE
      << setw(LAT_SIZE)
      << theLrLat
      << SPACE
      << setw(EASTING_SIZE)
      << theLrEasting
      << SPACE
      << setw(NORTHING_SIZE) 
      << theLrNorthing

      << LL_DESC
      << setw(LON_SIZE)
      << theLlLon
      << SPACE
      << setw(LAT_SIZE)
      << theLlLat
      << SPACE
      << setw(EASTING_SIZE)
      << theLlEasting
      << SPACE
      << setw(NORTHING_SIZE) 
      << theLlNorthing

      << resetiosflags(ios::right);

   os << BANDS_PRESENT_DESC
      << setw(BANDS_PRESENT_SIZE) 
      << theBandsPresentString

      << BLOCKING_FACTOR_DESC
      << setw(BLOCKING_FACTOR_SIZE) 
      << theBlockingFactor

      << RECORD_LENGTH_DESC
      << setw(RECORD_LENGTH_SIZE) 
      << theRecordSize
         
      << SUN_ELEVATION_DESC
      << setw(SUN_ELEVATION_SIZE) 
      << theSunElevation
         
      << SUN_AZIMUTH_DESC
      << setw(SUN_AZIMUTH_SIZE) 
      << theSunAzimuth
         
      << CENTER_DESC 
      << setw(LON_SIZE) 
      << theCenterLon 
      << SPACE
      << setw(LAT_SIZE)
      << theCenterLat
      << SPACE
      << setiosflags(ios::right)
      << setw(EASTING_SIZE)
      << theCenterEasting
      << SPACE
      << setw(NORTHING_SIZE)
      << theCenterNorthing
      << setw(CENTER_SAMPLE_SIZE)
      << theCenterSample
      << setw(CENTER_LINE_SIZE)
      << theCenterLine

      << OFFSET_DESC
      << setw(OFFSET_SIZE)
      << theOffset

      << REV_DESC
      << setw(FORMAT_VERSION_SIZE)
      << theFormatVersion << flush;  // Byte count of file should be 1536.

}


//***************************************************************************
// PRIVATE METHOD:
// ossimFfRevb::loadFromStream(istream& is)
// Initializes data members from an EOSAT Fast Format Rev B header.
//***************************************************************************
void ossimFfRevb::loadFromStream(istream& is)
{
   if (!is)
   {
      theErrorStatus = OSSIM_ERROR;

      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimFfRevb::loadFromStream:\n"
                                          << "Null stream passed in.  Returning from method."
                                          << std::endl;
      return;
   }

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

   int i; // For iterations.

   is.seekg(PRODUCT_ORDER_NUMBER_OFFSET, ios::beg);
   is.get(theProductOrderNumber, 
          PRODUCT_ORDER_NUMBER_SIZE + 1,
          ' ');
   if (checkStream(is)) return;
 
   is.seekg(PATH_ROW_NUMBER_OFFSET, ios::beg);
   is.get(thePathRowNumber, PATH_ROW_NUMBER_SIZE + 1);
   if (checkStream(is)) return;

   is.seekg(DATE_OFFSET, ios::beg);
   is.get(theAcquisitionDate, DATE_SIZE + 1);
   if (checkStream(is)) return;

   is.seekg(SAT_NUMBER_OFFSET, ios::beg);
   is.get(theSatNumber, SAT_NUMBER_SIZE + 1);
   if (checkStream(is)) return;

   is.seekg(INSTRUMENT_TYPE_OFFSET, ios::beg);
   is.get(theInstrumentType, INSTRUMENT_TYPE_SIZE + 1);
   if (checkStream(is)) return;

   is.seekg(PRODUCT_TYPE_OFFSET, ios::beg);
   is.get(theProductType, PRODUCT_TYPE_SIZE+ 1);
   if (checkStream(is)) return;

   is.seekg(PRODUCT_SIZE_OFFSET, ios::beg);
   is.get(theProductSize, PRODUCT_SIZE_SIZE+ 1);
   if (checkStream(is)) return;

   is.seekg(MAP_SHEET_NAME_OFFSET, ios::beg);
   is.get(theMapSheetName, MAP_SHEET_SIZE + 1);
   if (checkStream(is)) return;

   is.seekg(PROCESSING_TYPE_OFFSET, ios::beg);
   is.get(theProcessingType, PROCESSING_TYPE_SIZE + 1);
   if (checkStream(is)) return;

   is.seekg(RESAMPLING_ALGO_OFFSET, ios::beg);
   is.get(theResampAlgorithm, RESAMPLING_ALGO_SIZE + 1);
   if (checkStream(is)) return;

   for (i=0; i<NUMBER_OF_BANDS; i++)
   {      
      is.seekg(RADIANCE_OFFSET[i], ios::beg);
      is.get(theBandRadiance[i], RADIANCE_SIZE + 1);
      if (checkStream(is)) return;
   }

   is.seekg(VOLUME_NUMBER_OFFSET, ios::beg);
   is.get(theVolumeNumber, VOLUME_NUMBER_SIZE + 1);
   if (checkStream(is)) return;

   is.seekg(FIRST_LINE_IN_VOLUME_OFFSET, ios::beg);
   is.get(tmpBuff, FIRST_LINE_IN_VOLUME_SIZE + 1);
   if (checkStream(is)) return;

   the1stLineInVolume = atoi(tmpBuff);

   is.seekg(LINES_PER_VOLUME_OFFSET, ios::beg);
   is.get(tmpBuff, LINES_PER_VOLUME_SIZE + 1);
   if (checkStream(is)) return;

   theLinesPerVolume = atoi(tmpBuff);
   
   is.seekg(ORIENTATION_ANGLE_OFFSET, ios::beg);
   is.get(tmpBuff, ORIENTATION_ANGLE_SIZE + 1);
   if (checkStream(is)) return;

   theOrientationAngle = atof(tmpBuff);

   is.seekg(MAP_PROJ_NAME_OFFSET, ios::beg);
   is.get(theMapProjName, MAP_PROJ_NAME_SIZE + 1);
   if (checkStream(is)) return;
   
   is.seekg(USGS_PROJ_NUMBER_OFFSET, ios::beg);
   is.get(tmpBuff, USGS_PROJ_NUMBER_SIZE + 1);
   if (checkStream(is)) return;

   theUsgsProjNumber = atoi(tmpBuff);

   is.seekg(USGS_MAP_ZONE_OFFSET, ios::beg);
   is.get(tmpBuff, USGS_MAP_ZONE_SIZE + 1);
   if (checkStream(is)) return;

   theUsgsMapZone = atoi(tmpBuff);

   //***
   // Get the fifteen projection parameters.
   //***
   for (i=0; i < NUMBER_OF_PROJECTION_PARAMETERS; i++)
   {
      is.seekg(PROJ_PARAM_OFFSET[i], ios::beg);
      is.get(theUsgsProjParam[i], USGS_PROJ_PARAMS_SIZE + 1);
      if (checkStream(is)) return;
   }
   
   is.seekg(ELLIPSOID_OFFSET, ios::beg);
   is.get(theEllipsoid, ELLIPSOID_SIZE + 1);
   if (checkStream(is)) return;

   is.seekg(MAJOR_AXIS_OFFSET, ios::beg);
   is.get(tmpBuff,  MAJOR_AXIS_SIZE+ 1);
   if (checkStream(is)) return;

   theSemiMajorAxis = atof(tmpBuff);
   
   is.seekg(MINOR_AXIS_OFFSET, ios::beg);
   is.get(tmpBuff,  MINOR_AXIS_SIZE+ 1);
   if (checkStream(is)) return;

   theSemiMinorAxis = atof(tmpBuff);

   is.seekg(PIXEL_GSD_OFFSET, ios::beg);
   is.get(tmpBuff, PIXEL_GSD_SIZE + 1);
   if (checkStream(is)) return;

   theGsd = atof(tmpBuff);
   
   is.seekg(PIXELS_PER_LINE_OFFSET, ios::beg);
   is.get(tmpBuff, PIXELS_PER_LINE_SIZE + 1);
   if (checkStream(is)) return;

   thePixelsPerLine = atoi(tmpBuff);

   is.seekg(LINES_PER_IMAGE_OFFSET, ios::beg);
   is.get(tmpBuff, LINES_PER_IMAGE_SIZE + 1);
   if (checkStream(is)) return;

   theLinesPerImage = atoi(tmpBuff);

   //***
   // Start of upper left data:  longitude, latitude, easting, and northing. 
   //***
   is.seekg(UL_LON_OFFSET, ios::beg);
   is.get(theUlLon, LON_SIZE + 1);
   if (checkStream(is)) return;

   is.seekg(UL_LAT_OFFSET, ios::beg);
   is.get(theUlLat, LAT_SIZE + 1);
   if (checkStream(is)) return;
   
   is.seekg(UL_EASTING_OFFSET, ios::beg);
   is.get(tmpBuff, EASTING_SIZE + 1);
   if (checkStream(is)) return;

   theUlEasting = atof(tmpBuff);

   is.seekg(UL_NORTHING_OFFSET, ios::beg);
   is.get(tmpBuff, NORTHING_SIZE + 1);
   if (checkStream(is)) return;

   theUlNorthing = atof(tmpBuff);

   //***
   // End of upper left data.
   //***

   //***
   // Start of upper right data: longitude, latitude, easting, and northing. 
   //***
   is.seekg(UR_LON_OFFSET, ios::beg);
   is.get(theUrLon, LON_SIZE + 1);
   if (checkStream(is)) return;
   
   is.seekg(UR_LAT_OFFSET, ios::beg);
   is.get(theUrLat, LAT_SIZE + 1);
   if (checkStream(is)) return;
   
   is.seekg(UR_EASTING_OFFSET, ios::beg);
   is.get(tmpBuff, EASTING_SIZE + 1);
   if (checkStream(is)) return;

   theUrEasting = atof(tmpBuff);

   is.seekg(UR_NORTHING_OFFSET, ios::beg);
   is.get(tmpBuff, NORTHING_SIZE + 1);
   if (checkStream(is)) return;

   theUrNorthing = atof(tmpBuff);
   
   //***
   // End of upper right data.
   //***

   //***
   // Start of lower right data: longitude, latitude, easting, and northing. 
   //***
   is.seekg(LR_LON_OFFSET, ios::beg);
   is.get(theLrLon, LON_SIZE + 1);
   if (checkStream(is)) return;
   
   is.seekg(LR_LAT_OFFSET, ios::beg);
   is.get(theLrLat, LAT_SIZE + 1);
   if (checkStream(is)) return;
   
   is.seekg(LR_EASTING_OFFSET, ios::beg);
   is.get(tmpBuff, EASTING_SIZE + 1);
   if (checkStream(is)) return;

   theLrEasting = atof(tmpBuff);

   is.seekg(LR_NORTHING_OFFSET, ios::beg);
   is.get(tmpBuff, NORTHING_SIZE + 1);
   if (checkStream(is)) return;

   theLrNorthing = atof(tmpBuff);

   //***
   // End of lower right data.
   //***

   //***
   // Start of lower left data:  longitude, latitude, easting, and northing. 
   //***
   is.seekg(LL_LON_OFFSET, ios::beg);
   is.get(theLlLon, LON_SIZE + 1);
   if (checkStream(is)) return;
   
   is.seekg(LL_LAT_OFFSET, ios::beg);
   is.get(theLlLat, LAT_SIZE + 1);
   if (checkStream(is)) return;
   
   is.seekg(LL_EASTING_OFFSET, ios::beg);
   is.get(tmpBuff, EASTING_SIZE + 1);
   if (checkStream(is)) return;

   theLlEasting = atof(tmpBuff);

   is.seekg(LL_NORTHING_OFFSET, ios::beg);
   is.get(tmpBuff, NORTHING_SIZE + 1);
   if (checkStream(is)) return;

   theLlNorthing = atof(tmpBuff);

   //***
   // End of lower left data.
   //***

   is.seekg(BANDS_PRESENT_OFFSET, ios::beg);
   is.get(theBandsPresentString, BANDS_PRESENT_SIZE + 1);
   if (checkStream(is)) return;

   is.seekg(BLOCKING_FACTOR_OFFSET, ios::beg);
   is.get(tmpBuff, BLOCKING_FACTOR_SIZE + 1);
   if (checkStream(is)) return;

   theBlockingFactor = atoi(tmpBuff);

   is.seekg(RECORD_LENGTH_OFFSET, ios::beg);
   is.get(tmpBuff, RECORD_LENGTH_SIZE + 1);
   if (checkStream(is)) return;

   theRecordSize = atoi(tmpBuff);

   is.seekg(SUN_ELEVATION_OFFSET, ios::beg);
   is.get(tmpBuff, SUN_ELEVATION_SIZE + 1);
   if (checkStream(is)) return;

   theSunElevation = atoi(tmpBuff);

   is.seekg(SUN_AZIMUTH_OFFSET, ios::beg);
   is.get(tmpBuff, SUN_AZIMUTH_SIZE + 1);
   if (checkStream(is)) return;

   theSunAzimuth = atoi(tmpBuff);

   //***
   // Start of scene center data:  longitude, latitude, easting, northing,
   // sample, line. 
   //***
   is.seekg(CENTER_LON_OFFSET, ios::beg);
   is.get(theCenterLon, LON_SIZE + 1);
   if (checkStream(is)) return;

   is.seekg(CENTER_LAT_OFFSET, ios::beg);
   is.get(theCenterLat, LAT_SIZE + 1);
   if (checkStream(is)) return;
   
   is.seekg(CENTER_EASTING_OFFSET, ios::beg);
   is.get(tmpBuff, EASTING_SIZE + 1);
   if (checkStream(is)) return;

   theCenterEasting = atof(tmpBuff);

   is.seekg(CENTER_NORTHING_OFFSET, ios::beg);
   is.get(tmpBuff, NORTHING_SIZE + 1);
   if (checkStream(is)) return;

   theCenterNorthing = atof(tmpBuff);

   is.seekg(CENTER_SAMPLE_OFFSET, ios::beg);
   is.get(tmpBuff, CENTER_SAMPLE_SIZE + 1);
   if (checkStream(is)) return;

   theCenterSample = atoi(tmpBuff);

   is.seekg(CENTER_LINE_OFFSET, ios::beg);
   is.get(tmpBuff, CENTER_LINE_SIZE + 1);
   if (checkStream(is)) return;

   theCenterLine = atoi(tmpBuff);

   //***
   // End of scene center data.
   //***

   is.seekg(OFFSET_OFFSET, ios::beg);
   is.get(tmpBuff, OFFSET_SIZE + 1);
   if (checkStream(is)) return;

   theOffset = atoi(tmpBuff);

   is.seekg(FORMAT_VERSION_OFFSET, ios::beg);
   is.get(theFormatVersion, FORMAT_VERSION_SIZE + 1);
   if (checkStream(is)) return;

}

//***************************************************************************
// PRIVATE METHOD:
// ossimFfRevb::checkStream(istream& is)
//
// Checks the stream.  If an error has occurred it sets theErrorStatus.
// Returns: theErrorStatus    ( 0 = OK,  1 = ERROR )
//***************************************************************************
int ossimFfRevb::checkStream(istream& is)
{
   //***
   // istreams set fault bits and the operator! is overloaded to return
   // true if an error has occurred.
   //***
   if (!is)
   {
      theErrorStatus = OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_FATAL) << "ossimFfRevb::checkStream ERROR: "
                                          << "Stream corrupted.   Returning from method."
                                          << std::endl;
   } 
   return theErrorStatus;
}
   
//***************************************************************************
// Function:
// ostream& operator<<(ostream& os, const ossimFfRevb& head)
//***************************************************************************
ostream& operator<<(ostream& os, const ossimFfRevb& head)
{
   head.print(os);

   return os;
}
