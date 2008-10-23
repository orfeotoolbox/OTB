//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken (dburken@imagelinks.com)
//
// Description:
//
// Contains class declaration for InterleaveTypeLUT.
//*******************************************************************
//  $Id: ossimInterleaveTypeLut.h 9968 2006-11-29 14:01:53Z gpotts $

#ifndef ossimInterleaveTypeLUT_HEADER
#define ossimInterleaveTypeLUT_HEADER

#include <ossim/base/ossimLookUpTable.h>
#include <ossim/base/ossimKeywordNames.h>

class OSSIMDLLEXPORT ossimInterleaveTypeLut : public ossimLookUpTable
{
public:

   ossimInterleaveTypeLut();

   virtual ~ossimInterleaveTypeLut(){}

   virtual ossimKeyword getKeyword() const
      { return ossimKeyword(ossimKeywordNames::INTERLEAVE_TYPE_KW, ""); }
};

#endif
