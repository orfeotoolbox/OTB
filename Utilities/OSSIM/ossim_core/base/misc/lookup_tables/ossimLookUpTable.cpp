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
//
// Description:
//
// Contains class declaration for 
//*******************************************************************
//  $Id: ossimLookUpTable.cpp,v 1.9 2004/06/04 14:28:46 dburken Exp $

#include <iostream>
using namespace std;

#include "ossimLookUpTable.h"
#include "base/data_types/ossimKeywordlist.h"

//*******************************************************************
// Protected Constructor:
//*******************************************************************
ossimLookUpTable::ossimLookUpTable(ossim_int32 table_size)
   :
      theTable(table_size)
{}

ossimLookUpTable::~ossimLookUpTable()
{
}

//*******************************************************************
// Public Method:
//*******************************************************************
ossimString ossimLookUpTable::getEntryString(ossim_int32 entry_number) const
{
   for (ossim_uint32 i=0; i<theTable.size(); ++i)
   {
      if (theTable[i].theKey == entry_number)
      {
         return theTable[i].theValue;
      }
   }

   return ossimString(); // Entry number not found.
}

//*******************************************************************
// Public Method:
//*******************************************************************
ossimString ossimLookUpTable::getTableIndexString(ossim_uint32 table_index) const
{
   if (table_index < theTable.size())
   {
      return theTable[table_index].theValue;
   }

   return ossimString(); // Index out of range.
}

ossimString ossimLookUpTable::operator[](ossim_int32 entry_number) const
{
   return getEntryString(entry_number);
}

//*******************************************************************
// Public Method:
//*******************************************************************
ossimString ossimLookUpTable::getEntryString(const ossimKeywordlist& kwl,
                                             const char* prefix) const
{
   ossimString s = kwl.find(prefix, getKeyword());
   return s;
}

//*******************************************************************
// Public Method:
//*******************************************************************
ossim_int32 ossimLookUpTable::getEntryNumber(const char*  entry_string,
                                             bool case_insensitive) const
{
   ossimString s1 = entry_string;
   s1 = s1.trim();
   for (ossim_uint32 i=0; i<theTable.size(); i++)
   {
      ossimString s2 = theTable[i].theValue;

      if (case_insensitive == true)
      {
         s1.downcase();
         s2.downcase();
      }
      
      if (s1 == s2)
      {
         return theTable[i].theKey;
      }
   }

   return ossimLookUpTable::NOT_FOUND; // Entry number not found.   
}

//*******************************************************************
// Public Method:
//*******************************************************************
ossim_int32 ossimLookUpTable::getEntryNumber(const ossimKeywordlist& kwl,
                                             const char* prefix,
                                             bool case_insensitive) const
{
   const char* lookup = kwl.find(prefix, getKeyword());

   if (lookup)
   {
      return getEntryNumber(lookup, case_insensitive);
   }

   return ossimLookUpTable::NOT_FOUND; // Entry number not found.   
}

ossim_uint32 ossimLookUpTable::getTableSize() const
{
   return theTable.size();
}

void ossimLookUpTable::dumpValues(std::ostream& out)const
{
  ossim_uint32 tableSize = getTableSize();
  ossim_uint32 i = 0;

  for(i = 0; i < tableSize; ++i)
    {
      out << theTable[0].theValue << std::endl;
    }
}
