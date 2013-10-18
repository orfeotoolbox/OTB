//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: CSEXRA tag class definition.
//
// Exploitation Reference Data TRE.
//
// See document STDI-0006-NCDRD Table 3.5-16 for more info.
// 
//----------------------------------------------------------------------------
// $Id

#include <cstring>
#include <istream>
#include <iostream>
#include <iomanip>

#include <ossim/support_data/ossimNitfCsexraTag.h>


RTTI_DEF1(ossimNitfCsexraTag, "ossimNitfCsexraTag", ossimNitfRegisteredTag);

ossimNitfCsexraTag::ossimNitfCsexraTag()
   : ossimNitfRegisteredTag()
{
   clearFields();
}

ossimNitfCsexraTag::~ossimNitfCsexraTag()
{
}

std::string ossimNitfCsexraTag::getRegisterTagName() const
{
   return std::string("CSEXRA");
}

void ossimNitfCsexraTag::parseStream(std::istream& in)
{
   clearFields();

   in.read(theSensor, 6);
   in.read(theTileFirstLine, 12);
   in.read(theImageTimeDuration, 12);
   in.read(theMaxGsd, 5);
   in.read(theAlongScanGsd, 5);
   in.read(theCrossScanGsd, 5);
   in.read(theGeoMeanGsd, 5);
   in.read(theAlongScanVertGsd, 5);
   in.read(theCrossScanVertGsd, 5);
   in.read(theGeoMeanVertGsd, 5);
   in.read(theGeoBetaAngle, 5);
   in.read(theDynamicRange, 5);
   in.read(theLine, 7);
   in.read(theSamples, 5);
   in.read(theAngleToNorth, 7);
   in.read(theObliquityAngle, 6);
   in.read(theAzOfObliquity, 7);
   in.read(theGrdCover, 1);
   in.read(theSnowDepthCategory, 1);
   in.read(theSunAzimuth, 7);
   in.read(theSunElevation, 7);
   in.read(thePredictedNiirs, 3);
   in.read(theCircularError, 3);
   in.read(theLinearError, 3);
}

void ossimNitfCsexraTag::writeStream(std::ostream& out)
{
   out.write(theSensor, 6);
   out.write(theTileFirstLine, 12);
   out.write(theImageTimeDuration, 12);
   out.write(theMaxGsd, 5);
   out.write(theAlongScanGsd, 5);
   out.write(theCrossScanGsd, 5);
   out.write(theGeoMeanGsd, 5);
   out.write(theAlongScanVertGsd, 5);
   out.write(theCrossScanVertGsd, 5);
   out.write(theGeoMeanVertGsd, 5);
   out.write(theGeoBetaAngle, 5);
   out.write(theDynamicRange, 5);
   out.write(theLine, 7);
   out.write(theSamples, 5);
   out.write(theAngleToNorth, 7);
   out.write(theObliquityAngle, 6);
   out.write(theAzOfObliquity, 7);
   out.write(theGrdCover, 1);
   out.write(theSnowDepthCategory, 1);
   out.write(theSunAzimuth, 7);
   out.write(theSunElevation, 7);
   out.write(thePredictedNiirs, 3);
   out.write(theCircularError, 3);
   out.write(theLinearError, 3);
}

ossim_uint32 ossimNitfCsexraTag::getSizeInBytes()const
{
   return 132;
}

void ossimNitfCsexraTag::clearFields()
{
   //---
   // No attempt made to set to defaults.
   // BCS-N's to '0's, BCS-A's to ' '(spaces)
   //---

   memset(theSensor, ' ',  6);
   memset(theTileFirstLine, '0', 12);
   memset(theImageTimeDuration, '0', 12);
   memset(theMaxGsd, '0', 5);
   memset(theAlongScanGsd, ' ', 5);
   memset(theCrossScanGsd, ' ', 5);
   memset(theGeoMeanGsd, ' ', 5);
   memset(theAlongScanVertGsd, ' ', 5);
   memset(theCrossScanVertGsd, ' ', 5);
   memset(theGeoMeanVertGsd, ' ', 5);
   memset(theGeoBetaAngle, ' ', 5);
   memset(theDynamicRange, '0', 5);
   memset(theLine, '0', 7);
   memset(theSamples, '0', 5);
   memset(theAngleToNorth, '0', 7);
   memset(theObliquityAngle, '0', 6);
   memset(theAzOfObliquity, '0', 7);
   memset(theGrdCover, '0', 1);
   memset(theSnowDepthCategory, '0', 1);
   memset(theSunAzimuth, '0', 7);
   memset(theSunElevation, '0', 7);
   memset(thePredictedNiirs, ' ', 3);
   memset(theCircularError, '0', 3);
   memset(theLinearError, '0', 3);

   theSensor[6] = '\0';
   theTileFirstLine[12] = '\0';
   theImageTimeDuration[12] = '\0';
   theMaxGsd[5] = '\0';
   theAlongScanGsd[5] = '\0';
   theCrossScanGsd[5] = '\0';
   theGeoMeanGsd[5] = '\0';
   theAlongScanVertGsd[5] = '\0';
   theCrossScanVertGsd[5] = '\0';
   theGeoMeanVertGsd[5] = '\0';
   theGeoBetaAngle[5] = '\0';
   theDynamicRange[5] = '\0';
   theLine[7] = '\0';
   theSamples[5] = '\0';
   theAngleToNorth[7] = '\0';
   theObliquityAngle[6] = '\0';
   theAzOfObliquity[7] = '\0';
   theGrdCover[1] = '\0';
   theSnowDepthCategory[1] = '\0';
   theSunAzimuth[7] = '\0';
   theSunElevation[7] = '\0';
   thePredictedNiirs[3] = '\0';
   theCircularError[3] = '\0';
   theLinearError[3] = '\0';
   
}

std::ostream& ossimNitfCsexraTag::print(
   std::ostream& out, const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += getRegisterTagName();
   pfx += ".";
   
   out << setiosflags(std::ios::left)
       << pfx << std::setw(24) << "CETAG:"
       << getRegisterTagName() << "\n"
       << pfx << std::setw(24) << "CEL:"
       << getSizeInBytes() << "\n"
       << pfx << std::setw(24) << "SENSOR:"
       << theSensor << "\n"
       << pfx << std::setw(24) << "TIME_FIRST_LINE_IMAGE:"
       << theTileFirstLine << "\n"
       << pfx << std::setw(24) << "TIME_IMAGE_DURATION:"
       << theImageTimeDuration << "\n"
       << pfx << std::setw(24) << "MAX_GSD:"
       << theMaxGsd << "\n"
       << pfx << std::setw(24) << "ALONG_SCAN_GSD:"
       << theAlongScanGsd << "\n"
       << pfx << std::setw(24) << "CROSS_SCAN_GSD:"
       << theCrossScanGsd << "\n"
       << pfx << std::setw(24) << "GEO_MEAN_GSD:"
       << theGeoMeanGsd << "\n"
       << pfx << std::setw(24) << "A_S_VERT_GSD:"
       << theAlongScanVertGsd << "\n"
       << pfx << std::setw(24) << "C_S_VERT_GSD:"
       << theCrossScanVertGsd << "\n"
       << pfx << std::setw(24) << "GEO_MEAN_VERT_GSD:"
       << theGeoMeanVertGsd << "\n"
       << pfx << std::setw(24) << "GEO_BETA_ANGLE:"
       << theGeoBetaAngle << "\n"
       << pfx << std::setw(24) << "DYNAMIC_RANGE:"
       << theDynamicRange << "\n"
       << pfx << std::setw(24) << "NUM_LINES:"
       << theLine << "\n"
       << pfx << std::setw(24) << "NUM_SAMPLES:"
       << theSamples << "\n"
       << pfx << std::setw(24) << "ANGLE_TO_NORTH:"
       << theAngleToNorth << "\n"
       << pfx << std::setw(24) << "OBLIQUITY_ANGLE:"
       << theObliquityAngle << "\n"
       << pfx << std::setw(24) << "AZ_OF_OBLIQUITY:"
       << theAzOfObliquity << "\n"
       << pfx << std::setw(24) << "GRD_COVER:"
       << theGrdCover << "\n"
       << pfx << std::setw(24) << "SNOW_DEPTH_CAT:"
       << theSnowDepthCategory << "\n"
       << pfx << std::setw(24) << "SUN_AZIMUTH:"
       << theSunAzimuth << "\n"
       << pfx << std::setw(24) << "SUN_ELEVATION:"
       << theSunElevation << "\n"
       << pfx << std::setw(24) << "PREDICTED_NIIRS:"
       << thePredictedNiirs << "\n"
       << pfx << std::setw(24) << "CIRCL_ERR:"
       << theCircularError << "\n"
       << pfx << std::setw(24) << "LINEAR_ERR:"
       << theLinearError<< "\n";
   
   return out;
}

ossimString ossimNitfCsexraTag::getSensor() const
{
   return ossimString(theSensor);
}
   
ossimString ossimNitfCsexraTag::getTimeFirstLineImage() const
{
   return ossimString(theTileFirstLine);
}
   
ossimString ossimNitfCsexraTag::getTimeImageDuration() const
{
   return ossimString(theImageTimeDuration);
}
   
ossimString ossimNitfCsexraTag::getMaxGsd() const
{
   return ossimString(theMaxGsd);
}
   
ossimString ossimNitfCsexraTag::getAlongScanGsd() const
{
   return ossimString(theAlongScanGsd);
}
   
ossimString ossimNitfCsexraTag::getCrossScanGsd() const
{
   return ossimString(theCrossScanGsd);
}
   
ossimString ossimNitfCsexraTag::getGeoMeanGsd() const
{
   return ossimString(theGeoMeanGsd);
}
   
ossimString ossimNitfCsexraTag::getAlongScanVerticalGsd() const
{
   return ossimString(theAlongScanVertGsd);
}
   
ossimString ossimNitfCsexraTag::getCrossScanVerticalGsd() const
{
   return ossimString(theCrossScanVertGsd);
}

ossimString ossimNitfCsexraTag::getGeoMeanVerticalGsd() const
{
   return ossimString(theGeoMeanVertGsd);
}

ossimString ossimNitfCsexraTag::getGeoBetaAngle() const
{
   return ossimString(theGeoBetaAngle);
}

ossimString ossimNitfCsexraTag::getDynamicRange() const
{
   return ossimString(theDynamicRange);
}
   
ossimString ossimNitfCsexraTag::getNumLines() const
{
   return ossimString(theLine);
}
   
ossimString ossimNitfCsexraTag::getNumSamples() const
{
   return ossimString(theSamples);
}
   
ossimString ossimNitfCsexraTag::getAngleToNorth() const
{
   return ossimString(theAngleToNorth);
}

ossimString ossimNitfCsexraTag::getObliquityAngle() const
{
   return ossimString(theObliquityAngle);
}

ossimString ossimNitfCsexraTag::getAzimuthOfObliquity() const
{
   return ossimString(theAzOfObliquity);
}
   
ossimString ossimNitfCsexraTag::getGroundCover() const
{
   return ossimString(theGrdCover);
}

ossimString ossimNitfCsexraTag::getSnowDepth() const
{
   return ossimString(theSnowDepthCategory);
}
   
ossimString ossimNitfCsexraTag::getSunAzimuth() const
{
   return ossimString(theSunAzimuth);
}

ossimString ossimNitfCsexraTag::getSunElevation() const
{
   return ossimString(theSunElevation);
}

ossimString ossimNitfCsexraTag::getPredictedNiirs() const
{
   return ossimString(thePredictedNiirs);
}
   
ossimString ossimNitfCsexraTag::getCE90() const
{
   return ossimString(theCircularError);
}

ossimString ossimNitfCsexraTag::getLE90() const
{
   return ossimString(theLinearError);
}
