//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Description:
//
// Contains class definition for ossimNormalizedS16RemapTable.
// 
//*******************************************************************
//  $Id: ossimNormalizedS16RemapTable.cpp 9963 2006-11-28 21:11:01Z gpotts $

#include <ossim/imaging/ossimNormalizedS16RemapTable.h>

ossim_float64 ossimNormalizedS16RemapTable::theTable[TABLE_ENTRIES];
bool          ossimNormalizedS16RemapTable::theTableIsInitialized = false;

ossimNormalizedS16RemapTable::ossimNormalizedS16RemapTable()
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

ossimNormalizedS16RemapTable::~ossimNormalizedS16RemapTable()
{}
