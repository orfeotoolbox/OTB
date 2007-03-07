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
// Contains class declaration for ossimScalarTypeLut.
// 
//*******************************************************************
//  $Id: ossimScalarTypeLut.h,v 1.4 2005/05/09 12:30:30 dburken Exp $

#ifndef ossimScalarTypeLut_HEADER
#define ossimScalarTypeLut_HEADER

#include <base/misc/lookup_tables/ossimLookUpTable.h>
#include <base/common/ossimKeywordNames.h>

/**
 * Used to convert from scalar type enumeration to strings and vice versa.
 * See ossimConstants.h for a complete list of enumerations.  This class is
 * a singleton, only one of them so all callers must go through the
 * instance method like:
 * ossimScalarTypeLut::instance()->getEntryString(scalar);
 */
class OSSIMDLLEXPORT ossimScalarTypeLut : public ossimLookUpTable
{
public:

   /**
    * Returns the static instance of an ossimScalarTypeLut object.
    */
   static ossimScalarTypeLut* instance();

   virtual ~ossimScalarTypeLut();

   ossimScalarType getScalarTypeFromString(const ossimString& s) const;
   
   virtual ossimKeyword getKeyword() const;

protected:
   ossimScalarTypeLut();

private:

   static ossimScalarTypeLut* theInstance;
};

#endif
