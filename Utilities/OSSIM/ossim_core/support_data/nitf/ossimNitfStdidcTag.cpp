//*******************************************************************
//
// LICENSE: LGPL
//
// See top level LICENSE.txt
// 
// Author:  Walt Bunch
// 
// Description:   NITF support data class for STDIDC - Standard ID extension.
//
// See:  STDI-000_v2.1 Table 7-3 for detailed description.
// 
//********************************************************************
// $Id: ossimNitfStdidcTag.cpp,v 1.3 2005/08/09 11:32:21 gpotts Exp $
#include <support_data/nitf/ossimNitfStdidcTag.h>

RTTI_DEF1(ossimNitfStdidcTag, "ossimNitfStdidcTag", ossimNitfRegisteredTag);

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

ossimNitfStdidcTag::ossimNitfStdidcTag()
{
   clearFields();
}

ossimNitfStdidcTag::~ossimNitfStdidcTag()
{
}

ossimString ossimNitfStdidcTag::getRegisterTagName()const
{
   return "STDIDC";
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
   memcpy(theAcqDate, acqDate.c_str(), MIN(ACQ_DATE_SIZE, acqDate.length()));
}

ossimString ossimNitfStdidcTag::getMission()const
{
  return ossimString(theMission);
}

void ossimNitfStdidcTag::setMission(ossimString mission)
{
   memset(theMission, ' ', MISSION_SIZE);
   memcpy(theMission, mission.c_str(), MIN(MISSION_SIZE, mission.length()));
}

ossimString ossimNitfStdidcTag::getPass()const
{
  return ossimString(thePass);
}

void ossimNitfStdidcTag::setPass(ossimString pass)
{
   memset(thePass, ' ', PASS_SIZE);
   memcpy(thePass, pass.c_str(), MIN(PASS_SIZE, pass.length()));
}

ossimString ossimNitfStdidcTag::getOpNum()const
{
  return ossimString(theOpNum);
}

void ossimNitfStdidcTag::setOpNum(ossimString opNum) 
{
   memset(theOpNum, ' ', OP_NUM_SIZE);
   memcpy(theOpNum, opNum.c_str(), MIN(OP_NUM_SIZE, opNum.length()));
}

ossimString ossimNitfStdidcTag::getStartSegment()const
{
  return ossimString(theStartSegment);
}

void ossimNitfStdidcTag::setStartSegment(ossimString startSegment) 
{
   memset(theStartSegment, ' ', START_SEGMENT_SIZE);
   memcpy(theStartSegment, startSegment.c_str(), MIN(START_SEGMENT_SIZE, startSegment.length()));
}

ossimString ossimNitfStdidcTag::getReproNum()const
{
  return ossimString(theReproNum);
}

void ossimNitfStdidcTag::setReproNum(ossimString reproNum) 
{
   memset(theReproNum, ' ', REPRO_NUM_SIZE);
   memcpy(theReproNum, reproNum.c_str(), MIN(REPRO_NUM_SIZE, reproNum.length()));
}

ossimString ossimNitfStdidcTag::getReplayRegen()const
{
  return ossimString(theReplayRegen);
}

void ossimNitfStdidcTag::setReplayRegen(ossimString replayRegen) 
{
   memset(theReplayRegen, ' ', REPLAY_REGEN_SIZE);
   memcpy(theReplayRegen, replayRegen.c_str(), MIN(REPLAY_REGEN_SIZE, replayRegen.length()));
}

ossimString ossimNitfStdidcTag::getBlankFill()const
{
  return ossimString(theBlankFill);
}

void ossimNitfStdidcTag::setBlankFill(ossimString blankFill) 
{
   memset(theBlankFill, ' ', BLANK_FILL_SIZE);
   memcpy(theBlankFill, blankFill.c_str(), MIN(BLANK_FILL_SIZE, blankFill.length()));
}

ossimString ossimNitfStdidcTag::getStartColumn()const
{
  return ossimString(theStartColumn);
}

void ossimNitfStdidcTag::setStartColumn(ossimString startColumn) 
{
   memset(theStartColumn, ' ', START_COLUMN_SIZE);
   memcpy(theStartColumn, startColumn.c_str(), MIN(START_COLUMN_SIZE, startColumn.length()));
}

ossimString ossimNitfStdidcTag::getStartRow()const
{
  return ossimString(theStartRow);
}

void ossimNitfStdidcTag::setStartRow(ossimString startRow) 
{
   memset(theStartRow, ' ', START_ROW_SIZE);
   memcpy(theStartRow, startRow.c_str(), MIN(START_ROW_SIZE, startRow.length()));
}

ossimString ossimNitfStdidcTag::getEndSegment()const
{
  return ossimString(theEndSegment);
}

void ossimNitfStdidcTag::setEndSegment(ossimString endSegment) 
{
   memset(theEndSegment, ' ', END_SEGMENT_SIZE);
   memcpy(theEndSegment, endSegment.c_str(), MIN(END_SEGMENT_SIZE, endSegment.length()));
}

ossimString ossimNitfStdidcTag::getEndColumn()const
{
  return ossimString(theEndColumn);
}

void ossimNitfStdidcTag::setEndColumn(ossimString endColumn) 
{
   memset(theEndColumn, ' ', END_COLUMN_SIZE);
   memcpy(theEndColumn, endColumn.c_str(), MIN(END_COLUMN_SIZE, endColumn.length()));
}

ossimString ossimNitfStdidcTag::getEndRow()const
{
  return ossimString(theEndRow);
}

void ossimNitfStdidcTag::setEndRow(ossimString endRow) 
{
   memset(theEndRow, ' ', END_ROW_SIZE);
   memcpy(theEndRow, endRow.c_str(), MIN(END_ROW_SIZE, endRow.length()));
}

ossimString ossimNitfStdidcTag::getCountry()const
{
  return ossimString(theCountry);
}

void ossimNitfStdidcTag::setCountry(ossimString country) 
{
   memset(theCountry, ' ', COUNTRY_SIZE);
   memcpy(theCountry, country.c_str(), MIN(COUNTRY_SIZE, country.length()));
}

ossimString ossimNitfStdidcTag::getWac()const
{
  return ossimString(theWac);
}

void ossimNitfStdidcTag::setWac(ossimString wac) 
{
   memset(theWac, ' ', WAC_SIZE);
   memcpy(theWac, wac.c_str(), MIN(WAC_SIZE, wac.length()));
}

ossimString ossimNitfStdidcTag::getLocation()const
{
  return ossimString(theLocation);
}

void ossimNitfStdidcTag::setLocation(ossimString location) 
{
   memset(theLocation, ' ', LOCATION_SIZE);
   memcpy(theLocation, location.c_str(), MIN(LOCATION_SIZE, location.length()));
}

ossimString ossimNitfStdidcTag::getField17()const
{
  return ossimString(theField17);
}

void ossimNitfStdidcTag::setField17(ossimString field17) 
{
   memset(theField17, ' ', FIELD17_SIZE);
   memcpy(theField17, field17.c_str(), MIN(FIELD17_SIZE, field17.length()));
}

ossimString ossimNitfStdidcTag::getField18()const
{
  return ossimString(theField18);
}

void ossimNitfStdidcTag::setField18(ossimString field18) 
{
   memset(theField18, ' ', FIELD18_SIZE);
   memcpy(theField18, field18.c_str(), MIN(FIELD18_SIZE, field18.length()));
}

