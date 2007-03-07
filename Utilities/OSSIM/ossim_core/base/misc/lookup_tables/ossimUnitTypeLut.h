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
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
//*******************************************************************
//  $Id: ossimUnitTypeLut.h,v 1.4 2003/05/13 11:31:03 dburken Exp $
#ifndef ossimUnitTypeLut_HEADER
#define ossimUnitTypeLut_HEADER

#include "ossimLookUpTable.h"
#include "base/common/ossimKeywordNames.h"

class OSSIMDLLEXPORT ossimUnitTypeLut : public ossimLookUpTable
{
public:

   ossimUnitTypeLut();

   virtual ~ossimUnitTypeLut(){}

   /*!
    *  Returns the entry number associated with the entry string passed in.
    *  Returns OSSIM_UNIT_UNKNOWN if entry string is not in the list.
    *  If case_insensitive == true(default), the test is case insensitive;
    *  else, the test will be case sensitive.
    */
   virtual ossim_int32 getEntryNumber(const char* entry_string,
                                bool case_insensitive = true) const
      {
         ossim_int32 result
            = ossimLookUpTable::getEntryNumber(entry_string, case_insensitive);
         if(result < 0)
         {
            return (ossim_int32)OSSIM_UNIT_UNKNOWN;
         }
         
         return result;
      }

   /*!
    *  Returns the entry number associated with the lookup table keyword
    *  entry in the Keywordlist passed in.  Returns NOT_FOUND(-1) if no
    *  matching entry.
    *  If case_insensitive == true(default), the test is case insensitive;
    *  else, the test will be case sensitive.
    */
   virtual ossim_int32 getEntryNumber(const ossimKeywordlist& kwl,
                                      const char* prefix=NULL,
                                      bool case_insensitive = true) const
      {
         ossim_int32 result =
            ossimLookUpTable::getEntryNumber(kwl, prefix, case_insensitive);
         if(result < 0)
         {
            return (ossim_int32)OSSIM_UNIT_UNKNOWN;
         }
         return result;
      }
   
   virtual ossimKeyword getKeyword() const
      { return ossimKeyword(ossimKeywordNames::UNITS_KW, ""); }
};

#endif
