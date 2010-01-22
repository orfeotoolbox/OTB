//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description:
// Base class for all ossim streams...  Derives from ossimReferenced.
//
//*******************************************************************
//  $Id: ossimStreamBase.h 11176 2007-06-07 19:45:56Z dburken $
//
#ifndef ossimStreamBase_HEADER
#define ossimStreamBase_HEADER

#include <ossim/base/ossimConstants.h>  /* for OSSIM_DLL */
#include <ossim/base/ossimReferenced.h> /* for base class */

class OSSIM_DLL ossimStreamBase : public ossimReferenced
{
public:
   /** @brief default constructor */
   ossimStreamBase();

   /** @brief virtual destructor */
   virtual ~ossimStreamBase();

   /**
    * @brief isCompressed method.
    * @return true if compressed false if not compressed.
    * @note This default implementation returns false.  Derived classes should
    * override if needed.
    */
   virtual bool isCompressed() const;
};
#endif /* end of "#ifndef ossimStreamBase_HEADER" */
