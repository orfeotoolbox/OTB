//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken (dburken@imagelinks.com)
// Description:
//
// Contains class definition for ossimNormalizedU16RemapTable.
//*******************************************************************
//  $Id: ossimNormalizedU16RemapTable.cpp 9963 2006-11-28 21:11:01Z gpotts $

#include <ossim/imaging/ossimNormalizedU16RemapTable.h>

double ossimNormalizedU16RemapTable::theTable[TABLE_ENTRIES];
bool   ossimNormalizedU16RemapTable::theTableIsInitialized = false;

ossimNormalizedU16RemapTable::ossimNormalizedU16RemapTable()
{
   if (!theTableIsInitialized)
   {
      //***
      // Initialize the remap table.
      //***
      for (ossim_int32 i = 0; i < TABLE_ENTRIES; i++)
      {
         ossim_float64 tmp = i;
         theTable[i] = tmp / 65535.0;
      }
      theTableIsInitialized = true;
   }
}
