//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken (dburken@imagelinks.com)
// Description:
//
// Contains class definition for ossimNormalizedU11RemapTable.
//*******************************************************************
//  $Id: ossimNormalizedU11RemapTable.cpp 9963 2006-11-28 21:11:01Z gpotts $

#include <ossim/imaging/ossimNormalizedU11RemapTable.h>

double ossimNormalizedU11RemapTable::theTable[TABLE_ENTRIES];
bool   ossimNormalizedU11RemapTable::theTableIsInitialized = false;

ossimNormalizedU11RemapTable::ossimNormalizedU11RemapTable()
{
   if (!theTableIsInitialized)
   {
      //***
      // Initialize the remap table.
      //***
      for (ossim_int32 i = 0; i < TABLE_ENTRIES; i++)
      {
         ossim_float64 tmp = i;
         theTable[i] = tmp / 2047.0;
      }
      theTableIsInitialized = true;
   }
}
