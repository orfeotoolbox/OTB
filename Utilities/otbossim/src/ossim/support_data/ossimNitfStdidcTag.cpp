//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Author:  Walt Bunch
// 
// Description:   NITF support data class for STDIDC - Standard ID extension.
//
// See:  STDI-000_v2.1 Table 7-3 for detailed description.
// 
//********************************************************************
// $Id: ossimNitfStdidcTag.cpp 19682 2011-05-31 14:21:20Z dburken $

#include <iostream>
#include <iomanip>
#include <ossim/support_data/ossimNitfStdidcTag.h>
#include <ossim/base/ossimStringProperty.h>

RTTI_DEF1(ossimNitfStdidcTag, "ossimNitfStdidcTag", ossimNitfRegisteredTag);
static const ossimString ACQDATE_KW = "ACQDATE";
static const ossimString MISSION_KW = "MISSION";
static const ossimString PASS_KW = "PASS";
static const ossimString OPNUM_KW = "OPNUM";
static const ossimString STARTSEGMENT_KW = "STARTSEGMENT";
static const ossimString REPRONUM_KW = "REPRONUM";
static const ossimString REPLAYREGEN_KW = "REPLAYREGEN";
static const ossimString BLANKFILL_KW = "BLANKFILL";
static const ossimString STARTCOLUMN_KW = "STARTCOLUMN";
static const ossimString STARTROW_KW = "STARTROW";
static const ossimString ENDSEGMENT_KW = "ENDSEGMENT";
static const ossimString ENDCOLUMN_KW = "ENDCOLUMN";
static const ossimString ENDROW_KW = "ENDROW";
static const ossimString COUNTRY_KW = "COUNTRY";
static const ossimString WAC_KW = "WAC";
static const ossimString LOCATION_KW = "LOCATION";


ossimNitfStdidcTag::ossimNitfStdidcTag()
{
   clearFields();
}

ossimNitfStdidcTag::~ossimNitfStdidcTag()
{
}

std::string ossimNitfStdidcTag::getRegisterTagName()const
{
   return std::string("STDIDC");
}

void ossimNitfStdidcTag::parseStream(std::istream& in)
{
   clearFields();
   
   in.read(theAcqDate,      ACQ_DATE_SIZE);
   in.read(theMission,      MISSION_SIZE);
   in.read(thePass,         PASS_SIZE);
   in.read(theOpNum,        OP_NUM_SIZE);
   in.read(theStartSegment, START_SEGMENT_SIZE);
   in.read(theReproNum,     REPRO_NUM_SIZE);
   in.read(theReplayRegen,  REPLAY_REGEN_SIZE);
   in.read(theBlankFill,    BLANK_FILL_SIZE);
   in.read(theStartColumn,  START_COLUMN_SIZE);
   in.read(theStartRow,     START_ROW_SIZE);
   in.read(theEndSegment,   END_SEGMENT_SIZE);
   in.read(theEndColumn,    END_COLUMN_SIZE);
   in.read(theEndRow,       END_ROW_SIZE);
   in.read(theCountry,      COUNTRY_SIZE);
   in.read(theWac,          WAC_SIZE);
   in.read(theLocation,     LOCATION_SIZE);
   in.read(theField17,      FIELD17_SIZE);
   in.read(theField18,      FIELD18_SIZE);
}

void ossimNitfStdidcTag::writeStream(std::ostream& out)
{
   out.write(theAcqDate,      ACQ_DATE_SIZE);
   out.write(theMission,      MISSION_SIZE);
   out.write(thePass,         PASS_SIZE);
   out.write(theOpNum,        OP_NUM_SIZE);
   out.write(theStartSegment, START_SEGMENT_SIZE);
   out.write(theReproNum,     REPRO_NUM_SIZE);
   out.write(theReplayRegen,  REPLAY_REGEN_SIZE);
   out.write(theBlankFill,    BLANK_FILL_SIZE);
   out.write(theStartColumn,  START_COLUMN_SIZE);
   out.write(theStartRow,     START_ROW_SIZE);
   out.write(theEndSegment,   END_SEGMENT_SIZE);
   out.write(theEndColumn,    END_COLUMN_SIZE);
   out.write(theEndRow,       END_ROW_SIZE);
   out.write(theCountry,      COUNTRY_SIZE);
   out.write(theWac,          WAC_SIZE);
   out.write(theLocation,     LOCATION_SIZE);
   out.write(theField17,      FIELD17_SIZE);
   out.write(theField18,      FIELD18_SIZE);
}

ossim_uint32 ossimNitfStdidcTag::getSizeInBytes()const
{
   return 89;
}

void ossimNitfStdidcTag::clearFields()
{
   memset(theAcqDate,      ' ', ACQ_DATE_SIZE);
   memset(theMission,      ' ', MISSION_SIZE);
   memset(thePass,         ' ', PASS_SIZE);
   memset(theOpNum,        ' ', OP_NUM_SIZE);
   memset(theStartSegment, ' ', START_SEGMENT_SIZE);
   memset(theReproNum,     ' ', REPRO_NUM_SIZE);
   memset(theReplayRegen,  ' ', REPLAY_REGEN_SIZE);
   memset(theBlankFill,    ' ', BLANK_FILL_SIZE);
   memset(theStartColumn,  ' ', START_COLUMN_SIZE);
   memset(theStartRow,     ' ', START_ROW_SIZE);
   memset(theEndSegment,   ' ', END_SEGMENT_SIZE);
   memset(theEndColumn,    ' ', END_COLUMN_SIZE);
   memset(theEndRow,       ' ', END_ROW_SIZE);
   memset(theCountry,      ' ', COUNTRY_SIZE);
   memset(theWac,          ' ', WAC_SIZE);
   memset(theLocation,     ' ', LOCATION_SIZE);
   memset(theField17,      ' ', FIELD17_SIZE);
   memset(theField18,      ' ', FIELD18_SIZE);

   theAcqDate[ACQ_DATE_SIZE]           = '\0';
   theMission[MISSION_SIZE]            = '\0';
   thePass[PASS_SIZE]                  = '\0';
   theOpNum[OP_NUM_SIZE]               = '\0';
   theStartSegment[START_SEGMENT_SIZE] = '\0';
   theReproNum[REPRO_NUM_SIZE]         = '\0';
   theReplayRegen[REPLAY_REGEN_SIZE]   = '\0';
   theBlankFill[BLANK_FILL_SIZE]       = '\0';
   theStartColumn[START_COLUMN_SIZE]   = '\0';
   theStartRow[START_ROW_SIZE]         = '\0';
   theEndSegment[END_SEGMENT_SIZE]     = '\0';
   theEndColumn[END_COLUMN_SIZE]       = '\0';
   theEndRow[END_ROW_SIZE]             = '\0';
   theCountry[COUNTRY_SIZE]            = '\0';
   theWac[WAC_SIZE]                    = '\0';
   theLocation[LOCATION_SIZE]          = '\0';
   theField17[FIELD17_SIZE]            = '\0';
   theField18[FIELD18_SIZE]            = '\0';
}

ossimString ossimNitfStdidcTag::getAcqDate()const
{
  return ossimString(theAcqDate);
}

void ossimNitfStdidcTag::setAcqDate(ossimString acqDate)
{
   memset(theAcqDate, ' ', ACQ_DATE_SIZE);
   memcpy(theAcqDate, acqDate.c_str(), std::min((size_t)ACQ_DATE_SIZE, acqDate.length()));
}

ossimString ossimNitfStdidcTag::getMission()const
{
  return ossimString(theMission);
}

void ossimNitfStdidcTag::setMission(ossimString mission)
{
   memset(theMission, ' ', MISSION_SIZE);
   memcpy(theMission, mission.c_str(), std::min((size_t)MISSION_SIZE, mission.length()));
}

ossimString ossimNitfStdidcTag::getPass()const
{
  return ossimString(thePass);
}

void ossimNitfStdidcTag::setPass(ossimString pass)
{
   memset(thePass, ' ', PASS_SIZE);
   memcpy(thePass, pass.c_str(), std::min((size_t)PASS_SIZE, pass.length()));
}

ossimString ossimNitfStdidcTag::getOpNum()const
{
  return ossimString(theOpNum);
}

void ossimNitfStdidcTag::setOpNum(ossimString opNum) 
{
   memset(theOpNum, ' ', OP_NUM_SIZE);
   memcpy(theOpNum, opNum.c_str(), std::min((size_t)OP_NUM_SIZE, opNum.length()));
}

ossimString ossimNitfStdidcTag::getStartSegment()const
{
  return ossimString(theStartSegment);
}

void ossimNitfStdidcTag::setStartSegment(ossimString startSegment) 
{
   memset(theStartSegment, ' ', START_SEGMENT_SIZE);
   memcpy(theStartSegment, startSegment.c_str(), std::min((size_t)START_SEGMENT_SIZE, startSegment.length()));
}

ossimString ossimNitfStdidcTag::getReproNum()const
{
  return ossimString(theReproNum);
}

void ossimNitfStdidcTag::setReproNum(ossimString reproNum) 
{
   memset(theReproNum, ' ', REPRO_NUM_SIZE);
   memcpy(theReproNum, reproNum.c_str(), std::min((size_t)REPRO_NUM_SIZE, reproNum.length()));
}

ossimString ossimNitfStdidcTag::getReplayRegen()const
{
  return ossimString(theReplayRegen);
}

void ossimNitfStdidcTag::setReplayRegen(ossimString replayRegen) 
{
   memset(theReplayRegen, ' ', REPLAY_REGEN_SIZE);
   memcpy(theReplayRegen, replayRegen.c_str(), std::min((size_t)REPLAY_REGEN_SIZE, replayRegen.length()));
}

ossimString ossimNitfStdidcTag::getBlankFill()const
{
  return ossimString(theBlankFill);
}

void ossimNitfStdidcTag::setBlankFill(ossimString blankFill) 
{
   memset(theBlankFill, ' ', BLANK_FILL_SIZE);
   memcpy(theBlankFill, blankFill.c_str(), std::min((size_t)BLANK_FILL_SIZE, blankFill.length()));
}

ossimString ossimNitfStdidcTag::getStartColumn()const
{
  return ossimString(theStartColumn);
}

void ossimNitfStdidcTag::setStartColumn(ossimString startColumn) 
{
   memset(theStartColumn, ' ', START_COLUMN_SIZE);
   memcpy(theStartColumn, startColumn.c_str(), std::min((size_t)START_COLUMN_SIZE, startColumn.length()));
}

ossimString ossimNitfStdidcTag::getStartRow()const
{
  return ossimString(theStartRow);
}

void ossimNitfStdidcTag::setStartRow(ossimString startRow) 
{
   memset(theStartRow, ' ', START_ROW_SIZE);
   memcpy(theStartRow, startRow.c_str(), std::min((size_t)START_ROW_SIZE, startRow.length()));
}

ossimString ossimNitfStdidcTag::getEndSegment()const
{
  return ossimString(theEndSegment);
}

void ossimNitfStdidcTag::setEndSegment(ossimString endSegment) 
{
   memset(theEndSegment, ' ', END_SEGMENT_SIZE);
   memcpy(theEndSegment, endSegment.c_str(), std::min((size_t)END_SEGMENT_SIZE, endSegment.length()));
}

ossimString ossimNitfStdidcTag::getEndColumn()const
{
  return ossimString(theEndColumn);
}

void ossimNitfStdidcTag::setEndColumn(ossimString endColumn) 
{
   memset(theEndColumn, ' ', END_COLUMN_SIZE);
   memcpy(theEndColumn, endColumn.c_str(), std::min((size_t)END_COLUMN_SIZE, endColumn.length()));
}

ossimString ossimNitfStdidcTag::getEndRow()const
{
  return ossimString(theEndRow);
}

void ossimNitfStdidcTag::setEndRow(ossimString endRow) 
{
   memset(theEndRow, ' ', END_ROW_SIZE);
   memcpy(theEndRow, endRow.c_str(), std::min((size_t)END_ROW_SIZE, endRow.length()));
}

ossimString ossimNitfStdidcTag::getCountry()const
{
  return ossimString(theCountry);
}

void ossimNitfStdidcTag::setCountry(ossimString country) 
{
   memset(theCountry, ' ', COUNTRY_SIZE);
   memcpy(theCountry, country.c_str(), std::min((size_t)COUNTRY_SIZE, country.length()));
}

ossimString ossimNitfStdidcTag::getWac()const
{
  return ossimString(theWac);
}

void ossimNitfStdidcTag::setWac(ossimString wac) 
{
   memset(theWac, ' ', WAC_SIZE);
   memcpy(theWac, wac.c_str(), std::min((size_t)WAC_SIZE, wac.length()));
}

ossimString ossimNitfStdidcTag::getLocation()const
{
  return ossimString(theLocation);
}

void ossimNitfStdidcTag::setLocation(ossimString location) 
{
   memset(theLocation, ' ', LOCATION_SIZE);
   memcpy(theLocation, location.c_str(), std::min((size_t)LOCATION_SIZE, location.length()));
}

ossimString ossimNitfStdidcTag::getField17()const
{
  return ossimString(theField17);
}

void ossimNitfStdidcTag::setField17(ossimString field17) 
{
   memset(theField17, ' ', FIELD17_SIZE);
   memcpy(theField17, field17.c_str(), std::min((size_t)FIELD17_SIZE, field17.length()));
}

ossimString ossimNitfStdidcTag::getField18()const
{
  return ossimString(theField18);
}

void ossimNitfStdidcTag::setField18(ossimString field18) 
{
   memset(theField18, ' ', FIELD18_SIZE);
   memcpy(theField18, field18.c_str(), std::min((size_t)FIELD18_SIZE, field18.length()));
}

std::ostream& ossimNitfStdidcTag::print(std::ostream& out,
                                        const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += getRegisterTagName();
   pfx += ".";
   
   out << setiosflags(std::ios::left)
       << pfx << std::setw(24) << "CETAG:"
       << getRegisterTagName() << "\n"
       << pfx << std::setw(24) << "CEL:"   << getSizeInBytes() << "\n"
       << pfx << std::setw(24) << "ACQDATE:"      << theAcqDate << "\n"
       << pfx << std::setw(24) << "MISSION:"      << theMission << "\n"
       << pfx << std::setw(24) << "PASS:"         << thePass << "\n"
       << pfx << std::setw(24) << "OPNUM:"        << theOpNum  << "\n"
       << pfx << std::setw(24) << "STARTSEGMENT:" << theStartSegment << "\n"
       << pfx << std::setw(24) << "REPRONUM:"     << theReproNum << "\n"
       << pfx << std::setw(24) << "REPLAYREGEN:"  << theReplayRegen << "\n"
       << pfx << std::setw(24) << "STARTCOLUMN:"  << theStartColumn << "\n"
       << pfx << std::setw(24) << "STARTROW:"     << theStartRow << "\n"
       << pfx << std::setw(24) << "ENDSEGMENT:"   << theEndSegment << "\n"
       << pfx << std::setw(24) << "ENDCOLUMN:"    << theEndColumn << "\n"
       << pfx << std::setw(24) << "ENDROW:"       << theEndRow << "\n"
       << pfx << std::setw(24) << "COUNTRY:"      << theCountry << "\n"
       << pfx << std::setw(24) << "WAC:"          << theWac << "\n"
       << pfx << std::setw(24) << "LOCATION:"     << theLocation << "\n";

   return out;
}

void ossimNitfStdidcTag::setProperty(ossimRefPtr<ossimProperty> property)
{
   ossimNitfRegisteredTag::setProperty(property);
}

ossimRefPtr<ossimProperty> ossimNitfStdidcTag::getProperty(const ossimString& name)const
{
   ossimProperty* result = 0;
   
   if(name == ACQDATE_KW)
   {
      result = new ossimStringProperty(name, theAcqDate);
   }
   else if(name == MISSION_KW)
   {
      result = new ossimStringProperty(name, theMission);
   }
   else if(name == PASS_KW)
   {
      result = new ossimStringProperty(name, thePass);
   }
   else if(name == OPNUM_KW)
   {
      result = new ossimStringProperty(name, theOpNum);
   }
   else if(name == STARTSEGMENT_KW)
   {
      result = new ossimStringProperty(name, theStartSegment);
   }
   else if(name == REPRONUM_KW)
   {
      result = new ossimStringProperty(name, theReproNum);
   }
   else if(name == REPLAYREGEN_KW)
   {
      result = new ossimStringProperty(name, theReplayRegen);
   }
   else if(name == BLANKFILL_KW)
   {
      result = new ossimStringProperty(name, theBlankFill);
   }
   else if(name == STARTCOLUMN_KW)
   {
      result = new ossimStringProperty(name, theStartColumn);
   }
   else if(name == STARTROW_KW)
   {
      result = new ossimStringProperty(name, theStartRow);
   }
   else if(name == ENDSEGMENT_KW)
   {
      result = new ossimStringProperty(name, theEndSegment);
   }
   else if(name == ENDCOLUMN_KW)
   {
      result = new ossimStringProperty(name, theEndColumn);
   }
   else if(name == ENDROW_KW)
   {
      result = new ossimStringProperty(name, theEndRow);
   }
   else if(name == COUNTRY_KW)
   {
      result = new ossimStringProperty(name,theCountry );
   }
   else if(name == WAC_KW)
   {
      result = new ossimStringProperty(name, theWac);
   }
   else if(name == LOCATION_KW)
   {
      result = new ossimStringProperty(name, theLocation);
   }
   else
   {
      return ossimNitfRegisteredTag::getProperty(name);
   }

   return result;
}

void ossimNitfStdidcTag::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimNitfRegisteredTag::getPropertyNames(propertyNames);

   propertyNames.push_back(ACQDATE_KW);
   propertyNames.push_back(MISSION_KW);
   propertyNames.push_back(PASS_KW);
   propertyNames.push_back(OPNUM_KW);
   propertyNames.push_back(STARTSEGMENT_KW);
   propertyNames.push_back(REPRONUM_KW);
   propertyNames.push_back(REPLAYREGEN_KW);
   propertyNames.push_back(BLANKFILL_KW);
   propertyNames.push_back(STARTCOLUMN_KW);
   propertyNames.push_back(STARTROW_KW);
   propertyNames.push_back(ENDSEGMENT_KW);
   propertyNames.push_back(ENDCOLUMN_KW);
   propertyNames.push_back(ENDROW_KW);
   propertyNames.push_back(COUNTRY_KW);
   propertyNames.push_back(WAC_KW);
   propertyNames.push_back(LOCATION_KW);
   
}

