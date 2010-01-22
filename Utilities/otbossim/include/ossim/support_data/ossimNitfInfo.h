//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: NITF Info object.
// 
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimNitfInfo_HEADER
#define ossimNitfInfo_HEADER

#include <iosfwd>
#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimInfoBase.h>

class ossimNitfFile;

/**
 * @brief NITF info class.
 *
 * Encapsulates the dumpnitf functionality.
 */
class OSSIM_DLL ossimNitfInfo : public ossimInfoBase
{
public:
   
   /** default constructor */
   ossimNitfInfo();

   /** virtual destructor */
   virtual ~ossimNitfInfo();

   /**
    * @brief open method.
    *
    * @param file File name to open.
    *
    * @return true on success false on error.
    */
   virtual bool open(const ossimFilename& file);
   
   /**
    * Print method.
    *
    * @param out Stream to print to.
    * 
    * @return std::ostream&
    */
   virtual std::ostream& print(std::ostream& out) const;

private:
   ossimNitfFile* theNitfFile;
};

#endif /* End of "#ifndef ossimNitfInfo_HEADER" */
