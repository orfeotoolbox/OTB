//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: ACFTB - Aircraft Infomation Extension Format tag class
// declaration.
//
// See document STDI-0002 (version 3), Appendix E, Table E-6 for more info.
// 
//----------------------------------------------------------------------------
// $Id

#include <cstring>
#include <istream>
#include <iostream>
#include <iomanip>

#include <ossim/support_data/ossimNitfAcftbTag.h>


RTTI_DEF1(ossimNitfAcftbTag, "ossimNitfAcftbTag", ossimNitfRegisteredTag);

ossimNitfAcftbTag::ossimNitfAcftbTag()
   : ossimNitfRegisteredTag()
{
   clearFields();
}

ossimNitfAcftbTag::~ossimNitfAcftbTag()
{
}

std::string ossimNitfAcftbTag::getRegisterTagName() const
{
   return std::string("ACFTB");
}

void ossimNitfAcftbTag::parseStream(std::istream& in)
{
   clearFields();

   in.read(theAcMsnId, AC_MSN_ID_SIZE);
   in.read(theAcTailNo, AC_TAIL_NO_SIZE);
   in.read(theAcTo, AC_TO_SIZE);
   in.read(theSensorIdType, SENSOR_ID_TYPE_SIZE);
   in.read(theSensorId, SENSOR_ID_SIZE);
   in.read(theSceneSource, SCENE_SOURCE_SIZE);
   in.read(theScNum, SCNUM_SIZE);
   in.read(thePDate, PDATE_SIZE);
   in.read(theImHostNo, IMHOSTNO_SIZE);
   in.read(theImReqID, IMREQID_SIZE);
   in.read(theMPlan, MPLAN_SIZE);
   in.read(theEntLoc, ENTLOC_SIZE);
   in.read(theLocAccy, LOC_ACCY_SIZE);
   in.read(theEntelv, ENTELV_SIZE);
   in.read(theElvUnit, ELV_UNIT_SIZE);
   in.read(theExitLoc, EXITLOC_SIZE);
   in.read(theExitElev, EXITELV_SIZE);
   in.read(theTMap, TMAP_SIZE);
   in.read(theRowSpacing, ROW_SPACING_SIZE);
   in.read(theRowSpacingUnits, ROW_SPACING_UNITS_SIZE);
   in.read(theColSpacing, COL_SPACING_SIZE);
   in.read(theColSpacingUnits, COL_SPACING_UINTS_SIZE);
   in.read(theFocalLength, FOCAL_LENGTH_SIZE);
   in.read(theSenserial, SENSERIAL_SIZE);
   in.read(theAbSwVer, ABSWVER_SIZE);
   in.read(theCalDate, CAL_DATE_SIZE);
   in.read(thePatchTot, PATCH_TOT_SIZE);
   in.read(theMtiTot, MTI_TOT_SIZE);
}

void ossimNitfAcftbTag::writeStream(std::ostream& out)
{
   out.write(theAcMsnId, AC_MSN_ID_SIZE);
   out.write(theAcTailNo, AC_TAIL_NO_SIZE);
   out.write(theAcTo, AC_TO_SIZE);
   out.write(theSensorIdType, SENSOR_ID_TYPE_SIZE);
   out.write(theSensorId, SENSOR_ID_SIZE);
   out.write(theSceneSource, SCENE_SOURCE_SIZE);
   out.write(theScNum, SCNUM_SIZE);
   out.write(thePDate, PDATE_SIZE);
   out.write(theImHostNo, IMHOSTNO_SIZE);
   out.write(theImReqID, IMREQID_SIZE);
   out.write(theMPlan, MPLAN_SIZE);
   out.write(theEntLoc, ENTLOC_SIZE);
   out.write(theLocAccy, LOC_ACCY_SIZE);
   out.write(theEntelv, ENTELV_SIZE);
   out.write(theElvUnit, ELV_UNIT_SIZE);
   out.write(theExitLoc, EXITLOC_SIZE);
   out.write(theExitElev, EXITELV_SIZE);
   out.write(theTMap, TMAP_SIZE);
   out.write(theRowSpacing, ROW_SPACING_SIZE);
   out.write(theRowSpacingUnits, ROW_SPACING_UNITS_SIZE);
   out.write(theColSpacing, COL_SPACING_SIZE);
   out.write(theColSpacingUnits, COL_SPACING_UINTS_SIZE);
   out.write(theFocalLength, FOCAL_LENGTH_SIZE);
   out.write(theSenserial, SENSERIAL_SIZE);
   out.write(theAbSwVer, ABSWVER_SIZE);
   out.write(theCalDate, CAL_DATE_SIZE);
   out.write(thePatchTot, PATCH_TOT_SIZE);
   out.write(theMtiTot, MTI_TOT_SIZE);
}

ossim_uint32 ossimNitfAcftbTag::getSizeInBytes()const
{
   return CEL_SIZE;
}

void ossimNitfAcftbTag::clearFields()
{
   // BCS-N's to '0's, BCS-A's to ' '(spaces)

   // clear
   memset(theAcMsnId, ' ', AC_MSN_ID_SIZE);
   memset(theAcTailNo, ' ', AC_TAIL_NO_SIZE);
   memset(theAcTo, ' ', AC_TO_SIZE);
   memset(theSensorIdType, ' ', SENSOR_ID_TYPE_SIZE);
   memset(theSensorId, ' ', SENSOR_ID_SIZE);
   memset(theSceneSource, ' ', SCENE_SOURCE_SIZE);
   memset(theScNum, ' ', SCNUM_SIZE);
   memset(thePDate, ' ', PDATE_SIZE);
   memset(theImHostNo, ' ', IMHOSTNO_SIZE);
   memset(theImReqID, ' ', IMREQID_SIZE);
   memset(theMPlan, ' ', MPLAN_SIZE);
   memset(theEntLoc, ' ', ENTLOC_SIZE);
   memset(theLocAccy, ' ', LOC_ACCY_SIZE);
   memset(theEntelv, ' ', ENTELV_SIZE);
   memset(theElvUnit, ' ', ELV_UNIT_SIZE);
   memset(theExitLoc, ' ', EXITLOC_SIZE);
   memset(theExitElev, ' ', EXITELV_SIZE);
   memset(theTMap, ' ', TMAP_SIZE);
   memset(theRowSpacing, ' ', ROW_SPACING_SIZE);
   memset(theRowSpacingUnits, ' ', ROW_SPACING_UNITS_SIZE);
   memset(theColSpacing, ' ', COL_SPACING_SIZE);
   memset(theColSpacingUnits, ' ', COL_SPACING_UINTS_SIZE);
   memset(theFocalLength, ' ', FOCAL_LENGTH_SIZE);
   memset(theSenserial, ' ', SENSERIAL_SIZE);
   memset(theAbSwVer, ' ', ABSWVER_SIZE);
   memset(theCalDate, ' ', CAL_DATE_SIZE);
   memset(thePatchTot, ' ', PATCH_TOT_SIZE);
   memset(theMtiTot, ' ', MTI_TOT_SIZE);


   // null terminate
   theAcMsnId[AC_MSN_ID_SIZE] = '\0';
   theAcTailNo[AC_TAIL_NO_SIZE] = '\0';
   theAcTo[AC_TO_SIZE] = '\0';
   theSensorIdType[SENSOR_ID_TYPE_SIZE] = '\0';
   theSensorId[SENSOR_ID_SIZE] = '\0';
   theSceneSource[SCENE_SOURCE_SIZE] = '\0';
   theScNum[SCNUM_SIZE] = '\0';
   thePDate[PDATE_SIZE] = '\0';
   theImHostNo[IMHOSTNO_SIZE] = '\0';
   theImReqID[IMREQID_SIZE] = '\0';
   theMPlan[MPLAN_SIZE] = '\0';
   theEntLoc[ENTLOC_SIZE] = '\0';
   theLocAccy[LOC_ACCY_SIZE] = '\0';
   theEntelv[ENTELV_SIZE] = '\0';
   theElvUnit[ELV_UNIT_SIZE] = '\0';
   theExitLoc[EXITLOC_SIZE] = '\0';
   theExitElev[EXITELV_SIZE] = '\0';
   theTMap[TMAP_SIZE] = '\0';
   theRowSpacing[ROW_SPACING_SIZE] = '\0';
   theRowSpacingUnits[ROW_SPACING_UNITS_SIZE] = '\0';
   theColSpacing[COL_SPACING_SIZE] = '\0';
   theColSpacingUnits[COL_SPACING_UINTS_SIZE] = '\0';
   theFocalLength[FOCAL_LENGTH_SIZE] = '\0';
   theSenserial[SENSERIAL_SIZE] = '\0';
   theAbSwVer[ABSWVER_SIZE] = '\0';
   theCalDate[CAL_DATE_SIZE] = '\0';
   thePatchTot[PATCH_TOT_SIZE] = '\0';
   theMtiTot[MTI_TOT_SIZE] = '\0';
}

ossimString ossimNitfAcftbTag::getFocalLength()const
{
   return ossimString(theFocalLength);
}

ossimString ossimNitfAcftbTag::getColumnSpacing()const
{
   return ossimString(theColSpacing);
}

ossimString ossimNitfAcftbTag::getColumnSpacingUnits()const
{
   return ossimString(theColSpacingUnits);
}

ossimString ossimNitfAcftbTag::getRowSpacing()const
{
   return ossimString(theRowSpacing);
}

ossimString ossimNitfAcftbTag::getRowSpacingUnits()const
{
   return ossimString(theRowSpacingUnits);
}


std::ostream& ossimNitfAcftbTag::print(
   std::ostream& out, const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += getRegisterTagName();
   pfx += ".";
   
   out << setiosflags(std::ios::left)
       << pfx << std::setw(24) << "CETAG:"
       << getRegisterTagName() << "\n"
       << pfx << std::setw(24) << "CEL:"   << getSizeInBytes() << "\n"
       << pfx << std::setw(24) << "AC_MSN_ID:"
       << theAcMsnId << "\n"
       << pfx << std::setw(24) << "AC_TAIL_NO:"
       << theAcTailNo << "\n"
       << pfx << std::setw(24) << "AC_TO:"
       << theAcTo << "\n"
       << pfx << std::setw(24) << "SENSOR_ID_TYPE:"
       << theSensorIdType << "\n"
       << pfx << std::setw(24) << "SENSOR_ID:"
       << theSensorId << "\n"
       << pfx << std::setw(24) << "SCENE_SOURCE:"
       << theSceneSource << "\n"
       << pfx << std::setw(24) << "SCNUM:"
       << theScNum << "\n"
       << pfx << std::setw(24) << "PDATE:"
       << thePDate << "\n"
       << pfx << std::setw(24) << "IMHOSTNO:"
       << theImHostNo << "\n"
       << pfx << std::setw(24) << "IMREQID:"
       << theImReqID << "\n"
       << pfx << std::setw(24) << "MPLAN:"
       << theMPlan << "\n"
       << pfx << std::setw(24) << "ENTLOC:"
       << theEntLoc << "\n"
       << pfx << std::setw(24) << "LOC_ACCY:"
       << theLocAccy << "\n"
       << pfx << std::setw(24) << "ENTELV:"
       << theEntelv << "\n"
       << pfx << std::setw(24) << "ELV_UNIT:"
       << theElvUnit << "\n"
       << pfx << std::setw(24) << "EXITLOC:"
       << theExitLoc << "\n"
       << pfx << std::setw(24) << "EXITELV:"
       << theExitElev << "\n"
       << pfx << std::setw(24) << "TMAP:"
       << theTMap << "\n"
       << pfx << std::setw(24) << "ROW_SPACING:"
       << theRowSpacing << "\n"
       << pfx << std::setw(24) << "ROW_SPACING_UNITS:"
       << theRowSpacingUnits << "\n"
       << pfx << std::setw(24) << "COL_SPACING:"
       << theColSpacing << "\n"
       << pfx << std::setw(24) << "COL_SPACING_UINTS:"
       << theColSpacingUnits << "\n"
       << pfx << std::setw(24) << "FOCAL_LENGTH:"
       << theFocalLength << "\n"
       << pfx << std::setw(24) << "SENSERIAL:"
       << theSenserial << "\n"
       << pfx << std::setw(24) << "ABSWVER:"
       << theAbSwVer << "\n"
       << pfx << std::setw(24) << "CAL_DATE:"
       << theCalDate << "\n"
       << pfx << std::setw(24) << "PATCH_TOT:"
       << thePatchTot << "\n"
       << pfx << std::setw(24) << "MTI_TOT:"
       << theMtiTot << "\n";
   
   return out;
}
