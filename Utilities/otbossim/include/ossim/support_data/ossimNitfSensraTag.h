//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Walt Bunch
//
// Description: Nitf support class for SENSRA - Sensor parameters extension.
// 
//********************************************************************
// $Id: ossimNitfSensraTag.h 22013 2012-12-19 17:37:20Z dburken $
#ifndef ossimNitfSensraTag_HEADER
#define ossimNitfSensraTag_HEADER
#include <ossim/support_data/ossimNitfRegisteredTag.h>

class OSSIM_DLL ossimNitfSensraTag : public ossimNitfRegisteredTag
{
public:

   enum
   {
      REF_ROW_SIZE         = 8,
      REF_COL_SIZE         = 8,
      SENSOR_MODEL_SIZE    = 6,
      SENSOR_MOUNT_SIZE    = 3,
      SENSOR_LOC_SIZE      = 21,
      SENSOR_ALT_SRC_SIZE  = 1,
      SENSOR_ALT_SIZE      = 6,
      SENSOR_ALT_UNIT_SIZE = 1,
      SENSOR_AGL_SIZE      = 5,
      SENSOR_PITCH_SIZE    = 7,
      SENSOR_ROLL_SIZE     = 8,
      SENSOR_YAW_SIZE      = 8,
      PLATFORM_PITCH_SIZE  = 7,
      PLATFORM_ROLL_SIZE   = 8,
      PLATFORM_HDG_SIZE    = 5,
      GROUND_SPD_SRC_SIZE  = 1,
      GROUND_SPEED_SIZE    = 6,
      GROUND_SPD_UNIT_SIZE = 1,
      GROUND_TRACK_SIZE    = 5,
      VERTICAL_VEL_SIZE    = 5,
      VERT_VEL_UNIT_SIZE   = 1,
      SWATH_FRAMES_SIZE    = 4,
      N_SWATHS_SIZE        = 4,
      SPOT_NUM_SIZE        = 3
      //                  -----
      //                   132
   };

   ossimNitfSensraTag();
   virtual ~ossimNitfSensraTag();

   virtual void parseStream(std::istream& in);
   virtual void writeStream(std::ostream& out);

   virtual void clearFields();

   /**
    * @brief Print method that outputs a key/value type format
    * adding prefix to keys.
    * @param out Stream to output to.
    * @param prefix Prefix added to key like "image0.";
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix) const;
   
  // The set methods below taking ossimString args will truncate and
  // pad with spaces, as necessary, to match enumed size
   void setRefRow(ossimString refRow);
   ossimString getRefRow()const;
   void setRefCol(ossimString refCol);
   ossimString getRefCol()const;
   void setSensorModel(ossimString sensorModel);
   ossimString getSensorModel()const;
   void setSensorMount(ossimString sensorMount);
   ossimString getSensorMount()const;
   void setSensorLoc(ossimString sensorLoc);
   ossimString getSensorLoc()const;
   void setSensorAltSrc(ossimString sensorAltSrc);
   ossimString getSensorAltSrc()const;
   void setSensorAlt(ossimString sensorAlt);
   ossimString getSensorAlt()const;
   void setSensorAltUnit(ossimString sensorAltUnit);
   ossimString getSensorAltUnit()const;
   void setSensorAgl(ossimString sensorAgl);
   ossimString getSensorAgl()const;
   void setSensorPitch(ossimString sensorPitch);
   ossimString getSensorPitch()const;
   void setSensorRoll(ossimString sensorRoll);
   ossimString getSensorRoll()const;
   void setSensorYaw(ossimString sensorYaw);
   ossimString getSensorYaw()const;
   void setPlatformPitch(ossimString platformPitch);
   ossimString getPlatformPitch()const;
   void setPlatformRoll(ossimString platformRoll);
   ossimString getPlatformRoll()const;
   void setPlatformHdg(ossimString platformHdg);
   ossimString getPlatformHdg()const;
   void setGroundSpdSrc(ossimString groundSpdSrc);
   ossimString getGroundSpdSrc()const;
   void setGroundSpeed(ossimString groundSpeed);
   ossimString getGroundSpeed()const;
   void setGroundSpdUnit(ossimString groundSpdUnit);
   ossimString getGroundSpdUnit()const;
   void setGroundTrack(ossimString groundTrack);
   ossimString getGroundTrack()const;
   void setVerticalVel(ossimString verticalVel);
   ossimString getVerticalVel()const;
   void setVertVelUnit(ossimString vertVelUnit);
   ossimString getVertVelUnit()const;
   void setSwathFrames(ossimString swathFrames);
   ossimString getSwathFrames()const;
   void setNSwaths(ossimString nSwaths);
   ossimString getNSwaths()const;
   void setSpotNum(ossimString spotNum);
   ossimString getSpotNum()const;

protected:

   /**
    * FIELD: REFROW
    *
    */
   char theRefRow[REF_ROW_SIZE+1];

   /**
    * FIELD: REFCOL
    *
    */
   char theRefCol[REF_COL_SIZE+1];

   /**
    * FIELD: SENSORMODEL
    *
    */
   char theSensorModel[SENSOR_MODEL_SIZE+1];

   /**
    * FIELD: SENSORMOUNT
    *
    */
   char theSensorMount[SENSOR_MOUNT_SIZE+1];

   /**
    * FIELD: SENSORLOC
    *
    */
   char theSensorLoc[SENSOR_LOC_SIZE+1];

   /**
    * FIELD: SENSORALTSRC
    *
    */
   char theSensorAltSrc[SENSOR_ALT_SRC_SIZE+1];

   /**
    * FIELD: SENSORALT
    *
    */
   char theSensorAlt[SENSOR_ALT_SIZE+1];

   /**
    * FIELD: SENSORALTUNIT
    *
    */
   char theSensorAltUnit[SENSOR_ALT_UNIT_SIZE+1];

   /**
    * FIELD: SENSORAGL
    *
    */
   char theSensorAgl[SENSOR_AGL_SIZE+1];

   /**
    * FIELD: SENSORPITCH
    *
    */
   char theSensorPitch[SENSOR_PITCH_SIZE+1];

   /**
    * FIELD: SENSORROLL
    *
    */
   char theSensorRoll[SENSOR_ROLL_SIZE+1];

   /**
    * FIELD: SENSORYAW
    *
    */
   char theSensorYaw[SENSOR_YAW_SIZE+1];

   /**
    * FIELD: PLATFORMPITCH
    *
    */
   char thePlatformPitch[PLATFORM_PITCH_SIZE+1];

   /**
    * FIELD: PLATFORMROLL
    *
    */
   char thePlatformRoll[PLATFORM_ROLL_SIZE+1];

   /**
    * FIELD: PLATFORMHDG
    *
    */
   char thePlatformHdg[PLATFORM_HDG_SIZE+1];

   /**
    * FIELD: GROUNDSPDSRC
    *
    */
   char theGroundSpdSrc[GROUND_SPD_SRC_SIZE+1];

   /**
    * FIELD: GROUNDSPEED
    *
    */
   char theGroundSpeed[GROUND_SPEED_SIZE+1];

   /**
    * FIELD: GROUNDSPDUNIT
    *
    */
   char theGroundSpdUnit[GROUND_SPD_UNIT_SIZE+1];

   /**
    * FIELD: GROUNDTRACK
    *
    */
   char theGroundTrack[GROUND_TRACK_SIZE+1];

   /**
    * FIELD: VERTICALVEL
    *
    */
   char theVerticalVel[VERTICAL_VEL_SIZE+1];

   /**
    * FIELD: VERTVELUNIT
    *
    */
   char theVertVelUnit[VERT_VEL_UNIT_SIZE+1];

   /**
    * FIELD: SWATHFRAMES
    *
    */
   char theSwathFrames[SWATH_FRAMES_SIZE+1];

   /**
    * FIELD: NSWATHS
    *
    */
   char theNSwaths[N_SWATHS_SIZE+1];

   /**
    * FIELD: SPOTNUM
    *
    */
   char theSpotNum[SPOT_NUM_SIZE+1];

TYPE_DATA   
};

#endif
