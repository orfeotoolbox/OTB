//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken (dburken@imagelinks.com)
// Description:
//
// Contains class definition for ossimNormalizedU8RemapTable.
//*******************************************************************
//  $Id: ossimNormalizedU8RemapTable.cpp 9963 2006-11-28 21:11:01Z gpotts $

#include <ossim/imaging/ossimNormalizedU8RemapTable.h>

double ossimNormalizedU8RemapTable::theTable[TABLE_ENTRIES];
bool   ossimNormalizedU8RemapTable::theTableIsInitialized = false;

ossimNormalizedU8RemapTable::ossimNormalizedU8RemapTable()
{
   if (!theTableIsInitialized)
   {
      //***
      // Initialize the remap table.
      //***
      for (ossim_int32 i = 0; i < TABLE_ENTRIES; i++)
      {
         ossim_float64 tmp = i;
         theTable[i] = tmp / 255.0;
      }
      theTableIsInitialized = true;
   }
}
