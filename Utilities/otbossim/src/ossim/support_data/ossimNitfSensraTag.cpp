//*******************************************************************

// LICENSE: LGPL
//
// see top level LICENSE.txt
// 
// Author: Walt Bunch
//
// Description: Nitf support class for SENSRA - Sensor parameters extension.
// 
//********************************************************************
// $Id: ossimNitfSensraTag.cpp 22013 2012-12-19 17:37:20Z dburken $

#include <istream>
#include <iomanip>
#include <ossim/support_data/ossimNitfSensraTag.h>

RTTI_DEF1(ossimNitfSensraTag, "ossimNitfSensraTag", ossimNitfRegisteredTag);


ossimNitfSensraTag::ossimNitfSensraTag()
   : ossimNitfRegisteredTag(std::string("SENSRA"), 132)
{
   clearFields();
}

ossimNitfSensraTag::~ossimNitfSensraTag()
{
}

void ossimNitfSensraTag::parseStream(std::istream& in)
{
   clearFields();
   
   in.read(theRefRow, REF_ROW_SIZE);
   in.read(theRefCol, REF_COL_SIZE);
   in.read(theSensorModel, SENSOR_MODEL_SIZE);
   in.read(theSensorMount, SENSOR_MOUNT_SIZE);
   in.read(theSensorLoc, SENSOR_LOC_SIZE);
   in.read(theSensorAltSrc, SENSOR_ALT_SRC_SIZE);
   in.read(theSensorAlt, SENSOR_ALT_SIZE);
   in.read(theSensorAltUnit, SENSOR_ALT_UNIT_SIZE);
   in.read(theSensorAgl, SENSOR_AGL_SIZE);
   in.read(theSensorPitch, SENSOR_PITCH_SIZE);
   in.read(theSensorRoll, SENSOR_ROLL_SIZE);
   in.read(theSensorYaw, SENSOR_YAW_SIZE);
   in.read(thePlatformPitch, PLATFORM_PITCH_SIZE);
   in.read(thePlatformRoll, PLATFORM_ROLL_SIZE);
   in.read(thePlatformHdg, PLATFORM_HDG_SIZE);
   in.read(theGroundSpdSrc, GROUND_SPD_SRC_SIZE);
   in.read(theGroundSpeed, GROUND_SPEED_SIZE);
   in.read(theGroundSpdUnit, GROUND_SPD_UNIT_SIZE);
   in.read(theGroundTrack, GROUND_TRACK_SIZE);
   in.read(theVerticalVel, VERTICAL_VEL_SIZE);
   in.read(theVertVelUnit, VERT_VEL_UNIT_SIZE);
   in.read(theSwathFrames, SWATH_FRAMES_SIZE);
   in.read(theNSwaths, N_SWATHS_SIZE);
   in.read(theSpotNum, SPOT_NUM_SIZE);
}

void ossimNitfSensraTag::writeStream(std::ostream& out)
{
   out.write(theRefRow, REF_ROW_SIZE);
   out.write(theRefCol, REF_COL_SIZE);
   out.write(theSensorModel, SENSOR_MODEL_SIZE);
   out.write(theSensorMount, SENSOR_MOUNT_SIZE);
   out.write(theSensorLoc, SENSOR_LOC_SIZE);
   out.write(theSensorAltSrc, SENSOR_ALT_SRC_SIZE);
   out.write(theSensorAlt, SENSOR_ALT_SIZE);
   out.write(theSensorAltUnit, SENSOR_ALT_UNIT_SIZE);
   out.write(theSensorAgl, SENSOR_AGL_SIZE);
   out.write(theSensorPitch, SENSOR_PITCH_SIZE);
   out.write(theSensorRoll, SENSOR_ROLL_SIZE);
   out.write(theSensorYaw, SENSOR_YAW_SIZE);
   out.write(thePlatformPitch, PLATFORM_PITCH_SIZE);
   out.write(thePlatformRoll, PLATFORM_ROLL_SIZE);
   out.write(thePlatformHdg, PLATFORM_HDG_SIZE);
   out.write(theGroundSpdSrc, GROUND_SPD_SRC_SIZE);
   out.write(theGroundSpeed, GROUND_SPEED_SIZE);
   out.write(theGroundSpdUnit, GROUND_SPD_UNIT_SIZE);
   out.write(theGroundTrack, GROUND_TRACK_SIZE);
   out.write(theVerticalVel, VERTICAL_VEL_SIZE);
   out.write(theVertVelUnit, VERT_VEL_UNIT_SIZE);
   out.write(theSwathFrames, SWATH_FRAMES_SIZE);
   out.write(theNSwaths, N_SWATHS_SIZE);
   out.write(theSpotNum, SPOT_NUM_SIZE);
}

void ossimNitfSensraTag::clearFields()
{
   memset(theRefRow, ' ', REF_ROW_SIZE);
   memset(theRefCol, ' ', REF_COL_SIZE);
   memset(theSensorModel, ' ', SENSOR_MODEL_SIZE);
   memset(theSensorMount, ' ', SENSOR_MOUNT_SIZE);
   memset(theSensorLoc, ' ', SENSOR_LOC_SIZE);
   memset(theSensorAltSrc, ' ', SENSOR_ALT_SRC_SIZE);
   memset(theSensorAlt, ' ', SENSOR_ALT_SIZE);
   memset(theSensorAltUnit, ' ', SENSOR_ALT_UNIT_SIZE);
   memset(theSensorAgl, ' ', SENSOR_AGL_SIZE);
   memset(theSensorPitch, ' ', SENSOR_PITCH_SIZE);
   memset(theSensorRoll, ' ', SENSOR_ROLL_SIZE);
   memset(theSensorYaw, ' ', SENSOR_YAW_SIZE);
   memset(thePlatformPitch, ' ', PLATFORM_PITCH_SIZE);
   memset(thePlatformRoll, ' ', PLATFORM_ROLL_SIZE);
   memset(thePlatformHdg, ' ', PLATFORM_HDG_SIZE);
   memset(theGroundSpdSrc, ' ', GROUND_SPD_SRC_SIZE);
   memset(theGroundSpeed, ' ', GROUND_SPEED_SIZE);
   memset(theGroundSpdUnit, ' ', GROUND_SPD_UNIT_SIZE);
   memset(theGroundTrack, ' ', GROUND_TRACK_SIZE);
   memset(theVerticalVel, ' ', VERTICAL_VEL_SIZE);
   memset(theVertVelUnit, ' ', VERT_VEL_UNIT_SIZE);
   memset(theSwathFrames, ' ', SWATH_FRAMES_SIZE);
   memset(theNSwaths, ' ', N_SWATHS_SIZE);
   memset(theSpotNum, ' ', SPOT_NUM_SIZE);

   theRefRow[REF_ROW_SIZE] = '\0';
   theRefCol[REF_COL_SIZE] = '\0';
   theSensorModel[SENSOR_MODEL_SIZE] = '\0';
   theSensorMount[SENSOR_MOUNT_SIZE] = '\0';
   theSensorLoc[SENSOR_LOC_SIZE] = '\0';
   theSensorAltSrc[SENSOR_ALT_SRC_SIZE] = '\0';
   theSensorAlt[SENSOR_ALT_SIZE] = '\0';
   theSensorAltUnit[SENSOR_ALT_UNIT_SIZE] = '\0';
   theSensorAgl[SENSOR_AGL_SIZE] = '\0';
   theSensorPitch[SENSOR_PITCH_SIZE] = '\0';
   theSensorRoll[SENSOR_ROLL_SIZE] = '\0';
   theSensorYaw[SENSOR_YAW_SIZE] = '\0';
   thePlatformPitch[PLATFORM_PITCH_SIZE] = '\0';
   thePlatformRoll[PLATFORM_ROLL_SIZE] = '\0';
   thePlatformHdg[PLATFORM_HDG_SIZE] = '\0';
   theGroundSpdSrc[GROUND_SPD_SRC_SIZE] = '\0';
   theGroundSpeed[GROUND_SPEED_SIZE] = '\0';
   theGroundSpdUnit[GROUND_SPD_UNIT_SIZE] = '\0';
   theGroundTrack[GROUND_TRACK_SIZE] = '\0';
   theVerticalVel[VERTICAL_VEL_SIZE] = '\0';
   theVertVelUnit[VERT_VEL_UNIT_SIZE] = '\0';
   theSwathFrames[SWATH_FRAMES_SIZE] = '\0';
   theNSwaths[N_SWATHS_SIZE] = '\0';
   theSpotNum[SPOT_NUM_SIZE] = '\0';
}

std::ostream& ossimNitfSensraTag::print(
   std::ostream& out, const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += getTagName();
   pfx += ".";
   
   out << std::setiosflags(std::ios::left)
       << pfx << std::setw(24) << "CETAG:"
       << getTagName() << "\n"
       << pfx << std::setw(24) << "CEL:" << getTagLength() << "\n"
       << pfx << std::setw(24) << "REFROW:" << theRefRow << "\n"
       << pfx << std::setw(24) << "REFCOL:" << theRefCol << "\n"
       << pfx << std::setw(24) << "SENSORMODEL:" << theSensorModel<< "\n"
       << pfx << std::setw(24) << "SENSORMOUNT:" << theSensorMount << "\n"
       << pfx << std::setw(24) << "SENSORLOC:" << theSensorLoc << "\n"
       << pfx << std::setw(24) << "SENSORALTSRC:" << theSensorAltSrc<< "\n"
       << pfx << std::setw(24) << "SENSORALT:" << theSensorAlt << "\n"
       << pfx << std::setw(24) << "SENSORALTUNIT:" << theSensorAltUnit << "\n"
       << pfx << std::setw(24) << "SENSORAGL:" << theSensorAgl << "\n"
       << pfx << std::setw(24) << "SENSORPITCH:" << theSensorPitch << "\n"
       << pfx << std::setw(24) << "SENSORROLL:" << theSensorRoll<< "\n"
       << pfx << std::setw(24) << "SENSORYAW:" << theSensorYaw << "\n"
       << pfx << std::setw(24) << "PLATFORMPITCH:" << thePlatformPitch << "\n"
       << pfx << std::setw(24) << "PLATFORMROLL:" << thePlatformRoll << "\n"
       << pfx << std::setw(24) << "PLATFORMHDG:" << thePlatformHdg << "\n"
       << pfx << std::setw(24) << "GROUNDSPDSRC:" << theGroundSpdSrc << "\n"
       << pfx << std::setw(24) << "GROUNDSPEED:" << theGroundSpeed << "\n"
       << pfx << std::setw(24) << "GROUNDSPDUNIT:" << theGroundSpdUnit << "\n"
       << pfx << std::setw(24) << "GROUNDTRACK:" << theGroundTrack << "\n"
       << pfx << std::setw(24) << "VERTICALVEL:" << theVerticalVel<< "\n"
       << pfx << std::setw(24) << "VERTVELUNIT:" << theVertVelUnit << "\n"
       << pfx << std::setw(24) << "SWATHFRAMES:" << theSwathFrames<< "\n"
       << pfx << std::setw(24) << "NSWATHS:" << theNSwaths << "\n"
       << pfx << std::setw(24) << "SPOTNUM:" << theSpotNum << "\n";
   
   return out;
}

ossimString ossimNitfSensraTag::getRefRow()const
{
   return theRefRow;
}

void ossimNitfSensraTag::setRefRow(ossimString refRow)
{
   memset(theRefRow, ' ', REF_ROW_SIZE);
   memcpy(theRefRow, refRow.c_str(), std::min((size_t)REF_ROW_SIZE, refRow.length()));
}

ossimString ossimNitfSensraTag::getRefCol()const
{
   return theRefCol;
}

void ossimNitfSensraTag::setRefCol(ossimString refCol)
{
   memset(theRefCol, ' ', REF_COL_SIZE);
   memcpy(theRefCol, refCol.c_str(), std::min((size_t)REF_COL_SIZE, refCol.length()));
}

ossimString ossimNitfSensraTag::getSensorModel()const
{
   return theSensorModel;
}

void ossimNitfSensraTag::setSensorModel(ossimString sensorModel)
{
   memset(theSensorModel, ' ', SENSOR_MODEL_SIZE);
   memcpy(theSensorModel, sensorModel.c_str(), std::min((size_t)SENSOR_MODEL_SIZE, sensorModel.length()));
}

ossimString ossimNitfSensraTag::getSensorMount()const
{
   return theSensorMount;
}

void ossimNitfSensraTag::setSensorMount(ossimString sensorMount)
{
   memset(theSensorMount, ' ', SENSOR_MOUNT_SIZE);
   memcpy(theSensorMount, sensorMount.c_str(), std::min((size_t)SENSOR_MOUNT_SIZE, sensorMount.length()));
}

ossimString ossimNitfSensraTag::getSensorLoc()const
{
   return theSensorLoc;
}

void ossimNitfSensraTag::setSensorLoc(ossimString sensorLoc)
{
   memset(theSensorLoc, ' ', SENSOR_LOC_SIZE);
   memcpy(theSensorLoc, sensorLoc.c_str(), std::min((size_t)SENSOR_LOC_SIZE, sensorLoc.length()));
}

ossimString ossimNitfSensraTag::getSensorAltSrc()const
{
   return theSensorAltSrc;
}

void ossimNitfSensraTag::setSensorAltSrc(ossimString sensorAltSrc)
{
   memset(theSensorAltSrc, ' ', SENSOR_ALT_SRC_SIZE);
   memcpy(theSensorAltSrc, sensorAltSrc.c_str(), std::min((size_t)SENSOR_ALT_SRC_SIZE, sensorAltSrc.length()));
}

ossimString ossimNitfSensraTag::getSensorAlt()const
{
   return theSensorAlt;
}

void ossimNitfSensraTag::setSensorAlt(ossimString sensorAlt)
{
   memset(theSensorAlt, ' ', SENSOR_ALT_SIZE);
   memcpy(theSensorAlt, sensorAlt.c_str(), std::min((size_t)SENSOR_ALT_SIZE, sensorAlt.length()));
}

ossimString ossimNitfSensraTag::getSensorAltUnit()const
{
   return theSensorAltUnit;
}

void ossimNitfSensraTag::setSensorAltUnit(ossimString sensorAltUnit)
{
   memset(theSensorAltUnit, ' ', SENSOR_ALT_UNIT_SIZE);
   memcpy(theSensorAltUnit, sensorAltUnit.c_str(), std::min((size_t)SENSOR_ALT_UNIT_SIZE, sensorAltUnit.length()));
}

ossimString ossimNitfSensraTag::getSensorAgl()const
{
   return theSensorAgl;
}

void ossimNitfSensraTag::setSensorAgl(ossimString sensorAgl)
{
   memset(theSensorAgl, ' ', SENSOR_AGL_SIZE);
   memcpy(theSensorAgl, sensorAgl.c_str(), std::min((size_t)SENSOR_AGL_SIZE, sensorAgl.length()));
}

ossimString ossimNitfSensraTag::getSensorPitch()const
{
   return theSensorPitch;
}

void ossimNitfSensraTag::setSensorPitch(ossimString sensorPitch)
{
   memset(theSensorPitch, ' ', SENSOR_PITCH_SIZE);
   memcpy(theSensorPitch, sensorPitch.c_str(), std::min((size_t)SENSOR_PITCH_SIZE, sensorPitch.length()));
}

ossimString ossimNitfSensraTag::getSensorRoll()const
{
   return theSensorRoll;
}

void ossimNitfSensraTag::setSensorRoll(ossimString sensorRoll)
{
   memset(theSensorRoll, ' ', SENSOR_ROLL_SIZE);
   memcpy(theSensorRoll, sensorRoll.c_str(), std::min((size_t)SENSOR_ROLL_SIZE, sensorRoll.length()));
}

ossimString ossimNitfSensraTag::getSensorYaw()const
{
   return theSensorYaw;
}

void ossimNitfSensraTag::setSensorYaw(ossimString sensorYaw)
{
   memset(theSensorYaw, ' ', SENSOR_YAW_SIZE);
   memcpy(theSensorYaw, sensorYaw.c_str(), std::min((size_t)SENSOR_YAW_SIZE, sensorYaw.length()));
}

ossimString ossimNitfSensraTag::getPlatformPitch()const
{
   return thePlatformPitch;
}

void ossimNitfSensraTag::setPlatformPitch(ossimString platformPitch)
{
   memset(thePlatformPitch, ' ', PLATFORM_PITCH_SIZE);
   memcpy(thePlatformPitch, platformPitch.c_str(), std::min((size_t)PLATFORM_PITCH_SIZE, platformPitch.length()));
}

ossimString ossimNitfSensraTag::getPlatformRoll()const
{
   return thePlatformRoll;
}

void ossimNitfSensraTag::setPlatformRoll(ossimString platformRoll)
{
   memset(thePlatformRoll, ' ', PLATFORM_ROLL_SIZE);
   memcpy(thePlatformRoll, platformRoll.c_str(), std::min((size_t)PLATFORM_ROLL_SIZE, platformRoll.length()));
}

ossimString ossimNitfSensraTag::getPlatformHdg()const
{
   return thePlatformHdg;
}

void ossimNitfSensraTag::setPlatformHdg(ossimString platformHdg)
{
   memset(thePlatformHdg, ' ', PLATFORM_HDG_SIZE);
   memcpy(thePlatformHdg, platformHdg.c_str(), std::min((size_t)PLATFORM_HDG_SIZE, platformHdg.length()));
}

ossimString ossimNitfSensraTag::getGroundSpdSrc()const
{
   return theGroundSpdSrc;
}

void ossimNitfSensraTag::setGroundSpdSrc(ossimString groundSpdSrc)
{
   memset(theGroundSpdSrc, ' ', GROUND_SPD_SRC_SIZE);
   memcpy(theGroundSpdSrc, groundSpdSrc.c_str(), std::min((size_t)GROUND_SPD_SRC_SIZE, groundSpdSrc.length()));
}

ossimString ossimNitfSensraTag::getGroundSpeed()const
{
   return theGroundSpeed;
}

void ossimNitfSensraTag::setGroundSpeed(ossimString groundSpeed)
{
   memset(theGroundSpeed, ' ', GROUND_SPEED_SIZE);
   memcpy(theGroundSpeed, groundSpeed.c_str(), std::min((size_t)GROUND_SPEED_SIZE, groundSpeed.length()));
}

ossimString ossimNitfSensraTag::getGroundSpdUnit()const
{
   return theGroundSpdUnit;
}

void ossimNitfSensraTag::setGroundSpdUnit(ossimString groundSpdUnit)
{
   memset(theGroundSpdUnit, ' ', GROUND_SPD_UNIT_SIZE);
   memcpy(theGroundSpdUnit, groundSpdUnit.c_str(), std::min((size_t)GROUND_SPD_UNIT_SIZE, groundSpdUnit.length()));
}

ossimString ossimNitfSensraTag::getGroundTrack()const
{
   return theGroundTrack;
}

void ossimNitfSensraTag::setGroundTrack(ossimString groundTrack)
{
   memset(theGroundTrack, ' ', GROUND_TRACK_SIZE);
   memcpy(theGroundTrack, groundTrack.c_str(), std::min((size_t)GROUND_TRACK_SIZE, groundTrack.length()));
}

ossimString ossimNitfSensraTag::getVerticalVel()const
{
   return theVerticalVel;
}

void ossimNitfSensraTag::setVerticalVel(ossimString verticalVel)
{
   memset(theVerticalVel, ' ', VERTICAL_VEL_SIZE);
   memcpy(theVerticalVel, verticalVel.c_str(), std::min((size_t)VERTICAL_VEL_SIZE, verticalVel.length()));
}

ossimString ossimNitfSensraTag::getVertVelUnit()const
{
   return theVertVelUnit;
}

void ossimNitfSensraTag::setVertVelUnit(ossimString vertVelUnit)
{
   memset(theVertVelUnit, ' ', VERT_VEL_UNIT_SIZE);
   memcpy(theVertVelUnit, vertVelUnit.c_str(), std::min((size_t)VERT_VEL_UNIT_SIZE, vertVelUnit.length()));
}

ossimString ossimNitfSensraTag::getSwathFrames()const
{
   return theSwathFrames;
}

void ossimNitfSensraTag::setSwathFrames(ossimString swathFrames)
{
   memset(theSwathFrames, ' ', SWATH_FRAMES_SIZE);
   memcpy(theSwathFrames, swathFrames.c_str(), std::min((size_t)SWATH_FRAMES_SIZE, swathFrames.length()));
}

ossimString ossimNitfSensraTag::getNSwaths()const
{
   return theNSwaths;
}

void ossimNitfSensraTag::setNSwaths(ossimString nSwaths)
{
   memset(theNSwaths, ' ', N_SWATHS_SIZE);
   memcpy(theNSwaths, nSwaths.c_str(), std::min((size_t)N_SWATHS_SIZE, nSwaths.length()));
}

ossimString ossimNitfSensraTag::getSpotNum()const
{
   return theSpotNum;
}

void ossimNitfSensraTag::setSpotNum(ossimString spotNum)
{
   memset(theSpotNum, ' ', SPOT_NUM_SIZE);
   memcpy(theSpotNum, spotNum.c_str(), std::min((size_t)SPOT_NUM_SIZE, spotNum.length()));
}

