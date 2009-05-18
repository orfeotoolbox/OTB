//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: CCF Info object.
// 
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimCcfInfo_HEADER
#define ossimCcfInfo_HEADER

#include <iosfwd>
#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimInfoBase.h>
#include <ossim/base/ossimFilename.h>

class ossimNitfFile;

/**
 * @brief NITF info class.
 *
 * Encapsulates the dumpnitf functionality.
 */
class OSSIM_DLL ossimCcfInfo : public ossimInfoBase
{
public:
   
   /** default constructor */
   ossimCcfInfo();

   /** virtual destructor */
   virtual ~ossimCcfInfo();

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
   ossimFilename theFile;
};

#endif /* End of "#ifndef ossimCcfInfo_HEADER" */
