//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: CSEXRA tag class declaration.
//
// Exploitation Reference Data TRE.
//
// See document STDI-0006-NCDRD Table 3.5-16 for more info.
// 
//----------------------------------------------------------------------------
// $Id

#ifndef ossimNitfCsexraTag_HEADER
#define ossimNitfCsexraTag_HEADER
#include <ossim/support_data/ossimNitfRegisteredTag.h>

class OSSIM_DLL ossimNitfCsexraTag : public ossimNitfRegisteredTag
{
public:
   /** @brief default constructor */
   ossimNitfCsexraTag();

   /** @brief destructor */
   virtual ~ossimNitfCsexraTag();

   /**
    * @brief Method to return tag name.
    * @return "CSEXRA" as an ossimString.
    */
   virtual ossimString getRegisterTagName()const;

   /** @brief Method to parse data from stream. */
   virtual void parseStream(std::istream& in);

   /** @brief Method to write data to stream. */
   virtual void writeStream(std::ostream& out);

   /** @return Byte size of this tag, 132 without CETAG and CEL fields. */
   virtual ossim_uint32 getSizeInBytes()const;

   /** @brief Method to clear all fields including null terminating. */
   virtual void clearFields();

   /**
    * @brief Print method that outputs a key/value type format
    * adding prefix to keys.
    * @param out Stream to output to.
    * @param prefix Prefix added to key like "image0.";
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix) const;
   
protected:

   /**
    * FIELD: SENSOR
    * 
    * 6 byte field BCS-A
    *
    * PAN MS
    */
   char theSensor[7];

   /**
    * FIELD: TIME_FIRST_LINE_IMAGE
    *
    * 12 byte field BCS-N Seconds (UTC)
    *
    * Time in seconds from midnight for the first line, synthetic array,
    * of the Dataset collection.
    * 
    * 00000.000000 to 86400.000000
    */
   char theTileFirstLine[13];

   /**
    * FIELD: TIME_IMAGE_DURATION
    *
    * 12 byte field BCS-N Seconds (UTC)
    *
    * Time Difference in seconds between the first line, synthetic array,
    * and the last line, synthetic array.
    * 
    * 00000.000000 to 86400.000000
    */
   char theImageTimeDuration[13];

   /**
    * FIELD: MAX_GSD
    *
    * 5 byte field BCS-N inches
    *
    * Predicted maximum mean ground sample distance (GSD)for the primary
    * target.
    *
    * 000.0 to 999.9
    */
   char theMaxGsd[6];

   /**
    * FIELD: ALONG_SCAN_GSD
    *
    * 5 byte field BCS-A inches
    *
    * Measured along scan GSD for the primary target.
    *
    * 000.0 to 999.9 or N/A
    */
   char theAlongScanGsd[6];

   /**
    * FIELD: CROSS_SCAN_GSD
    *
    * 5 byte field BCS-A inches
    *
    * Measured cross scan GSD for the primary target.
    *
    * 000.0 to 999.9 or N/A
    */
   char theCrossScanGsd[6];

   /**
    * FIELD: GEO_MEAN_GSD
    *
    * 5 byte field BCS-A inches
    *
    * Measured mean GSD for the primary target.
    *
    * 000.0 to 999.9 or N/A
    */
   char theGeoMeanGsd[6];

   /**
    * FIELD: A_S_VERT_GSD
    *
    * 5 byte field BCS-A inches
    *
    * Measured along scan vertical GSD for the primary target.
    *
    * 000.0 to 999.9 or N/A
    */
   char theAlongScanVertGsd[6];

   /**
    * FIELD: C_S_VERT_GSD
    *
    * 5 byte field BCS-A inches
    *
    * Measured cross scan vertical GSD for the primary target.
    *
    * 000.0 to 999.9 or N/A
    */
   char theCrossScanVertGsd[6];

   /**
    * FIELD: GEO_MEAN_VERT_GSD
    *
    * 5 byte field BCS-A inches
    *
    * Measured mean vertical GSD for the primary target.
    *
    * 000.0 to 999.9 or N/A
    */
   char theGeoMeanVertGsd[6];

   /**
    * FIELD: GEO_BETA_ANGLE
    *
    * 5 byte field BCS-A Degrees
    *
    * Angle on ground (Earth tangent plane) between along scan and cross scan
    * directions.
    *
    * 00.0 to 180.0 or N/A
    */
   char theGeoBetaAngle[6];
   
   /**
    * FIELD: DYNAMIC_RANGE
    *
    * 5 byte field BCS-N
    *
    * Dynamic range extent of pixel values in the image.
    *
    * 00000 to 02047 for PAN and MS.
    */
   char theDynamicRange[6];  

   /**
    * FIELD: NUM_LINES
    *
    * 7 byte field BCS-N
    *
    * 0000101 to 9999999
    */
   char theLine[8];
   
   /**
    * FIELD: NUM_SAMPLES
    *
    * 5 byte field BCS-N
    *
    * 00101 to 99999
    */
   char theSamples[6];  

   /**
    * FIELD: ANGLE_TO_NORTH
    *
    * 7 byte field BCS-N Degrees
    *
    * Angle in degrees measured clockwise from the first row of the image to
    * true North at image start time.
    * 
    * 000.000 to 360.000
    */
   char theAngleToNorth[8];

   /**
    * FIELD: OBLIQUITY_ANGLE
    *
    * 6 byte field BCS-N Degrees
    *
    * Obliquity angle measured from target local vertical.  I.e., the angle
    * betweent the local North-Earth-Down (NED) horizontal and the optical
    * axis of the image at the time of closest approach.
    * 
    * 00.000 to 90.000
    */
   char theObliquityAngle[7];

   /**
    * FIELD: AZ_OF_OBLIQUITY
    *
    * 7 byte field BCS-N Degrees
    *
    * Azimuth of the target-SV line-of-sight vectory projected in the target
    * local horizontal plane, measured clockwise from True North, computed
    * at Image Start Time.  The Velocity Control Point on the focal plane
    * is projected to this azimuth projection in the target local
    * horizontal plane.
    * 
    * 000.000 to 360.000
    */
   char theAzOfObliquity[8];
   
   /**
    * FIELD: GRD_COVER
    *
    * 1 byte field BCS-N
    *
    * Snow or no snow
    *
    * 1 = snow, 0 = no snow, 9 = not available.
    */
   char theGrdCover[2];

    /**
    * FIELD: SNOW_DEPTH_CAT
    *
    * 1 byte field BCS-N
    *
    * Provides the weighted average of the snow depth values for all of
    * the grids that overlap the tasked image area.
    *
    * 0 = 0 inches,
    * 1 = 1-8 inches or ice,
    * 2 = 9-17 inches,
    * 3 = greater than 17 inches
    * 9 = Not Available
    */
   char theSnowDepthCategory[2];

   /**
    * FIELD: SUN_AZIMUTH   
    *
    * 7 byte field BCS-N Degrees
    *
    * Azimuth of the target-sun line-of-sight vector projected in the
    * target local horizontal plane, measured clockwise from True North,
    * calculated at Image Start Time.
    * 
    * 000.000 to 360.000
    */
   char theSunAzimuth[8];

   /**
    * FIELD: SUN_ELEVATION  
    *
    * 7 byte field BCS-N Degrees
    *
    * The sun elevation angle from the local target plane to the sun
    * calculated at Image_Start_Time, where the local target plane is
    * referenced by the Target Centered - Earth Fixed, ST coordinate frame.
    * 
    * -90.000 to +90.000
    */
   char theSunElevation[8];
   
   /**
    * FIELD: PREDICTED_NIIRS
    *
    * 3 byte field BCS-A
    *
    * Imagery NIIRS value. 
    *
    * 0.0 to 9.0 or N/A
    */
   char thePredictedNiirs[4];

   /**
    * FIELD: CIRCL_ERR
    *
    * 3 byte field BCS-N Feet
    *
    * Predicted CE/90 in the geolocation in the scene.
    *
    * 000 to 999
    */
   char theCircularError[4];

   /**
    * FIELD: LINEAR_ERR
    *
    * 3 byte field BCS-N Feet
    *
    * Predicted LE/90 in the geolocation in the scene
    *
    * 000 to 999
    */
   char theLinearError[4];
   
TYPE_DATA   
};

#endif /* matches #ifndef ossimNitfCsexraTag_HEADER */
