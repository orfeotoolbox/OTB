//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimUnitTypeLut.h 11959 2007-10-31 19:22:56Z gpotts $
#ifndef ossimUnitTypeLut_HEADER
#define ossimUnitTypeLut_HEADER

#include <ossim/base/ossimLookUpTable.h>
#include <ossim/base/ossimKeywordNames.h>

class OSSIMDLLEXPORT ossimUnitTypeLut : public ossimLookUpTable
{
public:

   /**
    * Returns the static instance of an ossimUnitTypeLut object.
    */
   static ossimUnitTypeLut* instance();

   virtual ~ossimUnitTypeLut();

   /*!
    *  Returns the entry number associated with the entry string passed in.
    *  Returns OSSIM_UNIT_UNKNOWN if entry string is not in the list.
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
   
   virtual ossimKeyword getKeyword() const;

protected:
   ossimUnitTypeLut();

private:
   static ossimUnitTypeLut* theInstance;
   
};

#endif
