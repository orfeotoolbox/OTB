//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
// 
// Description:  This class gives access to the Data Set Identification
//               (DSI) of a DTED Level 1 file.
//
//********************************************************************
// $Id: ossimDtedDsi.cpp,v 1.11 2005/01/11 18:08:57 dburken Exp $

#include <fstream>
using namespace std;

#include <support_data/dted/ossimDtedDsi.h>
#include <base/context/ossimNotifyContext.h>

//**************************************************************************
// CONSTRUCTOR
//**************************************************************************
ossimDtedDsi::ossimDtedDsi(const ossimFilename& dted_file, ossim_int32 offset)
   :
      theRecSen(),
      theSecurityCode(),
      theField3(),
      theField4(),
      theProductLevel(),
      theField7(),
      theField8(),
      theEdition(),
      theMatchMergeVersion(),
      theMaintenanceDate(),
      theMatchMergeDate(),
      theMaintenanceCode(),
      theProducerCode(),
      theField15(),
      theProductStockSpecNumber(),
      theProductSpecNumber(),
      theProductSpecDate(),
      theVerticalDatum(),
      theHorizontalDatum(),
      theField21(),
      theCompilationDate(),
      theField23(),
      theLatOrigin(),
      theLonOrigin(),
      theLatSW(),
      theLonSW(),
      theLatNW(),
      theLonNW(),
      theLatNE(),
      theLonNE(),
      theLatSE(),
      theLonSE(),
      theOrientation(),
      theLatInterval(),
      theLonInterval(),
      theNumLatPoints(),
      theNumLonLines(),
      theCellIndicator(),
      theField40(),
      theField41(),
      theField42(),
      theStartOffset(offset),
      theStopOffset(0)
{
   // Check to see that dted file exists.
   if(!dted_file.exists())
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimDtedDsi::ossimDtedDsi: \nThe DTED file does not exist: " << dted_file << std::endl;
      return;
   }

   // Check to see that the dted file is readable.
   if(!dted_file.isReadable())
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimDtedDsi::ossimDtedDsi: The DTED file is not readable: " << dted_file << std::endl;
      return;
   }
   
   // Open the dted file for reading.
   ifstream in(dted_file.c_str());
   if(!in)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimDtedDsi::ossimDtedDsi: Unable to open the DTED file: " << dted_file << std::endl;
      return;
   }

   parse(in);

   in.close();
}

//**************************************************************************
// CONSTRUCTOR
//**************************************************************************
ossimDtedDsi::ossimDtedDsi(istream&  in, ossim_int32 offset)
   :
      theRecSen(),
      theSecurityCode(),
      theField3(),
      theField4(),
      theProductLevel(),
      theField7(),
      theField8(),
      theEdition(),
      theMatchMergeVersion(),
      theMaintenanceDate(),
      theMatchMergeDate(),
      theMaintenanceCode(),
      theProducerCode(),
      theField15(),
      theProductStockSpecNumber(),
      theProductSpecNumber(),
      theProductSpecDate(),
      theVerticalDatum(),
      theHorizontalDatum(),
      theField21(),
      theCompilationDate(),
      theField23(),
      theLatOrigin(),
      theLonOrigin(),
      theLatSW(),
      theLonSW(),
      theLatNW(),
      theLonNW(),
      theLatNE(),
      theLonNE(),
      theLatSE(),
      theLonSE(),
      theOrientation(),
      theLatInterval(),
      theLonInterval(),
      theNumLatPoints(),
      theNumLonLines(),
      theCellIndicator(),
      theField40(),
      theField41(),
      theField42(),
      theStartOffset(offset),
      theStopOffset(0)
{
   parse(in);
}

void ossimDtedDsi::parse(istream& in)
{

   char tmp_chars[26]; // For blank field reads.
   
   // Seek to the start of the record.
   in.seekg(theStartOffset, ios::beg);
   
   // Parse theRecSen
   in.read(theRecSen, FIELD1_SIZE);
   theRecSen[FIELD1_SIZE] = '\0';
   
   if(!(strncmp(theRecSen, "DSI", 3) == 0))
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      return;
   }

   // Parse theSecurityCode
   in.read(theSecurityCode, FIELD2_SIZE);
   theSecurityCode[FIELD2_SIZE] = '\0';
   
   // Parse Field 3
   in.read(theField3, FIELD3_SIZE);
   theField3[FIELD3_SIZE] = '\0';
   
   // Parse Field 4
   in.read(theField4, FIELD4_SIZE);
   theField4[FIELD4_SIZE] = '\0';
   
   // Parse Field 5 (currently blank)
   in.read(tmp_chars, FIELD5_SIZE);
   
   // Parse theProductLevel
   in.read(theProductLevel, FIELD6_SIZE);
   theProductLevel[FIELD6_SIZE] = '\0';
   
   // Parse Field 7
   in.read(theField7, FIELD7_SIZE);
   theField7[FIELD7_SIZE] = '\0';
   
   // Parse Field 8
   in.read(theField8, FIELD8_SIZE);
   theField8[FIELD8_SIZE] = '\0';
   
   // Parse theEdition
   in.read(theEdition, FIELD9_SIZE);
   theEdition[FIELD9_SIZE] = '\0';
   
   // Parse theMatchMergeVersion
   in.read(theMatchMergeVersion, FIELD10_SIZE);
   theMatchMergeVersion[FIELD10_SIZE] = '\0';
   
   // Parse theMaintenanceDate
   in.read(theMaintenanceDate, FIELD11_SIZE);
   theMaintenanceDate[FIELD11_SIZE] = '\0';
   
   // Parse theMatchMergeDate
   in.read(theMatchMergeDate, FIELD12_SIZE);
   theMatchMergeDate[FIELD12_SIZE] = '\0';
   
   // Parse theMaintenanceCode
   in.read(theMaintenanceCode, FIELD13_SIZE);
   theMaintenanceCode[FIELD13_SIZE] = '\0';
   
   // Parse theProducerCode
   in.read(theProducerCode, FIELD14_SIZE);
   theProducerCode[FIELD14_SIZE] = '\0';
   
   // Parse Field 15
   in.read(theField15, FIELD15_SIZE);
   theField15[FIELD15_SIZE] = '\0';
   
   // Parse theProductStockSpecNumber
   in.read(theProductStockSpecNumber, FIELD16_SIZE);
   theProductStockSpecNumber[FIELD16_SIZE] = '\0';
   
   // Parse theProductSpecNumber
   in.read(theProductSpecNumber, FIELD17_SIZE);
   theProductSpecNumber[FIELD17_SIZE] = '\0';
   
   // Parse theProductSpecDate
   in.read(theProductSpecDate, FIELD18_SIZE);
   theProductSpecDate[FIELD18_SIZE] = '\0';
   
   // Parse theVerticalDatum
   in.read(theVerticalDatum, FIELD19_SIZE);
   theVerticalDatum[FIELD19_SIZE] = '\0';
   
   // Parse theHorizontalDatum
   in.read(theHorizontalDatum, FIELD20_SIZE);
   theHorizontalDatum[FIELD20_SIZE] = '\0';
   
   // Parse Field 21
   in.read(theField21, FIELD21_SIZE);
   theField21[FIELD21_SIZE] = '\0';
   
   // Parse theCompilationDate
   in.read(theCompilationDate, FIELD22_SIZE);
   theCompilationDate[FIELD22_SIZE] = '\0';
   
   // Parse Field 23
   in.read(theField23, FIELD23_SIZE);
   theField23[FIELD23_SIZE] = '\0';
   
   // Parse theLatOrigin
   in.read(theLatOrigin, FIELD24_SIZE);
   theLatOrigin[FIELD24_SIZE] = '\0';
   
   // Parse theLonOrigin
   in.read(theLonOrigin, FIELD25_SIZE);
   theLonOrigin[FIELD25_SIZE] = '\0';
   
   // Parse theLatSW
   in.read(theLatSW, FIELD26_SIZE);
   theLatSW[FIELD26_SIZE] = '\0';
   
   // Parse theLonSW
   in.read(theLonSW, FIELD27_SIZE);
   theLonSW[FIELD27_SIZE] = '\0';
   
   // Parse theLatNW
   in.read(theLatNW, FIELD28_SIZE);
   theLatNW[FIELD28_SIZE] = '\0';
   
   // Parse theLonNW
   in.read(theLonNW, FIELD29_SIZE);
   theLonNW[FIELD29_SIZE] = '\0';
   
   // Parse theLatNE
   in.read(theLatNE, FIELD30_SIZE);
   theLatNE[FIELD30_SIZE] = '\0';
   
   // Parse theLonNE
   in.read(theLonNE, FIELD31_SIZE);
   theLonNE[FIELD31_SIZE] = '\0';
   
   // Parse theLatSE
   in.read(theLatSE, FIELD32_SIZE);
   theLatSE[FIELD32_SIZE] = '\0';
   
   // Parse theLonSE
   in.read(theLonSE, FIELD33_SIZE);
   theLonSE[FIELD33_SIZE] = '\0';
   
   // Parse theOrientation
   in.read(theOrientation, FIELD34_SIZE);
   theOrientation[FIELD34_SIZE] = '\0';
   
   // Parse theLatInterval
   in.read(theLatInterval, FIELD35_SIZE);
   theLatInterval[FIELD35_SIZE] = '\0';
   
   // Parse theLonInterval
   in.read(theLonInterval, FIELD36_SIZE);
   theLonInterval[FIELD36_SIZE] = '\0';
   
   // Parse theNumLatPoints
   in.read(theNumLatPoints, FIELD37_SIZE);
   theNumLatPoints[FIELD37_SIZE] = '\0';
   
   // Parse theNumLonLines
   in.read(theNumLonLines, FIELD38_SIZE);
   theNumLonLines[FIELD38_SIZE] = '\0';
   
   // Parse theCellIndicator
   in.read(theCellIndicator, FIELD39_SIZE);
   theCellIndicator[FIELD39_SIZE] = '\0';
   
   // Parse Field 40
   in.read(theField40, FIELD40_SIZE);
   theField40[FIELD40_SIZE] = '\0';
   
   // Parse Field 41
   in.read(theField41, FIELD41_SIZE);
   theField41[FIELD41_SIZE] = '\0';
   
   // Parse Field 42
   in.read(theField42, FIELD42_SIZE);
   theField42[FIELD42_SIZE] = '\0';

   // Set the stop offset.
   theStopOffset = theStartOffset + DSI_LENGTH;
}

ossimString ossimDtedDsi::recoginitionSentinel() const
{
   return theRecSen;
}

ossimString ossimDtedDsi::securityCode() const
{
   return ossimString(theSecurityCode);
}

ossimString ossimDtedDsi::productLevel() const
{
   return ossimString(theProductLevel);
}

ossimString ossimDtedDsi::edition() const
{
   return ossimString(theEdition);
}

ossimString ossimDtedDsi::matchMergeVersion() const
{
   return ossimString(theMatchMergeVersion);
}

ossimString ossimDtedDsi::maintanenceDate() const
{
   return ossimString(theMaintenanceDate);
}

ossimString ossimDtedDsi::matchMergeDate() const
{
   return ossimString(theMatchMergeDate);
}

ossimString ossimDtedDsi::maintenanceCode() const
{
   return ossimString(theMaintenanceCode);
}

ossimString ossimDtedDsi::producerCode() const
{
   return ossimString(theProducerCode);
}

ossimString ossimDtedDsi::productStockSpecNumber() const
{
   return ossimString(theProductStockSpecNumber);
}

ossimString ossimDtedDsi::productSpecNumber() const
{
   return ossimString(theProductSpecNumber);
}

ossimString ossimDtedDsi::productSpecDate() const
{
   return ossimString(theProductSpecDate);
}

ossimString ossimDtedDsi::verticalDatum() const
{
   return ossimString(theVerticalDatum);
}

ossimString ossimDtedDsi::horizontalDatum() const
{
   return ossimString(theHorizontalDatum);
}

ossimString ossimDtedDsi::compilationDate() const
{
   return ossimString(theCompilationDate);
}

ossimString ossimDtedDsi::latOrigin() const
{
   return ossimString(theLatOrigin);
}

ossimString ossimDtedDsi::lonOrigin() const
{
   return ossimString(theLonOrigin);
}

ossimString ossimDtedDsi::latSW() const
{
   return ossimString(theLatSW);
}

ossimString ossimDtedDsi::lonSW() const
{
   return ossimString(theLonSW);
}

ossimString ossimDtedDsi::latNW() const
{
   return ossimString(theLatNW);
}

ossimString ossimDtedDsi::lonNW() const
{
   return ossimString(theLonNW);
}

ossimString ossimDtedDsi::latNE() const
{
   return ossimString(theLatNE);
}

ossimString ossimDtedDsi::lonNE() const
{
   return ossimString(theLonNE);
}

ossimString ossimDtedDsi::latSE() const
{
   return ossimString(theLatSE);
}

ossimString ossimDtedDsi::lonSE() const
{
   return ossimString(theLonSE);
}

ossimString ossimDtedDsi::orientation() const
{
   return ossimString(theOrientation);
}

ossimString ossimDtedDsi::latInterval() const
{
   return ossimString(theLatInterval);
}

ossimString ossimDtedDsi::lonInterval() const
{
   return ossimString(theLonInterval);
}

ossim_int32  ossimDtedDsi::numLatPoints() const
{
   return (theNumLatPoints ? atoi(theNumLatPoints) : 0);
}

ossim_int32  ossimDtedDsi::numLonLines() const
{
   return (theNumLonLines ? atoi(theNumLonLines) : 0);
}

ossim_int32  ossimDtedDsi::cellIndicator() const
{
   return (theCellIndicator ? atoi(theCellIndicator) : 0);
}

ossim_int32 ossimDtedDsi::startOffset() const
{
   return theStartOffset;
}
ossim_int32 ossimDtedDsi::stopOffset() const
{
   return theStopOffset;
}


//**************************************************************************
// operator <<
//**************************************************************************
ostream& operator<<( ostream& os, const ossimDtedDsi& dsi)
{
   os << "\nDTED Header (DSI):"
      << "\n-------------------------------"
      << "\nRecoginition Sentinel:     " << dsi.theRecSen
      << "\nSecurity Code:             " << dsi.theSecurityCode
      << "\nProduct Level:             " << dsi.theProductLevel
      << "\nEdition:                   " << dsi.theEdition
      << "\nMatch/Merge Version:       " << dsi.theMatchMergeVersion
      << "\nMaintenance Date:          " << dsi.theMaintenanceDate
      << "\nMatch/Merge Date:          " << dsi.theMatchMergeDate
      << "\nMaintenance Code:          " << dsi.theMaintenanceCode
      << "\nProducer Code:             " << dsi.theProducerCode
      << "\nProduct Stock Spec Number: " << dsi.theProductStockSpecNumber
      << "\nProduct Spec Number:       " << dsi.theProductSpecNumber
      << "\nVertical Datum:            " << dsi.theVerticalDatum
      << "\nHorizontal Datum:          " << dsi.theHorizontalDatum
      << "\nCompilation Date:          " << dsi.theCompilationDate
      << "\nLat Origin:                " << dsi.theLatOrigin
      << "\nLon Origin:                " << dsi.theLonOrigin
      << "\nLat SW:                    " << dsi.theLatSW
      << "\nLon SW:                    " << dsi.theLonSW
      << "\nLat NW:                    " << dsi.theLatNW
      << "\nLon NW:                    " << dsi.theLonNW
      << "\nLat NE:                    " << dsi.theLatNE
      << "\nLon NE:                    " << dsi.theLonNE
      << "\nLat SE:                    " << dsi.theLatSE
      << "\nLon SE:                    " << dsi.theLonSE
      << "\nOrientation:               " << dsi.theOrientation
      << "\nLat Interval:              " << dsi.theLatInterval
      << "\nLon Interval:              " << dsi.theLonInterval
      << "\nNumber of Lat Points:      " << dsi.theNumLatPoints
      << "\nNumber of Lon Lines:       " << dsi.theNumLonLines
      << "\nCell Indicator:            " << dsi.theCellIndicator
      << "\nStart Offset:              " << dsi.theStartOffset
      << "\nStop Offset:               " << dsi.theStopOffset
      << endl;

   return os;
}

ossimDtedDsi::ossimDtedDsi(const ossimDtedDsi& source)
{}

const ossimDtedDsi& ossimDtedDsi::operator=(const ossimDtedDsi& rhs)
{
   return rhs;
}
