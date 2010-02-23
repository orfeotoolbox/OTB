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
      //
      // Specialized for elevation, make -32768 and -32767 map to 0 since
      // DTED NULL is -32767.
      //
      // NOTE: Zero will always remap back to -32768 with this hack.  This
      // could cause issues on writers that use pixFromNorm(). (drb)
      //---
      theTable[0] = 0.0; // Index zero always for null.
      theTable[1] = 0.0; // Specialized for DTED.

      for (ossim_int32 i = 2; i < ENTRIES; ++i)
      {
         theTable[i] = static_cast<ossim_float64>(i)/DENOMINATOR;
      }

      theTableIsInitialized = true;
   }
}

ossimNormalizedS16RemapTable::~ossimNormalizedS16RemapTable()
{}
