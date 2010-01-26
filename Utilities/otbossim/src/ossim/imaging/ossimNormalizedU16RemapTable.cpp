//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Description:
//
// Contains class definition for ossimNormalizedU16RemapTable.
// 
//*******************************************************************
//  $Id: ossimNormalizedU16RemapTable.cpp 15743 2009-10-17 13:00:45Z dburken $

#include <ossim/imaging/ossimNormalizedU16RemapTable.h>

ossim_float64 ossimNormalizedU16RemapTable::theTable[TABLE_ENTRIES];
bool          ossimNormalizedU16RemapTable::theTableIsInitialized = false;


ossimNormalizedU16RemapTable::ossimNormalizedU16RemapTable()
   : ossimNormalizedRemapTable()
{
   if (!theTableIsInitialized)
   {
      const ossim_int32   ENTRIES     = getEntries();
      const ossim_float64 DENOMINATOR = getNormalizer();
      
      //---
      // Initialize the remap table.
      //---
      theTable[0] = 0.0; // Index zero always for null.
      for (ossim_int32 i = 1; i < ENTRIES; ++i)
      {
         theTable[i] = static_cast<ossim_float64>(i)/DENOMINATOR;
      }

      theTableIsInitialized = true;
   }
}

ossimNormalizedU16RemapTable::~ossimNormalizedU16RemapTable()
{}


