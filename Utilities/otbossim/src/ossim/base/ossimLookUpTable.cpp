//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken (dburken@imagelinks.com)
//
// Description:
//
// Contains class declaration for 
//*******************************************************************
//  $Id: ossimLookUpTable.cpp 19448 2011-04-26 15:15:07Z dburken $

#include <iostream>
using namespace std;

#include <ossim/base/ossimLookUpTable.h>
#include <ossim/base/ossimKeywordlist.h>

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
   // Filter out trivial solution of NULL string:
   ossimString s1 = entry_string;
   if (!s1.empty())
   {
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
   return (ossim_uint32)theTable.size();
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
