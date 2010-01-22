//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Contains class declaration for ossimScalarTypeLut.
// 
//*******************************************************************
//  $Id: ossimScalarTypeLut.h 11914 2007-10-28 18:28:18Z dburken $

#ifndef ossimScalarTypeLut_HEADER
#define ossimScalarTypeLut_HEADER

#include <ossim/base/ossimLookUpTable.h>
#include <ossim/base/ossimKeywordNames.h>

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
