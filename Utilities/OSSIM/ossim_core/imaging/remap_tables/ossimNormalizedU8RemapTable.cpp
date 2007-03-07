//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author:  David Burken (dburken@imagelinks.com)
// Description:
//
// Contains class definition for ossimNormalizedU8RemapTable.
//*******************************************************************
//  $Id: ossimNormalizedU8RemapTable.cpp,v 1.3 2003/05/13 11:31:06 dburken Exp $

#include "ossimNormalizedU8RemapTable.h"

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
