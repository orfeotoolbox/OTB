//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Interface for info factories.
// 
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimInfoFactoryInterface_HEADER
#define ossimInfoFactoryInterface_HEADER

#include <ossim/base/ossimConstants.h>

class ossimFilename;
class ossimInfoBase;


/**
 * @brief Info factory.
 */
class OSSIM_DLL ossimInfoFactoryInterface
{
public:

   /** default constructor */
   ossimInfoFactoryInterface(){}

   /** virtual destructor */
   virtual ~ossimInfoFactoryInterface(){}

   /**
    * @brief Pure virtual create method.  Derived classes must implement to
    * be concrete.
    *
    * @param file Some file you want info for.
    *
    * @return ossimInfoBase* on success 0 on failure.  Caller is responsible
    * for memory.
    */
   virtual ossimInfoBase* create(const ossimFilename& file) const = 0;
};

#endif /* End of "#ifndef ossimInfoFactoryInterface_HEADER" */
