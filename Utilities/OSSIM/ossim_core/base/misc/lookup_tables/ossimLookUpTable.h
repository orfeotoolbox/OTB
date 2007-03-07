//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Contains class declaration for LookUpTable. 
//*******************************************************************
//  $Id: ossimLookUpTable.h,v 1.12 2005/05/13 18:57:48 dburken Exp $

#ifndef ossimLookUpTable_HEADER
#define ossimLookUpTable_HEADER

#include <vector>

#include "base/common/ossimConstants.h"
#include "base/data_types/ossimString.h"
#include "base/data_types/ossimKeyword.h"

class OSSIMDLLEXPORT ossimKeywordlist;

//*******************************************************************
// CLASS:  LookUpTable
//*******************************************************************
class OSSIMDLLEXPORT ossimLookUpTable
{
public:

   enum
   {
      NOT_FOUND    = -1
   };
   
   virtual ~ossimLookUpTable();

   /*!
    *  @return
    *  Returns the entry string associated with the entry number (key) passed
    *  in. Returns empty string if entry number is not in the list.
    */
   virtual ossimString getEntryString(ossim_int32 entry_number) const;

   /*!
    *  @param table_index Index into the table
    *  Note:  This is not the key mapped to the but the index into the table.
    *  @return
    *  Returns the entry string associated with the table index passed in.
    *  Returns empty string if index is out of range.
    */
   virtual ossimString getTableIndexString(ossim_uint32 table_index) const;

   /*!
    *  Returns the entry string associated with the entry number passed in.
    *  Returns empty string if entry number is not in the list.
    */
   virtual ossimString operator[](ossim_int32 entry_number) const;

   /*!
    *  Returns the entry string associated with lookup table keyword entry
    *  in the Keywordlist passed in.
    *  Returns empty string if keyword entry is not in the Keywordlist.
    */
   virtual ossimString getEntryString(const ossimKeywordlist& kwl,
                                      const char* prefix=0) const;

   /*!
    *  Returns the entry number associated with the entry string passed in.
    *  Returns NOT_FOUND(-1) if entry string is not in the list.
    *  If case_insensitive == true(default), the test is case insensitive;
    *  else, the test will be case sensitive.
    */
   virtual ossim_int32 getEntryNumber(const char* entry_string,
                                      bool case_insensitive = true) const;

   /*!
    *  Returns the entry number associated with the lookup table keyword
    *  entry in the Keywordlist passed in.  Returns NOT_FOUND(-1) if no
    *  matching entry.
    *  If case_insensitive == true(default), the test is case insensitive;
    *  else, the test will be case sensitive.
    */
   virtual ossim_int32 getEntryNumber(const ossimKeywordlist& kwl,
                                      const char* prefix=0,
                                      bool case_insensitive = true) const;

   /*!
    *  Returns keyword for lookups from a Keywordlist.
    */
   virtual ossimKeyword getKeyword() const=0;

   virtual ossim_uint32 getTableSize() const;
  
  void dumpValues(std::ostream& out)const;
protected:

   ossimLookUpTable(ossim_int32 table_size);

   class ossimKeyValueMap
   {
   public:
      ossim_int32  theKey;
      ossimString theValue;
   };

   std::vector<ossimKeyValueMap>  theTable;
   
private:
   ossimLookUpTable(){} // disallow...
};

#endif
