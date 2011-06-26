//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: AIMIDB - Additional Image ID Extension Format tag class
// definition.
//
// See document STDI-0002 (version 3), Appendix E, Table E-3 for more info.
// 
//----------------------------------------------------------------------------
// $Id

#include <cstring>
#include <istream>
#include <iostream>
#include <iomanip>

#include <ossim/support_data/ossimNitfAimidbTag.h>


RTTI_DEF1(ossimNitfAimidbTag, "ossimNitfAimidbTag", ossimNitfRegisteredTag);

ossimNitfAimidbTag::ossimNitfAimidbTag()
   : ossimNitfRegisteredTag()
{
   clearFields();
}

ossimNitfAimidbTag::~ossimNitfAimidbTag()
{
}

std::string ossimNitfAimidbTag::getRegisterTagName() const
{
   return std::string("AIMIDB");
}

void ossimNitfAimidbTag::parseStream(std::istream& in)
{
   clearFields();

   in.read(theAcquisitionDate, ACQUISITION_DATE_SIZE);
   in.read(theMissionNumber, MISSION_NO_SIZE);
   in.read(theMissionIdentification, MISSION_IDENTIFICATION_SIZE);
   in.read(theFlightNo, FLIGHT_NO_SIZE);
   in.read(theOpNum, OP_NUM_SIZE);
   in.read(theCurrentSegment, CURRENT_SEGMENT_SIZE);
   in.read(theReproNum, REPRO_NUM_SIZE);
   in.read(theReplay, REPLAY_SIZE);
   in.read(theReserved1, RESERVED_1_SIZE);
   in.read(theStartTileColumn, START_TILE_COLUMN_SIZE);
   in.read(theStartTileRow, START_TILE_ROW_SIZE);
   in.read(theEndSegment, END_SEGMENT_SIZE);
   in.read(theTileColumn, END_TILE_COLUMN_SIZE);
   in.read(theTileRow, END_TILE_ROW_SIZE);
   in.read(theCountry, COUNTRY_SIZE);
   in.read(theReserved2, RESERVED_2_SIZE);
   in.read(theLocation, LOCATION_SIZE);
   in.read(theReserved3, RESERVED_3_SIZE);
}

void ossimNitfAimidbTag::writeStream(std::ostream& out)
{
   out.write(theAcquisitionDate, ACQUISITION_DATE_SIZE);
   out.write(theMissionNumber, MISSION_NO_SIZE);
   out.write(theMissionIdentification, MISSION_IDENTIFICATION_SIZE);
   out.write(theFlightNo, FLIGHT_NO_SIZE);
   out.write(theOpNum, OP_NUM_SIZE);
   out.write(theCurrentSegment, CURRENT_SEGMENT_SIZE);
   out.write(theReproNum, REPRO_NUM_SIZE);
   out.write(theReplay, REPLAY_SIZE);
   out.write(theReserved1, RESERVED_1_SIZE);
   out.write(theStartTileColumn, START_TILE_COLUMN_SIZE);
   out.write(theStartTileRow, START_TILE_ROW_SIZE);
   out.write(theEndSegment, END_SEGMENT_SIZE);
   out.write(theTileColumn, END_TILE_COLUMN_SIZE);
   out.write(theTileRow, END_TILE_ROW_SIZE);
   out.write(theCountry, COUNTRY_SIZE);
   out.write(theReserved2, RESERVED_2_SIZE);
   out.write(theLocation, LOCATION_SIZE);
   out.write(theReserved3, RESERVED_3_SIZE);
}

ossim_uint32 ossimNitfAimidbTag::getSizeInBytes()const
{
   return CEL_SIZE;
}

void ossimNitfAimidbTag::clearFields()
{
   // BCS-N's to '0's, BCS-A's to ' '(spaces)

   // clear
   memset(theAcquisitionDate, ' ', ACQUISITION_DATE_SIZE);
   memset(theMissionNumber, ' ', MISSION_NO_SIZE);
   memset(theMissionIdentification, ' ', MISSION_IDENTIFICATION_SIZE);
   memset(theFlightNo, ' ', FLIGHT_NO_SIZE);
   memset(theOpNum, ' ', OP_NUM_SIZE);
   memset(theCurrentSegment, ' ', CURRENT_SEGMENT_SIZE);
   memset(theReproNum, ' ', REPRO_NUM_SIZE);
   memset(theReplay, ' ', REPLAY_SIZE);
   memset(theReserved1, ' ', RESERVED_1_SIZE);
   memset(theStartTileColumn, ' ', START_TILE_COLUMN_SIZE);
   memset(theStartTileRow, ' ', START_TILE_ROW_SIZE);
   memset(theEndSegment, ' ', END_SEGMENT_SIZE);
   memset(theTileColumn, ' ', END_TILE_COLUMN_SIZE);
   memset(theTileRow, ' ', END_TILE_ROW_SIZE);
   memset(theCountry, ' ', COUNTRY_SIZE);
   memset(theReserved2, ' ', RESERVED_2_SIZE);
   memset(theLocation, ' ', LOCATION_SIZE);
   memset(theReserved3, ' ', RESERVED_3_SIZE);

   // null terminate
   theAcquisitionDate[ACQUISITION_DATE_SIZE] = '\0';
   theMissionNumber[MISSION_NO_SIZE] = '\0';
   theMissionIdentification[MISSION_IDENTIFICATION_SIZE] = '\0';
   theFlightNo[FLIGHT_NO_SIZE] = '\0';
   theOpNum[OP_NUM_SIZE] = '\0';
   theCurrentSegment[CURRENT_SEGMENT_SIZE] = '\0';
   theReproNum[REPRO_NUM_SIZE] = '\0';
   theReplay[REPLAY_SIZE] = '\0';
   theReserved1[RESERVED_1_SIZE] = '\0';
   theStartTileColumn[START_TILE_COLUMN_SIZE] = '\0';
   theStartTileRow[START_TILE_ROW_SIZE] = '\0';
   theEndSegment[END_SEGMENT_SIZE] = '\0';
   theTileColumn[END_TILE_COLUMN_SIZE] = '\0';
   theTileRow[END_TILE_ROW_SIZE] = '\0';
   theCountry[COUNTRY_SIZE] = '\0';
   theReserved2[RESERVED_2_SIZE] = '\0';
   theLocation[LOCATION_SIZE] = '\0';
   theReserved3[RESERVED_3_SIZE] = '\0';
}

std::ostream& ossimNitfAimidbTag::print(
   std::ostream& out, const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += getRegisterTagName();
   pfx += ".";
   
   out << setiosflags(std::ios::left)
       << pfx << std::setw(24) << "CETAG:"
       << getRegisterTagName() << "\n"
       << pfx << std::setw(24) << "CEL:"   << getSizeInBytes() << "\n"
       << pfx << std::setw(24) << "ACQUISITION_DATE:"
       << theAcquisitionDate << "\n"
       << pfx << std::setw(24) << "MISSION_NO:"
       << theMissionNumber << "\n"
       << pfx << std::setw(24) << "MISSION_IDENTIFICATION:"
       << theMissionIdentification<< "\n"
       << pfx << std::setw(24) << "FLIGHT_NO:"
       << theFlightNo << "\n"
       << pfx << std::setw(24) << "OP_NUM:"
       << theOpNum << "\n"
       << pfx << std::setw(24) << "CURRENT_SEGMENT:"
       << theCurrentSegment << "\n"
       << pfx << std::setw(24) << "REPRO_NUM:"
       << theReproNum<< "\n"
       << pfx << std::setw(24) << "REPLAY:"
       << theReplay<< "\n"
       << pfx << std::setw(24) << "START_TILE_COLUMN:"
       << theStartTileColumn << "\n"
       << pfx << std::setw(24) << "START_TILE_ROW:"
       << theStartTileRow << "\n"
       << pfx << std::setw(24) << "END_SEGMENT:"
       << theEndSegment << "\n"
       << pfx << std::setw(24) << "END_TILE_COLUMN:"
       << theTileColumn << "\n"
       << pfx << std::setw(24) << "END_TILE_ROW:"
       << theTileRow << "\n"
       << pfx << std::setw(24) << "COUNTRY:"
       << theCountry << "\n"
       << pfx << std::setw(24) << "LOCATION:"
       << theLocation << "\n";
   
   return out;
}
