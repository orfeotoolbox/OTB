//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Description:
//
// Contains class definition for ossimNormalizedU11RemapTable.
// 
//*******************************************************************
//  $Id: ossimNormalizedU11RemapTable.cpp 15743 2009-10-17 13:00:45Z dburken $

#include <ossim/imaging/ossimNormalizedU11RemapTable.h>

ossim_float64 ossimNormalizedU11RemapTable::theTable[TABLE_ENTRIES];
bool          ossimNormalizedU11RemapTable::theTableIsInitialized = false;

ossimNormalizedU11RemapTable::ossimNormalizedU11RemapTable()
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

ossimNormalizedU11RemapTable::~ossimNormalizedU11RemapTable()
{}
